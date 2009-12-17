/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Camera Application Engine implementation class
*
*/



#ifndef CAEENGINEIMP_H
#define CAEENGINEIMP_H

//  INCLUDES
#include <ecam.h>
#include <videorecorder.h>
#include "CamCControllerCustomCommands.h"
#include "CaeEngine.h"
#include "CaeEngineExtInterface.h"
#include "CaeCommon.h"
#include "CaeStillEncoder.h"
#include "CaeStillDecoder.h"
#include "CaeCallbackActive.h"              // For making method calls from callbacks.
#include "CaeDbgUtilities.h"                // For debugging.

#include <e32std.h>

// CONSTANTS
const TInt KVideoTimesIntervalDefault = 1000000; // Default time interval for Video Recording times call-back.

const TInt KVideoTimesDelay = 100000; // Delay in starting to generate video times, in microseconds.

const TInt KCameraPriority = -1; // Relative priority (-100...100) of camera HW

const TUid KUidBurstModeVFOptimization = {0x4A758866}; // Custom command interface UID used by some products

// FORWARD DECLARATIONS
class CVideoRecorderUtility;
class RWsSession;
class CWsScreenDevice;
class RWindowBase;
class CFbsBitmap;
class CCaeStillQualityLevelsCont;
class CCaeVideoQualityLevelsCont;
class CCaeVideoTimes;
class CCaeStillBurst;
class CCaeStillStatesActive;


// CLASS DECLARATIONS

/**
* Camera application engine implementation class.
*/

NONSHARABLE_CLASS( CCaeEngineImp ): public CCaeEngine, 
                                    public MCameraObserver,
                                    public MExtensionCallbackInterface,
					                public MCaeExtEngineInfoCallbackInterface,
                                    public MVideoRecorderUtilityObserver,
                                    public MCameraObserver2
    {

    friend void CCaeCallbackActive::RunL();
	friend void CCaeCallbackActive::DoCancel();

    public: // Construction and destruction

        static CCaeEngineImp* NewL();

        virtual ~CCaeEngineImp();

    
    public: // General methods / CCaeEngine.h

        void SetCamAppEngineObserver( 
            MCamAppEngineObserver& aObserver );

        void GetInfo( 
            TCamAppEngineInfo& aInfo ) const;

        void InitL( 
            TBool aCreateSnapImage = ETrue );

        void InitL( 
            TUid  aSdUidStillQltyLevels, 
            TBool aCreateSnapImage = ETrue );

        void InitVideoRecorderL();
        
        void InitVideoRecorderL( 
            TUid aSdUidVideoQltyLevels );

        void Reserve();

        void Release();

        void PowerOn();

        void PowerOff();

    
    public: // Camera settings / CCaeEngine.h

        void SetZoomModeL( 
            TZoomMode aZoomMode = EZoomModeDigital );

        TZoomMode ZoomMode() const;

        void SetZoomValueL( 
            TInt aZoomValue = 0 );

        TInt ZoomValue() const;

        void SetBrightnessL( 
            TInt aBrightness = 0 );

        TInt Brightness() const;

        void SetContrastL( 
            TInt aContrast = 0 );

        TInt Contrast() const;

        void SetExposureModeL( 
            CCamera::TExposure aExposureMode = CCamera::EExposureAuto );

        CCamera::TExposure ExposureMode() const;

        void SetWhiteBalanceModeL( 
            CCamera::TWhiteBalance aWhiteBalanceMode = CCamera::EWBAuto );

        CCamera::TWhiteBalance WhiteBalanceMode() const;

        void SetFlashModeL( 
            CCamera::TFlash aFlashMode = CCamera::EFlashNone );

        CCamera::TFlash FlashMode() const;

        void ResetToDefaultsL();
    
    
    public: // View Finder / CCaeEngine.h

        void SetViewFinderMirrorL( 
            TBool aMirror );

        TBool ViewFinderMirror() const;

        void StartViewFinderBitmapsL( 
            TSize& aVfFrameSize );

        void StartViewFinderBitmapsL( 
            TSize& aVfFrameSize, 
            TRect& aCropRect );
        
        void StartViewFinderDirectL( 
            RWsSession&      aWs, 
            CWsScreenDevice& aScreenDevice, 
            RWindowBase&     aWindow, 
            TRect&           aScreenRect );

        void StartViewFinderDirectL( 
            RWsSession&      aWs, 
            CWsScreenDevice& aScreenDevice, 
            RWindowBase&     aWindow, 
            TRect&           aScreenRect, 
            TRect&           aCropRect );
        
        void StopViewFinder();

        TBool IsViewFinding() const;

        void GetViewFinderSize( 
            TSize& aVfFrameSize );

    
    public: // Still image capturing / CCaeEngine.h

        void PrepareStillCaptureL( 
            TInt aStillQualityIndex );
        
        void PrepareStillCaptureL( 
            TInt         aStillQualityIndex, 
            const TRect& aCropRect );

        void PrepareStillCaptureL( 
            const TSize&     aSize, 
            CCamera::TFormat aFormat,
            TInt             aCompressionQuality );

        void PrepareStillCaptureL( 
            const TSize&     aSize, 
            CCamera::TFormat aFormat, 
            TInt             aCompressionQuality, 
            const TRect&     aCropRect );

        TInt StillQualityIndex() const;

        void GetStillFrameSize( 
            TInt   aStillQualityIndex, 
            TSize& aSize ) const;

        TInt EstimatedStillSizeInBytes( 
            TInt aStillQualityIndex ) const;

        void CaptureStill(); 
        
        void CancelCaptureStill();

    
    public: // Video recording / CCaeEngine.h
    
        void SetVideoRecordingFileNameL( 
            const TDesC& aVideoClipFileName );

        void PrepareVideoRecordingL( 
            TInt aVideoQualityIndex );

        void PrepareVideoRecordingL( 
            const TSize&  aFrameSize, 
            TReal32       aFrameRate, 
            TInt          aBitRate, 
            TBool         aAudioEnabled,
            const TDesC8& aMimeType, 
            const TDesC&  aPreferredSupplier, 
            const TDesC8& aVideoType = KNullDesC8, 
            const TDesC8& aAudioType = KNullDesC8 );

        void CloseVideoRecording();

        TInt VideoQualityIndex() const;

        void GetVideoFrameSize( 
            TInt   aVideoQualityIndex, 
            TSize& aSize ) const;

        TReal32 VideoFrameRate( 
            TInt aVideoQualityIndex ) const;

        TInt EstimatedVideoRecordingBitRateL( 
            TInt aVideoQualityIndex ) const;

        void SetVideoClipMaxSizeL( 
            TInt aMaxClipSizeInBytes = 0 );

        TInt VideoClipMaxSize() const;

        void SetVideoAudioL( 
            TBool aAudioEnabled );

        TBool VideoAudio() const;

        void SetVideoTimesIntervalL( 
            TTimeIntervalMicroSeconds aInterval );

        TTimeIntervalMicroSeconds VideoTimesInterval() const;

        void StartVideoRecording();

        void StopVideoRecording();

        void PauseVideoRecording();

        void ResumeVideoRecording();

        TTimeIntervalMicroSeconds RemainingVideoRecordingTime() const;

        TBool IsVideoRecording() const;

    public: // New overloading method for preparing video recording / CCaeEngine.h
    
        void PrepareVideoRecordingL( 
            const TSize&  aFrameSize, 
            TReal32       aFrameRate, 
            TInt          aBitRate, 
            TBool         aAudioEnabled,
            TInt          aAudioBitRate, 
            const TDesC8& aMimeType, 
            const TDesC&  aPreferredSupplier, 
            const TDesC8& aVideoType, 
            const TDesC8& aAudioType );

    public: // Still image burst capturing interface methods / CCaeEngine.h

        void SetCaeStillBurstObserver( 
            MCaeStillBurstObserver& aObserver );

        TInt SetStillCaptureImageCountL( 
            TInt aImageCount );

        TInt StillCaptureImageCount() const;

        void SetStillBurstCaptureIntervalL( 
            TTimeIntervalMicroSeconds aInterval );

        TTimeIntervalMicroSeconds StillBurstCaptureInterval() const;

        void StopStillBurstCapture();

        void PrepareStillCaptureL( 
            TInt         aStillQualityIndex, 
            TSize&       aSnapSize );

        void PrepareStillCaptureL( 
            const TSize&     aSize, 
            CCamera::TFormat aFormat,  
            TInt             aCompressionQuality, 
            const TRect&     aCropRect,
            TSize&           aSnapSize );

        void SetEvCompensationL( 
            TInt aEvIndex = 0 );

        TInt EvCompensation() const;
        
    public: // Method for switching snap-image creation on/off. / CCaeEngine.h
    
        void SetSnapImageCreation( 
            TBool aCreateSnapImage = ETrue );

    public: // New methods since 2.6 and 2.8 / CCaeEngine.h

    	static CCaeEngineImp* NewL(
    	    TInt aCameraIndex, TInt aDisplayIndex );

    	void EnumerateStillCaptureSizes(
    	    TSize& aSize,
    	    TInt aSizeIndex,
    	    CCamera::TFormat aFormat ) const;

    	void SetJpegQuality(
    	    TInt aQuality );

    	TInt JpegQuality() const;

        TAny* CustomInterface(
			TUid aInterface );

        void SetSnapImageSizeL(
            TSize& aSize );

        void SetSnapImageColorMode(
            TDisplayMode aMode );

        void GetOrderL( 
			RCaeOrderedFeatureList& aOrderedFeatureList );

        void SetOrderL( 
			const RCaeOrderedFeatureList& aOrderedFeatureList );

        void SetSnapImageSourceL(
            TSnapImageSource aSnapImageSource );

        void SetImageCodecsL( 
        	TUid aDecoderUid, TUid aEncoderUid );

        TInt EnumerateVideoFrameSizeL(const TSize& aSize);
        
        TInt CCameraHandle() const;
        	
        void SetCaeExtensionModeL( TBool aExtModeActive, TBool aCreateSnapImage );

        void ProcessExtViewFinderFrameReadyL( CFbsBitmap& aFrame );

        void ProcessExtSnapImageL( CFbsBitmap& aSnapImage );

        void ProcessExtCapturedImageL( CFbsBitmap& aBitmap, TBool aLastImage );
        
        void ProcessExtCapturedImageL( HBufC8* aImageData, TBool aLastImage );
        
        void ProcessExtCapturedImageL( TDesC8& aImageDataDes, TBool aLastImage );

        void ProcessExtCancel();

        void SkippedExtensionListL( RCaeOrderedFeatureList& aDisabledExtensions );

        void SetSkippedExtensionList( RCaeOrderedFeatureList& aDisabledExtensions );

        void EnableVideoRecording();
        
        void DisableVideoRecording();

        TInt SetAsyncVideoStopMode( TBool aAsyncVideoStopEnabled );

    public: // New public methods

        /**
        * Checks if still image burst capture should be completed.
        * @since 2.6
        * @return Boolean indicating if the burst should be completed.
        */
        TBool CheckForStillBurstCompletion();
        
    private: // From Camera Observer / ECam.h

        void ReserveComplete( 
            TInt aError );

        void PowerOnComplete( 
            TInt aError );

        void ViewFinderFrameReady( 
            CFbsBitmap& aFrame );

        void ImageReady( 
            CFbsBitmap* aBitmap, 
            HBufC8*     aData, 
            TInt        aError );

        void FrameBufferReady( 
            MFrameBuffer* /*aFrameBuffer*/, 
            TInt          /*aError*/ ) {}; // Empty default because not in use.
   
    private: // From Video Recorder Utility Observer / VideoRecorder.h

        void MvruoOpenComplete( 
            TInt aError );

        void MvruoPrepareComplete( 
            TInt aError );

        void MvruoRecordComplete( 
            TInt aError );

        void MvruoEvent( 
            const TMMFEvent& aEvent );

    
    private: // Private member methods

        /**
        * C++ constructor.
        */
        CCaeEngineImp();

        /**
        * Symbian OS 2nd phase constructor that can leave.
        * @param aCameraIndex Index from 0 to CamerasAvailable()-1 inclusive 
        * specifying the camera device to use.
        * @param aDisplayIndex Display index specifying the display to use.
        * @return void
        */
        void ConstructL( TInt aCameraIndex, TInt aDisplayIndex );

        /**
        * Finds all extension dlls and populates the extension arrays.
        * @since 2.8
        * @return void
        */
		void PopulateExtensionArrayL();

    //// General and settings related private methods. ////

        /**
        * Initializes the info class ("structure").
        * @since 2.1
        * @param aCamera Camera object.
        * @return void
        */
        void InitializeInfo( 
            const CCamera& aCamera );

        /**
        * Checks that power is on. If not, then leaves.
        * @since 2.1
        * @return void
        */
        void CheckPowerL();

        /**
        * Cancels view finder, still capturing, and video recording.
        * @since 2.1
        * @return void
        */
        void CancelAllActivities();

        /**
        * Re-sets settings to the previous values (before releasing camera).
        * @since 2.1
        * @return void
        */
        void ResetToPreviousSettingsL();

    //// Video recording related private methods. ////

        /**
        * Cancels video recording.
        * @since 2.1
        * @return void
        */
        void CancelVideoRecording();

        /**
        * Prepares video recording settings.
        * @since 2.1
        * @return void
        */
        void PrepareVideoSettingsL();

        /**
        * Changes video clip file name.
        * @since 2.1
        * @return void
        */
        void ChangeVideoFileNameL();

        /**
        * Finds video type and format UIDs using ECOM.
        * @since 2.1
        * @param aMimeType MIME type.
        * @param aPreferredSupplier Preferred supplier of video encoder.
        * @return void
        */
        void FindVideoUidsL(
            const TDesC8& aMimeType, 
            const TDesC&  aPreferredSupplier );

        /**
        * Convert audio type from TDesC8 to TFourCC.
        * @since 2.1
        * @param aAudioType Audio type.
        * @return Audio type TFourCC code
        */
        TFourCC ConvertAndSetVideoAudioTypeL(
            const TDesC8& aAudioType );

        /**
        * Generates video recording times via
        * MCamAppEngineObserver::McaeoVideoRecordingTimes implemented by client.
        * @since 2.1
        * @param aEngineImp Camera Application Engine implementation object (this) 
        * @return ETrue
        */
        static TInt VideoRecordingTimesCallback( 
            TAny* aEngineImp );

    //// Still image burst capturing related private methods. ////

        /**
        * Gets option flag for still burst support.
        * @since 2.1
        * @return Option flag for still burst support
        */
        TCamAppEngineInfo::TOptions StillBurstSupport();

        /**
        * Compares two extension interface array items.
        * @since 2.8
        * @param aFirst First item
        * @param aSecond Second item
        * @return TInt Result of the comparison
        */
        static TBool MatchEqualImplUid( const TCaeExtensionInterfaceImplItem& aFirst, const TCaeExtensionInterfaceImplItem& aSecond );

        /**
        * Compares two extension interface array items.
        * @since 2.8
        * @param aFirst First item
        * @param aSecond Second item
        * @return TInt Result of the comparison
        */
        static TBool MatchEqualImplPtr( const TCaeExtensionInterfaceImplItem& aFirst, const TCaeExtensionInterfaceImplItem& aSecond );

        /**
        * Compares two extension interface array items.
        * @since 2.8
        * @param aFirst First item
        * @param aSecond Second item
        * @return TInt Result of the comparison
        */
        static TBool MatchSmallerInitialPriorityAndImplUid( const TCaeExtensionInterfaceImplItem& aFirst, const TCaeExtensionInterfaceImplItem& aSecond );

        /**
        * Compares two extension interface array items.
        * @since 2.8
        * @param aFirst First item
        * @param aSecond Second item
        * @return TInt Result of the comparison
        */
        static TBool Match( const TCaeOrderedFeatureListItem& aFirst, const TCaeOrderedFeatureListItem& aSecond );

        /**
        * Updates item order in the internal array or checks aOrderedFeatureArray validity.
        * @since 2.8
        * @param aOrderedFeatureArray Feature array sorted by the wanted calling order in an interface.
        * @param iUpdate Whether to update the internal raays or just to check validity of aOrderedFeatureArray.
        * @return void.
        */
		void DoSetOrderL( const RCaeOrderedFeatureList& aOrderedFeatureList, TBool iUpdate );

	private: // From MCaeEngineExtensionInterface

		TBool IsInterfaceSupported( TUid aInterfaceUid );

		TInt AddExtensionInterface( TUid aInterfaceUid, TUid aExtensionUid, TAny* aImplementationPtr, TInt aInitialPriority = 0 );

		TInt RemoveExtensionInterface( TUid aInterfaceUid, TAny* aImplementationPtr );

		TInt AddCustomInterface( TUid aInterfaceUid, TAny* aImplementationPtr );

		TAny* GetCallbackInterface( TUid aInterfaceUid );

        void RegisterFlags( TUint32 aFlags );

        void DeregisterFlags( TUint32 aFlags );

	private: // From MCaeExtEngineInfoCallbackInterface

		TSize McaeExtStillImageSize();

		TSize McaeExtSnapImageSize();

		TSize McaeExtViewFinderSize();

		void McaeExtCheckPowerL();

		TInt McaeExtDisplayIndex();

    private: // From Camera Observer2 / ECam.h

        void HandleEvent(const TECAMEvent& aEvent);

        void ViewFinderReady(MCameraBuffer& aCameraBuffer,TInt aError);

        void ImageBufferReady(MCameraBuffer& aCameraBuffer,TInt aError);
    
        void VideoBufferReady(MCameraBuffer& aCameraBuffer,TInt aError);

    private: // Data
    
    //// Pointers. ////

        // Camera API implementation object.
        CCamera* iCamera;

        // State machine for still capturing.
        CCaeStillStatesActive* iStillStatesActive; 

		// All CAE Extension modules
		RPointerArray<MExtension> iExtension;

		// All extension interface implementations.
		RArray<TCaeExtensionInterfaceImplListItem> iExtInterfaceImplementationLists;

		// All custom interface implementations.
		RArray<TCaeCustomInterfaceImplItem> iExtCustomInterfaceImplementations;

        // Video Recorder API implementation object.
        CVideoRecorderUtility* iVideoRecorder;

        // Camera Application Engine observer.
        MCamAppEngineObserver* iCaeObserver;

        // Camera Application Engine info object.
        TCamAppEngineInfo* iInfo;

        // Still image capturing quality levels container.
        CCaeStillQualityLevelsCont* iStillQualityLevelsCont;

        // Video recording quality levels container.
        CCaeVideoQualityLevelsCont* iVideoQualityLevelsCont;

        // Active object for calling Camera Application Engine methods 
        // indirectly from callbacks.
        CCaeCallbackActive* iCallbackActive;

        // Video recording frame size.
        // This is ptr because video recorder is not necessarily initialized.
        TSize* iVideoFrameSize;

        // Video recording frame size (to be prepared).
        // This is ptr because video recorder is not necessarily initialized.
        TSize* iVideoFrameSizePrep;

        // Video recording times generator.
        CCaeVideoTimes *iVideoTimes;

        // Video recording times callback.
        TCallBack* iVideoTimesCallback;

        // Video clip file name.
        HBufC* iVideoClipFileName;
        
        // Video Recorder video type.
        HBufC8* iVideoType;

    //// Booleans (mostly reporting state). ////

        // Boolean indicating if camera module is reserved for the engine.
        TBool iReserved;

        // Boolean indicating if camera module power is on.
        TBool iPowerOn;

        // Boolean indicating if video recording is initialized.
        TBool iVideoInitialized;

        // Boolean indicating if still image capture is prepared.
        TBool iStillPrepared;

        // Boolean indicating if video recording is opened.
        TBool iVideoOpened;

        // Boolean indicating if video recording is prepared.
        TBool iVideoPrepared;

        // Boolean indicating if view finder is currently running.
        TBool iViewFinderRunning;

        // Boolean indicating if video recording is currently running.
        TBool iVideoRecordingRunning;

        // Boolean indicating if video recording is currently paused.
        TBool iVideoRecordingPaused;

        // Boolean indicating if the engine should reset its basic settings 
        // to the previous ones when re-reserving.
        TBool iResetToPreviousSettings;

    //// Camera module settings related variables. ////

        // Current zoom mode.
        TZoomMode iZoomMode;

        // Current zoom value.
        TInt iZoomValue;

        // Current brightness value.
        TInt iBrightness;

        // Current contrast value.
        TInt iContrast;

        // Camera exposure mode.
        CCamera::TExposure iExposureMode;

        // Camera white balance mode.
        CCamera::TWhiteBalance iWhiteBalanceMode;

        // Camera flash mode.
        CCamera::TFlash iFlashMode;

    //// View finder related variables. ////

        // View finder size when viewfinder has been started.
        TSize iTrueViewFinderSize;

    //// Still capturing related variables. ////

        // Still image capture quality level index currently prepared.
        TInt iStillQualityIndex; 

        // Display index
        TInt iDisplayIndex;

      //// Video recording related variables. ////

        // Video recording quality level index currently prepared.
        TInt iVideoQualityIndex;

        // Handle to the camera used.
        TInt iCameraHandle;

        // Video Recorder controller UID.
        TUid iVideoControllerUid;

        // Video Recorder format UID.
        TUid iVideoFormatUid;

        // Video Recorder audio type.
        TFourCC iVideoAudioType;

        // Interval for calling McaeoVideoRecordingTimes.
        TTimeIntervalMicroSeconds32 iVideoTimesInterval;

		// Previous valid elapsed time value
    	TTimeIntervalMicroSeconds iPrevTimeElapsed;
    	
		// Previous valid remaining time value
    	TTimeIntervalMicroSeconds iPrevTimeRemaining;
    	
    //// Video recording parameters prepared. ////
        
        // Video recording frame rate.
        TReal32 iVideoFrameRate;

        // Video recording bit rate.
        TInt iVideoBitRate;

        // Boolean indicating if audio is enabled with video recording.
        TBool iVideoAudioEnabled;

        // Video recording audio bit rate.
        TInt iVideoAudioBitRate;

        // Video recording maximum clip size in bytes.
        TInt iMaxClipSizeInBytes;

    //// Video recording parameters to be prepared. ////

        // Video recording frame rate (to be prepared).
        TReal32 iVideoFrameRatePrep;

        // Video recording bit rate (to be prepared).
        TInt iVideoBitRatePrep;

        // Boolean indicating if audio is enabled with video recording (to be prepared).
        TBool iVideoAudioEnabledPrep;

        // Video recording audio bit rate (to be prepared).
        TInt iVideoAudioBitRatePrep;

        // Video recording maximum clip size in bytes (to be prepared).
        TInt iMaxClipSizeInBytesPrep;

        // Boolean indicating if parameters are/were prepared in video recording preparation..
        TBool iPrepPars;

        // Boolean indicating if video audio bit rate should be prepared.
        TBool iPrepareVideoAudioBitRate;
        
        // Boolean indicating if video stopping should be asynchronously or synchronously (default).
        TBool iAsyncVideoStopEnabled;
    };


#endif // CAEENGINEIMP_H
