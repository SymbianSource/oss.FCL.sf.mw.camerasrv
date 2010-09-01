/*
* Copyright (c) 2003-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Camera Application Engine image queue extension processing class
*
*/



// INCLUDE FILES

#include "CaeEngineImp.h"                   // For LOGTEXT
#include <fbs.h>                            // For CFbsBitmap

#include "CaeImageQueueExtPro.h"            // Still image burst capturing class.
#include "CaeImageItemExtPro.h"             // Still image item class.


// CONSTANTS
const TInt KCaeStillBurstImageQueueGranularity = 6; // optimized count of burst images

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaeImageQueueExtPro::NewL
// -----------------------------------------------------------------------------
//
CCaeImageQueueExtPro* CCaeImageQueueExtPro::NewL()
    {
    LOGTEXT( _L( "Cae: CCaeImageQueueExtPro::NewL() entering" ) );

    CCaeImageQueueExtPro* self = new( ELeave ) CCaeImageQueueExtPro;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOGTEXT( _L( "Cae: CCaeImageQueueExtPro::NewL() returning" ) );

    return self;
    }

    
// -----------------------------------------------------------------------------
// CCaeImageQueueExtPro::CCaeImageQueueExtPro
// Destroys image queue.
// -----------------------------------------------------------------------------
//
CCaeImageQueueExtPro::~CCaeImageQueueExtPro()
    {
    LOGTEXT( _L( "Cae: CCaeImageQueueExtPro::CaeImageQueueExtPro() entering" ) );

    ResetAndDestroyImages();
    delete iImageQueue;

    LOGTEXT( _L( "Cae: CCaeImageQueueExtPro::CaeImageQueueExtPro() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeImageQueueExtPro::CaeImageQueueExtPro
// -----------------------------------------------------------------------------
//
CCaeImageQueueExtPro::CCaeImageQueueExtPro() 
    {
    }


// -----------------------------------------------------------------------------
// CCaeImageQueueExtPro::ConstructL
// Allocates image queue.
// -----------------------------------------------------------------------------
//
void CCaeImageQueueExtPro::ConstructL()
    {
    LOGTEXT( _L( "Cae: CCaeImageQueueExtPro::ConstructL() entering" ) );

    iImageQueue = new( ELeave ) RPointerArray<CCaeImageItemExtPro>( 
        KCaeStillBurstImageQueueGranularity );

    LOGTEXT( _L( "Cae: CCaeImageQueueExtPro::ConstructL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeImageQueueExtPro::AppendImage
// Appends an image item object to image queue.
// -----------------------------------------------------------------------------
//
TInt CCaeImageQueueExtPro::AppendImage( 
    CFbsBitmap*  aBitmap, 
    HBufC8*      aImageData,
    TBool        aLastImage,
    TBool        aSnapImage )
    {
    LOGTEXT( _L( "Cae: CCaeImageQueueExtPro::AppendImage() entering" ) );

    TInt result( KErrNone );

    CCaeImageItemExtPro* imageItem = new CCaeImageItemExtPro();
    if ( imageItem ) 
        {
        // Move image data to image item
        imageItem->iImageData = aImageData;
        imageItem->iBitmap = aBitmap;
        imageItem->iLastImage = aLastImage;
        imageItem->iSnapImage = aSnapImage;

        // Add image to the queue.
        result = iImageQueue->Append( imageItem );
        if ( result != KErrNone ) 
            {
            // In case of error, delete only imageItem, not the actual data
            imageItem->iBitmap = NULL;
            imageItem->iImageData = NULL;
            delete imageItem;
            imageItem = NULL;
            }
        }
    else 
        {
        result = KErrNoMemory;
        }

    LOGTEXT( _L( "Cae: CCaeImageQueueExtPro::AppendImage() returning" ) );

    return result;
    }


// -----------------------------------------------------------------------------
// CCaeImageQueueExtPro::ImageCount
// -----------------------------------------------------------------------------
//
TInt CCaeImageQueueExtPro::ImageCount() const
    {
    // Return the count of image items.
    return iImageQueue->Count();
    }


// -----------------------------------------------------------------------------
// CCaeImageQueueExtPro::GetNextImage
// Fetches and deletes the next image from image queue.
// -----------------------------------------------------------------------------
//
TInt CCaeImageQueueExtPro::GetNextImage( 
    CFbsBitmap*&   aBitmap,     // output
    HBufC8*&       aImageData,  // output
    TBool&         aLastImage,  // output
    TBool&         aSnapImage ) // output
    {
    LOGTEXT2( _L( "Cae: CCaeImageQueueExtPro::GetNextImage() entering, %d images in queue" ), iImageQueue->Count() );

    TInt error( KErrNone );

    if ( iImageQueue->Count() > 0 )
        {
        // Get image data and bitmap pointer from the queue image item.
        aImageData = (*iImageQueue)[0]->iImageData;
        aBitmap    = (*iImageQueue)[0]->iBitmap;
        aLastImage = (*iImageQueue)[0]->iLastImage;
        aSnapImage = (*iImageQueue)[0]->iSnapImage;

        // Do not delete the data. Delete the image item and remove from queue.
        (*iImageQueue)[0]->iBitmap = NULL;
        (*iImageQueue)[0]->iImageData = NULL;
        delete (*iImageQueue)[0];
        iImageQueue->Remove( 0 );
        }
    else 
        {
        aBitmap = NULL;
        aImageData = NULL;
        aLastImage = EFalse;
        aSnapImage = EFalse;
        error = KErrUnderflow;
        }

    LOGTEXT2( _L( "Cae: CCaeImageQueueExtPro::GetNextImage() returning: %d" ), error );

    return error;
    }


// -----------------------------------------------------------------------------
// CCaeImageQueueExtPro::ResetAndDestroyImages()
// Reset and destroy image queue.
// -----------------------------------------------------------------------------
//
void CCaeImageQueueExtPro::ResetAndDestroyImages()
    {
    LOGTEXT( _L( "Cae: CCaeImageQueueExtPro::ResetAndDestroyImages()" ) );

    if ( iImageQueue ) 
        {
        // Reset and destroy the image items from queue.
        iImageQueue->ResetAndDestroy();
        }
    }


//  End of File  
