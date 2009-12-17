/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Still Image Decoder for Camera Application Engine
*
*/



// INCLUDE FILES
#include <eikenv.h>

#include "CaeStillConverter.h"
#include "CaeStillDecoder.h"
#include "CaeStillCommon.h"

#ifdef CAE_TEST_VERSION
#include "CaeStillConverterTestErrors.h"
#endif


// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CCaeStillDecoder::NewL
// Two-phased constructor.
// ---------------------------------------------------------
//
EXPORT_C CCaeStillDecoder* CCaeStillDecoder::NewL()
    {
    CCaeStillDecoder* self = new( ELeave ) CCaeStillDecoder;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------
// CCaeStillDecoder::~CCaeStillDecoder
// Destructor.
// ---------------------------------------------------------
//
EXPORT_C CCaeStillDecoder::~CCaeStillDecoder()
    {
    delete iDecoder;
    delete iDecodedImage;
    iFs.Close(); 

    // For RTRT code coverage analysis.
    // #pragma attol insert _ATCPQ_DUMP(0);
    }


// ---------------------------------------------------------
// CCaeStillDecoder::SetObserver
// Sets Still Decoder observer.
// ---------------------------------------------------------
//
EXPORT_C void CCaeStillDecoder::SetObserver( 
    MCaeStillDecoderObserver* aObserver )
    {
    iObserver = aObserver;
    }


// -----------------------------------------------------------------------------
// CCaeStillDecoder::SetImageCodecL
// Sets the specific image codec implementation to be used in decoding.
// -----------------------------------------------------------------------------
//
void CCaeStillDecoder::SetImageCodecL( 
	TUid aCodecUid )
    {
    iImageCodecUid = aCodecUid;
    }

        
// ---------------------------------------------------------
// CCaeStillDecoder::ConvertHBufC8ToCFbsBitmapL
// Creates CImageItem object, places it in the internal 
// queue and starts memory image to bitmap conversion.
// ---------------------------------------------------------
//
EXPORT_C void CCaeStillDecoder::ConvertHBufC8ToCFbsBitmapL( 
    HBufC8* aImageBuffer,
    TDisplayMode aTargetBitmapMode,
    const TSize& aTargetBitmapSize,
    const TSize& aFullyScaledTargetBitmapSize )
    {
    LOGTEXT( _L( "Cae: CCaeStillDecoder::ConvertHBufC8ToCFbsBitmapL() entering" ) );

    CleanupStack::PushL( aImageBuffer );

    // Create image item and set up image data.

    CImageItem* imageItem = new( ELeave ) CImageItem;
    imageItem->iBitmapDisplayMode = aTargetBitmapMode;
    imageItem->iImageBuffer = aImageBuffer;
    imageItem->iBitmapSize = aTargetBitmapSize;
    imageItem->iFullyScaledBitmapSize = aFullyScaledTargetBitmapSize;
    CleanupStack::Pop( aImageBuffer );
    CleanupStack::PushL( imageItem ); 

    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaeConvertHBufC8ToCFbsBitmapErrorL();
    #endif    

    // Add image to the queue.
    User::LeaveIfError ( iImageQueue->Append( imageItem ) );

    CleanupStack::Pop( imageItem );

    // Start image conversion if not busy. Busy means that e.g. conversion is currently running. 
    if ( !IsBusy() )
        {
        TRAPD( error, ConvertL() );
        if ( error != KErrNone )
            {
            ConversionComplete( error );
            }
        }

    LOGTEXT( _L( "Cae: CCaeStillDecoder::ConvertHBufC8ToCFbsBitmapL() returning" ) );
    }


// ---------------------------------------------------------
// CCaeStillDecoder::Cleanup
// Destroy all allocations.
// ---------------------------------------------------------
//
EXPORT_C void CCaeStillDecoder::Cleanup()
    {
    if ( iDecoder )
        {
        iDecoder->Cancel();
        delete iDecoder;
        iDecoder = NULL;
        }

    if ( iState != EIdle )
        {
        delete iDecodedImage;
        iDecodedImage = NULL;
        }
    
    if ( iImageQueue )
        {
        iImageQueue->ResetAndDestroy();
        }
    }


// ---------------------------------------------------------
// CCaeStillDecoder::CCaeStillDecoder
// Default constructor. 
// This can NOT leave.
// ---------------------------------------------------------
//
CCaeStillDecoder::CCaeStillDecoder()
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------
// CCaeStillDecoder::ConstructL
// Symbian 2nd phase constructor that can leave.
// ---------------------------------------------------------
//
void CCaeStillDecoder::ConstructL()
    {
    CCaeStillConverter::ConstructL();
    User::LeaveIfError( iFs.Connect() );
    }


// ---------------------------------------------------------
// CCaeStillDecoder::DoCancel
// From CActive, implements cancellation of an outstanding request.
// ---------------------------------------------------------
//
void CCaeStillDecoder::DoCancel()
    {
    Cleanup();
    iState = EIdle;
    }


// ---------------------------------------------------------
// CCaeStillDecoder::ConvertL
// Converts bitmap to memory JPEG image.
// ---------------------------------------------------------
//
void CCaeStillDecoder::ConvertL()
    {
    LOGTEXT( _L( "Cae: CCaeStillDecoder::ConvertL() entering" ) );

    iState = EConvert;
    
    // Use the default codec or the specific codec
    iDecoder = CImageDecoder::DataNewL( iFs, *(*iImageQueue)[0]->iImageBuffer, 
    	CImageDecoder::EOptionNone, KNullUid, KNullUid, iImageCodecUid );
    
    iDecodedImage = NULL;
    iDecodedImage = new( ELeave ) CFbsBitmap;

    // Set target bitmap size
    TSize targetBitmapSize = (*iImageQueue)[0]->iBitmapSize;

    const TFrameInfo& frameInfo = iDecoder->FrameInfo( 0 );
    TUid impUid = iDecoder->ImplementationUid();
    LOGTEXT3( _L( "Cae: CCaeStillDecoder::ConvertL() Requested decoder: %x, Found decoder: %x" ), iImageCodecUid.iUid, impUid.iUid );
    LOGTEXT2( _L( "Cae: CCaeStillDecoder::ConvertL() Free scaling support: %d" ), frameInfo.iFlags & TFrameInfo::EFullyScaleable );

    //
    // Use free scaling always with special decoder or with any decoder that supports it
    //
    if ( (( impUid == KUidSpecialFreeScalingDecoder ) || ( frameInfo.iFlags & TFrameInfo::EFullyScaleable ))  
         && ( (*iImageQueue)[0]->iFullyScaledBitmapSize != TSize( 0, 0 ) ) )
        {
        LOGTEXT( _L( "Cae: CCaeStillDecoder::ConvertL(). Use free scaling in decoding" ) );
        targetBitmapSize = (*iImageQueue)[0]->iFullyScaledBitmapSize;
        }

    User::LeaveIfError( iDecodedImage->Create(
        targetBitmapSize, 
        (*iImageQueue)[0]->iBitmapDisplayMode ) );

    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaeStillConvertErrorL();
    #endif    

    iState = EConvert;
    iStatus = KRequestPending;
    iDecoder->Convert( &iStatus, *iDecodedImage );
    SetActive();

    LOGTEXT( _L( "Cae: CCaeStillDecoder::ConvertL() returning" ) );
    }


// ---------------------------------------------------------
// CCaeStillDecoder::ConversionComplete
// Perfoms necessary cleanup and delivers result to the client.
// ---------------------------------------------------------
//
void CCaeStillDecoder::ConversionComplete( 
    TInt aError )
    {
    LOGTEXT( _L( "Cae: CCaeStillDecoder::ConversionComplete() entering" ) );
    #ifdef _DEBUG
    if ( aError ) 
        {
        LOGTEXT( _L( "Cae: CCaeStillDecoder::ConversionComplete(): error detected" ) );
        }
    #endif

    // Delete the decoder object and image queue item
    if ( iDecoder )
        {
        iDecoder->Cancel();
        delete iDecoder;
        iDecoder = NULL;
        }

    // To be outputted via call-back.
    HBufC8* imageBuffer = NULL;

    if ( iImageQueue->Count() > 0 )
        {
        imageBuffer = (*iImageQueue)[0]->iImageBuffer;

        (*iImageQueue)[0]->iImageBuffer = NULL; // Prevent deletion of source image.
        delete ( *iImageQueue )[0];

        // Remove item pointer from the queue and compress the queue.
        iImageQueue->Remove( 0 ); // Remove item pointer from the queue and compress the queue.
        }

    iState = EIdle;

    const TInt KZeroImageSize = 0; // Image size argument currently not in use.

    // Call back the client to deliver the result image. Gives away the ownership of the imageBuffer
    // and iDecodedImage.
    iObserver->McaesdoCFbsBitmapImageReady( imageBuffer, iDecodedImage, aError, KZeroImageSize );
    iDecodedImage = NULL; // Ownership has been given away

    LOGTEXT( _L( "Cae: CCaeStillDecoder::ConversionComplete() returning" ) );
    }


//  End of File  
