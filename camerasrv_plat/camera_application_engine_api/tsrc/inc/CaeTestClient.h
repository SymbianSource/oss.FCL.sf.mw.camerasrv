/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: CaeTestClient.h
*
*/
#ifndef CAE_TEST_CLIENT_H
#define CAE_TEST_CLIENT_H

#include <e32base.h>
#include <f32file.h>
#include <gdi.h>


#include "CaeEngine.h"

#include "CaeTestVariation.h"

#include "ECamColorToneCustomInterface.h"

#ifdef CAE_TEST_VERSION
#include "CaeEngineImpTestErrors.h"
#include "CaeStillConverterTestErrors.h"
#endif

const TInt KNormalTimingDelay = 1000;
const TInt KLongTimingDelay = 5*1000;

const TInt KImgWidthSubQCIF  = 128; 
const TInt KImgHeightSubQCIF = 96; 

const TInt KImgWidthQCIF     = 176; 
const TInt KImgHeightQCIF    = 144; 

const TInt KImgWidthCIF      = 352; 
const TInt KImgHeightCIF     = 288; 

const TInt KImgWidthQQVGA    = 160; 
const TInt KImgHeightQQVGA   = 120; 

const TInt KImgWidthQVGA     = 320; 
const TInt KImgHeightQVGA    = 240; 

const TInt KImgWidthVGA      = 640; 
const TInt KImgHeightVGA     = 480; 

const TInt KImgWidthSVGA      = 800; 
const TInt KImgHeightSVGA     = 600; 

const TInt KImgWidthTooBig   = 4096; 
const TInt KImgHeightTooBig  = 4096; 

const TInt KImgWidthMP1      = 1152; 
const TInt KImgHeightMP1     = 864; 

const TInt KImgWidthMP1M3      = 1280; 
const TInt KImgHeightMP1M3     = 960; 

const TInt KImgWidthMP2        = 1600; 
const TInt KImgHeightMP2       = 1200; 

const TInt KImgWidthMP3        = 2048; 
const TInt KImgHeightMP3       = 1536; 

const TInt KMaxVideoCodecSet     = 10; 

enum TestClientLeavingCodes
    {
    KErrGetNotEqualToValuePreviouslySet = 98
    };

#if ( defined (__WINS__) || defined (__WINSCW) )
#define KFileNameBase _L("c:\\")
_LIT(KProcessFileName, "c:\\CaeTestClient.dat");
_LIT(KiSnapFileName, "C:\\cae_test_out_snapimage.mbm");
_LIT(KiStillFileName, "C:\\cae_test_out_stillimage.jpg");
_LIT(KiVideoFileName, "C:\\cae_test_out_videoclip.3gp");
_LIT(KiVideoFileName2, "C:\\cae_test_out_videoclip2.3gp");
_LIT(KiVideoFileName3, "C:\\cae_test_out_videoclip3.3gp");
_LIT(KiVideoFileName4, "C:\\cae_test_out_videoclip4.3gp");
#else
#define KFileNameBase _L("e:\\temp\\")
_LIT(KProcessFileName, "e:\\temp\\CaeTestClient.dat");
_LIT(KiSnapFileName, "e:\\temp\\cae_test_out_snapimage.mbm");
_LIT(KiStillFileName, "e:\\temp\\cae_test_out_stillimage.jpg");
_LIT(KiVideoFileName, "e:\\temp\\cae_test_out_videoclip.3gp");
_LIT(KiVideoFileName2, "e:\\temp\\cae_test_out_videoclip2.3gp");
_LIT(KiVideoFileName3, "e:\\temp\\cae_test_out_videoclip3.3gp");
_LIT(KiVideoFileName4, "e:\\temp\\cae_test_out_videoclip4.3gp");
#endif

#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif


#if MIRAGE_X_PROD1
const TInt KDefaultStillImageWidth = KImgWidthMP1;
const TInt KDefaultStillImageHeight = KImgHeightMP1;
const CCamera::TFormat KDefaultStillDataFormat = CCamera::EFormatExif;
#elif (MIRAGE_X_PROD2) || (MIRAGE_X_PROD4)
const TInt KDefaultStillImageWidth = KImgWidthMP1M3;
const TInt KDefaultStillImageHeight = KImgHeightMP1M3;
const CCamera::TFormat KDefaultStillDataFormat = CCamera::EFormatExif;
#elif MIRAGE_X_PROD7
const TInt KDefaultStillImageWidth = KImgWidthMP2;
const TInt KDefaultStillImageHeight = KImgHeightMP2;
const CCamera::TFormat KDefaultStillDataFormat = CCamera::EFormatExif;
#else
const TInt KDefaultStillImageWidth = KImgWidthVGA;
const TInt KDefaultStillImageHeight = KImgHeightVGA;
const CCamera::TFormat KDefaultStillDataFormat = CCamera::EFormatFbsBitmapColor16M;
#endif

const TInt KSymbianJpegDecoderValue = 0x101F45D7;
const TInt KSymbianJpegEncoderValue = 0x101F45D9;

const TInt KOptimizedJpegDecoderValue = 0x101FF555;
const TInt KOptimizedJpegEncoderValue = 0x101FF554;


class CCaeTestClient : public CTimer /* also CActive */, 
                       public MCamAppEngineObserver, 
                       public MCameraObserver,
                       public MCameraObserver2,
                       public MCaeStillBurstObserver
    {
    public:
        
        enum TestClientActions
            {
	        ENone,
            EDefault, 

            EEncodingError,
            EDecodingError,
            EPowerError,
            EReserveError,
            EStillCaptureError,
            EVideoRecorderOpenError,
            EVideoRecorderPrepareError,
            EVideoRecordingCompleteError,
            EPrepareVideoSettingsError,
            ESnapImageError, 
            EStillImageError,
            EStillConvertError1, 
            EStillConvertError2, 
            EStillConvertError3, 

            EInit, 
            EInitVideoRecorder, 
            EInitVideoRecorderWithUid, 
            EGetInfo,
            ESwitchPowerOnWhenInitialized, 
            ESwitchPowerOnWhenStillPrepared, 
            ESwitchPowerOffWhenStillPrepared, 
            ESwitchPowerOffWhenVfRunning, 
            ESwitchPowerOnWhenVideoPrepared, 
            ESwitchPowerOnWhenRecordingVideo, 
            ESwitchPowerOffWhenVideoPrepared, 
            ESwitchPowerOffWhenRecordingVideo, 
            EGetStillQualityIndexWhenEngineInitialized, 
            EGetStillQualityIndexWhenPrepared, 
            EGetVideoQualityIndexWhenEngineInitialized, 
            EGetVideoQualityIndexWhenPrepared, 
            EStartVideoRecordingWhenInitialized, 
            EStartVideoRecordingWhenRecording, 
            EStopVideoRecordingWhenInitialized, 
            EStopVideoRecordingWhenPrepared, 
            EIsVideoRecordingWhenPrepared, 
            EIsVideoRecordingWhenRecording, 
            ECaptureStillWhenEngineInitialized, 

            EStartVfWhenInitNotReady,
            EStartStillOpModeWhenInitNotReady,
            EStartVideoOpModeWhenInitNotReady,
            
            ECancelCaptureStillWhenInitialized, 

            ESwitchPowerOffAndOn,
            EPowerOffPowerOff,
            ERelease,
            EReleaseRelease,
            EReserveReserve,
            EReleaseReserve,
            EReleaseReserveCapture,
            EReleaseReserveRecord,
            
            EDestroyEngineWhenStillPrepareReady,
            EDestroyEngineWhenSnapImageReady,
            EDestroyEngineWhenStillCaptureReady,
            EDestroyEngineWhenSnapImageReadyInStillBurst,
            EDestroyEngineWhenStillImageReadyInStillBurst,
            EDestroyEngineWhenStillBurstReady, 
            EDestroyEngineWhenVideoPrepareReady,
            EDestroyEngineWhenVideoRecordingOn,
            EDestroyEngineWhenVideoRecordingPaused,
            EDestroyEngineWhenVideoRecordingReady,
            
            EEnumerateCaptureSizes, 
            
            ESpecifiedPrepareStill, 
            ESpecifiedPrepareStillWhenPrepared, 
            ESpecifiedPrepareAndCaptureStill, 
            ESpecifiedPrepareAndCaptureStill2nd, 
            EPrepareStillCapturing,
            EPrepareStillCapturingWhenPrepared, 
            EPrepareAndCaptureStill,
            EPrepareAndCaptureAndCancelStill, 
            EPrepareAndCaptureAndCancelStillAtSnap,  

            EStartAndStopVfWhenStillPreparedAndCaptured, 
            EPrepareStillCaptureWhenPowerOff, 
            EPrepareStillCaptureWhenRecording,

            EPrepareCroppedStillCapturing, 

            EPrepareAndCaptureStillBurst,
            ESpecPrepAndCaptureStillBurst, 
            EPrepareAndSetStillBurstCaptureInterval,
            EPrepareAndCaptureAndCancelStillBurst, 
            EPrepareAndCaptureAndCancelStillBurstAtMoment, 
            EPrepareAndCaptureAndCancelStillBurstAtSnap, 
            EPrepareAndCaptureAndStopStillBurst,
            EPrepareAndCaptureCaptureStillBurst,
            ESpecPrepAndCaptureCaptureStillBurst,  
            EPrepareAndCaptureStillBurstCaptureStill,
            ESpecPrepAndCaptureStillBurstCaptureStill, 

            EPrepareAndCaptureStillBurstExif,
            EPrepareAndCaptureAndCancelStillBurstExif,
			EStartVfPrepareCaptureStillStartAndStopVfExif,
			EStartVfPrepareCaptureStillStartAndStopVfSnapExif,
			ECaptureStillSetSnapSizeExif,
			ECaptureStillSetSnapColorModeExif,
            ECaptureStillAfterPowerOff,
            ECaptureStillAfterPowerOffOn,
            
            ESetVideoFileNameWhenInitialized, 
            ESetVideoFileNameWhenNotInitialized, 
            ESetEmptyVideoFileNameWhenInitialized, 
            ESetVideoFileNameWhenPrepared,
            ESetVideoFileNameWhenRecording,

            ESpecifiedPrepareVideo, 
            ESpecifiedPrepareVideoWhenPrepared, 
            ESpecifiedPrepareAndRecordVideo, 
            ESpecifiedPrepareAndRecordVideoNoInit, 
            EPrepareVideoRecording,
            EPrepareVideoRecordingWhenPrepared,
            EPrepareAndRecordVideo,
            EPrepareRecordPauseStopVideo, 
            EPrepareRecordPauseResumeStopVideo,
            EPrepareRecordPausePauseStopVideo, 
            EPauseVideoWhenPrepared, 
            EResumeVideoWhenPrepared,
            EResumeVideoWhenNotPaused, 
            EStartAndStopVfWhenVideoPreparedAndRecorded,
            ESpecifiedPrepareAudioBrAndRecordVideo,
            ESpecifiedPrepareAudioBrAndRecordVideoAT0,
            ESpecifiedPrepareAudioBrAndRecordVideoATInv,
   
            EStartStillOpModeStopVf, 
            EStartStillOpModeCaptureStartAndStopVf, 

            EStartVideoOpModeStopVf, 
            EStartVideoOpModeRecordStartAndStopVf, 

            EStartAndStopVf,
            EStartAndStopCroppedVf,
            EStartAndStopDirectVf, 
            EStartAndStopCroppedDirectVf,
            ESetViewFinderMirror,
            EStartVfPrepareCaptureStillStartAndStopVf,
            EStartVfPrepareRecordVideoStartAndStopVf, 
            EStartVfStartVf,
            EStartVfWhenPowerOff,

            ESetExposureModeWhenInitialized,
			ESetEvCompensationWhenInitialized,
            ESetWhiteBalanceModeWhenInitialized,
            ESetFlashModeWhenInitialized,
            ESetDigiZoomWhenInitialized, 
            ESetBrightnessWhenInitialized, 
            ESetContrastWhenInitialized,
            ESetClipMaxSizeWhenVideoNotPrepared,

            ESetExposureModeWhenStillPrepared,
			ESetEvCompensationWhenStillPrepared,
            ESetWhiteBalanceModeWhenStillPrepared,
            ESetFlashModeWhenStillPrepared,
            ESetDigiZoomWhenStillPrepared, 
            ESetBrightnessWhenStillPrepared, 
            ESetContrastWhenStillPrepared,
            ESetJpegQualityWhenStillPrepared, 
            ESetJpegCodecWhenStillPrepared, 

            ESetExposureModeWhenVideoPrepared,
            ESetEvCompensationWhenVideoPrepared,
            ESetWhiteBalanceModeWhenVideoPrepared,
            ESetFlashModeWhenVideoPrepared,            
            ESetDigiZoomWhenVideoPrepared, 
            ESetBrightnessWhenVideoPrepared, 
            ESetContrastWhenVideoPrepared, 
            ESetClipMaxSizeWhenVideoPrepared, 
            ESetAudioWhenVideoPrepared,
            EGetAudioWhenVideoNotInitialized,
            ESetVideoTimesIntervalWhenVideoPrepared,

            ESetExposureModeWhenVideoRecording,
			ESetEvCompensationWhenVideoRecording,
            ESetWhiteBalanceModeWhenVideoRecording,
            ESetFlashModeWhenVideoRecording,            
            ESetDigiZoomWhenVideoRecording, 
            ESetBrightnessWhenVideoRecording, 
            ESetContrastWhenVideoRecording,
            ESetClipMaxSizeWhenVideoRecording,
            ESetClipMaxSizeWhenVideoNotInitialized,
            ESetAudioWhenVideoRecording,
            ESetAudioWhenVideoNotInitialized,
            ESetVideoTimesIntervalWhenVideoRecording,

            EResetToDefaults,

            EGetStillFrameSizeWhenInitialized,
            EEstimatedStillSizeInBytesWhenInitialized,
            EGetStillFrameSizeWhenPrepared,
            EEstimatedStillSizeInBytesWhenPrepared,

			ESetSnapImageSizeBeforePrepare,
			ESetSnapImageSizeDuringAndAfterPrepare,

            EGetVideoFrameSizeWhenInitialized,
            EVideoFrameRateWhenInitialized,
            EEstimatedVideoRecordingBitRateWhenInitialized,
            EGetVideoFrameSizeWhenPrepared,
            EVideoFrameRateWhenPrepared,
            EEstimatedVideoRecordingBitRateWhenPrepared,

            ECaptureCapture,
            ESpecPrepCaptureCapture, 
            ECaptureRecord,
            ERecordCapture,
            ERecordRecord,

            ERecordCaptureComplexSequence1,
            ERecordCaptureComplexSequence2, 

            ERemainingTimeWhenVideoPrepared,
            ERemainingTimeWhenVideoNotInitialized,

            EGetCustomInterface,
            ESetColorTone,
            ESetColorToneWrongParams,
            ESetColorToneVideoRecording,
            ESetColorToneRelease,

            EInitUid,

            ESetDisplayIndex,
            ESetCameraIndexLow,
            ESetCameraIndexHigh,
            
            EEnumerateVideoFrameSize,
			ECCameraHandle,
			
			EProcessExtViewFinderFrameReady,
			EProcessExtSnapImage,
			EProcessExtCapturedImage,
			EProcessExtCapturedImageTDesC8,
			EProcessExtCapturedImageCFbsBitmap,
			EProcessExtCapturedImageBurst,
			EProcessExtCapturedImageBurstTDesC8,
			EProcessExtCancel,
			EProcessExtCancel2,
			EInstalledExtensionList,
			ESkippedExtensionList,
			ESetSkippedExtensionList,
			EProcessExtCapturedImageNoInit
            };

        enum TestClientOpMode
            {
	        EClientOpModeStill,
            EClientOpModeVideo
            };

    public:
        
        static CCaeTestClient* NewL();
        
        virtual ~CCaeTestClient();

        void SetSnapImageCreation( TBool aCreateSnapImage );

        void SetRunningWithViewFinder( TBool aRunWithViewFinder );

        void SetViewFinderSize( const TSize& aSize );
        
        void SetStillQualityLevelIndex( TInt aQualityLevelIndex );
        
        void SetVideoQualityLevelIndex( TInt aQualityLevelIndex );
        
        void SetStillFormat( CCamera::TFormat aFormat );
        
        void SetStillCompressionQuality( TInt aCompressionQuality );

        void SetStillFrameSize( const TSize& aSize );

		void SetSnapImageSize( const TSize& aSize );

        void SetSnapImageSource( CCaeEngine::TSnapImageSource aSnapImageSource );

		void SetSnapImageColorMode( TDisplayMode aMode );

		void SetImageCodecs( TUid aDecoderUid, TUid aEncoderUid );

		void SetCropRect( const TRect& aRect );

        void SetStillBurstLength( TInt aLength );
        
        void SetVideoFrameSize( const TSize& aSize );
        
        void SetVideoFrameRate( TReal32 aFrameRate );
        
        void SetVideoBitRate( TInt aBitRate );

        void SetVideoAudio( TBool aAudioEnabled );
        
        void SetAudioBitRate( TInt aAudioBitRate );

        void SetFileNameBase( const TFileName& aFileNameBase );

        void SetVfFileName( const TFileName& aFileName ); 

        void SetVideoStopMode( TBool aAsyncVideoStopEnabled );

        void SetVfFileCountToSave( TInt aCountOfVfFilesToSave );

        void SetSnapFileName( const TFileName& aFileName );

        void SetStillFileName( const TFileName& aFileName );
        
        void SetVideoFileName( const TFileName& aFileName );

        void SetTimeout( TTimeIntervalMicroSeconds32 aTimeout );

        void SetStillBurstCaptureInterval( TTimeIntervalMicroSeconds aInterval );
        
        void SetExposureMode( TInt aExposureMode );

        void SetEvCompensation( TInt aEvCompensation );

        void SetWhiteBalanceMode( TInt aWhiteBalanceMode );

        void SetFlashMode( TInt aFlashMode );

        void SetBrightness( TInt aBrightness );

        void SetContrast( TInt aContrast );

        void SetZoomMode( TInt aZoomMode );

        void SetDigiZoomValue( TInt aDigiZoomValue );

        void SetVideoClipMaxSize( TInt aMaxClipSizeInBytes );

        void SetVideoTimesInterval( TTimeIntervalMicroSeconds aInterval );

        void SetOpMode( TestClientOpMode aOpMode );
        
        void SetDisplayIndex( TInt aDisplayIndex );

        void SetStopOnErrorInMcaeoSnapImageReady( TBool aStopOnError );
        
        void SetStopOnErrorInMcaeoStillImageReady( TBool aStopOnError );
        
        void GetInfo( TCamAppEngineInfo& aInfo ) const;
        
        void GetStillFrameSize( TSize& aSize ) const;

        TInt EstimatedStillSizeInBytes() const;

        void GetVideoFrameSize( TSize& aSize ) const;

        TReal32 VideoFrameRate() const;
        
        TInt EstimatedVideoRecordingBitRate() const;


        TReal32 ViewFinderFrameFrequency();
    
        TInt CountInitComplete();

        TInt CountStillPrepareComplete();

        TInt CountStillBurstOn();

        TInt CountStillBurstCaptureMoment();

        TInt CountStillBurstComplete();

        TInt CountImageDeliveredStillBurstComplete();

        TInt CountVideoPrepareComplete();

        TInt CountViewFinderFrameReady();

        TInt CountSnapImageReady();

        TInt CountStillImageReady();

        TInt CountVideoRecordingOn();

        TInt CountVideoRecordingComplete();

        TInt CountVideoRecordingStopped();

        TInt CountVideoRecordingPaused();

        TInt CountVideoRecordingTimes();

        void SetCustomInterfaceUid( TUid aCustomInterface );

        TAny* CustomInterfacePointer();

        void SetColorToneValue( MCameraColorToneSettings::TColorTone );

        void SetVideoCodecSet( TInt aValue );

    public:

        virtual void RunTestActionL( TestClientActions aAction ) = 0;


    public:
        // from observer

        void McaeoInitComplete( TInt aError );

        void McaeoStillPrepareComplete( TInt aError );

        void McaesboStillBurstCaptureMoment( TInt aError );

        void McaesboStillBurstComplete( TInt aImageCountDelivered, 
                                        TInt aError );

        void McaeoVideoPrepareComplete( TInt aError );

        void McaeoViewFinderFrameReady( CFbsBitmap& aFrame, TInt aError );

        void McaeoSnapImageReady( const CFbsBitmap& aBitmap, TInt aError );

        void McaeoStillImageReady( CFbsBitmap* aBitmap, HBufC8* aData, TInt aError );

        void McaeoVideoRecordingOn( TInt aError );

        void McaeoVideoRecordingComplete( TInt aError );

        void McaeoVideoRecordingStopped();

        void McaeoVideoRecordingPaused( TInt aError );

	    void McaeoVideoRecordingTimes( TTimeIntervalMicroSeconds aTimeElapsed, 
                                       TTimeIntervalMicroSeconds aTimeRemaining, 
                                       TInt aError );


    protected:

	    CCaeTestClient();

        void ConstructL();

    protected:
        
        void InitStateVariables();

        void StopOnError( TInt aError );


    private:
        
        // from CActive
        void RunL();
        
        // from CActive
        void DoCancel();

    private:
        
        virtual void RunTrappedL() = 0;
        
    private: // From MCameraObserver / ECam.h

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


        /**
        * From MCameraObserver2 Informs that client should handle a event
        */
        void HandleEvent(const TECAMEvent& aEvent);

        /**
        * From MCameraObserver2 Informs that viewfinder frame is ready
        */
        void ViewFinderReady(MCameraBuffer& aCameraBuffer,TInt aError);

        /**
        * From MCameraObserver2 Informs that a new captured image is ready
        */
        void ImageBufferReady(MCameraBuffer& aCameraBuffer,TInt aError);
    
        /**
        * From MCameraObserver2 Informs that a new captured video is ready
        */
        void VideoBufferReady(MCameraBuffer& aCameraBuffer,TInt aError);


    protected:

        // Camera Application Engine.
        CCaeEngine* iCamAppEngine;
        CCamera*    iCameraDup;

        // Output data file.
        RFile iFile; 
        RFs   iFsSession;

        // Camera Application Engine info object.
        TCamAppEngineInfo iInfo;

        // File name base (e.g. "e:\\")
        TFileName               iFileNameBase;

        // View finder image file name.
        TFileName               iVfFileName;
        
        // Still image filename.
        TFileName               iSnapFileName;
        TFileName               iStillFileName;
        TFileName               iVideoFileName;
        TFileName               iVideoFileName2;
        TFileName               iVideoFileName3;
        TFileName               iVideoFileName4;

        // For counting View Finder frames.
        TInt                    iVfFrameCount;
        TInt                    iVfFrameCountToSave;

        // For counting Snap images.
        TInt                    iSnapImageCount;

        TInt                    iImageCountToCapture;
        
        TRect                   iCropRect;

        TSize                   iSizeEmpty;

        TInt  iError;

        TBool iStop;

        TBool iCreateSnapImage;

        TBool iRunWithViewFinder;
        TBool iStopViewFinder;

        TBool iRunlForSnapImage;
        TBool iRunlForStillImage;
        
        TBool iStopWhenCompleted;
        TBool iContinueWhenCompleted;

        TBool iInitReady;
        TBool iPowerOn;
        
        TBool iViewFinderStarted;
        TBool iViewFindingUsed;
        
        TBool iStillPrepareReady;
        TBool iSnapImageReady;
        TBool iStillCaptureReady;
        TBool iStillCapturingOn;
        TBool iStillCaptured;

        TBool iStillBurstCaptureMoment;
        TBool iStillBurstReady;
        TBool iStopStillBurst;
        TBool iCancelStillBurst;
        TBool iCancelStillBurstAfterVfFrame;

        TBool iVideoPrepareReady;
        TBool iVideoRecordingReady;
        TBool iVideoRecordingPaused;
        TBool iVideoRecordingResumed;
        TBool iVideoRecordingOn;
        TBool iVideoRecorded;

        TBool iStoppedOnError;

        TInt  iCancelCount;

        TInt  iCountInitComplete;
        TInt  iCountStillPrepareComplete;
        TInt  iCountStillBurstCaptureMoment;
        TInt  iCountStillBurstComplete;
        TInt  iCountVideoPrepareComplete;
        TInt  iCountViewFinderFrameReady;
        TInt  iCountSnapImageReady;
        TInt  iCountStillImageReady;
        TInt  iCountVideoRecordingOn;
        TInt  iCountVideoRecordingComplete;
        TInt  iCountVideoRecordingStopped;
        TInt  iCountVideoRecordingPaused;
        TInt  iCountVideoRecordingTimes;

        TInt  iImageCountPreparedForStillBurst;
        TInt  iImageCountDeliveredForStillBurst;

        TTimeIntervalMicroSeconds32     iTimeout;
        TTimeIntervalMicroSeconds     iStillBurstCaptureInterval;

        TTime iNextRunLTime;

        TestClientOpMode iOpMode;

	    TestClientActions    iAction;
	    TestClientActions    iSavedAction;

        TBool   iSaveSnapImage; 
        TBool   iSaveStillImage; 
        TBool   iSaveVideoClip; 
        TBool   iAcceptVideoCompletionError;
        TBool   iStopOnErrorInMcaeoSnapImageReady;
        TBool   iStopOnErrorInMcaeoStillImageReady;

        // TBool   iDestroyTheEngineInSnapImageReady;
        // TBool   iDestroyTheEngineInStillImageReady;
        
        TInt    iActionCycle;

        TInt    iStillQualityLevelIndex;
        TInt    iVideoQualityLevelIndex;

        TSize   iViewFinderSize;

        TSize   iStillFrameSize;

        TSize   iSnapImageSize;
        
        CCaeEngine::TSnapImageSource iSnapImageSource;

        CCamera::TFormat    iStillDataFormat;
        TDisplayMode		iSnapImageColorMode;

        TInt    iStillCompressionQuality;
        
        TUid 	iImageDecoderUid;
        TUid 	iImageEncoderUid;

        TInt    iStillBurstLength;

        TSize   iVideoFrameSize;
        TReal32 iVideoFrameRate;
        TInt    iVideoBitRate;
        TBool   iVideoAudioEnabled;
        TInt    iAudioBitRate;
        TInt    iVideoMaxClipSizeInBytes;

        TTimeIntervalMicroSeconds iVideoTimesInterval;

        TInt    iExposureMode;
        TInt    iEvCompensation;
        TInt    iWhiteBalanceMode;
        TInt    iFlashMode;
        TInt    iZoomMode;
        TInt    iDigiZoomValue;
        TInt    iBrightness;
        TInt    iContrast;

        TInt    iDisplayIndex;

        TSize   iStillFrameSizeSet;
        TInt    iEstimatedStillSizeInBytesSet;
        TSize   iVideoFrameSizeSet;
        TReal32 iVideoFrameRateSet;
        TInt    iEstimatedVideoBitRateSet;

        TUid    iCustomInterfaceUid;
        TAny*   iCustomInterfacePointer;
        MCameraColorToneSettings::TColorTone iColorToneValue;
        TUint32 iTestedColorTones;
        TBool   iReleaseDone;
        TBool   iReserveDone;

        TInt    iVideoCodecSet;
        const TDesC8* iMimeTypeSet[KMaxVideoCodecSet];
        const TDesC*  iPreferredSupplierSet[KMaxVideoCodecSet];
        const TDesC8* iVideoTypeSet[KMaxVideoCodecSet];
        const TDesC8* iAudioTypeSet[KMaxVideoCodecSet];

        TInt iCameraHandle;
        
        CFbsBitmap* iBitmapSave;
        HBufC8* iDataSave;
        TBool iBitmapSendDone;
        TBool iExtensionMode;
        
        TBool iVideoStopModeCommand;
        TBool iAsyncVideoStopEnabled;


    };

#endif // CAE_TEST_CLIENT_H
