/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef CAESTILLSTATESACTIVE_H
#define CAESTILLSTATESACTIVE_H

// INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <s32file.h>
#include <fbs.h>
#include <ecam.h>
#include "CaeEngineExtInterface.h"
#include "CaeCommon.h"
#include "CaeEngine.h"
#include "CaeStillEncoder.h"
#include "CaeStillDecoder.h"
#include "CaeDbgUtilities.h"                // For debugging.


// CONSTANTS

#ifdef _DEBUG
const TInt KOneSecond = 1000000; // This many microseconds in a second
#endif

// Optimal snap bitmap size concerning memory usage and snap image quality
const TInt KCaeDefaultSnapWidth = 640;
const TInt KCaeDefaultSnapHeight = 480;

_LIT(KCaePanicText, "CamAppEngine");
enum TCaePanic
    {
    ECaePanicInvalidState = 1   
    };

const TInt KCaeDefaultCompressionQuality = 90;
const CCamera::TFormat KBaseStillInputFormat1 = CCamera::EFormatFbsBitmapColor16M;
const CCamera::TFormat KBaseStillInputFormat2 = CCamera::EFormatFbsBitmapColor64K;
const TDisplayMode KCaeDefaultDisplayMode = EColor64K; // The default display mode for the snap image


// FORWARD DECLARATIONS
class CCaeEngineImp;
class CCaeStillBurst;
class CCaeImageQueueExtPro;

// CLASS DECLARATION

/**
* Still capure state machine implementation class
*/

NONSHARABLE_CLASS( CCaeStillStatesActive ) : public CActive,
                                             public MCaeStillEncoderObserver, 
                                             public MCaeStillDecoderObserver
    {
    public: // Enums

        /**
        * Events for the state machine
        */
        enum TCaeEvent
            {
            // External event
            ECaeEventNone							= 0,

            // Start 
            ECaeEventStartStillCapture				= 1,

            // Capturing 
            ECaeEventImageCaptureReady				= 100,
            ECaeEventBurstImageCaptureReady			= 101,
            ECaeEventBurstCaptureReady				= 102,
            ECaeEventAppendCapturedBurstImageReady	= 103,        
            ECaeEventViewFinderForBurstReady		= 104, 
           
            // Processing
            ECaeEventBurstImageFetchReady			= 200,
            ECaeEventDecodeToBitmapReady			= 201,
            ECaeEventExtractExifMetaDataReady		= 202,
            ECaeEventProcessCapturedImageStepReady	= 203,
            ECaeEventProcessCapturedImageAllReady	= 204,
            ECaeEventProcessSnapImageStepReady		= 205,
            ECaeEventProcessSnapImageAllReady		= 206,
            ECaeEventDeliverSnapImageReady			= 207,  
            ECaeEventProcessStillImageStepReady		= 208,
            ECaeEventProcessStillImageAllReady		= 209,
            ECaeEventEncodeToJpegReady				= 210,
            ECaeEventDeliverStillBurstImageReady	= 211,  
            ECaeEventDeliverStillImageReady			= 212,  
           
            // Processing
            ECaeEventImageQueueExtPro               = 300,

            // End of process    
            ECaeEventEnd							= 900
            };
   
    public: // Constructors and destructor
        
        /**
        * Symbian OS two-phased constructor.
        * @since 2.8
        * @param aCamera Reference to Camera API.
        * @param aInfo Reference to the engine info struct
        * @param aProcessImageImplList Reference to list of the extensions implementing MCaeExtProcessImageInterface.
        * @return instance of the object
        */
        static CCaeStillStatesActive* NewL( 
            CCamera& aCamera,
            const TCamAppEngineInfo& aInfo,
            RArray<TCaeExtensionInterfaceImplItem>& aProcessImageImplList );

        /**
        * Destructor.
        * @since 2.8
        */
        virtual ~CCaeStillStatesActive();

    public: // Delegated from CCaeEngine class method calls

        void SetCamAppEngineObserver( 
            MCamAppEngineObserver& aObserver );

        void SetSnapImageCreation( 
            TBool aCreateSnapImage = ETrue );

        void SetSnapImageSourceL(
            CCaeEngine::TSnapImageSource aSnapImageSource );

        void SetSnapImageSizeL(
            TSize& aSize );

        void SetSnapImageColorMode(
            TDisplayMode aMode );

     	void SetJpegQuality(
    	    TInt aQuality );

    	TInt JpegQuality() const;

        void SetImageCodecsL( 
        	TUid aDecoderUid, TUid aEncoderUid );

        void SetCaeStillBurstObserver( 
            MCaeStillBurstObserver& aObserver );

        TInt SetStillCaptureImageCountL( 
            TInt aImageCount );

        TInt StillCaptureImageCount() const;

        void SetStillBurstCaptureIntervalL( 
            TTimeIntervalMicroSeconds aInterval );

        TTimeIntervalMicroSeconds StillBurstCaptureInterval() const;

        void StopStillBurstCapture();

        void CancelCaptureStill();

    public: // New methods

        /**
        * Called asynchronously when the captured image is ready. 
        * @since 2.8
        * @param aBitmap A pointer to an image held in CFbsBitmap form
        * @param aData A pointer to an image held in HBufC8 in previously specified format.
        * @param aError SymbianOS standard error code.
        * @return void.
        */
        void ImageReady( 
            CFbsBitmap* aBitmap, 
            HBufC8*     aData, 
            TInt        aError );

        /**
        * Called asynchronously when the view finder frame is ready. 
        * @since 2.8
        * @return void
        */
        void ViewFinderFrameReady();

        /**
        * Get current still image size. 
        * @since 2.8
        * @return Still image size in pixels.
        */
        TSize StillImageSize() const;

        /**
        * Get current snap image size. 
        * @since 2.8
        * @return Snap image size in pixels.
        */
        TSize SnapImageSize() const;

        /**
        * Set view finder mode enabled or disabled.
        * @since 2.8
        * @param aIsEnabled Tell if the view finder is enabled.
        * @return void.
        */
     	void SetViewFinderMode(
    	    TBool aIsEnabled );

        /**
        * Check if the state machine has been started. 
        * @since 2.8
        * @return State of the state machine.
        */
        TBool IsRunning() const;

        /** 
        * Register flags which affect general extension handling in the engine. 
        * @since 2.8
        * @param aFlags Flags to register.
        * @return void.
        */
        void RegisterFlags( TUint32 aFlags );

        /** 
        * Deregister flags which have been registered before.
        * @since 2.8
        * @param aFlags Flags to deregister.
        * @return void.
        */
        void DeregisterFlags( TUint32 aFlags );

        /**
        * Prepare the state machine for still image capture . 
        * The returned snap image size is always equal or bigger in both 
        * dimensions than the given size. The returned size is stepwise 
        * downscaled  from the captured image size. The parameter aSnapSize 
        * is ignored if that is not supported for the image format used.
        * @since 2.8
        * @param aSize Still image size.
        * @param aFormat Still image format.
        * @param aCropRect Desired cropping/clipping rectangle. The size (0,0) means that this parameter is ignored.
        * @param aSnapSize Desired snap image size. Returns the real snap image size.
        * @return void
        */
        void PrepareStillCaptureL( 
            const TSize&     aSize, 
            CCamera::TFormat aFormat,  
            const TRect&     aCropRect,
            TSize&           aSnapSize );

        /**
        * Send event to the state machine. Usually only ECaeEventStartStillCapture
        * event is send outside the class. 
        * @since 2.8
        * @param aEvent Event to the state machine.
        * @param aDelay The delay that the event will occur after.
        * @return void.
        */
		void Event( TCaeEvent aEvent, TTimeIntervalMicroSeconds32 aDelay = 0 );
    
        /**
        * Recover and handle error cases. This could mean also ending of 
        * the state machine, but not always (e.g. in burst mode ). 
        * @since 2.8
        * @param aError Error code.
        * @return void.
        */
        void ErrorRecovery( TInt aError );
        
        /**
        * Start capturing without active object delay.
        * @since 3.2
        * @return void.
        */
        void StartQuickCapture();

        /**
        * Gets the extension mode status.
        * @since 3.2
        * @return ETrue if extension mode is avtive.
        */
        TBool ExtModeActive();

        /**
        * Sets the extension mode on/off.
        * @since 3.2
        * @return void.
        */
        void SetExtModeActiveL( TBool aExtModeActive );

        /**
        * Do the extension processing for snap image.
        * @since 3.2
        * @return error code.
        */
        TInt ProcessExtSnapImage( CFbsBitmap* aSnapImage );


        /**
        * Do the extension processing for captured image.
        * @since 3.2
        * @return error code.
        */
        TInt ProcessExtCapturedImage( HBufC8* aImageData, TBool aLastImage  );

        /**
        * Do the extension processing for captured bitmap image.
        * @since 3.2
        * @return error code.
        */
        TInt ProcessExtCapturedImage( CFbsBitmap* aImageBitmapData, TBool aLastImage  );

        /**
        * Set the burst mode optimization mode  
        * @since 3.2
        * @return void.
        */
        void SetBurstModeVFOptimization( TBool aBurstModeVFOptimization );

    private: // Enums

        /**
        * States of the state machine
        */
		enum TCaeState
			{
			ECaeStateNone                               = 0,

            // Capturing 
            ECaeStateCapturing                          = 100,
            ECaeStateCapturingBurst                     = 101,
            ECaeStateAppendingBurstImage                = 102,
            ECaeStateBurstWaitingForViewFinder          = 103,

            // Processing
            ECaeStateFetchingNextBurstImage				= 200,
            ECaeStateDecodingCapturedImageToBitmap      = 201,
            ECaeStateExtractingExifMetaData             = 202,
            ECaeStateExtensionsProcessingCapturedImage  = 203,
            ECaeStateDecodingToSnapImage                = 204,
            ECaeStateExtensionsProcessingSnapImage      = 205,
            ECaeStateDeliveringSnapImage                = 206,
            ECaeStateExtensionsProcessingStillImage     = 207,
            ECaeStateEncodingToJpeg                     = 208,
            ECaeStateDeliveringStillImage               = 209,
            ECaeStateDeliveringStillBurstImage          = 210,
            ECaeStateCompletingStillBurst               = 211
			};
        
    private: // Constructors
        
        /**
        * C++ constructor.
        * @since 2.8
        * @param aCamAppEngine Reference to CCaeEngineImp instance
        */
        CCaeStillStatesActive( 
            CCamera& aCamera,
            const TCamAppEngineInfo& aInfo,
            RArray<TCaeExtensionInterfaceImplItem>& aProcessImageImplList );

        /**
        * Symbian OS 2nd phase constructor that can leave.
        * @since 2.8
        * @return void
        */
        void ConstructL();

    private: // Methods from CActive 
        
        void RunL();

        void DoCancel();

        void Cancel();

    private: // From Still Encoder Observer / CaeStillConverter.h

        void McaeseoHBufC8ImageReady( 
            CFbsBitmap* aBitmap, 
            HBufC8*     aImageData, 
            TInt        aError,
            TInt        aImageSize );
        
    private: // From Still Decoder Observer / CaeStillConverter.h

       void McaesdoCFbsBitmapImageReady( 
            HBufC8*     aImageData, 
            CFbsBitmap* aBitmap, 
            TInt        aError,
            TInt        aImageSize );
        
    private: // Event handlers for state transitions

        /**
        * Handle errors from extenal processing, e.g. from the extensions. 
        * @since 2.8
        * @return void. 
        */
        void HandleExternalError();

        /**
        * Handle events from extenal processing, e.g. from the extensions. 
        * @since 2.8
        * @return void. 
        */
        void HandleExternalEvent(); 

        /**
        * Handle start event. 
        * @since 2.8
        * @return void. 
        */
        void HandleStart();

        /**
        * Handle event when an image has been captured. 
        * @since 2.8
        * @return void. 
        */
        void HandleImageCaptureReady();

        /**
        * Handle event when a burst image has been appended to the array. 
        * @since 2.8
        * @return void. 
        */
        void HandleAppendCapturedBurstImageReady();

        /**
        * Handle event when a burst image has been fetched from the array. 
        * @since 2.8
        * @return void. 
        */
        void HandleBurstImageFetchReady();

        /**
        * Handle event when a bitmap has been decoded. 
        * @since 2.8
        * @return void. 
        */
        void HandleDecodeToBitmapReady();

        /**
        * Handle event when all the extensions have been processed the captured image. 
        * @since 2.8
        * @return void. 
        */
        void HandleProcessCapturedImageAllReady();

        /**
        * Handle event all extensions have been processed the still image.
        * @since 2.8
        * @return void. 
        */
        void HandleProcessStillImageAllReady();

        /**
        * Handle event when a still bitmap has been encoded to Jpeg.
        * @since 2.8
        * @return void. 
        */
       void HandleEncodeToJpegReady();

        /**
        * Handle event when the still burst image has been delivered to the client. 
        * @since 2.8
        * @return void. 
        */
        void HandleDeliverStillBurstImageReady();
        
        /**
        * Handle event when there are images in extension processing queue
        * @since 3.2
        * @return void. 
        */
        void HandleImageQueueExtPro();

    private: // Action functions for states

        /**
        * Start still (single) image capturing. 
        * @since 2.8
        * @return void. 
        */
        void DoCaptureStill();

        /**
        * Start still burst image capturing. 
        * @since 2.8
        * @return void. 
        */
        void DoCaptureStillBurst();

        /**
        * Start still burst capturing for one image. 
        * @since 2.8
        * @return void. 
        */
        void DoCaptureStillBurstImage();

        /**
        * Start appending captured burst image to the array. 
        * @since 2.8
        * @return void. 
        */
        void DoAppendCapturedBurstImageToArray();

        /**
        * Start fetching the next burst image from the array. 
        * @since 2.8
        * @return void. 
        */
        void DoFetchNextBurstImage();

        /**
        * Start decoding the captured image to the bitmap. 
        * @since 2.8
        * @return void. 
        */
        void DoDecodeCapturedImageToBitmap();

        /**
        * Start processing the captured image in the extensions.
        * @since 2.8
        * @return void. 
        */
        void DoExtensionsProcessCapturedImage();

        /**
        * Start processing the snap image in the extensions.
        * @since 2.8
        * @return void. 
        */
        void DoExtensionsProcessSnapImage();

        /**
        * Start delivering the snap image to the client.
        * @since 2.8
        * @return void. 
        */
        void DoDeliverSnapImage();

        /**
        * Start processing the still image in the extensions.
        * @since 2.8
        * @return void. 
        */
        void DoExtensionsProcessStillImage();

        /**
        * Start encoding to Jpeg image.
        * @since 2.8
        * @return void. 
        */
        void DoEncodeStillImageToJpeg();

        /**
        * Start delivering the still burst image to the client.
        * @since 2.8
        * @return void. 
        */
        void DoDeliverStillBurstImage();

        /**
        * Start delivering the still image to the client.
        * @since 2.8
        * @return void. 
        */
        void DoDeliverStillImage();

        /**
        * Start completing the still burst.
        * @since 2.8
        * @return void. 
        */
        void DoCompleteStillBurst();

    private: // Helper methods

        /**
        * Get the current display mode from the system.
        * @since 2.8
        * @param aDisplayMode Returned display mode.
        * @return TBool Whether the display mode was found.
        */
        TBool GetSystemDisplayMode( TDisplayMode& aDisplayMode );

        /**
        * If needed, calculates the stepwise downscaled size for the snap bitmap.
        * @since 2.8
        * @param aSnapSize Desired snap image size. Returns the real snap image size.
        * @return void
        */
        void CalcDownscaledSnapSize(
            TSize& aSnapSize ) const;

        /**
        * Gets the thumbnal from an Exif image.
        * @since 2.8
        * @return  
        */
        void GetThumbnailL( 
            HBufC8*& aReadThumbnail, 
            TSize&   aThumbnailSize );
            
        /**
        * The output image type for the client.
        * @since 2.8
        * @return Tell if the output format is bitmap. 
        */
        TBool IsBitmapOutput();

        /**
        * Retrieves/fetches still image size index from Camera API.
        * @since 2.8
        * @param aSize Still image frame size
        * @param aStillDataFormat Still image data format
        * @return Still image size index
        */
        TInt RetrieveStillSizeIndex( 
            const TSize&     aSize, 
            CCamera::TFormat aStillDataFormat ) const;

        /**
        * Cancel the state machine actions and release and cancel internal objects.  
        * @since 2.8
        * @return void. 
        */
        void CancelAndCleanup();

        /**
        * Check for still burst completion.
        * @since 2.8
        * @return Return whether the still burst should be completed.
        */
        TBool CheckForStillBurstCompletion();

        /**
        * Delete images and continue burst. Ignore error for an individual image.
        * @since 2.8
        * @param aError Error code.
        * @return void.
        */
        void BurstErrorRecovery( TInt aError );

        /**
        * Complete still image burst capture but do not delete still burst object. 
        * @since 2.8
        * @return void. 
        */
        void CompleteStillBurst();

        /**
        * Delete still burst object.
        * @since 2.8
        * @return void. 
        */
        void DeleteStillBurst();

    private:

    //// Objects ////

        // Timer used for delayed calls.
        RTimer iDelayedCallTimer; 

        // File server handle for file operations.
        RFs iFs;
        
        // Camera API reference.
        CCamera& iCamera; 

        // Engine info reference.
        const TCamAppEngineInfo& iInfo;

        // Camera Application Engine observer.
        MCamAppEngineObserver* iCaeObserver;

        // Still image encoder object.
        CCaeStillEncoder* iStillEncoder; 

        // Still image decoder object.
        CCaeStillDecoder* iStillDecoder;

        // Still burst observer.
        MCaeStillBurstObserver* iCaeStillBurstObserver;

        // Still image burst object, contains all the images captured in a burst.
        CCaeStillBurst* iStillBurst; 

        // Still image queue object for extension handling (used only when extension mode is active),
        // contains all the images that are waiting for extension handling and delivery to UI.
        CCaeImageQueueExtPro* iImageQueueExtPro; 


    //// Data ////

		// All extension interface implementations for image processing.
		RArray<TCaeExtensionInterfaceImplItem>& iProcessImageImplList; 

		// Image data in image format structure.
		HBufC8* iImageData; 

		// Image data in bitmap.
		CFbsBitmap* iBitmap;
		
		// Snap image.
		CFbsBitmap* iSnapBitmap; 

		// Exif header.
		HBufC8* iImageHeaderData; 

        // Empty Snap image.
        CFbsBitmap* iEmptySnapImage;

        // Snap image source (input for decoding).
        CCaeEngine::TSnapImageSource iSnapImageSource;

         // Snap image size.
        TSize iSnapImageSize; 

         // Optimal snap image size.
        TSize iOptimalSnapImageSize; 

         // Snap image color mode.
        TDisplayMode iSnapImageColorMode; 

        // Still image capturing frame size.
        TSize iStillFrameSize;

        // Still image capture input format from Camera API.
        CCamera::TFormat iStillInputFormat;
        
        // Still image capture output format for client.
        CCamera::TFormat iStillOutputFormat;

        // Still image compression quality value.
        TInt iStillCompressionQuality;

        // Time delay to be inserted between captures in a burst.
        TTimeIntervalMicroSeconds32 iStillBurstCaptureInterval;

        // Count of processed (encoded/decoded) burst images.
        TInt iCountOfProcessedBurstImages;

        // Count of delivered burst images (delivered to the client).
        TInt iCountOfDeliveredBurstImages;


    //// State information ////

		// Current state
		TCaeState iCurrentState;

        // The first error detected during the burst.
        TInt iFirstStillBurstError;

		// Current Extension array index 
		TInt iCurrentExtensionIndex;

        // Delayed event id
        TCaeEvent iDelayedEvent;


    //// Flags ////

        // Boolean indicating if the engine should create the snap image (for client's display purposes).
        TBool iCreateSnapImage;

        // Boolean indicating if still image capturing is currently cancelled.
        TBool iStillCancelled;

        // Boolean indicating if delivering burst images has started.
        TBool iStillBurstDeliveryStarted;

        // Boolean indicating if still burst is stopped.
        TBool iStillBurstStopped;

        // Boolean indicating if a view finder frame has been received.
        TBool iViewFinderFrameReceived;

        // Boolean indicating if the view finder has been enabled.
        TBool iIsViewFinderEnabled;

        // Boolean indicating if the extension mode is selected
        TBool iExtModeActive;
      
        // Boolean indicating if this is the last burst image for extension processing
        TBool iLastImageExtPro;
      
        // Boolean indicating if still image capture is prepared.
        TBool iStillPrepared;

    //// Flags using reference count ////

        // Boolean indicating if full color snap image is required for extensions.
        TInt32 iRequireFullColorSnapInputImageRefCount;

        // Boolean indicating if the VF burst optimization is used
        TBool iBurstModeVFOptimization;

#ifdef _DEBUG
        // Performance debugging variables
        TTime iCaptureStartTime;
        TTime iImageReadyTime;
        TTime iFinalImageReadyTime;
#endif
    };

#endif // CAESTILLSTATESACTIVE_H

// End of File
