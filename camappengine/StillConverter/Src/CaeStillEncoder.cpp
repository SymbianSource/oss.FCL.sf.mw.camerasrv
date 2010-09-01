/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Still Image Encoder for Camera Application Engine
*
*/



// INCLUDE FILES
#include <eikenv.h>

#include "CaeStillConverter.h"
#include "CaeStillEncoder.h"
#include "CaeStillCommon.h"

#ifdef CAE_TEST_VERSION
#include "CaeStillConverterTestErrors.h"
#endif


// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CCaeStillEncoder::NewL
// Two-phased constructor.
// ---------------------------------------------------------
//
EXPORT_C CCaeStillEncoder* CCaeStillEncoder::NewL()
    {
    CCaeStillEncoder* self = new( ELeave ) CCaeStillEncoder;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------
// CCaeStillEncoder::~CCaeStillEncoder
// Destructor.
// ---------------------------------------------------------
//
EXPORT_C CCaeStillEncoder::~CCaeStillEncoder()
    {
    Cancel();
    Cleanup();
    
    // For RTRT code coverage analysis.
    // #pragma attol insert _ATCPQ_DUMP(0);
    }


// ---------------------------------------------------------
// CCaeStillEncoder::SetObserver
// Sets Still encoder observer.
// ---------------------------------------------------------
//
EXPORT_C void CCaeStillEncoder::SetObserver( 
    MCaeStillEncoderObserver* aObserver )
    {
    iObserver = aObserver;
    }


// -----------------------------------------------------------------------------
// CCaeStillEncoder::SetImageCodecL
// Sets the specific image codec implementation to be used in encoding.
// -----------------------------------------------------------------------------
//
void CCaeStillEncoder::SetImageCodecL( 
	TUid aCodecUid )
    {
    iImageCodecUid = aCodecUid;
    }

// ---------------------------------------------------------
// CCaeStillEncoder::SetCompressionQuality
// Sets compression quality.
// ---------------------------------------------------------
//
EXPORT_C void CCaeStillEncoder::SetCompressionQuality( 
    TInt aQuality )
    {
    iJpegQuality = Min( KJpegQualityMax, Max( KJpegQualityMin, aQuality ) );
    }


// ---------------------------------------------------------
// CCaeStillEncoder::CompressionQuality
// Gets compression quality.
// ---------------------------------------------------------
//
EXPORT_C TInt CCaeStillEncoder::CompressionQuality() const
    {
    return iJpegQuality;
    }


// ---------------------------------------------------------
// CCaeStillEncoder::ConvertCFbsBitmapToHBufC8L
// Creates CImageItem object, places it in the internal 
// queue and starts bitmap to memory image conversion.
// ---------------------------------------------------------
//
EXPORT_C void CCaeStillEncoder::ConvertCFbsBitmapToHBufC8L( 
    CFbsBitmap* aBitmap,  
    TTargetFormat aTargetFormat )
    {
    LOGTEXT( _L( "Cae: CCaeStillEncoder::ConvertCFbsBitmapToHBufC8L() entering" ) );

    CleanupStack::PushL( aBitmap );

    // Create image item and set up image data.

    CImageItem* imageItem = new( ELeave ) CImageItem;

    imageItem->iBitmap = aBitmap;

    CleanupStack::Pop( aBitmap );
    CleanupStack::PushL( imageItem );

    switch ( aTargetFormat )
        {
        case ETargetFormatJPEG:
        default:
            {
            imageItem->iImageTypeUid = KImageTypeJPGUid;

            imageItem->iFrameImageData = CFrameImageData::NewL(); 
            // ( do not push member variable to cleanup stack )

            TJpegImageData* imagedata = new( ELeave ) TJpegImageData;
            CleanupStack::PushL( imagedata );

            imagedata->iSampleScheme = KCaeJpegColorSampling;
            imagedata->iQualityFactor = iJpegQuality;

            User::LeaveIfError ( imageItem->iFrameImageData->AppendImageData( imagedata ) );

            CleanupStack::Pop( imagedata );
            break;
            }
        }

    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaeConvertCFbsBitmapToHBufC8ErrorL();
    #endif    

    // Append image to the queue.
    User::LeaveIfError ( iImageQueue->Append( imageItem ) );

    CleanupStack::Pop( imageItem );

    // Start image conversion if not busy.
    if ( !IsBusy() )
        {
        TRAPD( error, ConvertL() );
        if ( error != KErrNone )
            {
            ConversionComplete( error );
            }
        }

    LOGTEXT( _L( "Cae: CCaeStillEncoder::ConvertCFbsBitmapToHBufC8L() returning" ) );
    }


// ---------------------------------------------------------
// CCaeStillEncoder::Cleanup
// Destroy all allocations.
// ---------------------------------------------------------
//
EXPORT_C void CCaeStillEncoder::Cleanup()
    {
    if ( iEncoder )
        {
        iEncoder->Cancel();
        delete iEncoder;
        iEncoder = NULL;
        }

    if ( iState != EIdle )
        {
        delete iEncodedImage;
        iEncodedImage = NULL;
        }
    
    if ( iImageQueue )
        {
        iImageQueue->ResetAndDestroy();
        }
    }


// ---------------------------------------------------------
// CCaeStillEncoder::CCaeStillEncoder
// Default constructor. 
// This can NOT leave.
// ---------------------------------------------------------
//
CCaeStillEncoder::CCaeStillEncoder() :
iJpegQuality( KJpegQualityMax )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------
// CCaeStillEncoder::ConstructL
// Symbian 2nd phase constructor that can leave.
// ---------------------------------------------------------
//
void CCaeStillEncoder::ConstructL()
    {
    CCaeStillConverter::ConstructL();
    }


// ---------------------------------------------------------
// CCaeStillEncoder::DoCancel
// From CActive, implements cancellation of an outstanding request.
// ---------------------------------------------------------
//
void CCaeStillEncoder::DoCancel()
    {
    Cleanup();
    iState = EIdle;
    }


// ---------------------------------------------------------
// CCaeStillEncoder::ConvertL
// Converts bitmap to memory JPEG image.
// Allocates Image Conversion Library (ICL) encoder and 
// starts conversion.
// ---------------------------------------------------------
//
void CCaeStillEncoder::ConvertL()
    {
    LOGTEXT( _L( "Cae: CCaeStillEncoder::ConvertL() entering" ) );

    // Do not delete the encoded image here, just set to NULL.
    iEncodedImage = NULL;

    // Allocate new ICL encoder.
    delete iEncoder;
    iEncoder = NULL;
    
    // Use the default codec or the specific codec
    iEncoder = CImageEncoder::DataNewL( iEncodedImage, 
    	CImageEncoder::EOptionNone, (*iImageQueue)[0]->iImageTypeUid, 
    	KNullUid, iImageCodecUid ); 

    LOGTEXT3( _L( "Cae: CCaeStillEncoder::ConvertL(). Requested encoder: %x, Found encoder: %x" ), iImageCodecUid.iUid, iEncoder->ImplementationUid().iUid );
    
    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaeStillConvertErrorL();
    #endif    

    iState = EConvert;
    iStatus = KRequestPending;
    iEncoder->Convert( &iStatus, 
                       *(*iImageQueue)[0]->iBitmap, 
                       (*iImageQueue)[0]->iFrameImageData );
    SetActive();

    LOGTEXT( _L( "Cae: CCaeStillEncoder::ConvertL() returning" ) );
    }


// ---------------------------------------------------------
// CCaeStillEncoder::ConversionComplete
// Perfoms necessary cleanup and delivers result 
// to the client.
// ---------------------------------------------------------
//
void CCaeStillEncoder::ConversionComplete( 
    TInt aError )
    {
    LOGTEXT( _L( "Cae: CCaeStillEncoder::ConversionComplete() entering" ) );
    #ifdef _DEBUG
    if ( aError ) 
        {
        LOGTEXT2( _L( "Cae: CCaeStillEncoder::ConversionComplete(): error detected: %d" ), aError );
        }
    #endif

    if ( iEncoder )
        {
        iEncoder->Cancel();
        delete iEncoder;
        iEncoder = NULL;
        }

    // To be outputted via call-back.
    CFbsBitmap* bitmap = NULL;
    
    if ( iImageQueue->Count() > 0 )
        {
        bitmap = (*iImageQueue)[0]->iBitmap;

        (*iImageQueue)[0]->iBitmap = NULL; // Prevent deletion of source image.
        delete ( *iImageQueue )[0];

        // Remove item pointer from the queue and compress the queue.
        iImageQueue->Remove( 0 );
        }

    iState = EIdle;

    const TInt KZeroImageSize = 0; // Image size argument currently not in use.

    // Call back the client to deliver the result image. Gives away the ownership of the bitmap 
    // and iEncodedImage.
    iObserver->McaeseoHBufC8ImageReady( bitmap, iEncodedImage, aError, KZeroImageSize );
    iEncodedImage = NULL; // Ownership has been given away

    LOGTEXT( _L( "Cae: CCaeStillEncoder::ConversionComplete() returning" ) );
    }


//  End of File  
