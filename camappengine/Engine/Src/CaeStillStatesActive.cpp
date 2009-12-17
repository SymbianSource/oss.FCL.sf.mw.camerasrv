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
* Description:  Still capture state machine implementation class
*
*/


// INCLUDE FILES
#include <hal.h>                    // For getting the display mode
#include <ExifRead.h>
#include "CaeStillStatesActive.h"
#include "CaeImageQueueExtPro.h"

#ifdef CAE_TEST_VERSION
#include "CaeEngineImpTestErrors.h" // For TEST_VERSION compilation only
#endif

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "CaeStillStatesActiveTraces.h"
#endif


// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::CCaeStillStatesActive()
// C++ constructor.
// Adds the object to the Active Scheduler.
// ---------------------------------------------------------------------------
//
CCaeStillStatesActive::CCaeStillStatesActive( 
    CCamera& aCamera,
    const TCamAppEngineInfo& aInfo,
    RArray<TCaeExtensionInterfaceImplItem>& aProcessImageImplList )
    : CActive( EPriorityStandard ), 
      iCamera( aCamera ),
      iInfo( aInfo ),
      iProcessImageImplList( aProcessImageImplList ),
      iStillInputFormat( CCamera::EFormatFbsBitmapColor16M ), 
      iStillOutputFormat( CCamera::EFormatFbsBitmapColor16M ), 
      iStillCompressionQuality( KCaeDefaultCompressionQuality ),
      iCurrentExtensionIndex( -1 )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::~CCaeStillStatesActive()
// Destructor. 
// Cancels operation and closes the timer.
// ---------------------------------------------------------------------------
//
CCaeStillStatesActive::~CCaeStillStatesActive()
    {
    Cancel();

    iDelayedCallTimer.Close();
    
    DeleteStillBurst(); // Using this removes dependency to still burst class

    CancelAndCleanup();

    // Check that extensions handle flag registration properly
    CAE_ASSERT_DEBUG( iRequireFullColorSnapInputImageRefCount == 0 );

    delete iEmptySnapImage;

    delete iStillDecoder;
    delete iStillEncoder;

	delete iSnapBitmap;
	delete iBitmap;
	delete iImageData;
	delete iImageHeaderData;
    delete iImageQueueExtPro;
    iFs.Close();
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::NewL()
// Symbian OS two-phased constructor.
// ---------------------------------------------------------------------------
//
CCaeStillStatesActive* CCaeStillStatesActive::NewL( 
    CCamera& aCamera,
    const TCamAppEngineInfo& aInfo,
    RArray<TCaeExtensionInterfaceImplItem>& aProcessImageImplList )
    {
    CCaeStillStatesActive* self = new( ELeave ) CCaeStillStatesActive( aCamera, aInfo, 
        aProcessImageImplList );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::ConstructL()
// Symbian OS 2nd phase constructor that can leave.
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::ConstructL()
    {
    // Get native screen mode. This can be used as default mode for the snap bitmap.
    iSnapImageColorMode = KCaeDefaultDisplayMode;
    (void)GetSystemDisplayMode( iSnapImageColorMode );
    LOGTEXT2( _L( "Cae: CCaeStillStatesActive::ConstructL(). Use display mode %d for the snap image" ), iSnapImageColorMode );

    // Create still image encoder object, e.g. for JPEG encoding.
    iStillEncoder = CCaeStillEncoder::NewL();
    iStillEncoder->SetObserver( this );

    // Create still image decoder object, e.g. for JPEG decoding.
    iStillDecoder = CCaeStillDecoder::NewL();
    iStillDecoder->SetObserver( this );

    // This empty snap image is currently used just for error cases.
    iEmptySnapImage = new( ELeave ) CFbsBitmap;

    // Create timer 
    iDelayedCallTimer.CreateLocal();
    
    // Connect to file server.
    User::LeaveIfError( iFs.Connect() );

    // Start in normal mode    
  	iExtModeActive = EFalse;
    iStillPrepared = EFalse;
    
    // Burst mode VF stopping optimization is not used as default
    iBurstModeVFOptimization = EFalse;

    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::GetSystemDisplayMode()
// In some devices Hal:Get(HAL::EDisplayMode) returns directly TDisplayMode, 
// in other devices it returns a display mode index.
// ---------------------------------------------------------------------------
//
TBool CCaeStillStatesActive::GetSystemDisplayMode( TDisplayMode& aDisplayMode )
    {
    TBool found = EFalse;
    TInt halInOut = 0;
    TInt err = HAL::Get( HAL::EDisplayMode, halInOut );
    if (  err == KErrNone )
        {       
        if ( halInOut >= EColor256)
            {
            // halInOut contains TDisplayMode
            aDisplayMode = (TDisplayMode) halInOut;
            found = ETrue;
            LOGTEXT( _L( "Cae: CCaeStillStatesActive::GetSystemDisplayMode(). HAL returned a display mode." ) );
            }
        else
            {
             // halInOut contains index
           LOGTEXT( _L( "Cae: CCaeStillStatesActive::GetSystemDisplayMode(). HAL returned index." ) );
            err = HAL::Get( HAL::EDisplayBitsPerPixel, halInOut );
            
            // Get the color mode. Note: grayscale modes are not supported. 
            if (  err == KErrNone )
                {
				switch (halInOut)
					{
				    case 4:
					    aDisplayMode = EColor16;
                        found = ETrue;
                        break;
			    	case 8:
					    aDisplayMode = EColor256;
                        found = ETrue;
                        break;
				    case 12:
				    	aDisplayMode = EColor4K;
                        found = ETrue;
                        break;
				    case 16:
					    aDisplayMode = EColor64K;
                        found = ETrue;
                        break;
				    case 24:
					    aDisplayMode = EColor16M;
                        found = ETrue;
                        break;
			    	case 32:
					    aDisplayMode = EColor16MU;
                        found = ETrue;
                        break;
				    default:
					    break;
					}
                }
            }
        } 

    #ifdef _DEBUG
    if ( err || !found )
        {
        TBuf<256> text;
        text.Format(_L( "Cae: CCaeStillStatesActive::GetSystemDisplayMode(). Cannot get the display mode from HW. Err:%d, found:%d" ), err, found );
        LOGTEXT( text );        
        }
    else
        {   
        LOGTEXT2( _L( "Cae: CCaeStillStatesActive::GetSystemDisplayMode(). Found the display mode:%d" ), aDisplayMode );
        }
    #endif

    if ( found && !err && aDisplayMode == EColor16MA )
        {
        aDisplayMode = EColor16MU;
        LOGTEXT2( _L( "Cae: CCaeStillStatesActive::GetSystemDisplayMode(). Display mode EColor16MA changed to EColor16MU (%d)" ), aDisplayMode );
        }

    return found;   
    }

// ---------------------------------------------------------------------------
// CCaeStillStatesActive::SetCamAppEngineObserver()
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetCamAppEngineObserver( MCamAppEngineObserver& aObserver )
    {
    iCaeObserver = &aObserver;    
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetSnapImageCreation()
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetSnapImageCreation( 
    TBool aCreateSnapImage )
    {
    iCreateSnapImage = aCreateSnapImage;
    }

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetSnapImageSourceL()
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetSnapImageSourceL( 
    CCaeEngine::TSnapImageSource aSnapImageSource )
    {
    LOGTEXT2( _L( "Cae: CCaeStillStatesActive::SetSnapImageSourceL(). aSnapImageSource=%d" ), aSnapImageSource );
    if ( ( aSnapImageSource < 0 ) || 
         ( aSnapImageSource > CCaeEngine::ESnapImageSourceThumbnail ) )
        {
        LOGTEXT( _L( "Cae: CCaeStillStatesActive::SetSnapImageSourceL(). leaving KErrNotSupported" ));
        User::Leave( KErrNotSupported );
        }

    iSnapImageSource = aSnapImageSource;
    }



// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetSnapImageSizeL()
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetSnapImageSizeL( 
    TSize& aSize )
    {
    LOGTEXT3( _L( "Cae: CCaeStillStatesActive::SetSnapImageSizeL(). iWidth=%d, iHeight=%d" ), aSize.iWidth, aSize.iHeight );
	// Calc downscaled size.
    // Use always the default snap size for the snap image. It is the best compromise 
    // between memory usage and image quality.
    TSize stepwiseScaledOptimalSize( KCaeDefaultSnapWidth, KCaeDefaultSnapHeight);
    CalcDownscaledSnapSize( stepwiseScaledOptimalSize ); 

    // Empty calculated size means that PrepareStillCapture() has not been called.
    if ( stepwiseScaledOptimalSize == TSize() ) 
        {
        LOGTEXT( _L( "Cae: CCaeStillStatesActive::SetSnapImageSizeL(). leaving KErrNotReady (no prepare)" ));
		User::Leave( KErrNotReady ); 
        }
    else
        {
        iOptimalSnapImageSize = stepwiseScaledOptimalSize;
        // If empty, use the still image size
        if ( aSize == TSize() )
            {
            aSize = iStillFrameSize;
            }
        iSnapImageSize = aSize;
        }
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SnapImageSize
// -----------------------------------------------------------------------------
TSize CCaeStillStatesActive::SnapImageSize() const
    {
    return( iSnapImageSize );
    }

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::StillImageSize
// -----------------------------------------------------------------------------
TSize CCaeStillStatesActive::StillImageSize() const
    {
    return( iStillFrameSize );
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetSnapImageColorMode
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetSnapImageColorMode( 
    TDisplayMode aMode )
    {
    iSnapImageColorMode = aMode;
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetJpegQuality
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetJpegQuality(
    TInt aQuality )
    {
    iStillCompressionQuality = aQuality;
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::JpegQuality
// -----------------------------------------------------------------------------
//
TInt CCaeStillStatesActive::JpegQuality() const
    {
    return( iStillCompressionQuality );
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetImageCodecsL
// Sets the specific image codec implementation to be used in decoding and 
// encoding.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetImageCodecsL( 
	TUid aDecoderUid, TUid aEncoderUid )
    {
    iStillEncoder->SetImageCodecL( aEncoderUid );
    iStillDecoder->SetImageCodecL( aDecoderUid );
    }

        
// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetViewFinderMode
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetViewFinderMode(
    TBool aIsEnabled )
    {
    iIsViewFinderEnabled = aIsEnabled;

    // Issue event, if the view finder is stopped, and the state machine is 
    // waiting for it. Do not issue the same event twice (it will panic).
    if ( !iIsViewFinderEnabled 
         && ( iCurrentState == CCaeStillStatesActive::ECaeStateBurstWaitingForViewFinder ) 
         && !( IsActive() && ( iStatus == CCaeStillStatesActive::ECaeEventViewFinderForBurstReady ) ) )
        {
        Event( CCaeStillStatesActive::ECaeEventViewFinderForBurstReady );        
        }
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::IsRunning
// Check if the state is other than ECaeStateNone.
// -----------------------------------------------------------------------------
//
TBool CCaeStillStatesActive::IsRunning() const
    {
    return( iCurrentState != CCaeStillStatesActive::ECaeStateNone );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::RegisterFlags
// Increment reference counters.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::RegisterFlags( TUint32 aFlags )
	{
    if ( aFlags & ECaeExtFlagRequireFullColorSnapInputImage )
        { 
        CAE_ASSERT_DEBUG( iRequireFullColorSnapInputImageRefCount < KMaxTInt32 );
        if ( iRequireFullColorSnapInputImageRefCount < KMaxTInt32 )
            {
            iRequireFullColorSnapInputImageRefCount++;
            }
        }
	}


// -----------------------------------------------------------------------------
// CCaeEngineImp::DeregisterFlags
// Decrement reference counters.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DeregisterFlags( TUint32 aFlags )
    {
    if ( aFlags & ECaeExtFlagRequireFullColorSnapInputImage )
        {
        CAE_ASSERT_DEBUG( iRequireFullColorSnapInputImageRefCount > 0 );
        if ( iRequireFullColorSnapInputImageRefCount > 0 )
            {
            iRequireFullColorSnapInputImageRefCount--;
            }
        }
    }

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::PrepareStillCaptureL
// Using cropping only if supported by Camera API impl.
// Calls RetrieveStillSizeIndex() that calls Camera API EnumerateCaptureSizes().
// Calls Camera API PrepareImageCaptureL().
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::PrepareStillCaptureL( 
    const TSize&      aFrameSize, 
    CCamera::TFormat  aFormat, 
    const TRect&      aCropRect,
    TSize&            aSnapSize )
    {
    LOGTEXT3( _L( "Cae: CCaeStillStatesActive::PrepareStillCaptureL() entering aFrameSize=(w=%d,h=%d)" ), aFrameSize.iWidth, aFrameSize.iHeight );

    // Handle empty crop rect
    TRect cropRect( aCropRect );
    if ( cropRect.IsEmpty() )
        cropRect = TRect(TPoint(0, 0 ), aFrameSize);

    CCamera::TFormat stillOutputFormat = aFormat;
    CCamera::TFormat stillInputFormat = aFormat;

    // Try if this size and format is supported by the Camera API.
    TInt sizeIndex = RetrieveStillSizeIndex( aFrameSize, aFormat );
    if ( sizeIndex != -1 )
        {
        stillInputFormat = aFormat;
        }
    // If the application requires Jpeg we can encode it from bitmap
    else if ( aFormat == CCamera::EFormatJpeg )
        {
        // Try if this size and format KBaseStillInputFormat1 
        // is supported by the Camera API.
        stillInputFormat = KBaseStillInputFormat1;
        sizeIndex = RetrieveStillSizeIndex( aFrameSize, stillInputFormat );
        if ( sizeIndex == -1 )
            {
            // Try if this size and format KBaseStillInputFormat2 
            // is supported by the Camera API.
            stillInputFormat = KBaseStillInputFormat2;
            sizeIndex = RetrieveStillSizeIndex( aFrameSize, stillInputFormat );
            if ( sizeIndex == -1 )
                {
                LOGTEXT2( _L( "Cae: CCaeStillStatesActive::PrepareStillCaptureL() leaving KErrNotSupported, stillInputFormat=%d" ), stillInputFormat );
                User::Leave( KErrNotSupported );
                }
            }
        }
    else 
        {
        LOGTEXT2( _L( "Cae: CCaeStillStatesActive::PrepareStillCaptureL() leaving KErrNotSupported, aFormat=%d" ), aFormat );
        User::Leave( KErrNotSupported );
        }

    // Prepare still image capturing on Camera API.
    if ( !( iInfo.iOptionsSupported & TCameraInfo::EImageClippingSupported ) ) 
        {
        iCamera.PrepareImageCaptureL( stillInputFormat, sizeIndex );
        }
    else 
        {
        iCamera.PrepareImageCaptureL( stillInputFormat, sizeIndex, cropRect );
        }

    // Store values 
    iStillFrameSize = aFrameSize;
    iStillInputFormat = stillInputFormat;
    iStillOutputFormat = stillOutputFormat;

    // Calc downscaled size if needed
    SetSnapImageSizeL( aSnapSize );     
    
    // Prepare done
    iStillPrepared = ETrue;
    
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::PrepareStillCaptureL() returning" ) );    
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::RetrieveStillSizeIndex
// Retrieves/fetches still image size index from Camera API.
// -----------------------------------------------------------------------------
//
TInt CCaeStillStatesActive::RetrieveStillSizeIndex( 
    const TSize& aFrameSize, 
    CCamera::TFormat aStillDataFormat ) const
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::RetrieveStillSizeIndex() entering" ) );

    TInt index( -1 );
    TInt i( 0 );
    TSize size;
    while ( ( i < iInfo.iNumImageSizesSupported ) && ( index == -1 ) )
        {
        iCamera.EnumerateCaptureSizes ( size, i, aStillDataFormat );
        if ( size == aFrameSize )
            {
            index = i;
            }
        i++;
        }

    LOGTEXT( _L( "Cae: CCaeStillStatesActive::RetrieveStillSizeIndex() returning" ) );

    return index;
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::CalcDownscaledSnapSize
// Calc suitable downscaled snap size. Assumed that integer arithmetic is used 
// also in CImageDecoder class for calculating image scaling to 1/2, 1/4 or 1/8 
// of size (i.e. no rounding). The scaled snap size is not allowed to be smaller
// than given snap size in any dimension. Do not do downscaling if aSnapSize is 
// empty.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::CalcDownscaledSnapSize(
    TSize& aSnapSize) const
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::CalcDownscaledSnapSize() entering" ) );    

    // The new snap image size is equal to frame size by default
    TSize newSnapSize = iStillFrameSize;

    // Calculate downscaled snap size
    if ( aSnapSize != TSize( 0, 0 ) 
        && ( ( iStillInputFormat == CCamera::EFormatExif ) || 
             ( iStillInputFormat == CCamera::EFormatJpeg )   ) )
        {
        // Try scaling to 1/8, 1/4 and 1/2
        for  ( TInt divider = 8; divider >= 2;  divider /= 2 ) 
            {
            TInt scaledWidth = iStillFrameSize.iWidth / divider;
            TInt scaledHeight = iStillFrameSize.iHeight / divider;
            if ( scaledHeight >= aSnapSize.iHeight && scaledWidth >= aSnapSize.iWidth ) 
                {
                newSnapSize = TSize( scaledWidth, scaledHeight );
                divider = -1; // exit loop
                }
            }
        }

    // Return new size
    aSnapSize = newSnapSize;
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::CalcDownscaledSnapSize() returning" ) );    
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::GetThumbnailL()
// Extracts thumbnail image and its size from Exif image.
// Uses Exif reader to extract the thumbnail and 
// ICL decoder to extract the size.
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::GetThumbnailL( 
    HBufC8*& aReadThumbnail, 
    TSize&   aThumbnailSize )
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::GetThumbnailL()" ) );    

    if ( iImageData && ( iStillOutputFormat == CCamera::EFormatExif ) )
        {
        // Instantiate exif reader.
        CExifRead* read = NULL;
        read = CExifRead::NewL( *iImageData );
        CleanupStack::PushL( read );
        
        // Get thumbnail.
        aReadThumbnail = read->GetThumbnailL();
        CleanupStack::PopAndDestroy( read );
        
        // Find out thumbnail size by using ICL decoder.
        // Check first that the size is valid.
        CImageDecoder* decoder = CImageDecoder::DataNewL( iFs, *aReadThumbnail ); 
        TFrameInfo frameInfo = decoder->FrameInfo();
        delete decoder;

        if ( frameInfo.iOverallSizeInPixels.iWidth > 0 && 
             frameInfo.iOverallSizeInPixels.iHeight > 0 )
            {
            aThumbnailSize = frameInfo.iOverallSizeInPixels;
            LOGTEXT3( _L( "Cae: CCaeStillStatesActive::GetThumbnailL(): extracted %dx%d thumbnail" ), 
                aThumbnailSize.iWidth, aThumbnailSize.iHeight );
            }
        else
            {
            LOGTEXT3(_L("Cae: CCaeStillStatesActive::GetThumbnailL(): leaving KErrCorrupt due invalid thumbnail size=(w=%d,h=%d)"), 
                frameInfo.iOverallSizeInPixels.iWidth, frameInfo.iOverallSizeInPixels.iHeight );
            delete aReadThumbnail;
            aReadThumbnail = NULL;
            aThumbnailSize = TSize( 0, 0 );
            User::Leave( KErrCorrupt );
            }
        }
    else if ( iImageData && ( iStillOutputFormat != CCamera::EFormatExif ) )
        {
        // we can't get the thumbnail from exif header if the exif header doesn't exist
        LOGTEXT( _L( "Cae: CCaeStillStatesActive::GetThumbnailL() leaving because iStillOutputFormat is not EFormatExif" ) );
        User::Leave( KErrNotFound );
        }
    }

// ---------------------------------------------------------------------------
// CCaeStillStatesActive::IsBitmapOutput()
// ---------------------------------------------------------------------------
//
TBool CCaeStillStatesActive::IsBitmapOutput()
    {
    return ( !( ( iStillOutputFormat == CCamera::EFormatJpeg ) 
              || ( iStillOutputFormat == CCamera::EFormatExif ) ) );
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::Event()
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::Event( TCaeEvent aEvent, TTimeIntervalMicroSeconds32 aDelay )
    {
    LOGTEXT2( _L( "Cae: CCaeStillStatesActive::Event(). aEvent=%d" ), aEvent );
    CAE_ASSERT_ALWAYS( !IsActive() );

    iDelayedEvent = CCaeStillStatesActive::ECaeEventNone;

    if ( aEvent != CCaeStillStatesActive::ECaeEventEnd )
        {
        if ( aDelay == TTimeIntervalMicroSeconds32( 0 ) )
            {
            // Issue request
            TRequestStatus* statusPtr = &iStatus;
            User::RequestComplete( statusPtr, aEvent );
            }
        else
            {
            // Use delayed request
            iDelayedCallTimer.After( iStatus, aDelay );
            iDelayedEvent = aEvent;
            }
    	SetActive();
        }
    else
        {
        iCurrentState = CCaeStillStatesActive::ECaeStateNone; // The end
        if ( ExtModeActive() )
            {
            // If there are no images in the queue, then extension processing is ended
            TInt count = iImageQueueExtPro->ImageCount();
            if ( count > 0 )
                {  // Continue and read next image from the queue
                Event( CCaeStillStatesActive::ECaeEventImageQueueExtPro );
                }
            }
        }
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::CancelCaptureStill()
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::CancelCaptureStill()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::CancelCaptureStill()" ) );

    if ( !ExtModeActive() ) // in extension mode capture/cancelling is not possible
        {
        iStillCancelled = ETrue;
        Cancel();
        CancelAndCleanup();
        }

    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::Cancel()
// Calls the base class Cancel().
// As calling this is not allowed outside this class, this is a private method
// and it overrides CActive::Cancel(). 
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::Cancel()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::Cancel()" ) );
    CActive::Cancel();
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::DoCancel()
// Cancel all pending actions. This does not cancel actions that use callback
// for signaling for completion, e.g. image encoding and decoding.
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoCancel()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoCancel()" ) );
    if ( iStatus == KRequestPending )
        {
        if ( ( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingCapturedImage )
             || ( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingSnapImage ) 
             || ( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingStillImage ) )
            {
            // Cancel extension processing            
            CAE_ASSERT_ALWAYS( iCurrentExtensionIndex != -1 );
            TAny* interfacePtr = iProcessImageImplList[iCurrentExtensionIndex].iImplPtr;
	        STATIC_CAST( MCaeExtProcessImageInterface*, interfacePtr )->CaeExtensionCancel();   
            }
        else if ( iDelayedEvent != CCaeStillStatesActive::ECaeEventNone )
            {
            // Cancel timer
            iDelayedCallTimer.Cancel();
            iDelayedEvent = CCaeStillStatesActive::ECaeEventNone;
            }
        else
            {
            User::Panic( KCaePanicText, ECaePanicInvalidState );
            }
        }
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::ErrorRecovery
//
// For still capture recovery: Delete images, cancel and cleanup. Client 
// callbacks should be called also in error cases, except when user has 
// cancelled the action. 
//
// Burst case is handled differently in BurstErrorRecovery().
//
// Note: If ownership is transferred, the image pointers should be NULLed before
// calling the observer method. That is because the observer method can call back
// CancelStill() which tries to delete images. That is no allowed if an image 
// should be owned by the observer.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::ErrorRecovery( TInt aError )
    {
    LOGTEXT2( _L( "Cae: CCaeStillStatesActive::ErrorRecovery(): %d" ), aError );

    // Check if snap image callback should be called. Do not call it twice.

    if ( ( iCurrentState <= CCaeStillStatesActive::ECaeStateDeliveringSnapImage ) // Callback not yet called
         && !iStillCancelled 
         && iCreateSnapImage ) 
        {
        if ( iCurrentState == CCaeStillStatesActive::ECaeStateDecodingCapturedImageToBitmap ) 
            {
            // Give decoded image to the client even if there is decoding error
            iCaeObserver->McaeoSnapImageReady( *iBitmap, aError );

            // Not needed any more. Delete now to free memory.
            delete iBitmap;
            iBitmap = NULL;
            }
        else
            {
            // Give empty snap image to the client
            iCaeObserver->McaeoSnapImageReady( *iEmptySnapImage, aError );
            }
        }

    // Check if still image callback should be called. Do not call it twice.

    if ( ( iCurrentState <= CCaeStillStatesActive::ECaeStateDeliveringStillImage ) // Callback not yet called
         && !iStillCancelled )
        {
        if ( iCurrentState == CCaeStillStatesActive::ECaeStateDecodingCapturedImageToBitmap ) 
            {
            // Give original image to the client even if there is decoding error.
            HBufC8* tmpImageData = iImageData;   
            iImageData = NULL; 
            iCaeObserver->McaeoStillImageReady( NULL, tmpImageData, aError );
            }
        else if ( iCurrentState == CCaeStillStatesActive::ECaeStateEncodingToJpeg )
            {
            // Give original image to the client even if there is encoding error
            HBufC8* tmpImageData = iImageData;   
            iImageData = NULL; // Ownership tranferred
            iCaeObserver->McaeoStillImageReady( NULL, tmpImageData, aError );            
            }
        else
            {
            // Give error to the client
            iCaeObserver->McaeoStillImageReady( NULL, NULL, aError );
            }

        // In burst case, increment delivery counter. 
        if ( iStillBurst )
            {
            iCountOfDeliveredBurstImages++;
            }
        }

    if ( !iStillCancelled )
        {
        // Handle still burst
        if ( iStillBurst )
            {
            BurstErrorRecovery( aError );
            }
        else
            {
            Cancel();
            CancelAndCleanup();
            }  
        }
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::CancelAndCleanup
// Cancel all actions and free resources.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::CancelAndCleanup()
    {
    iDelayedEvent = CCaeStillStatesActive::ECaeEventNone;

    CompleteStillBurst();

    if ( iStillEncoder )
        {
        iStillEncoder->Cancel();
        iStillEncoder->Cleanup(); 
        }

    if ( iStillDecoder )
        {
        iStillDecoder->Cancel();
        iStillDecoder->Cleanup();
        }    

    // Delete all that is not NULL already. 
    delete iSnapBitmap;
    iSnapBitmap = NULL;
    delete iBitmap;
    iBitmap = NULL;
    delete iImageData;
    iImageData = NULL;
    delete iImageHeaderData;
    iImageHeaderData = NULL;
    if ( iImageQueueExtPro )
        {
        iImageQueueExtPro->ResetAndDestroyImages();
        }    

    iCurrentState = CCaeStillStatesActive::ECaeStateNone;   
    }

// ---------------------------------------------------------------------------
// CCaeStillStatesActive::RunL()
// The main function that receives all events. Simple events are also handled 
// here. For more complex events, specific handler functions are called.
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::RunL()
    {
    LOGTEXT3( _L( "Cae: CCaeStillStatesActive::RunL() entering, iStatus=%d, iCurrentState=%d" ), iStatus.Int(), iCurrentState );

    // Handle external errors 

    if ( iStatus.Int() < 0 )
        {
        HandleExternalError();
        return;
        }

    // Handle all state transitions

    switch( iStatus.Int() )
        {
        case CCaeStillStatesActive::ECaeEventNone:
            HandleExternalEvent();
            break;

        case CCaeStillStatesActive::ECaeEventStartStillCapture:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateNone );
            HandleStart();
            break;

        case CCaeStillStatesActive::ECaeEventImageCaptureReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateCapturing );
            HandleImageCaptureReady();
            break;

        case CCaeStillStatesActive::ECaeEventBurstImageCaptureReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateCapturingBurst );
            iCurrentState = CCaeStillStatesActive::ECaeStateAppendingBurstImage;            
            DoAppendCapturedBurstImageToArray();
            break;

        case CCaeStillStatesActive::ECaeEventAppendCapturedBurstImageReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateAppendingBurstImage );
            HandleAppendCapturedBurstImageReady();            
            break;

        case CCaeStillStatesActive::ECaeEventViewFinderForBurstReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateBurstWaitingForViewFinder );
            iCurrentState = CCaeStillStatesActive::ECaeStateCapturingBurst;            
            DoCaptureStillBurstImage();            
            break;

        case CCaeStillStatesActive::ECaeEventBurstCaptureReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateCapturingBurst );
            iCurrentState = CCaeStillStatesActive::ECaeStateFetchingNextBurstImage;    
            iStatus = CCaeStillStatesActive::ECaeEventNone;
            DoFetchNextBurstImage();
            break;

        case CCaeStillStatesActive::ECaeEventBurstImageFetchReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateFetchingNextBurstImage );
            HandleBurstImageFetchReady();
            break;

        case CCaeStillStatesActive::ECaeEventDecodeToBitmapReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateDecodingCapturedImageToBitmap );
            HandleDecodeToBitmapReady();
            break;

        case CCaeStillStatesActive::ECaeEventExtractExifMetaDataReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateExtractingExifMetaData );
            iCurrentState = CCaeStillStatesActive::ECaeStateExtensionsProcessingCapturedImage;         
            iStatus = CCaeStillStatesActive::ECaeEventNone;
            DoExtensionsProcessCapturedImage();    
            break;

        case CCaeStillStatesActive::ECaeEventProcessCapturedImageStepReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingCapturedImage );
            iStatus = CCaeStillStatesActive::ECaeEventNone;
            DoExtensionsProcessCapturedImage();
            break;

        case CCaeStillStatesActive::ECaeEventProcessCapturedImageAllReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingCapturedImage );
            HandleProcessCapturedImageAllReady();
            break;

        case CCaeStillStatesActive::ECaeEventProcessSnapImageStepReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingSnapImage );
            iStatus = CCaeStillStatesActive::ECaeEventNone;
            DoExtensionsProcessSnapImage();    
            break;

        case CCaeStillStatesActive::ECaeEventProcessSnapImageAllReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingSnapImage );
            iCurrentState = CCaeStillStatesActive::ECaeStateDeliveringSnapImage;         
            iStatus = CCaeStillStatesActive::ECaeEventNone;
            DoDeliverSnapImage();    
            break;

        case CCaeStillStatesActive::ECaeEventDeliverSnapImageReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateDeliveringSnapImage );
            iCurrentState = CCaeStillStatesActive::ECaeStateExtensionsProcessingStillImage;         
            iStatus = CCaeStillStatesActive::ECaeEventNone;
            DoExtensionsProcessStillImage();    
            break;

        case CCaeStillStatesActive::ECaeEventProcessStillImageStepReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingStillImage );
            iStatus = CCaeStillStatesActive::ECaeEventNone;
            DoExtensionsProcessStillImage();
            break;

        case CCaeStillStatesActive::ECaeEventProcessStillImageAllReady:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingStillImage );
            HandleProcessStillImageAllReady();
            break;

        case CCaeStillStatesActive::ECaeEventEncodeToJpegReady:
             CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateEncodingToJpeg );
            HandleEncodeToJpegReady();
            break;

        case CCaeStillStatesActive::ECaeEventDeliverStillBurstImageReady:
            // We can end up here from any state, if there occurs an error
            HandleDeliverStillBurstImageReady();
            break;

        case CCaeStillStatesActive::ECaeEventImageQueueExtPro:
            CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateNone );
            HandleImageQueueExtPro();
            break;

        default:
            CAE_ASSERT_DEBUG( iCurrentState != iCurrentState ); // Always false
            break;
        }

    LOGTEXT( _L( "Cae: CCaeStillStatesActive::RunL() returning" ) );
    }


// ---------------------------------------------------------------------------
// EVENT HANDLERS
//
// Event handlers should decide what is the next state and next action after the 
// current event. Modes and flags are checked here leaving the actual action 
// functions as simple as possible. 
// ---------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::HandleExternalError
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleExternalError() 
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleExternalError()" ));

    CAE_ASSERT_DEBUG( ( ( iDelayedEvent != CCaeStillStatesActive::ECaeEventNone )
                       || ( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingCapturedImage ) 
                       || ( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingSnapImage ) 
                       || ( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingStillImage ) ) ); 
         
    // Cancelling delayed event timer is not considered as error, but early completion              
    if ( (iStatus.Int() == KErrCancel ) && ( iDelayedEvent != CCaeStillStatesActive::ECaeEventNone ) )
        {
    	LOGTEXT2( _L( "Cae: CCaeStillStatesActive::HandleExternalError(). Delayed event %d completed early." ), iDelayedEvent);
    	HandleExternalEvent();
        }
    else
    	{
    	ErrorRecovery( iStatus.Int() );                        		
    	}    

    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::HandleExternalEvent
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleExternalEvent() 
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleExternalEvent()" ));
    CAE_ASSERT_DEBUG( ( ( iDelayedEvent != CCaeStillStatesActive::ECaeEventNone )
                       || ( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingCapturedImage ) 
                       || ( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingSnapImage ) 
                       || ( iCurrentState == CCaeStillStatesActive::ECaeStateExtensionsProcessingStillImage ) ) ); 

        
    if ( iDelayedEvent != CCaeStillStatesActive::ECaeEventNone )
        {
        // Timer has finished, issue delayed event now
        Event( iDelayedEvent );
        }
    else          
        {
        switch ( iCurrentState )
            {
            case ECaeStateExtensionsProcessingCapturedImage:
                Event( CCaeStillStatesActive::ECaeEventProcessCapturedImageStepReady );
                break;

            case ECaeStateExtensionsProcessingSnapImage:
                Event( CCaeStillStatesActive::ECaeEventProcessSnapImageStepReady );
                break;

            case ECaeStateExtensionsProcessingStillImage:
                Event( CCaeStillStatesActive::ECaeEventProcessStillImageStepReady );
                break;

            default:
                CAE_ASSERT_DEBUG( iCurrentState !=  iCurrentState ); // Always false
                break;

            }
        }
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::HandleStart()
// Start either single or burst image capture.
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleStart()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleStart()" ));
    CAE_ASSERT_ALWAYS( ( iBitmap == NULL ) && ( iSnapBitmap == NULL ) );
    CAE_ASSERT_ALWAYS( ( iImageData == NULL ) && ( iImageHeaderData == NULL ) );
    
    iStatus = CCaeStillStatesActive::ECaeEventNone;
    if ( iStillBurst )
        {               
        iCurrentState = CCaeStillStatesActive::ECaeStateCapturingBurst;            
        DoCaptureStillBurst();
        }
    else
        {
        iCurrentState = CCaeStillStatesActive::ECaeStateCapturing;            
        DoCaptureStill();
        }   
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::HandleImageCaptureReady()
// Decode the captured image, extract Exif metadata, or process the 
// image by the extensions.  
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleImageCaptureReady()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleImageCaptureReady()" ));

    iStatus = CCaeStillStatesActive::ECaeEventNone;

    if ( !iBitmap                   // Does not already exist
         && ( iCreateSnapImage ) )    // Snap bitmap required by UI
            /* Not yet implemented
            || iRequireStillImageAsBitmapRefCount ) ) // Still bitmap required by any extensions
            */
        {
        iCurrentState = CCaeStillStatesActive::ECaeStateDecodingCapturedImageToBitmap;            
        DoDecodeCapturedImageToBitmap();
        }
    else
        {
        /* Not yet implemented
        if ( iRequireImageFormatHeaderRefCount )
            {
            iCurrentState = CCaeStillStatesActive::ECaeStateExtractingExifMetaData;            
            DoExtractExifMetaData();
           }
        else
        */
            {
            iCurrentState = CCaeStillStatesActive::ECaeStateExtensionsProcessingCapturedImage;            
            DoExtensionsProcessCapturedImage();    
            }
        }
    }

// ---------------------------------------------------------------------------
// CCaeStillStatesActive::HandleDecodeToBitmapReady()
// Extract Exif metadata, or process the captured image by the 
// extensions.
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleDecodeToBitmapReady()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleDecodeToBitmapReady()" ));
    CAE_ASSERT_ALWAYS( ( iBitmap != NULL ) && ( iSnapBitmap == NULL ) );
    CAE_ASSERT_ALWAYS( ( iImageData != NULL ) && ( iImageHeaderData == NULL ) );

    iStatus = CCaeStillStatesActive::ECaeEventNone;

    if ( iCreateSnapImage )    // Snap bitmap required by UI
        /* Not yet implemented
       && !iRequireStillImageAsBitmapRefCount ) // Still bitmap not required by any extensions
       */
        {
        // Decoded bitmap is used for the snap bitmap only
        iSnapBitmap = iBitmap;
        iBitmap = NULL;
        }    

     /* Not yet implemented
    if ( iRequireImageFormatHeaderRefCount )
        {
        iCurrentState = CCaeStillStatesActive::ECaeStateExtractingExifMetaData;            
        DoExtractExifMetaData();
        }
    else
    */
        {
        iCurrentState = CCaeStillStatesActive::ECaeStateExtensionsProcessingCapturedImage;            
        DoExtensionsProcessCapturedImage();    
        }
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::HandleProcessCapturedImageAllReady()
// Process the snap image or the still image by the extensions.
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleProcessCapturedImageAllReady()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleProcessCapturedImageAllReady()" ));

    iStatus = CCaeStillStatesActive::ECaeEventNone;
    if ( iCreateSnapImage )
        {
        iCurrentState = CCaeStillStatesActive::ECaeStateExtensionsProcessingSnapImage;            
        DoExtensionsProcessSnapImage();    
        }
    else
        {
        iCurrentState = CCaeStillStatesActive::ECaeStateExtensionsProcessingStillImage;            
        DoExtensionsProcessStillImage();
        }
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::HandleProcessStillImageAllReady()
// Encode the still bitmap to Jpeg, or deliver the 
// still image to the client.
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleProcessStillImageAllReady()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleProcessStillImageAllReady()" ));

    iStatus = CCaeStillStatesActive::ECaeEventNone;

    // Encode to Jpeg if there is a separate (Exif) header or bitmap to 
    // Jpeg coversion is needed for client.
    if ( iImageHeaderData 
         || ( ( iStillOutputFormat == CCamera::EFormatJpeg ) && !iImageData ) )
        {
        iCurrentState = CCaeStillStatesActive::ECaeStateEncodingToJpeg;            
        DoEncodeStillImageToJpeg();
        }
   else
        {
        if ( iStillBurst )
            {
            iCurrentState = CCaeStillStatesActive::ECaeStateDeliveringStillBurstImage;            
            DoDeliverStillBurstImage();
            }
        else
            {
            iCurrentState = CCaeStillStatesActive::ECaeStateDeliveringStillImage;            
            DoDeliverStillImage();
            }
        }
    }
    

// ---------------------------------------------------------------------------
// CCaeStillStatesActive::HandleEncodeToJpegReady()
// Deliver the still image to the client.
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleEncodeToJpegReady()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleEncodeToJpegReady()" ));

    iStatus = CCaeStillStatesActive::ECaeEventNone;
    if ( iStillBurst )
        {
        iCurrentState = CCaeStillStatesActive::ECaeStateDeliveringStillBurstImage;            
        DoDeliverStillBurstImage();
        }
    else
        {
        iCurrentState = CCaeStillStatesActive::ECaeStateDeliveringStillImage;            
        DoDeliverStillImage();
        }
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::HandleImageQueueExtPro()
// Deliver the still image to the client.
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleImageQueueExtPro()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleImageQueueExtPro()" ));

    CFbsBitmap* bitmap = NULL;
    HBufC8*     imagedata = NULL;
    TBool       lastimage = EFalse;
    TBool       snapimage = EFalse;

    // Get next snap or main image from image queue
    if ( iImageQueueExtPro )
        {
        TInt result = iImageQueueExtPro->GetNextImage( bitmap, imagedata, lastimage, snapimage );

        // If prepare has not been done then format has default value
        if ( !iStillPrepared )
            {
            iStillOutputFormat = CCamera::EFormatExif; // Only exif/jpeg is supported
            }
        if ( result == KErrNone )
            {
            iLastImageExtPro = lastimage;
            if ( bitmap && snapimage ) // Snap image
                {
                LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleImageQueueExtPro() Snap bitmap" ));
                iStatus = CCaeStillStatesActive::ECaeEventNone;
                iCurrentState = CCaeStillStatesActive::ECaeStateExtensionsProcessingSnapImage;            
                iSnapBitmap = bitmap;
                Event( CCaeStillStatesActive::ECaeEventProcessSnapImageStepReady );
                }
            else if ( imagedata && !snapimage) // Main captured image (jpeg)
                {
                LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleImageQueueExtPro() JPG imagedata" ));
                iStatus = CCaeStillStatesActive::ECaeEventNone;
                iCurrentState = CCaeStillStatesActive::ECaeStateExtensionsProcessingStillImage;            
                iImageData = imagedata;
                Event( CCaeStillStatesActive::ECaeEventProcessStillImageStepReady );
                }
            else if ( bitmap && !snapimage) // Main captured image (bitmap)
                {
                LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleImageQueueExtPro() bitmap imagedata" ));
                iStatus = CCaeStillStatesActive::ECaeEventNone;
                iCurrentState = CCaeStillStatesActive::ECaeStateDeliveringSnapImage;            
                iBitmap = bitmap;
                Event( CCaeStillStatesActive::ECaeEventDeliverSnapImageReady );
                }
            else 
                {
                LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleImageQueueExtPro() iImageQueueExtPro return empty image" ));
                }
            }
        else
            {
            LOGTEXT2( _L( "Cae: CCaeStillStatesActive::HandleImageQueueExtPro() iImageQueueExtPro error %d" ), result);
            }
        }
    else
        {
        LOGTEXT( _L( "Cae: CCaeStillStatesActive::HandleImageQueueExtPro() iImageQueueExtPro not found" ));
        }

    }


// ---------------------------------------------------------------------------
// EXTERNAL CALLBACKS
//
// Callbacks from external objects are mapped to the internal events.
// ---------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::ImageReady
// Store the captured image and send event to the state machine.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::ImageReady( 
    CFbsBitmap* aBitmap, 
    HBufC8*     aImageData, 
    TInt        aError )
    {
    LOGTEXT2( _L( "Cae: CCaeStillStatesActive::ImageReady() entering, aError=%d" ), aError );
    CAE_ASSERT_ALWAYS( ( iBitmap == NULL ) && ( iSnapBitmap == NULL ) );
    CAE_ASSERT_ALWAYS( ( iImageData == NULL ) && ( iImageHeaderData == NULL ) );
    
    #ifdef _DEBUG
    // Performance debugging
    iImageReadyTime.HomeTime();
    TTimeIntervalMicroSeconds convertInterval =
        iImageReadyTime.MicroSecondsFrom( iCaptureStartTime );
    LOGTEXT2( _L( "Cae: CCaeStillStatesActive::ImageReady(): Captured image ready %f seconds after capture" ),
                   I64LOW( convertInterval.Int64() ) * 1.0 / KOneSecond );
    #endif

    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaeImageReadyError( aError );
    #endif    

    // Get ownership and store pointers
    iBitmap = aBitmap;
    iImageData = aImageData;

    if ( iStillBurst )
        {
        iFirstStillBurstError =  iFirstStillBurstError ? iFirstStillBurstError : aError;
        Event( CCaeStillStatesActive::ECaeEventBurstImageCaptureReady );
        }

    // Single image capture
    else if ( !aError )
        {
        Event( CCaeStillStatesActive::ECaeEventImageCaptureReady );
        }
    else
        {
        // Handle error
        ErrorRecovery( aError );
        }

    LOGTEXT( _L( "Cae: CCaeStillStatesActive::ImageReady() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::McaesdoCFbsBitmapImageReady
// Store original and decoded images and send event to the state machine.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::McaesdoCFbsBitmapImageReady( 
    HBufC8*     aImageData, 
    CFbsBitmap* aBitmap, 
    TInt        aError,
    TInt        /*aImageSize*/ ) // Not yet used
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::McaesdoCFbsBitmapImageReady() entering" ) );
    CAE_ASSERT_ALWAYS( ( iBitmap == NULL ) && ( iSnapBitmap == NULL ) );
    CAE_ASSERT_ALWAYS( iImageHeaderData == NULL );
    CAE_ASSERT_ALWAYS( !iStillCancelled );

   #ifdef _DEBUG
    // Performance debugging
    iFinalImageReadyTime.HomeTime();
    TTimeIntervalMicroSeconds convertInterval =
        iFinalImageReadyTime.MicroSecondsFrom( iCaptureStartTime );
    LOGTEXT2( _L( "Cae: CCaeEngineImp::McaesdoCFbsBitmapImageReady(): Final decoded image ready %f seconds after capture" ),
                   I64LOW( convertInterval.Int64() ) * 1.0 / KOneSecond );
    #endif

    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaeMcaesdoCFbsBitmapImageReadyError( aError );
    #endif
    
    // Get ownership and store pointers
    iBitmap = aBitmap;

    // Delete image data if not the original (extracted thumbnail was used as decoder input).
    if ( iImageData != NULL )
        {
        delete ( aImageData ); // Delete (exif) thumbnail source image
        }
    else
        {
        iImageData = aImageData; // Store original still image
        }

    if ( !aError )
        {
        Event( CCaeStillStatesActive::ECaeEventDecodeToBitmapReady );
        }
    else
        {
        // Handle error
        ErrorRecovery( aError );
        }

    LOGTEXT( _L( "Cae: CCaeStillStatesActive::McaesdoCFbsBitmapImageReady() returning" ) );
    }
    

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::McaeseoHBufC8ImageReady
// Store original and encoded images and send event to the state machine.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::McaeseoHBufC8ImageReady( 
    CFbsBitmap* aBitmap, 
    HBufC8*     aImageData, 
    TInt        aError,
    TInt        /*aImageSize*/ )
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::McaeseoHBufC8ImageReady() entering" ) );
    CAE_ASSERT_ALWAYS( ( iBitmap == NULL ) && ( iSnapBitmap == NULL ) );
    CAE_ASSERT_ALWAYS( ( iImageData == NULL ) && ( iImageHeaderData == NULL ) );
    CAE_ASSERT_ALWAYS( !iStillCancelled );
    
    #ifdef _DEBUG
    // Performance debugging
    iFinalImageReadyTime.HomeTime();
    TTimeIntervalMicroSeconds convertInterval =
        iFinalImageReadyTime.MicroSecondsFrom( iCaptureStartTime );
    LOGTEXT2(_L( "Cae: CCaeStillStatesActive::McaeseoHBufC8ImageReady(): Final encoded image ready %f seconds after capture" ), 
        I64LOW( convertInterval.Int64() ) * 1.0 / KOneSecond);
    #endif

    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaeMcaeseoHBufC8ImageReadyError( aError );
    #endif
    
    // Get ownership and store pointers
    iBitmap = aBitmap;
    iImageData = aImageData;

    if ( !aError )
        {
        Event( CCaeStillStatesActive::ECaeEventEncodeToJpegReady );
        }
    else 
        {
        // Handle error
        ErrorRecovery( aError );
        }

    LOGTEXT( _L( "Cae: CCaeStillStatesActive::McaeseoHBufC8ImageReady() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::ViewFinderFrameReady
// Send event if the state machine is waiting for the view finder. 
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::ViewFinderFrameReady()
    {
	iViewFinderFrameReceived = ETrue;

    if ( iCurrentState == CCaeStillStatesActive::ECaeStateBurstWaitingForViewFinder 
         && !IsActive() ) // To make sure the previous ECaeEventViewFinderForBurstReady has been handled
        {
        Event( CCaeStillStatesActive::ECaeEventViewFinderForBurstReady );        
        }
    }


// ---------------------------------------------------------------------------
// ACTION FUNCTIONS FOR STATES
//
// To keep it simple, action functions should do only one task each if 
// possible. If there is a lot of conditions, make several action functions 
// and call them from event handlers. 
// ---------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoCaptureStill
// Start image capturing in Camera API.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoCaptureStill()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoCaptureStill() entering" ) );

    #ifdef _DEBUG
    // Performance debugging
    iCaptureStartTime.HomeTime();
    #endif

    iViewFinderFrameReceived = EFalse;
    iStillCancelled = EFalse;

    iCamera.CaptureImage(); // Start capture

    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoCaptureStill() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoDecodeCapturedImageToBitmap
//
// Decode to bitmap. CCaeStillDecoder calls McaesdoCFbsBitmapImageReady 
// callback when decoding is ready.
// Decodes Exif thumbnail to bitmap if requested (by iSnapImageSource).
// Note that the callback can be called synchronously by iStillDecoder, if 
// an error occurs in the still decoder. In that case also  ErrorRecovery() 
// has been called in McaesdoCFbsBitmapImageReady() *before* the method 
// below returns.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoDecodeCapturedImageToBitmap()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoDecodeCapturedImageToBitmap()" ) );
    HBufC8* tmpImageData = NULL;
    TSize   tmpSnapImageSize( 0, 0 );
    TInt    error( KErrNone );
    
    // Use thumbnail for snap image basis if requested to use it.
    if ( iSnapImageSource == CCaeEngine::ESnapImageSourceThumbnail ) 
        {
        TRAP( error, GetThumbnailL( tmpImageData, tmpSnapImageSize ) );
        if ( error )
            {
            LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoDecodeCapturedImageToBitmap(): Thumbnail extraction failed" ) );
            delete( tmpImageData );
            tmpImageData = iImageData;
            tmpSnapImageSize = iOptimalSnapImageSize;
	        iImageData = NULL; 
            }
        }
    else // Use the captured image for snap image basis.
        {
        tmpImageData = iImageData;
        tmpSnapImageSize = iOptimalSnapImageSize;
	    iImageData = NULL; 
        }

    // Ownership will be transferred. Null the pointer already before the method
    // call because it can call McaesdoCFbsBitmapImageReady() callback synchronously 
    // inside the method.
    // Always decode and scale to the correct size if free scaling is supported.
    TDisplayMode displayMode = iRequireFullColorSnapInputImageRefCount ? 
        EColor16M : iSnapImageColorMode;
    TRAP( error, iStillDecoder->ConvertHBufC8ToCFbsBitmapL( 
            tmpImageData, displayMode, tmpSnapImageSize, iSnapImageSize) );

    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaeCreateAndDeliverSnapImageError( error );
    #endif

    if ( error )
        {
        // Handle error.
        ErrorRecovery( error );
        }
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoExtensionsProcessCapturedImage
//
// Call the extensions for processing the captured image. Subsequent synchronous
// methods are called inside a loop. After an asynchronous method, the extension
// signals the state machine which calls this function again for the next 
// extension.  
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoExtensionsProcessCapturedImage()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoExtensionsProcessCapturedImage()" ) );
    
    OstTrace0( CAMERASRV_PERFORMANCE, CCAESTILLSTATESACTIVE_DOEXTENSIONSPROCESSCAPTUREDIMAGE, "e_CAM_CAE_OPERATIONS 1" );   //CAE_ENGINE_OPERATIONS_START
        
	TBool isAsyncOperation = EFalse;

	// Execute synchronous operations in loop. Exit the loop if there is an 
	// asynchronous function or a syncronous function has returned error status.  		
	while ( iStatus == KErrNone  
            && !iStillCancelled
		    && !isAsyncOperation 
		    && ( ++iCurrentExtensionIndex <  iProcessImageImplList.Count() ) )
		{

        TAny* interfacePtr = iProcessImageImplList[iCurrentExtensionIndex].iImplPtr;

        // Call extensions. Note that references to image pointers are given as
        // parameters. The engine has shared the ownership with the extensions,
        // which can also delete and create new images.
        if ( iProcessImageImplList[iCurrentExtensionIndex].iIsActive )
            {
    		isAsyncOperation = STATIC_CAST( MCaeExtProcessImageInterface*, interfacePtr )->ProcessCapturedImage(
                iStatus, iBitmap, iImageData, iImageHeaderData );
            }
        else
            {
	        LOGTEXT2(_L("Cae: CCaeStillStatesActive::DoExtensionsProcessCapturedImage().Skipping extension %x"),
	            iProcessImageImplList[iCurrentExtensionIndex].iImplUid.iUid);
            }
	    
	    #ifdef _DEBUG
	    if ( ( iStatus != KErrNone ) && ( iStatus != KRequestPending ) )
	        {
	        LOGTEXT3(_L("Cae: CCaeStillStatesActive::DoExtensionsProcessCapturedImage().Error: %d in ProcessCapturedImage() for Extension %x"),
	            iStatus.Int(), iProcessImageImplList[iCurrentExtensionIndex].iImplUid.iUid);
	        }
	    #endif
		}

    if (!iStillCancelled)
        {
	    // Note: For an asynchronous operation the status can be KErrNone 
	    //       if the operation has already finished and this thread has 
	    //       been signaled. 
		if ( ( iStatus == KErrNone || iStatus == KRequestPending ) && isAsyncOperation )
			{
			// Come back after an asynchronous operation has finished
			SetActive();
			}
		else
			{
            iCurrentExtensionIndex = -1;
            if ( iStatus == KErrNone )
                {
                OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAESTILLSTATESACTIVE_DOEXTENSIONSPROCESSCAPTUREDIMAGE, "e_CAM_CAE_OPERATIONS 0" );  //CAE_ENGINE_OPERATIONS_END
                Event( CCaeStillStatesActive::ECaeEventProcessCapturedImageAllReady );            
                }
            else
                {
                // Note: negative status values are handled as "external errors"
                // in RunL(). Check that the received iStatus is negative, extension can return any value
                
                if ( iStatus.Int() <= 0 )
                    {
                    Event( (CCaeStillStatesActive::TCaeEvent)iStatus.Int() );    
                    }
                else
                    {
                    Event( (CCaeStillStatesActive::TCaeEvent) KErrGeneral );    
                    }
                }
			}
        }
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoExtensionsProcessSnapImage
//
// Call the extensions for processing the snap image. Subsequent synchronous
// methods are called inside a loop. After an asynchronous method, the extension
// signals the state machine which calls this function again for the next 
// extension.  
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoExtensionsProcessSnapImage()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoExtensionsProcessSnapImage()" ) );
    
    OstTrace0( CAMERASRV_PERFORMANCE, CCAESTILLSTATESACTIVE_DOEXTENSIONSPROCESSSNAPIMAGE, "e_CAM_CAE_OPERATIONS 1" );   //CAE_ENGINE_OPERATIONS_START
        
	TBool isAsyncOperation = EFalse;

	// Execute synchronous operations in loop. Exit the loop if there is an 
	// asynchronous function or a syncronous function has returned error status.  
	
	while ( iStatus == KErrNone  
            && !iStillCancelled
		    && !isAsyncOperation 
		    && ( ++iCurrentExtensionIndex <  iProcessImageImplList.Count() ) )
		{

        TAny* interfacePtr = iProcessImageImplList[iCurrentExtensionIndex].iImplPtr;

        // Call extensions. Note that references to image pointers are given as
        // parameters. The engine has shared the ownership with the extensions,
        // which can also delete and create new images.
        if ( iProcessImageImplList[iCurrentExtensionIndex].iIsActive )
            {
    		isAsyncOperation = STATIC_CAST( MCaeExtProcessImageInterface*, interfacePtr )->ProcessSnapImage(
                iStatus, iBitmap, iSnapBitmap );
            }
        else
            {
	        LOGTEXT2(_L("Cae: CCaeStillStatesActive::DoExtensionsProcessSnapImage(). Skipping Extension %x"),
	            iProcessImageImplList[iCurrentExtensionIndex].iImplUid.iUid );
            }
	    
	    #ifdef _DEBUG
	    if ( ( iStatus != KErrNone ) && ( iStatus != KRequestPending ) )
	        {
	        LOGTEXT3(_L("Cae: CCaeStillStatesActive::DoExtensionsProcessSnapImage(). Error %d in ProcessSnapImage() for Extension %x"),
	            iStatus.Int(), iProcessImageImplList[iCurrentExtensionIndex].iImplUid.iUid );
	        }
	    #endif
		}

    if (!iStillCancelled)
        {
		// Note: For an asynchronous operation the status can be KErrNone 
		//       if the operation has already finished and this thread has 
		//       been signaled. 
		if ( ( iStatus == KErrNone || iStatus == KRequestPending ) && isAsyncOperation )
			{
			// Come back after an asynchronous operation has finished
			SetActive();
			}
		else
			{
            iCurrentExtensionIndex = -1;
            if ( iStatus == KErrNone )
                {
                OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAESTILLSTATESACTIVE_DOEXTENSIONSPROCESSSNAPIMAGE, "CamAppEngine_Perf:e_CAM_CAE_OPERATIONS 0" );    //CAE_ENGINE_OPERATIONS_END
                Event( CCaeStillStatesActive::ECaeEventProcessSnapImageAllReady );            
                }
            else
                {
                // Note: negative status values are handled as "external errors"
                // in RunL().
                if ( iStatus.Int() <= 0 )
                    {
                    Event( (CCaeStillStatesActive::TCaeEvent)iStatus.Int() );    
                    }
                else
                    {
                    Event( (CCaeStillStatesActive::TCaeEvent) KErrGeneral );    
                    }
                }
			}
        }
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoDeliverSnapImage
// Deliver the snap image to the client.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoDeliverSnapImage()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoDeliverSnapImage()" ) );
    CAE_ASSERT_ALWAYS( !iStillCancelled );

    if ( iSnapBitmap )
        {
        iCaeObserver->McaeoSnapImageReady( *iSnapBitmap, KErrNone );  
        delete( iSnapBitmap );
        iSnapBitmap = NULL;   
        }
    else
        {
        iCaeObserver->McaeoSnapImageReady( *iBitmap, KErrNone );  
        }
   
    if ( !iStillCancelled )
        {
        if ( ExtModeActive() )
            {
            Event( CCaeStillStatesActive::ECaeEventEnd ); // extension mode operation is completed
            }
        else
            {
            Event( CCaeStillStatesActive::ECaeEventDeliverSnapImageReady );
            }
        }
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoExtensionsProcessStillImage
//
// Call the extensions for processing the still image. Subsequent synchronous
// methods are called inside a loop. After an asynchronous method, the extension
// signals the state machine which calls this function again for the next 
// extension.  
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoExtensionsProcessStillImage()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoExtensionsProcessStillImage()" ) );
    
    OstTrace0( CAMERASRV_PERFORMANCE, CCAESTILLSTATESACTIVE_DOEXTENSIONSPROCESSSTILLIMAGE, "e_CAM_CAE_OPERATIONS 1" );  //CAE_ENGINE_OPERATIONS_START
        
	TBool isAsyncOperation = EFalse;

	// Execute synchronous operations in loop. Exit the loop if there is an 
	// asynchronous function or a syncronous function has returned error status.  
	
	while ( iStatus == KErrNone  
            && !iStillCancelled
		    && !isAsyncOperation 
		    && ( ++iCurrentExtensionIndex <  iProcessImageImplList.Count() ) )
		{

        TAny* interfacePtr = iProcessImageImplList[iCurrentExtensionIndex].iImplPtr;

        // Call extensions. Note that references to image pointers are given as
        // parameters. The engine has shared the ownership with the extensions,
        // which can also delete and create new images.
        if ( iProcessImageImplList[iCurrentExtensionIndex].iIsActive )
            {
    		isAsyncOperation = STATIC_CAST( MCaeExtProcessImageInterface*, interfacePtr )->ProcessStillImage(
                iStatus, iBitmap, iImageData, iImageHeaderData );
            }
        else
            {
	        LOGTEXT2(_L("Cae: CCaeStillStatesActive::DoExtensionsProcessStillImage(). Skipping extension %x"),
	            iProcessImageImplList[iCurrentExtensionIndex].iImplUid.iUid);
            }
	    
	    #ifdef _DEBUG
	    if ( ( iStatus != KErrNone ) && ( iStatus != KRequestPending ) )
	        {
	        LOGTEXT3(_L("Cae: CCaeStillStatesActive::DoExtensionsProcessStillImage(). Error %d in ProcessStillImage() for Extension %x"),
	            iStatus.Int(), iProcessImageImplList[iCurrentExtensionIndex].iImplUid.iUid);
	        }
	    #endif
		}

    if (!iStillCancelled)
        {
		// Note: For an asynchronous operation the status can be KErrNone 
		//       if the operation has already finished and this thread has 
		//       been signaled. 
		if ( ( iStatus == KErrNone || iStatus == KRequestPending ) && isAsyncOperation )
			{
			// Come back after an asynchronous operation has finished
			SetActive();
			}
		else
			{
            iCurrentExtensionIndex = -1;
            if ( iStatus == KErrNone )
                {
                OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAESTILLSTATESACTIVE_DOEXTENSIONSPROCESSSTILLIMAGE, "e_CAM_CAE_OPERATIONS 0" ); //CAE_ENGINE_OPERATIONS_END
                Event( CCaeStillStatesActive::ECaeEventProcessStillImageAllReady );            
                }
            else
                {
                // Note: negative status values are handled as "external errors"
                // in RunL().
                if ( iStatus.Int() <= 0 )
                    {
                    Event( (CCaeStillStatesActive::TCaeEvent)iStatus.Int() );    
                    }
                else
                    {
                    Event( (CCaeStillStatesActive::TCaeEvent) KErrGeneral );    
                    }
                }
			}
        }
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoEncodeStillImageToJpeg
//
// Encode bitmap to the Jpeg image. CCaeStillEncoder calls 
// McaeseoHBufC8ImageReady callback when encoding is ready.
// Note that the callback can be called synchronously by , iStillEncoder
// if an error occurs in the still encoder. In that case also
// ErrorRecovery() has been called in McaeseoHBufC8ImageReady() *before* 
// the method below returns.
//
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoEncodeStillImageToJpeg()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoEncodeStillImageToJpeg()" ) );
    // Ownership will be transferred. Null the pointer already before the method
    // call because it can call McaesdoCFbsBitmapImageReady() callback synchronously 
    // inside the method.
    CFbsBitmap* tmpBitmap = iBitmap;   
	iBitmap = NULL; 
	iStillEncoder->SetCompressionQuality( iStillCompressionQuality );
	TRAPD( error, iStillEncoder->ConvertCFbsBitmapToHBufC8L( tmpBitmap ) );

    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaeCreateAndDeliverStillImageError( error );
    #endif

    if ( error )
        {
        // Handle error
        ErrorRecovery( error );
        }
   }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoDeliverStillImage
// 
// Deliver the still image to the client and delete internal images after that.
// Note: If ownership is transferred, the image pointers should be NULLed before
// calling the observer method. That is because the observer method can call back
// CancelStill() which tries to delete images. That is no allowed as images 
// should be owned by the observer.
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoDeliverStillImage()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::DoDeliverStillImage()" ) );
    CAE_ASSERT_ALWAYS( !iStillCancelled && ( iSnapBitmap == NULL ) );

    // Deliver the still image to the client.
    if ( IsBitmapOutput() )
        {
        CFbsBitmap* tmpBitmap = iBitmap;
        iBitmap = NULL; // ownership is transferred      
        OstTrace0( CAMERASRV_PERFORMANCE, CCAESTILLSTATESACTIVE_DODELIVERSTILLIMAGE, "e_CAM_ENG_SHOT_TO_STILL 0" ); //CAE_ENGINE_SHOT_TO_STILL_END
        iCaeObserver->McaeoStillImageReady( tmpBitmap, NULL, KErrNone );
        }
    else
        {
        HBufC8* tmpImageData = iImageData;   
        iImageData = NULL; // Ownership is tranferred
        OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAESTILLSTATESACTIVE_DODELIVERSTILLIMAGE, "e_CAM_ENG_SHOT_TO_STILL 0" );    //CAE_ENGINE_SHOT_TO_STILL_END
        iCaeObserver->McaeoStillImageReady( NULL, tmpImageData, KErrNone );
        }
 
    // Delete all that is not NULL already. 
    delete iImageData;
    iImageData = NULL;
    delete iImageHeaderData;
    iImageHeaderData = NULL;
    delete iBitmap;
    iBitmap = NULL;

    Event( CCaeStillStatesActive::ECaeEventEnd ); // The end. Actuall does no issue any event. 
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::StartQuickCapture()
// 
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::StartQuickCapture()
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::StartQuickCapture()" ) );
    CAE_ASSERT_DEBUG( iCurrentState == CCaeStillStatesActive::ECaeStateNone );
    HandleStart();
    }

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::ExtModeActive()
// 
// -----------------------------------------------------------------------------
//
TBool CCaeStillStatesActive::ExtModeActive()
    {
    LOGTEXT2( _L( "Cae: CCaeStillStatesActive::ExtModeActive() %d" ), iExtModeActive );
    return( iExtModeActive );
    }

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetExtModeActiveL()
// 
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetExtModeActiveL( TBool aExtModeActive )
    {
    LOGTEXT2( _L( "Cae: CCaeStillStatesActive::SetExtModeActiveL() %d" ), aExtModeActive );
    iExtModeActive = aExtModeActive;
    iStillCancelled = EFalse;           // clear previous cancel request of still capture

    // Create new instance of still image queue object.
    if ( aExtModeActive && !iImageQueueExtPro )
        {
        iImageQueueExtPro = CCaeImageQueueExtPro::NewL();
        }
    else if ( !aExtModeActive )
        {
        delete iImageQueueExtPro;
        iImageQueueExtPro = 0;
        }

    }

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::ProcessExtSnapImage
// Process extension for snap image
// -----------------------------------------------------------------------------
//
TInt CCaeStillStatesActive::ProcessExtSnapImage( CFbsBitmap* aSnapImage )
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::ProcessExtSnapImage()" ));
    
    TInt result = iImageQueueExtPro->AppendImage( aSnapImage, NULL, EFalse, ETrue );
    if ( result == KErrNone &&                                      // image was saved ok
        iCurrentState == CCaeStillStatesActive::ECaeStateNone &&    // state machine is idle
        iImageQueueExtPro->ImageCount() < 2 )                       // queue was empty
        {
        // Send event to start processing, state machine will stop when queue is empty
        // and return then back to ECaeStateNone state
        Event( CCaeStillStatesActive::ECaeEventImageQueueExtPro );
        }
    return ( result );
    }

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::ProcessExtCapturedImage
// Process extension for captured jpg image
// -----------------------------------------------------------------------------
//
TInt CCaeStillStatesActive::ProcessExtCapturedImage( HBufC8* aImageData, TBool aLastImage  )
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::ProcessExtCapturedImage() jpg" ));

    TInt result = KErrNone;
    result = iImageQueueExtPro->AppendImage( NULL, aImageData, aLastImage, EFalse );

    if ( result == KErrNone &&                                      // image was saved ok
        iCurrentState == CCaeStillStatesActive::ECaeStateNone &&    // state machine is idle
        iImageQueueExtPro->ImageCount() < 2 )                       // queue was empty
        {
        // Send event to start processing, state machine will stop when queue is empty
        // and return then back to ECaeStateNone state
        Event( CCaeStillStatesActive::ECaeEventImageQueueExtPro );
        }

    return ( result );
    }

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::ProcessExtCapturedImage
// Process extension for captured bitmap image
// -----------------------------------------------------------------------------
//
TInt CCaeStillStatesActive::ProcessExtCapturedImage( CFbsBitmap* aImageBitmapData, TBool aLastImage  )
    {
    LOGTEXT( _L( "Cae: CCaeStillStatesActive::ProcessExtCapturedImage() bitmap" ));

    TInt result = KErrNone;
    result = iImageQueueExtPro->AppendImage( aImageBitmapData, NULL, aLastImage, EFalse );

    if ( result == KErrNone &&                                      // image was saved ok
        iCurrentState == CCaeStillStatesActive::ECaeStateNone &&    // state machine is idle
        iImageQueueExtPro->ImageCount() < 2 )                       // queue was empty
        {
        // Send event to start processing, state machine will stop when queue is empty
        // and return then back to ECaeStateNone state
        Event( CCaeStillStatesActive::ECaeEventImageQueueExtPro );
        }

    return ( result );
    }

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetBurstModeVFOptimization()
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetBurstModeVFOptimization(
    TBool aBurstModeVFOptimization )
    {
    LOGTEXT2( _L( "Cae: CCaeStillStatesActive::SetBurstModeVFOptimization(%d)" ), aBurstModeVFOptimization);
    iBurstModeVFOptimization = aBurstModeVFOptimization;
    }

// End of File  
