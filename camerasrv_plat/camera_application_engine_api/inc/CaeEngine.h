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
* Description:  Camera Application Engine
*
*/



#ifndef CAEENGINE_H
#define CAEENGINE_H

//  INCLUDES
#include <e32base.h>
#include <gdi.h>
#include <ecam.h>

// FORWARD DECLARATIONS
class RWsSession;
class CWsScreenDevice;
class RWindowBase;
class CFbsBitmap;

// CONSTANTS

// Service UIDs. Services are usually implemented in plug-in extension modules.

const TUid KCaeProcessImageServiceUid = { 0x101F856A };
const TUid KCaeSettingsServiceUid = { 0x101F8569 };


// CLASS DECLARATIONS

/**
* The feature list item. The features that implement each service are sorted according 
* to the calling order in the engine.
*
* @lib CAEENGINE.LIB
* @since 2.8
*/

struct TCaeOrderedFeatureListItem
    {
    /** The UID of the service interface */
    TUid iServiceUid; 

    /** The UID of the feature. It identifies the extension that implements this service. */
    TUid iFeatureUid;
    };

typedef RArray<TCaeOrderedFeatureListItem> RCaeOrderedFeatureList;

/**
* Information about camera and the engine.
*
* @lib CAEENGINE.LIB
* @since 2.1
*/
class TCamAppEngineInfo
    {
    
    public:
        // Camera Application Engine options.
        enum TOptions
            {
            /** No options supported */
            ENoCaeOptionsSupported      = 0x0000, 
            /** Still capturing burst mode flag */
            EStillBurstSupported        = 0x0001,
            /** Ev compensation flag */
            EEvCompensationSupported    = 0x0002
            };

    public:

        // Camera hardware version present.
        TVersion iHardwareVersion;
        // Camera driver software version present.
        TVersion iSoftwareVersion;
        // Actual orientation of the camera.
        TCameraInfo::TCameraOrientation iOrientation;

        // Bitfield of CCamera::TOptions available. 
        TUint32 iOptionsSupported;
        // Bitfield of CCamera::TFlash modes available.
        TUint32 iFlashModesSupported;
        // Bitfield of CCamera::TExposure modes available.
        TUint32 iExposureModesSupported;
        // Bitfield of CCamera::TWhiteBalance modes available.
        TUint32 iWhiteBalanceModesSupported;

        // Minimum zoom value allowed. Must be negative or 
        // zero (for not supported).
        TInt iMinZoom;
        // Maximum zoom value allowed. Must be positive or 
        // zero (for not supported).
        TInt iMaxZoom;
        // Maximum digital zoom value allowed. Must be positive or 
        // zero (for not supported).
        TInt iMaxDigitalZoom;

        // Image size multiplier corresponding to minimum zoom value. 
        // Must be between 0 and 1 inclusive.
        TReal32 iMinZoomFactor;
        // Image size multiplier corresponding to maximum zoom value. 
        // Must be greater than or equal to 1.
        TReal32 iMaxZoomFactor;
        // Image size multiplier corresponding to maximum digital zoom value. 
        // Must be greater than or equal to 1.
        TReal32 iMaxDigitalZoomFactor;

        // Count of still image capturing sizes allowed.
        TInt iNumImageSizesSupported;
        // Bitfield of still image CCamera::TFormat values supported.
        TUint32 iImageFormatsSupported;
    
        // Count of still image capturing quality levels initialized.
        TInt iNumStillQualityLevelsSupported;
        // Count of video recording quality levels initialized.
        TInt iNumVideoQualityLevelsSupported;

        // Bitfield of CCaeEngine::TOptions available. 
        TUint32 iCaeOptionsSupported;

        // Integer (e.g. -9) that corresponds to minimum EV compensation value.
        TInt iMinEvCompensation;
        // Integer (e.g. 9) that corresponds to maximum EV compensation value.
        TInt iMaxEvCompensation;
        // Minimum camera EV compensation value (e.g. -3.0).
        TReal32 iMinEvCompensationValue;
        // Maximum camera EV compensation value (e.g. 3.0).
        TReal32 iMaxEvCompensationValue;
    };


/**
* Camera Application Engine observer class.
* Mixin base class for the clients of the engine.
*
* @lib CAEENGINE.LIB
* @since 2.1
*/
class MCamAppEngineObserver
    {

    public:

        /**
        * Called asynchronously when CCaeEngine::Init(), CCaeEngine::PowerOn(), 
        * or CCaeEngine::Reserve() completes.
        * Indicates if Camera Application Engine is ready for operation, 
        * the camera is reserved and its power is switched on.
        * @since 2.1
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code.
        * @return void
        */
        virtual void McaeoInitComplete( TInt aError ) = 0;

        /**
        * Called (possibly asynchronously) when preparing of still image 
        * capturing completes after 
        * PrepareStillCaptureL() has been called.
        * @since 2.1
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code.
        * @return void
        */
        virtual void McaeoStillPrepareComplete( TInt aError ) = 0;

        /**
        * Called asynchronously when preparing of video recording completes 
        * after PrepareVideoRecordingL() has been called.
        * May be called second time with an error code after a successful preparation 
        * if video recording loses its prepared state for some reason (e.g. audio HW 
        * is reserved for some other application).
        * @since 2.1
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code.
        * @return void
        */
        virtual void McaeoVideoPrepareComplete( TInt aError ) = 0;

        /**
        * Called asynchronously when a view finder bitmap is ready for display 
        * CCaeEngine::StartViewFinderBitmapsL(...) has been called. 
        * Called repeatedly until CCaeEngine::StopViewFinder() is called.  
        * The bitmap should be drawn synchronously as it will be re-used as soon as this function returns.
        * @since 2.1
        * @param aFrame Reference to a bitmap containing view finder data. Does not transfer ownership.
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code.
        * @return void
        */
        virtual void McaeoViewFinderFrameReady( CFbsBitmap& aFrame, 
            TInt aError ) = 0;

        /**
        * Called asynchronously when CCaeEngine::CaptureStill() is called and 
        * snap-image for displaying is ready.
        * @since 2.1
        * @param aBitmap Constant reference to a bitmap set on success for Symbian OS bitmap formats. 
        * Does not transfer ownership. Bitmap contents can not be changed.
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code.
        * @return void
        */
        virtual void McaeoSnapImageReady( const CFbsBitmap& aBitmap, 
            TInt aError ) = 0;

        /**
        * Called asynchronously when CCaeEngine::CaptureStill() is called and 
        * image for storaging is ready.
        * Depending on the prepared image data format, delivers either 
        * Symbian OS bitmap image or an image in another format, not both. 
        * The bitmap/the other format image must be used and deleted before 
        * returning.
        * @since 2.1
        * @param aBitmap Pointer to a bitmap transferring ownership and set on success 
        * for Symbian OS bitmap formats.
        * @param aData Pointer to image data transferring ownership and set on success 
        * for non-Symbian OS bitmap or other image data formats.
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code.
        * @return void
        */
        virtual void McaeoStillImageReady( CFbsBitmap* aBitmap, 
            HBufC8* aData, 
            TInt aError ) = 0;

        /**
        * Called (possibly asynchronously) when video recording is running 
        * after CCaeEngine::StartVideoRecording() or 
        * CCaeEngine::ResumeVideoRecording() has been called.
        * @since 2.1
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code.
        * @return void
        */
        virtual void McaeoVideoRecordingOn( TInt aError ) = 0;

        /**
        * Called (possibly asynchronously) when video recording is paused after 
        * CCaeEngine::PauseVideoRecording() has been called.
        * @since 2.1
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code.
        * @return void
        */
        virtual void McaeoVideoRecordingPaused( TInt aError ) = 0;

        /**
        * Called (possibly asynchronously) when video recording is completed 
        * after CCaeEngine::StopVideoRecording() has been called or 
        * recording has been completed for some other reason.
        * @since 2.1
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code.
        * if aError == KErrDiskFull, then disk storage is full.
        * if aError == KErrCompletion, then clip max size was reached.
        * @return void
        */
        virtual void McaeoVideoRecordingComplete( TInt aError ) = 0;

        /**
        * Called asynchronously and repeatedly with timed intervals 
        * when video clip recording is running after 
        * CCaeEngine::StartVideoRecording() or 
        * CCaeEngine::ResumeVideoRecording() has been called.
        * Also, called once by video recording pause operation 
        * by CCaeEngine::PauseVideoRecording() and stop operation 
        * by CCaeEngine::StopVideoRecording().
        * @since 2.1
        * @param aTimeElapsed Returns recording time elapsed from video recording start.
        * @param aTimeRemaining Returns estimated video recording time left 
        * limited by storage capacity or max clip size.
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code.
        * @return void
        */
        virtual void McaeoVideoRecordingTimes( 
            TTimeIntervalMicroSeconds aTimeElapsed, 
            TTimeIntervalMicroSeconds aTimeRemaining, 
            TInt aError ) = 0;

        /**
        * Called asynchronously when video recording is stopped and the CCaeEngine::SetAsyncVideoStopMode()
        * has been enabled. After this client may play sounds etc. while waiting the
        * McaeoVideoRecordingComplete call.
        * @since 5.0
        * @return void
        */
        virtual void McaeoVideoRecordingStopped() 
            {
            
            };
    };


/**
* Camera Application Engine still burst observer class.
* Mixin base class for the clients of Camera Application Engine.
*
* @lib CAEENGINE.LIB
* @since 2.1
*/
class MCaeStillBurstObserver
    {

    public:

        /**
        * Called asynchronously (multiple times) when still image burst 
        * capturing is running and an image is captured after 
        * CCaeEngine::CaptureStill() has been called. 
        * Called once for every still image capture during the burst (just before the capture).
        * @since 2.1
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code.
        * @return void
        */
        virtual void McaesboStillBurstCaptureMoment( TInt aError ) = 0;

        /**
        * Called asynchronously when still image burst capturing is completed 
        * after CCaeEngine::CaptureStill() has been called.
        * Before completion the engine has delivered all the captured images 
        * using McaeoSnapImageReady() and McaeoStillImageReady() methods of 
        * MCamAppEngineObserver.
        * @since 2.1
        * @param aImageCountDelivered Count of delivered burst images.
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code.
        * @return void
        */
        virtual void McaesboStillBurstComplete( 
            TInt aImageCountDelivered, 
            TInt aError ) = 0;
    };
    
    
/**
* The Camera Application Engine interface (abstract base class).
*
* @lib CAEENGINE.LIB
* @since 2.1
*/
class CCaeEngine : public CBase
    {
    
    public: // Enumerations

        // Possible operation modes.
        enum TOperationMode
            {
            EOperationModeStill     = 0x00, // Still capturing mode (default).
            EOperationModeVideo     = 0x01  // Video recording mode.
            };

        // Possible viewfinder types.
        enum TViewFinder
            {
            EViewFinderBitmaps      = 0x00, // Bitmap-based (default).
            EViewFinderDirect       = 0x01  // Direct Screen Access.
            };

        // Possible zooming modes.
        enum TZoomMode
            {
            EZoomModeDigital        = 0x00, // Digital zoom (default).
            EZoomModeOptical        = 0x01, // Optical zoom.
            EZoomModeOpticalDigital = 0x02  // Optical+digital zoom.
            };

        // Possible snap image generating modes.
        enum TSnapImageSource
            {
            ESnapImageSourceOriginal, // Create snap image from the captured (JPEG/Exif) image.
            ESnapImageSourceThumbnail // Create snap image from the thumbnail of the captured (Exif) image.
            };

    public: // Creation

        /**
        * A static (factory) function to create the class instance.
        * @since 2.1
        * @return Pointer to a fully constructed CCaeEngine object. 
        * Ownership is passed to the caller.
        */
        IMPORT_C static CCaeEngine* NewL();

        /**
        * Sets the Camera Application Engine observer.
        * @since 2.1
        * @param aObserver Reference to Camera Application Engine observer.
        * @return void
        */
        virtual void SetCamAppEngineObserver( 
            MCamAppEngineObserver& aObserver ) = 0;

    public: // General methods

        /**
        * Gets information about the supported Camera Application Engine and 
        * Camera features.
        * @since 2.1
        * @param aInfo Info about supported features.
        * @return void
        */
        virtual void GetInfo( TCamAppEngineInfo& aInfo ) const = 0;

        /**
        * Initializes Camera Application Engine for still image capturing. 
        * Reserves the camera and switches the camera power on. 
        * Intialization must be done before all other operations, except 
        * SetCamAppEngineObserver and GetInfo. 
        * Calls observer method McaeoInitComplete().
        * @since 2.1
        * @param aCreateSnapImage Boolean indicating if "snap-images" 
        * should be created, leading to calls of McaeoSnapImageReady.
        * @return void
        */
        virtual void InitL( TBool aCreateSnapImage = ETrue ) = 0;

        /**
        * Initialize the engine for still image capturing. 
        * Switches also camera power on. 
        * Intialization must be done before all other operations, except 
        * SetCamAppEngineObserver and GetInfo. 
        * Calls observer method McaeoInitComplete().
        * @since 2.1
        * @param aSdUidStillQltyLevels Still capturing quality levels Shared Data initialization file UID.
        * @param aCreateSnapImage Boolean indicating if "snap-images" should be created 
        * leading to calls of McaeoSnapImageReady.
        * @return void
        */
        virtual void InitL( TUid aSdUidStillQltyLevels, 
                            TBool aCreateSnapImage = ETrue ) = 0;

        /**
        * Initializes video recording. 
        * The initialization must be done before video recording operations.
        * @since 2.1
        * @return void
        */
        virtual void InitVideoRecorderL() = 0;

        /**
        * Initializes video recording. 
        * The initialization must be done before video recording operations.
        * @since 2.1
        * @param aSdUidVideoQltyLevels Video recording quality levels Shared Data initialization file UID.
        * @return void
        */
        virtual void InitVideoRecorderL( TUid aSdUidVideoQltyLevels ) = 0;

        /**
        * Reserves the camera into use and switches the camera power on. 
        * Calls observer method McaeoInitComplete().
        * @since 2.1
        * @return void
        */
        virtual void Reserve() = 0;
        
        /**
        * Switches the camera power off and releases the camera.
        * @since 2.1
        * @return void
        */
        virtual void Release() = 0;

        /**
        * Switches the camera power on. 
        * Calls observer method McaeoInitComplete().
        * @since 2.1
        * @return void
        */
        virtual void PowerOn() = 0;
        
        /**
        * Switches the camera power off.
        * @since 2.1
        * @return void
        */
        virtual void PowerOff() = 0;

    public: // Camera settings

        /**
        * Sets the zoom mode.
        * @since 2.1
        * @param aZoomMode Zoom mode to set.
        * @return void
        */
        virtual void SetZoomModeL( TZoomMode aZoomMode = EZoomModeDigital ) = 0;

        /**
        * Gets the current zoom mode.
        * @since 2.1
        * @return The current zoom mode.
        */
        virtual TZoomMode ZoomMode() const = 0;

        /**
        * Sets the zoom value.
        * @since 2.1
        * @param aZoomValue Index of the zoom factor to set.
        * @return void
        */
        virtual void SetZoomValueL( TInt aZoomValue = 0 ) = 0;

        /**
        * Gets the current zoom value.
        * @since 2.1
        * @return Index of the current zoom factor.
        */
        virtual TInt ZoomValue() const = 0;

        /**
        * Sets the brightness.
        * @since 2.1
        * @param aBrightness Brightness value in the range ECam.h SetBrightnessL allows.
        * @return void
        */
        virtual void SetBrightnessL( TInt aBrightness = 0 ) = 0; 

        /**
        * Gets the current brightness setting.
        * @since 2.1
        * @return Brightness value.
        */
        virtual TInt Brightness() const = 0;

        /**
        * Sets the contrast.
        * @since 2.1
        * @param aContrast Contrast value in the range ECam.h SetContrastL allows.
        * @return void
        */
        virtual void SetContrastL( TInt aContrast = 0 ) = 0; 
        
        /**
        * Gets the current contrast setting.
        * @since 2.1
        * @return Contrast value.
        */
        virtual TInt Contrast() const = 0;

        /**
        * Sets the exposure mode.
        * @since 2.1
        * @param aExposureMode Exposure mode in the range ECam.h SetExposureL allows.
        * @return void
        */
        virtual void SetExposureModeL( 
            CCamera::TExposure aExposureMode = CCamera::EExposureAuto ) = 0;

        /**
        * Gets the current exposure mode.
        * @since 2.1
        * @return Exposure mode.
        */
        virtual CCamera::TExposure ExposureMode() const = 0;

        /**
        * Sets the white balance mode.
        * @since 2.1
        * @param aWhiteBalanceMode White balance mode in the range ECam.h SetWhiteBalanceL allows.
        * @return void
        */
        virtual void SetWhiteBalanceModeL( 
            CCamera::TWhiteBalance aWhiteBalanceMode = CCamera::EWBAuto ) = 0;

        /**
        * Gets the current white balance mode.
        * @since 2.1
        * @return White balance mode.
        */
        virtual CCamera::TWhiteBalance WhiteBalanceMode() const = 0;

        /**
        * Sets the flash mode.
        * @since 2.1
        * @param aFlashMode Flash mode in the range ECam.h SetFlashL allows.
        * @return void
        */
        virtual void SetFlashModeL( 
            CCamera::TFlash aFlashMode = CCamera::EFlashNone ) = 0;

        /**
        * Gets the current flash mode.
        * @since 2.1
        * @return Flash mode.
        */
        virtual CCamera::TFlash FlashMode() const = 0;

        /**
        * Resets to the default setting values.
        * Resets the following settings: exposure mode, white balance mode, 
        * zoom mode, zoom value, flash mode, brightness, and contrast. 
        * @since 2.1
        * @return void
        */
        virtual void ResetToDefaultsL() = 0;
    
    public: // View finder

        /**
        * Sets/resets view finder image mirroring.
        * @since 2.1
        * @param aMirror Boolean indicating whether view finder image should be mirrored or not.
        * @return void
        */
        virtual void SetViewFinderMirrorL( TBool aMirror = EFalse ) = 0;

        /**
        * Queries if view finder image mirroring is on.
        * @since 2.1
        * @return Boolean indicating whether view finder image is currently mirrored or not.
        */
        virtual TBool ViewFinderMirror() const = 0;

        /**
        * Starts the bitmap-based view finder.
        * The engine starts to call observer method McaeoViewFinderFrameReady() repeatedly.
        * @since 2.1
        * @param aSize Desired view finder size.
        * @return void
        */
        virtual void StartViewFinderBitmapsL( TSize& aSize ) = 0;

        /**
        * Starts the bitmap-based view finder.
        * The engine starts to call observer method McaeoViewFinderFrameReady() repeatedly.
        * @since 2.1
        * @param aSize Desired view finder size.
        * @param aCropRect Desired cropping/clipping rectangle.
        * @return void
        */
        virtual void StartViewFinderBitmapsL(
            TSize& aSize, 
            TRect& aCropRect ) = 0;

        /**
        * Starts the direct screen access view finder.
        * @since 2.1
        * @param aWs Window server session.
        * @param aScreenDevice Software device screen.
        * @param aWindow Client-side handle to a server-side window.
        * @param aScreenRect Portion of screen in screen co-ordinates.
        * @return void
        */
        virtual void StartViewFinderDirectL( 
            RWsSession& aWs, 
            CWsScreenDevice& aScreenDevice, 
            RWindowBase& aWindow, 
            TRect& aScreenRect ) = 0;
        
        /**
        * Starts the direct screen access view finder.
        * @since 2.1
        * @param aWs Window server session.
        * @param aScreenDevice Software device screen.
        * @param aWindow Client-side handle to a server-side window.
        * @param aScreenRect Portion of screen in screen co-ordinates.
        * @param aCropRect Desired cropping/clipping rectangle.
        * @return void
        */
        virtual void StartViewFinderDirectL(
            RWsSession& aWs, 
            CWsScreenDevice& aScreenDevice, 
            RWindowBase& aWindow, 
            TRect& aScreenRect, 
            TRect& aCropRect ) = 0;

        /**
        * Stops the view finder.
        * @since 2.1
        * @return void
        */
        virtual void StopViewFinder() = 0;

        /**
        * Queries if view finder is running.
        * @since 2.1
        * @return Boolean indicating whether view finder is running or not.
        */
        virtual TBool IsViewFinding() const = 0;

        /**
        * Gets the current view finder size.
        * @since 2.1
        * @param aSize View finder size.
        * @return void
        */
        virtual void GetViewFinderSize( TSize& aSize ) = 0;

    public: // Still image capturing

        /**
        * Prepares Camera Application Engine for still image capture on 
        * specified quality level.
        * Calls observer method McaeoStillPrepareComplete().
        * @since 2.1
        * @param aStillQualityIndex Still image quality level index.
        * Imaging parameters linked to quality levels are defined by Shared Data ini-file. 
        * @return void
        */
        virtual void PrepareStillCaptureL( TInt aStillQualityIndex ) = 0;

        /**
        * Prepares Camera Application Engine for still image capture on 
        * specified quality level.
        * Calls observer method McaeoStillPrepareComplete().
        * @since 2.1
        * @param aStillQualityIndex Still image quality level index. 
        * Can have values 0 ... TCamAppEngineInfo::iNumStillQualityLevelsSupported - 1
        * @param aCropRect Desired cropping/clipping rectangle.
        * @return void
        */
        virtual void PrepareStillCaptureL( 
            TInt aStillQualityIndex, 
            const TRect& aCropRect ) = 0;

        /**
        * Prepares Camera Application Engine for still image capture with 
        * specified image size and format.
        * Calls observer method McaeoStillPrepareComplete().
        * @since 2.1
        * @param aSize Still image size.
        * @param aFormat Still image format.
        * @param aCompressionQuality Still image compression quality [0...100].
        * @return void
        */
        virtual void PrepareStillCaptureL(
            const TSize& aSize, 
            CCamera::TFormat aFormat,  
            TInt aCompressionQuality ) = 0;

        /**
        * Prepares Camera Application Engine for still image capture with specified image size and format.
        * Calls observer method McaeoStillPrepareComplete().
        * @since 2.1
        * @param aSize Still image size.
        * @param aFormat Still image format.
        * @param aCompressionQuality Still image compression quality [0...100].
        * @param aCropRect Desired cropping/clipping rectangle.
        * @return void
        */
        virtual void PrepareStillCaptureL(
            const TSize& aSize, 
            CCamera::TFormat aFormat,  
            TInt aCompressionQuality, 
            const TRect& aCropRect ) = 0;

        /**
        * Gets the current prepared still quality level index.
        * @since 2.1
        * @return Still quality level index.
        */
        virtual TInt StillQualityIndex() const = 0;

        /**
        * Gets the still image frame size for the specified quality level.
        * @since 2.1
        * @param aStillQualityIndex Still capture quality index.
        * @param aSize Still image frame size.
        * @return void
        */
        virtual void GetStillFrameSize(
            TInt aStillQualityIndex, 
            TSize& aSize ) const = 0;

        /**
        * Gets the estimated still image size in bytes for the 
        * specified quality level.
        * @since 2.1
        * @param aStillQualityIndex Still capture quality index.
        * @return Estimated still capturing image size in bytes.
        */
        virtual TInt EstimatedStillSizeInBytes( 
            TInt aStillQualityIndex ) const = 0;

        /**
        * Captures a still image or a still image burst.
        * Calls first observer method McaeoSnapImageReady() and then 
        * McaeoStillImageReady().
        * McaeoSnapImageReady() is not called if engine initialization 
        * has been done 
        * with parameter aCreateSnapImage == EFalse.
        * @since 2.1
        * @return void
        */
        virtual void CaptureStill() = 0;

        /**
        * Cancels the on-going still image (single/burst) capture request.
        * @since 2.1
        * @return void
        */
        virtual void CancelCaptureStill() = 0;

    public: // Video recording

        /**
        * Sets the file name for the video clip to be recorded.
        * @since 2.1
        * @param aVideoClipFileName File name.
        * @return void
        */
        virtual void SetVideoRecordingFileNameL( 
            const TDesC& aVideoClipFileName ) = 0;

        /**
        * Prepares Camera Application Engine for video recording on 
        * specified quality level.
        * Calls observer method McaeoVideoPrepareComplete().
        * @since 2.1
        * @param aVideoQualityIndex Video quality level index. 
        * Can have values 0 ... TCamAppEngineInfo::iNumVideoQualityLevelsSupported - 1
        * @return void
        */
        virtual void PrepareVideoRecordingL( TInt aVideoQualityIndex ) = 0;

        /**
        * Prepares Camera Application Engine for video recording with 
        * specified recording parameters.
        * Calls observer method McaeoVideoPrepareComplete().
        * @since 2.1
        * @param aFrameSize Frame size.
        * @param aFrameRate Frame rate.
        * @param aBitRate Bit rate.
        * @param aAudioEnabled Boolean indicating whether audio should be enabled or not.
        * @param aMimeType MIME type.
        * @param aPreferredSupplier Preferred supplier of video encoder.
        * @param aVideoType Video type.
        * @param aAudioType Audio type.
        * @return void
        */
        virtual void PrepareVideoRecordingL( 
            const TSize& aFrameSize, 
            TReal32 aFrameRate, 
            TInt aBitRate, 
            TBool aAudioEnabled,
            const TDesC8& aMimeType, 
            const TDesC& aPreferredSupplier, 
            const TDesC8& aVideoType = KNullDesC8, 
            const TDesC8& aAudioType = KNullDesC8 ) = 0;
        /**
        * Closes (prepared) video recording to free resources.
        * @since 2.1
        * @return void
        */
        virtual void CloseVideoRecording() = 0;

        /**
        * Gets the current prepared video quality level index.
        * @since 2.1
        * @return Video quality level index.
        */
        virtual TInt VideoQualityIndex() const = 0;
        
        /**
        * Gets the video frame size for the specified video quality level.
        * @since 2.1
        * @param aVideoQualityIndex Video quality level index.
        * @return Video frame size.
        */
        virtual void GetVideoFrameSize(
            TInt aVideoQualityIndex, 
            TSize& aSize ) const = 0;

        /**
        * Gets the video frame rate for the specified video quality level.
        * @since 2.1
        * @param aVideoQualityIndex Video quality level index.
        * @return Video frame rate.
        */
        virtual TReal32 VideoFrameRate( TInt aVideoQualityIndex ) const = 0;

        /**
        * Gets the estimated video recording bit rate including audio bit rate 
        * for the specified video quality level.
        * @since 2.1
        * @param aVideoQualityIndex Video quality level index.
        * @return Video recording bit rate rate including audio bit rate.
        */
        virtual TInt EstimatedVideoRecordingBitRateL( 
            TInt aVideoQualityIndex ) const = 0;

        /**
        * Sets the maximum size in bytes for a video clip.
        * @since 2.1
        * @param aMaxClipSizeInBytes Maximum video clip size measured in bytes.
        * If set to 0, then no limit but the available storage space.
        * @return void
        */
        virtual void SetVideoClipMaxSizeL( TInt aMaxClipSizeInBytes = 0 ) = 0;
        
        /**
        * Gets the current maximum size in bytes for a video clip.
        * @since 2.1
        * @return Video clip maximum size.
        */
        virtual TInt VideoClipMaxSize() const = 0;

        /**
        * Sets audio enabled or disabled for video recording.
        * @since 2.1
        * @param aAudioEnabled Boolean indicating whether audio should be enabled or not for video recording.
        * @return void
        */
        virtual void SetVideoAudioL( TBool aAudioEnabled ) = 0;
        
        /**
        * Queries if audio is enabled or disabled for video recording.
        * @since 2.1
        * @return Boolean indicating whether audio is enabled or disabled for video recording.
        */
        virtual TBool VideoAudio() const = 0;

        /**
        * Sets the time interval for video recording time info.
        * @since 2.1
        * @param aInterval Time interval in microseconds for video recording time info.
        * If zero is given as interval, then video recording time is not generated.
        * @return void
        */
        virtual void SetVideoTimesIntervalL( 
            TTimeIntervalMicroSeconds aInterval ) = 0;

        /**
        * Gets the current time interval for video recording time info.
        * @since 2.1
        * @return Time interval in microseconds for video recording time info.
        */
        virtual TTimeIntervalMicroSeconds VideoTimesInterval() const = 0;

        /**
        * Starts video recording.
        * Calls observer method McaeoVideoRecordingOn().
        * @since 2.1
        * @return void
        */
        virtual void StartVideoRecording() = 0;

        /**
        * Stops video recording.
        * Calls observer method McaeoVideoRecordingComplete().
        * @since 2.1
        * @return void
        */
        virtual void StopVideoRecording() = 0;

        /**
        * Pauses video recording.
        * Calls observer method McaeoVideoRecordingPaused().
        * @since 2.1
        * @return void
        */
        virtual void PauseVideoRecording() = 0;

        /**
        * Resumes video recording after pause.
        * Calls observer method McaeoVideoRecordingOn().
        * @since 2.1
        * @return void
        */
        virtual void ResumeVideoRecording() = 0;

        /**
        * Gets the remaining video recording time before recording has started.
        * @since 2.1
        * @return Remaining video recording time.
        */
        virtual TTimeIntervalMicroSeconds RemainingVideoRecordingTime() 
            const = 0;

        /**
        * Queries if video recording is running.
        * @since 2.1
        * @return Boolean indicating whether video recording is running or not.
        */
        virtual TBool IsVideoRecording() const = 0;

    public: // New overloading method for preparing video recording

        /**
        * Prepares Camera Application Engine for video recording with specified recording parameters.
        * Calls observer method McaeoVideoPrepareComplete().
        * @since 2.1
        * @param aFrameSize Frame size.
        * @param aFrameRate Frame rate.
        * @param aBitRate Bit rate.
        * @param aAudioEnabled Boolean indicating whether audio should be enabled or not.
        * @param aAudioBitRate Audio bit rate.
        * @param aMimeType MIME type.
        * @param aPreferredSupplier Preferred supplier of video encoder.
        * @param aVideoType Video type.
        * @param aAudioType Audio type.
        * @return void
        */
        virtual void PrepareVideoRecordingL(
            const TSize&  aFrameSize, 
            TReal32       aFrameRate, 
            TInt          aBitRate, 
            TBool         aAudioEnabled,
            TInt          aAudioBitRate, 
            const TDesC8& aMimeType, 
            const TDesC&  aPreferredSupplier, 
            const TDesC8& aVideoType, 
            const TDesC8& aAudioType ) = 0;

    public: // Still image burst capturing

        /**
        * Sets the Camera Application Engine still image burst capturing 
        * observer.
        * @since 2.1
        * @param aObserver Camera Application Engine still burst observer.
        * @return void
        */
        virtual void SetCaeStillBurstObserver( 
            MCaeStillBurstObserver& aObserver ) = 0;

        /**
        * Sets the image count for still image (burst) capturing.
        * Note: Inputting value 1 switches back to normal still capturing.
        * @since 2.1
        * @param aImageCount The desired count of images to capture (in a burst). 
        * @return The count of images to be captured (in a burst).
        */
        virtual TInt SetStillCaptureImageCountL( TInt aImageCount ) = 0;

        /**
        * Gets the image count for still image (burst) capturing.
        * @since 2.1
        * @return Count of images to capture (in a burst).
        */
        virtual TInt StillCaptureImageCount() const = 0;

        /**
        * Sets the time interval between single captures in still burst.
        * @since 2.1
        * @param aInterval Time interval in microseconds.
        * @return void
        */
        virtual void SetStillBurstCaptureIntervalL( 
            TTimeIntervalMicroSeconds aInterval ) = 0;

        /**
        * Gets the time interval between single captures in still burst.
        * @since 2.1
        * @return Time interval in microseconds.
        */
        virtual TTimeIntervalMicroSeconds StillBurstCaptureInterval() const = 0;

        /**
        * Stops still image burst capturing. 
        * Delivers the burst images captured before call. 
        * This is different from CancelCaptureStill that cancels the operation and 
        * prevents delivering the captured image.
        * @since 2.1
        * @return void
        */
        virtual void StopStillBurstCapture() = 0;

    public: 

        /**
        * Prepare Camera Application Engine for still image capture on 
        * specified quality level with desired snap image size. 
        * The returned snap image size is always equal or bigger in both 
        * dimensions than the given size. The returned size is stepwise 
        * downscaled from the captured image size. The parameter aSnapSize 
        * is ignored if the image format does not support it in the engine.
        * Calls observer method McaeoStillPrepareComplete().
        * @since 2.1
        * @param aStillQualityIndex Still image quality level index. 
        * Can have values 0 ... TCamAppEngineInfo::iNumStillQualityLevelsSupported - 1
        * @param aSnapSize Desired snap image size. Returns the real snap image size.
        * @return void
        */
        virtual void PrepareStillCaptureL(
            TInt aStillQualityIndex, 
            TSize& aSnapSize ) = 0;

        /**
        * Prepare Camera Application Engine for still image capture with 
        * specified image size, format, and desired snap image size. 
        * The returned snap image size is always equal or bigger in both 
        * dimensions than the given size. The returned size is stepwise 
        * downscaled  from the captured image size. The parameter aSnapSize 
        * is ignored if the image format does not support it in the engine.
        * Calls observer method McaeoStillPrepareComplete().
        * @since 2.1
        * @param aSize Still image size.
        * @param aFormat Still image format.
        * @param aCompressionQuality Still image compression quality [0...100].
        * @param aCropRect Desired cropping/clipping rectangle. The size (0,0) means that this parameter is ignored.
        * @param aSnapSize Desired snap image size. Returns the real snap image size.
        * @return void
        */
        virtual void PrepareStillCaptureL(
            const TSize& aSize, 
            CCamera::TFormat aFormat,  
            TInt aCompressionQuality, 
            const TRect& aCropRect,
            TSize& aSnapSize ) = 0;

        /**
        * Sets the exposure value (EV) compensation index.
        * @since 2.1
        * @param aEvIndex Exposure value compensation index to set.
        * @return void
        */
        virtual void SetEvCompensationL( TInt aEvIndex = 0 ) = 0;

        /**
        * Gets the current exposure value (EV) compensation index.
        * @since 2.1
        * @return Exposure value compensation index.
        */
        virtual TInt EvCompensation() const = 0;
                
        /**
        * Sets snap image creation on/off.
        * @since 2.1
        * @param aCreateSnapImage Boolean indicating if "snap-images" for
        * displaying should be created, leading to calls of McaeoSnapImageReady.
        * @return void
        */
        virtual void SetSnapImageCreation( 
            TBool aCreateSnapImage = ETrue ) = 0;
        
        /** 
        * A static (factory) function to create the class instance.
        * @since 2.6
        * @param aCameraIndex Index from 0 to CamerasAvailable()-1 inclusive 
        * specifying the camera device to use.
        * @return Pointer to a fully constructed CCaeEngine object. 
        * Ownership is passed to the caller.
        */
        IMPORT_C static CCaeEngine* NewL( 
            TInt aCameraIndex );
        
        /** 
        * Gets the number of cameras on the device.
        * @since 2.6
        * @return Count of cameras present on the device. 
        */
        IMPORT_C static TInt CamerasAvailable();
        
        /** 
        * Enumerates through the available image capture sizes. 
        * Returns the image size based on the specified size index and format. 
        * The largest image resolution is returned for index 0, the smallest 
        * for index TCamAppEngineInfo::iNumImageSizesSupported-1.
        * The size index must be in the range 0 to 
        * TCamAppEngineInfo::iNumImageSizesSupported-1 inclusive.
        * @since 2.6
        * @param aSize Returned image size.
        * @param aSizeIndex Size index.
        * @param aFormat The image format. 
        * @return void
        */
        virtual void EnumerateStillCaptureSizes(
            TSize& aSize, 
            TInt aSizeIndex, 
            CCamera::TFormat aFormat ) const = 0;
        
        /** 
        * Sets the quality value to use with JPEG and Exif image formats. 
        * @since 2.6
        * @param aQuality The quality value to use, clamped to the range 1 to 100.
        * @return void
        */
        virtual void SetJpegQuality(
            TInt aQuality ) = 0;
        
        /** 
        * Gets the currently set JPEG quality value.
        * Returns 0 if not previously prepared or set.
        * @since 2.6
        * @return The currently set JPEG quality value. 
        */
        virtual TInt JpegQuality() const = 0;

        /**
        * Gets a custom interface. The client has to cast the returned pointer.
        * @since 2.6
        * @return  "TAny*" Custom interface pointer. NULL if the requested interface is not supported.
        */
        virtual TAny* CustomInterface(
        	TUid aInterface ) = 0;

        /** 
        * Sets the size of the snap image bitmap.
        * @since 2.8
        * @param aSize The size of the snap bitmap. Corrected size is returned. 
        * @return void
        */
        virtual void SetSnapImageSizeL(
            TSize& aSize ) = 0;
         /** 
        * Sets the color mode of the snap image bitmap
        * @since 2.8
        * @param aMode The color mode of the snap bitmap.
        * @return void
        */
        virtual void SetSnapImageColorMode(
            TDisplayMode aMode ) = 0;

        /**
        * Gets array of features per interfaces. Each feature is ordered in
        * the current calling order e.g. when certain image prosessing 
        * operation is applied to the captured image. If SetOrder() has not yet
        * been called, the returned order is the default order.
        * @since 2.8
        * @param aOrderedFeatureArray Feature array sorted by the current calling order in an interface.
        * @return  void
        */
        virtual void GetOrderL( RCaeOrderedFeatureList& aOrderedFeatureList ) = 0;

        /**
        * Sets array of features per interfaces. Each feature should be ordered
        * according to the wanted calling order e.g. to define when certain image 
        * prosessing operation is applied to the captured image. If this method
        * leaves, the internal array has not been changed.
        * @since 2.8
        * @param aOrderedFeatureArray Feature array sorted by the wanted calling order in an interface.
        * @return void. 
        */
        virtual void SetOrderL( const RCaeOrderedFeatureList& aOrderedFeatureList ) = 0;

        /** 
        * A static (factory) function to create the class instance.
        * @since 2.6
        * @param aCameraIndex Index from 0 to CamerasAvailable()-1 inclusive 
        * specifying the camera device to use.
        * @param aDisplayIndex Display index specifying the display to use.
        * @return Pointer to a fully constructed CCaeEngine object. 
        * Ownership is passed to the caller.
        */
        IMPORT_C static CCaeEngine* NewL( 
            TInt aCameraIndex, TInt aDisplayIndex );
        /** 
        * Sets the source of the snap image bitmap.
        * @since 2.8
        * @param aSnapImageSource The source of the snap image bitmap.
        * @return void
        */
        virtual void SetSnapImageSourceL(
            TSnapImageSource aSnapImageSource ) = 0;

        /** 
        * Sets the specific image codec implementation to be used in decoding and encoding.
        * @since 3.1
        * @param aDecoderUid The UID of the specific image decoder to be used. KNullUid means the default decoder.
        * @param aEncoderUid The UID of the specific image encoder to be used. KNullUid means the default encoder.
        * @return void
        */
        virtual void SetImageCodecsL( 
        	TUid aDecoderUid, TUid aEncoderUid ) = 0;

        /** 
        * Enumerate Video Frame Size.
        * @since 3.2
        * @param aSize enumerated size
        * @return size index of camera HW. Returns -1 if aSize is not supported.
        */
        virtual TInt EnumerateVideoFrameSizeL(const TSize& aSize) = 0;
        
        /** 
        * Gets the device-unique handle of camera object owned by the CCaeEngine.
        * @since 3.2
        * @return  Camera object handle
        */
        virtual TInt CCameraHandle() const = 0;

        /**
        * Set CamAppEngine internal camera state to reserved and power state on. 
        * This method is used when client uses Camera-API directly through the duplicate instance in still mode,
        * but wants to use CAE in video recording mode without using the CAE InitL.
        * @since 3.2
        * @return void
        */
        virtual void EnableVideoRecording() = 0;
        
        /**
        * Set CamAppEngine internal camera state to released and power state off.
        * @since 3.2
        * @return void
        */
        virtual void DisableVideoRecording() = 0;

        /** 
        * Sets CAE to Extension processing mode.
        * @since 3.2
        * @param aExtModeActive Boolean indicating if extension mode is enabled or disabled.
        * @param aCreateSnapImage Boolean indicating if "snap-images" should be created by CAE.
        * @return void
        */
        virtual void SetCaeExtensionModeL( TBool aExtModeActive, TBool aCreateSnapImage ) = 0;

        /** 
        * Process extension for view finder.
        * @since 3.2
        * @param aFrame The view finder frame bitmap that is processed.
        * @return void
        */
        virtual void ProcessExtViewFinderFrameReadyL( CFbsBitmap& aFrame ) = 0;

        /** 
        * Process extension for snap image.
        * @since 3.2
        * @param aSnapImage The bitmap of snap image. Does not transfer ownership.
        * @return void
        */
        virtual void ProcessExtSnapImageL( CFbsBitmap& aSnapImage ) = 0;
        
        /** 
        * Process extension for captured image.
        * @since 3.2
        * @param aBitmap The captured bitmap image. Does not transfer ownership.
        * @param aLastImage Set when last image of the burst is completed.
        * @return void
        */
        virtual void ProcessExtCapturedImageL( CFbsBitmap& aBitmap, TBool aLastImage ) = 0;

        /** 
        * Process extension for captured image.
        * @since 3.2
        * @param aImageData The captured image, if it is a formatted image and type of HBufC8. Ownership is transferred.
        * @param aLastImage Set when last image of the burst is completed.
        * @return void
        */
        virtual void ProcessExtCapturedImageL( HBufC8* aImageData, TBool aLastImage ) = 0;

        /** 
        * Process extension for captured image.
        * @since 3.2
        * @param aImageDataDes The captured image, if it is a formatted image and type of TDesC8. Does not transfer ownership.
        * @param aLastImage Set when last image of the burst is completed.
        * @return void
        */
        virtual void ProcessExtCapturedImageL( TDesC8& aImageDataDes, TBool aLastImage ) = 0;

        /** 
        * Cancels extensions processing
        * @since 3.2
        * @return void
        */
        virtual void ProcessExtCancel() = 0;

        /** 
        * Get list of skipped extensions. Application may define which extensions are skipped during image processing. 
        * @since 3.2
        * @param aSkippedExtensions The list of skipped extensions TUid. 
        * @return void
        */
        virtual void SkippedExtensionListL( RCaeOrderedFeatureList& aSkippedExtensions ) = 0;

        /** 
        * Set list of skipped extensions. Application may define which extensions are skipped during image processing.
        * The GetOrderL() returns the list of all installed extensions.
        * @since 3.2
        * @param aSkippedExtensions The list of installed and skipped extensions TUid. 
        * @return void
        */
        virtual void SetSkippedExtensionList( RCaeOrderedFeatureList& aSkippedExtensions ) = 0;

        /** 
        * Set asynchronous video recording stop mode. When Enabled then StopVideoRecording will return quickly and
        * MCamAppEngineObserver callbacks McaeoVideoRecordingStopped and McaeoVideoRecordingComplete
        * are called asynchronously.
        * @since 5.0
        * @param aAsyncVideoStopEnabled The async stop state. 
        * @return error code KErrNone if supported by the video framework and mode was changed.
        */
        virtual TInt SetAsyncVideoStopMode( TBool aAsyncVideoStopEnabled ) = 0;

    };

#endif // CAEENGINE_H
