/*
* Copyright (c) 2005 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Camera Application Engine still image burst capturing 
*                interface implementation
*
*/


// INCLUDE FILES
#include "CaeEngineImp.h"                   // Engine implementation header.
#include "CaeStillBurst.h"                  // Still capturing burst mode class.
#include "CaeStillStatesActive.h"           // Still capturing burst mode class.

#ifdef CAE_TEST_VERSION
#include "CaeEngineImpTestErrors.h"         // For TEST_VERSION compilation only
#endif


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetCaeStillBurstObserver
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetCaeStillBurstObserver( 
    MCaeStillBurstObserver& aObserver )
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::SetCaeStillBurstObserver()" ) );
    iCaeStillBurstObserver = &aObserver;
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetStillCaptureImageCountL
// Delete old instance and create a new burst object if required. 
// -----------------------------------------------------------------------------
//
TInt CCaeStillStatesActive::SetStillCaptureImageCountL( 
    TInt aImageCount )
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::SetStillCaptureImageCountL() entering" ) );

    // Leave if still capturing is running.
    if ( IsRunning() ) 
        {
        LOGTEXT( _L( "Cae: CCaeStillStatesActive::SetStillCaptureImageCountL() leaving KErrNotReady (IsRunning)" ) );
        User::Leave( KErrNotReady );
        }
    
    // Leave if the image count requested is invalid.
    if ( aImageCount <= 0 ) 
        {
        LOGTEXT2( _L( "Cae: invalid input argument aImageCount = %d, leaving KErrArgument" ), aImageCount );
        User::Leave( KErrArgument );
        }

    // Delete possible old still burst instance.
    DeleteStillBurst();

    // Prepare for burst capturing if requested image count more than 1.
    // Otherwise iStillBurst stays NULL.
    if ( aImageCount > 1 )
        {
        // Create new instance of still image burst object.
        iStillBurst = CCaeStillBurst::NewL();

        // Set parameter(s).
        iStillBurst->SetLengthL( aImageCount );

        LOGTEXT2( _L( "Cae: image count set successfully = %d" ), aImageCount );
        }

    LOGTEXT( _L( "Cae: CCaeStillStatesActive::SetStillCaptureImageCountL() returning" ) );    

    return aImageCount;
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::StillCaptureImageCount
// -----------------------------------------------------------------------------
//
TInt CCaeStillStatesActive::StillCaptureImageCount() const
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::StillCaptureImageCount() entering" ) );
    
    // Nonexistence of still burst object implies image count of 1 
    // (= normal single still capture).
    TInt imageCount( 1 );
    if ( iStillBurst ) 
        {
        imageCount = iStillBurst->Length();
        }
        
    LOGTEXT2( _L( "Cae: CCaeStillStatesActive::StillCaptureImageCount() returning: %d" ), imageCount );
    
    return imageCount;
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetStillBurstCaptureIntervalL
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetStillBurstCaptureIntervalL( 
    TTimeIntervalMicroSeconds aInterval )
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::SetStillBurstCaptureIntervalL()" ) );

    if ( IsRunning() ) 
        {
        LOGTEXT( _L( "Cae: CCaeStillStatesActive::SetStillBurstCaptureIntervalL(), leaving KErrNotReady" ) );
        User::Leave( KErrNotReady );
        }
        
    // Note: KMaxTInt32 in microseconds is about 35 minutes
    if ( aInterval < TTimeIntervalMicroSeconds( 0 ) || 
         aInterval > TTimeIntervalMicroSeconds( KMaxTInt32 ) )
        {
        LOGTEXT( _L( "Cae: CCaeStillStatesActive::SetStillBurstCaptureIntervalL(), leaving KErrArgument" ) );
        User::Leave( KErrArgument );
        }

    iStillBurstCaptureInterval = I64INT( aInterval.Int64() );
    }
        

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::StillBurstCaptureInterval
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CCaeStillStatesActive::StillBurstCaptureInterval() const 
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::StillBurstCaptureInterval()" ) );
    
    TInt64 intInterval = iStillBurstCaptureInterval.Int();

    TTimeIntervalMicroSeconds interval( intInterval );

    return interval;
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::StopStillBurstCapture
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::StopStillBurstCapture()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::StopStillBurstCapture()" ) );

    iStillBurstStopped = ETrue;
    
    // Complete early any pending delayed event 
	if ( iDelayedEvent != CCaeStillStatesActive::ECaeEventNone )
    	{
        iDelayedCallTimer.Cancel();
        }
    }


// ==================== PRIVATE MEMBER FUNCTIONS ===============================


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::HandleAppendCapturedBurstImageReady()
// Fetch the first burst image from the array, or continue burst capturing.
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleAppendCapturedBurstImageReady()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleAppendCapturedBurstImageReady()" ));

    iStatus = CCaeStillStatesActive::ECaeEventNone;

    if ( iStillBurst->IsBurstCaptured() || iStillBurstStopped )
        {
        // Fetch the first burst image from the array

        iCurrentState = CCaeStillStatesActive::ECaeStateFetchingNextBurstImage;
        iStillBurstDeliveryStarted = ETrue;            
        DoFetchNextBurstImage();
        }
    else
        {         
        // Continue burst capturing.
        // If view finder is enabled, at least one view finder image 
        // should be received before the next capture.
        // Otherwise, start directly the next capture.

         // wait alway next iViewFinderFrameReceived if optimization is not used
         // with optimization old VF updates are acccepted
        if ( iIsViewFinderEnabled &&
             !(iBurstModeVFOptimization && iViewFinderFrameReceived ) )
            {
            LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleAppendCapturedBurstImageReady(). Waiting for the view finder." ) );
            iCurrentState = CCaeStillStatesActive::ECaeStateBurstWaitingForViewFinder;            
            }
        else        
            {
            iCurrentState = CCaeStillStatesActive::ECaeStateCapturingBurst;            
            DoCaptureStillBurstImage();
            }
        }
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::HandleBurstImageFetchReady()
// Decode the captured image, extract Exif metadata, or process the 
// image by the extensions.  
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleBurstImageFetchReady()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleBurstImageFetchReady()" ));

    iStatus = CCaeStillStatesActive::ECaeEventNone;

    if ( !iBitmap                   // Does not already exist
         && ( iCreateSnapImage ) )   // Snap bitmap required by UI
        {
        // Need to decode first.
        iCurrentState = CCaeStillStatesActive::ECaeStateDecodingCapturedImageToBitmap;            
        DoDecodeCapturedImageToBitmap();
        }
    else
        {
        // No need to decode.
            {
            iCurrentState = CCaeStillStatesActive::ECaeStateExtensionsProcessingCapturedImage;            
            DoExtensionsProcessCapturedImage();    
            }
        }
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::HandleDeliverStillBurstImageReady()
// Complete still burst or fetch the next image.
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleDeliverStillBurstImageReady()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleDeliverStillBurstImageReady()" ));
    CAE_ASSERT_ALWAYS( iStillBurst && ( iBitmap == NULL ) && ( iImageData == NULL ) && ( iSnapBitmap == NULL ) );

    iStatus = CCaeStillStatesActive::ECaeEventNone;
    if ( CheckForStillBurstCompletion() )
        {
        iCurrentState = CCaeStillStatesActive::ECaeStateCompletingStillBurst;            
        DoCompleteStillBurst();
        }
    else
        {
        iCurrentState = CCaeStillStatesActive::ECaeStateFetchingNextBurstImage;            
        DoFetchNextBurstImage();
        }
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoCaptureStillBurst
// Start the burst and capture the first burst image.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoCaptureStillBurst()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoCaptureStillBurst() entering" ) );
    MEM();
 
    iFirstStillBurstError = KErrNone;
    iStillBurstDeliveryStarted = EFalse;
        
    // Do not wait for the view finder for the first capture
    iViewFinderFrameReceived = ETrue; 
    iStillCancelled = EFalse;

    // Capture the first image in the burst.
    DoCaptureStillBurstImage();

    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoCaptureStillBurst() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoCaptureStillBurstImage
// Capture one image for the burst.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoCaptureStillBurstImage()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoCaptureStillBurstImage() entering" ) );
    MEM();
    CAE_ASSERT_ALWAYS( !iStillCancelled );

    // Notify client about capturing moment.
    if ( !iStillBurstStopped )
        {
        iCaeStillBurstObserver->McaesboStillBurstCaptureMoment( KErrNone );       
        }

    // Note: If still burst is cancelled in the callback, the state 
    // machine has already been reseted. Any member data is not allowed
    // to be changed any more. Just return from this function. 

    if ( !iStillCancelled && !iStillBurstStopped ) 
        {
        LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoCaptureStillBurstImage() capture next image" ) );
        iViewFinderFrameReceived = EFalse;
        iCamera.CaptureImage();
        }

    // Separate "if" (instead of "else if") due 
    // the possibility of stopping or cancelling in callback above.
    if ( !iStillCancelled ) // Return immediately, if cancelled
        {
        if ( iStillBurstStopped )
            {
            // Deliver the images to the client if the burst was stopped.
            // This call delivers the first image in the burst.
            if ( !iStillBurstDeliveryStarted )
                {
                LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoCaptureStillBurstImage(): burst stopped, starting delivery" ) );
                iStillBurstDeliveryStarted = ETrue;
                Event( CCaeStillStatesActive::ECaeEventBurstCaptureReady );
                }
        
            }
        }

    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoCaptureStillBurstImage() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoAppendCapturedBurstImageToArray
// Append the captured image to the still burst array.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoAppendCapturedBurstImageToArray()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoAppendCapturedBurstImageToArray() entering" ) );
    CAE_ASSERT_ALWAYS( !iStillCancelled );
    MEM();

    TInt error = iFirstStillBurstError; // We may have had error during previous capture phase

    if ( !error ) 
        {
        // Put the image into buffer.
        // All appended images will have KErrNone code.
        // AppendImage does not delete the image if unable to append (error is returned).
        error = iStillBurst->AppendImage( iBitmap, iImageData, KErrNone ); 
        }

    if ( !error ) 
        {
        // Ownership transferred
        iBitmap = NULL;
        iImageData = NULL;
        }
    else
        {
        LOGTEXT2( _L( "Cae: CCaeStillStatesActive::DoAppendCapturedBurstImageToArray(): error detected: %d" ), error );
    	    
        // Delete images
        delete( iBitmap );
        iBitmap = NULL;
	    delete( iImageData );
        iImageData = NULL;

        iFirstStillBurstError =  iFirstStillBurstError ? iFirstStillBurstError : error;
        iStillBurstStopped = ETrue; // Stop burst if an error was detected.    
        }

    if ( !iStillBurstStopped && !iStillBurst->IsBurstCaptured() ) 
        {
        // Activate (delayed) capturing of new image if burst is not complete yet.
        Event( CCaeStillStatesActive::ECaeEventAppendCapturedBurstImageReady, iStillBurstCaptureInterval );
        }
    else  
        {
        // Start delivering immediately
        Event( CCaeStillStatesActive::ECaeEventAppendCapturedBurstImageReady );
        }

    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoAppendCapturedBurstImageToArray() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoFetchNextBurstImage
// Get the next image from the burst array.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoFetchNextBurstImage()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoFetchNextBurstImage() entering" ) );
    MEM();
    CAE_ASSERT_ALWAYS( ( iBitmap == NULL ) && ( iImageData == NULL ) && ( iSnapBitmap == NULL ) );
    CAE_ASSERT_ALWAYS( !iStillCancelled );
    CAE_ASSERT_ALWAYS( iStillBurst );

    if ( ExtModeActive() ) // Extension processing mode
        {
            Event( CCaeStillStatesActive::ECaeEventEnd ); // extension mode operation is completed when queue is empty
        }
    else  // Normal mode
        {
        LOGTEXT2( _L( "Cae: CCaeStillStatesActive::DoFetchNextBurstImage(): count of burst images in queue = %d" ), iStillBurst->ImageCount() );

        // Get next image from the array

        CFbsBitmap* bitmap = NULL; 
        HBufC8* imageData = NULL;
        TInt imageError( KErrNone );

        TInt error = iStillBurst->GetNextImage( bitmap, imageData, imageError );

        // If there is no items in the array, return the first error
        if ( (error == KErrUnderflow) && iFirstStillBurstError)
            {
            error = iFirstStillBurstError;
            }

        error = error ? error : imageError;
        iImageData = imageData;
        iBitmap = bitmap;

        if ( error )
            {
            // Handle error
            ErrorRecovery( error );                    
            }
        else
            {
            Event( CCaeStillStatesActive::ECaeEventBurstImageFetchReady );
            }
        }
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoFetchNextBurstImage() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::CheckForStillBurstCompletion
// -----------------------------------------------------------------------------
//
TBool CCaeStillStatesActive::CheckForStillBurstCompletion()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::CheckForStillBurstCompletion()" ) );
    MEM();
    
    TBool burstShouldBeCompleted( EFalse );

    if ( iStillBurst )
        {
        if ( ExtModeActive() )  // In extension mode client knows when completed
            {
            burstShouldBeCompleted = iLastImageExtPro;
            }
        else if ( iCountOfProcessedBurstImages >= iStillBurst->CaptureCount() ) // Normal mode
            {
            // All captures have now been processed, ready to complete the burst.
            LOGTEXT2( _L( "Cae: CCaeStillStatesActive::CheckForStillBurstCompletion(): Burst complete, count of images = %d" ), iCountOfProcessedBurstImages );
            burstShouldBeCompleted = ETrue;
            }
        }

    return( burstShouldBeCompleted );
    }
    

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::BurstErrorRecovery
// Recover from burst errors.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::BurstErrorRecovery( TInt aError )
    {
    // Handle burst image mode errors   
    
    // Store error code
    iFirstStillBurstError =  iFirstStillBurstError ? iFirstStillBurstError : aError;

    iCountOfProcessedBurstImages++;

    // Delete all images which are not deleted already. 
    delete iSnapBitmap;
    iSnapBitmap = NULL;
    delete iBitmap;
    iBitmap = NULL;
    delete iImageData;
    iImageData = NULL;

    // Ignore error and continue processing
    Event( CCaeStillStatesActive::ECaeEventDeliverStillBurstImageReady );       
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoCompleteStillBurst
// Complete still image burst capture. 
// Calls the observer method McaesboStillBurstComplete if capturing was not 
// cancelled.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoCompleteStillBurst()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoCompleteStillBurst() entering" ) );

    if ( !iStillCancelled && IsRunning() )
        {
        // Notify client about completing the burst.
        iCaeStillBurstObserver->McaesboStillBurstComplete( iCountOfDeliveredBurstImages, iFirstStillBurstError );
        LOGTEXT2( _L( "Cae: CCaeStillStatesActive::DoCompleteStillBurst(): Count of delivered burst images = %d" ), iCountOfDeliveredBurstImages );
        }
    iFirstStillBurstError = KErrNone;
    CompleteStillBurst();
    Event( CCaeStillStatesActive::ECaeEventEnd ); // The end. Actuall does not issue any event. 

    MEM();
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoCompleteStillBurst() returning" ) );
    }

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoDeliverStillBurstImage
//
// Deliver the still burst image to the client and delete internal images after that.
// Note: If ownership is transferred, the image pointers should be NULLed before
// calling the observer method. That is because the observer method can call back
// CancelStill() which tries to delete images. That is no allowed as images 
// should be owned by the observer.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoDeliverStillBurstImage()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoDeliverStillBurstImage()" ) );
    CAE_ASSERT_ALWAYS( !iStillCancelled && ( iSnapBitmap == NULL ) );

    // Deliver the still burst image to the client.
    if ( IsBitmapOutput() )
        {
        // Give original image to the client even if there is decoding error.
        CFbsBitmap* tmpBitmap = iBitmap;
        iBitmap = NULL; // ownership is transferred      
        iCaeObserver->McaeoStillImageReady( tmpBitmap, NULL, KErrNone );
        }
    else
        {
        // Give original image to the client even if there is encoding error
        HBufC8* tmpImageData = iImageData;   
        iImageData = NULL; // Ownership is tranferred
        iCaeObserver->McaeoStillImageReady( NULL, tmpImageData, KErrNone );
        }

    iCountOfDeliveredBurstImages++;
    iCountOfProcessedBurstImages++;

    // Delete all that is not NULL already. 
    delete iImageData;
    iImageData = NULL;
    delete iImageHeaderData;
    iImageHeaderData = NULL;
    delete iBitmap;
    iBitmap = NULL;

    Event( CCaeStillStatesActive::ECaeEventDeliverStillBurstImageReady );
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::CompleteStillBurst()
// Complete still image burst capture but do not delete still burst object. 
// Calls the observer method McaesboStillBurstComplete if capturing was not 
// cancelled.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::CompleteStillBurst()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::CompleteStillBurst() entering" ) );
    MEM();
    if ( iStillBurst )
        {
        iStillBurst->ResetAndDestroyImages();
        }
  
    iCountOfProcessedBurstImages = 0;
    iCountOfDeliveredBurstImages = 0;
    iStillBurstStopped = EFalse;
    MEM();
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::CompleteStillBurst() returning" ) );
   }

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DeleteStillBurst
// Delete still burst object and reset burst related variables.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DeleteStillBurst()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DeleteStillBurst() entering" ) );

    delete iStillBurst;
    iStillBurst = NULL;

    iCountOfProcessedBurstImages = 0;
    iCountOfDeliveredBurstImages = 0;

    iStillBurstStopped = EFalse;

    MEM();
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DeleteStillBurst() returning" ) );    
    }


//  End of File  
