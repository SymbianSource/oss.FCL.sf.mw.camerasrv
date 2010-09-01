/*
* Copyright (c) 2003,2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Camera Application Engine still image burst class
*
*/



// INCLUDE FILES

#include "CaeEngineImp.h"                   // For LOGTEXT
#include <fbs.h>                            // For CFbsBitmap
#include <hal.h>                            // For HAL

#include "CaeStillBurst.h"                  // Still image burst capturing class.
#include "CaeImageItem.h"                   // Still image item class.


// CONSTANTS
const TInt KCaeStillBurstImageQueueGranularity = 6;
const TInt KCaeMinImageHeapSize = 0; 

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaeStillBurst::NewL
// -----------------------------------------------------------------------------
//
CCaeStillBurst* CCaeStillBurst::NewL()
    {
    LOGTEXT( _L( "Cae: CCaeStillBurst::NewL() entering" ) );

    CCaeStillBurst* self = new( ELeave ) CCaeStillBurst;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOGTEXT( _L( "Cae: CCaeStillBurst::NewL() returning" ) );

    return self;
    }

    
// -----------------------------------------------------------------------------
// CCaeStillBurst::~CCaeStillBurst
// Destroys image queue.
// -----------------------------------------------------------------------------
//
CCaeStillBurst::~CCaeStillBurst()
    {
    LOGTEXT( _L( "Cae: CCaeStillBurst::~CCaeStillBurst() entering" ) );

    ResetAndDestroyImages();

    if ( iImageHeap )
        {
        iImageHeap->Close();
        }

    delete iImageQueue;

    LOGTEXT( _L( "Cae: CCaeStillBurst::~CCaeStillBurst() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeStillBurst::CCaeStillBurst
// -----------------------------------------------------------------------------
//
CCaeStillBurst::CCaeStillBurst() 
    {
    }


// -----------------------------------------------------------------------------
// CCaeStillBurst::ConstructL
// Allocates image queue.
// -----------------------------------------------------------------------------
//
void CCaeStillBurst::ConstructL()
    {
    LOGTEXT2( _L( "Cae: CCaeStillBurst::ConstructL() entering, min heap: %d" ), KCaeMinImageHeapSize );

    iImageQueue = new( ELeave ) RPointerArray<CCaeImageItem>( 
        KCaeStillBurstImageQueueGranularity );

    TInt maxMem;
    User::LeaveIfError( HAL::Get( HAL::EMemoryRAM, maxMem ) );
    #if (defined (__WINS__) || defined (__WINSCW__))
    maxMem /= 2; // Fix emulator -4 error
    #endif

    iImageHeap = User::ChunkHeap( NULL, KCaeMinImageHeapSize, maxMem );

    #ifdef _DEBUG
    if ( !iImageHeap )
        {
        LOGTEXT( _L( "Cae: CCaeStillBurst::ConstructL() Error: Cannot create iImageHeap !" ) );
        }
    #endif

    User::LeaveIfNull( iImageHeap );

    LOGTEXT( _L( "Cae: CCaeStillBurst::ConstructL() returning" ) );
    }



// -----------------------------------------------------------------------------
// CCaeStillBurst::SetLengthL
// -----------------------------------------------------------------------------
//
void CCaeStillBurst::SetLengthL( 
    TInt aLength )
    {
    LOGTEXT2( _L( "Cae: CCaeStillBurst::SetLengthL(): length: %d" ), aLength );

    // Leave if not supported.
    if ( aLength < 1 )
        {
        LOGTEXT( _L( "Cae: CCaeStillBurst::SetLengthL(): leaving KErrArgument" ));
        User::Leave( KErrArgument );
        }

    iDesiredLength = aLength;
    }


// -----------------------------------------------------------------------------
// CCaeStillBurst::Length
// -----------------------------------------------------------------------------
//
TInt CCaeStillBurst::Length() const
    {
    LOGTEXT2( _L( "Cae: CCaeStillBurst::Length(): length: %d" ), iDesiredLength );

    return iDesiredLength;
    }


// -----------------------------------------------------------------------------
// CCaeStillBurst::AppendImage
// Appends an image item object to image queue.
// -----------------------------------------------------------------------------
//
TInt CCaeStillBurst::AppendImage( 
    CFbsBitmap*  aBitmap, 
    HBufC8*      aImageData, 
    TInt         aError )
    {
    LOGTEXT2( _L( "Cae: CCaeStillBurst::AppendImage() entering: %d" ), aError );

    TInt result( KErrNone );

    CCaeImageItem* imageItem = new CCaeImageItem( *iImageHeap );
    if ( imageItem ) 
        {
        if (aImageData)
            {
            // Copy image data from HBufC8 to image heap
            imageItem->iImageData = iImageHeap->Alloc( aImageData->Size() );
            if ( imageItem->iImageData )
                {
                imageItem->iImageDataSize = aImageData->Size();
                Mem::Copy( imageItem->iImageData, aImageData->Ptr(), 
                    imageItem->iImageDataSize );
                }
            else
                {
                result = KErrNoMemory;
                }
            }
        if ( result == KErrNone ) 
            {
            imageItem->iBitmap = aBitmap;
            imageItem->iError  = aError;
            // Add image to the queue.
            result = iImageQueue->Append( imageItem );
            }
        if ( result == KErrNone ) 
            {
            iCountOfBurstAppends++;
            LOGTEXT2( _L( "Cae: CCaeStillBurst::AppendImage(): count of burst appends = %d" ), iCountOfBurstAppends );

            // Given aImageData can be deleted now as it has successfully 
            // been copied to imageItem->iImageData
            delete aImageData;
            }
        else
            {
            // Do not delete the bitmap if error, delete just imageItem 
            // which deletes also iImageData.
            imageItem->iBitmap = NULL;
            delete imageItem;
            imageItem = NULL;
            }
        }
    else 
        {
        result = KErrNoMemory;
        }

    LOGTEXT( _L( "Cae: CCaeStillBurst::AppendImage() returning" ) );

    return result;
    }


// -----------------------------------------------------------------------------
// CCaeStillBurst::CaptureCount
// -----------------------------------------------------------------------------
//
TInt CCaeStillBurst::CaptureCount() const
    {
    // Return the count of appends (captures) made.
    return iCountOfBurstAppends;
    }


// -----------------------------------------------------------------------------
// CCaeStillBurst::ImageCount
// -----------------------------------------------------------------------------
//
TInt CCaeStillBurst::ImageCount() const
    {
    // Return the count of image items.
    return iImageQueue->Count();
    }


// -----------------------------------------------------------------------------
// CCaeStillBurst::GetNextImage
// Fetches and deletes the next image from image queue.
// -----------------------------------------------------------------------------
//
TInt CCaeStillBurst::GetNextImage( 
    CFbsBitmap*&   aBitmap,     // output
    HBufC8*&       aImageData,  // output
    TInt&          aError )     // output
    {
    LOGTEXT2( _L( "Cae: CCaeStillBurst::GetNextImage() entering, %d images in queue" ), iImageQueue->Count() );

    TInt error( KErrNone );

    if ( iImageQueue->Count() > 0 )
        {

        if ( (*iImageQueue)[0]->iImageData )
            {
            // Copy image data from queue image item.
            aImageData = HBufC8::New( (*iImageQueue)[0]->iImageDataSize );
            if ( aImageData )
                {
                aImageData->Des().Copy( (TUint8*)(*iImageQueue)[0]->iImageData, 
                    (*iImageQueue)[0]->iImageDataSize );
                }
            else
                {
                error = KErrNoMemory;
                }
            }

        if ( error == KErrNone )
            {
            // Get bitmap pointer and image error value from the queue image item.
            aBitmap     = (*iImageQueue)[0]->iBitmap;
            aError      = (*iImageQueue)[0]->iError;

            #ifdef _DEBUG
            if ( aError )
                {
                LOGTEXT2( _L( "Cae: CCaeStillBurst::GetNextImage(): This image has error: %d" ), aError );
                }
            #endif

            // Do not delete the bitmap. Delete the image item which deletes iImageData also.
            (*iImageQueue)[0]->iBitmap = NULL;
            delete (*iImageQueue)[0];
            iImageQueue->Remove( 0 );
            }

        if ( ( iImageQueue->Count() == 0 ) || error ) 
            {
            // Give free memory to system from the top of heap.
            iImageHeap->Compress(); 
            LOGTEXT( _L( "Cae: CCaeStillBurst::GetNextImage(): last image fetched, compressed the heap" ) );
            MEM();
            }
        }
    else 
        {
        aBitmap = NULL;
        aImageData = NULL;
        error = KErrUnderflow;
        }

    LOGTEXT2( _L( "Cae: CCaeStillBurst::GetNextImage() returning: %d" ), error );

    return error;
    }


// -----------------------------------------------------------------------------
// CCaeStillBurst::IsBurstCaptured
// -----------------------------------------------------------------------------
//
TBool CCaeStillBurst::IsBurstCaptured() const
    {
    TBool isComplete( EFalse );
    if ( iCountOfBurstAppends >= iDesiredLength )
        {
        isComplete = ETrue;
        }    
    return isComplete;
    }


// -----------------------------------------------------------------------------
// CCaeStillBurst::ResetAndDestroyImages()
// Reset and destroy image queue.
// -----------------------------------------------------------------------------
//
void CCaeStillBurst::ResetAndDestroyImages()
    {
    LOGTEXT( _L( "Cae: CCaeStillBurst::ResetAndDestroyImages()" ) );

    if ( iImageQueue ) 
        {
        // Reset and destroy the image items from queue.
        iImageQueue->ResetAndDestroy();
        }
    if ( iImageHeap )
        {
        iImageHeap->Compress(); // Give free memory to system from the top of heap
        }
    iCountOfBurstAppends = 0;
    }


//  End of File  
