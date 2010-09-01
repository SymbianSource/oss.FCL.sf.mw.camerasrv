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
* Description: CaeTestRelease.h
*
*/
#ifndef CAE_TEST_RELEASE_H
#define CAE_TEST_RELEASE_H


#include <e32base.h>
#include "CaeTestVariation.h"

#include "CaeTestStatesClient.h"
#include "CaeTestSettingsClient.h"

class CCaeTestRelease : public CTestCase
{
public:

	CCaeTestRelease ();
    ~CCaeTestRelease ();

	// Allocate the resources for one test function
    void setUpL ();

	// Free the resources reserved in setUpL()
	void tearDown ();

	// A function to collect and return a suite of tests
    static MTest* suiteL ();

protected:

	// Own test functions that use assertions and may leave:
    void TestInitL();
    void TestInitVideoRecorderL();
    void TestPowerOffAndOnL();
    void TestReleaseAndReserveL();
    void TestCCameraHandleL();

    void TestPowerOnWhenInitializedL();
    void TestPowerOnWhenStillCapturePreparedL();
    void TestPowerOffWhenStillCapturePreparedL();
    void TestPowerOffWhenViewfinderRunningL();
    void TestPowerOnWhenVideoRecordingPreparedL();
    void TestPowerOnWhenRecordingL();
    void TestPowerOffWhenVideoRecordingPreparedL();
    void TestPowerOffWhenRecordingL();

    void TestViewFinderStartAndStopQqvgaL();
    void TestViewFinderStartAndStopSubqcifL();
    void TestCroppedViewFinderStartAndStopL();
    void TestViewFinderDirectStartAndStopL();
    void TestCroppedViewFinderDirectStartAndStopL();

    void TestCaptureQualityLevel0L();
    void TestCaptureQualityLevel1L();
    void TestCaptureQualityLevel2L();

    void TestCaptureVga16ML();
    void TestCaptureVga64KL();
    void TestCaptureVga4KL();

    void TestCaptureQqvga16ML();
    void TestCaptureQqvga64KL();
    void TestCaptureQqvga4KL();

    void TestCaptureMp1_16ML();
    void TestCaptureMp1_64KL();
    void TestCaptureMp1_4KL();
    
    void TestCaptureJpegMp1L();

    void TestCaptureMp1M3_16ML();
    void TestCaptureMp1M3_64KL();
    void TestCaptureMp1M3_4KL();
    
    void TestCaptureJpegMp1M3L();

    void TestCaptureMp2_16ML();
    void TestCaptureMp2_64KL();
    void TestCaptureMp2_4KL();
    void TestCaptureJpegMp2L();
    void TestCaptureExifMp2L();
    void TestCaptureExifSVGAL();

	void TestCaptureCodecL();
	void TestCaptureCodec2L();

    void TestCaptureCaptureL();
    void TestCaptureCaptureSpecPrepL();

    void TestPrepareStillCapture0L();
    void TestPrepareStillCapture1L();
    void TestPrepareStillCapture2L();
    void TestPrepareStillCapture0WhenPreparedL();
    void TestSpecPrepareSCSupportedFormatWhenInitializedL();

    void TestCaptureStillWhenEngineInitializedL();

    void TestCaptureStillBurst3L();
    void TestCaptureStillBurst0L();
    void TestCaptureStillBurst100L();

    void TestCancelCaptureStillBurst3L();
    void TestStopCaptureStillBurstAfter2L();
    
    void TestSpecPrepCaptureStillBurst3L();
    
    void TestCaptureStillBurstInterval0L();
    void TestCaptureStillBurstInterval1000000L();
    void TestCaptureStillBurstIntervalM1L();

	void TestCaptureStillBurstExif3L();
	void TestCaptureStillBurstExif6L();
	void TestCaptureStillBurstExif6NSL();
	void TestCaptureStillBurstExif6_ThumbnailL();
	void TestCaptureStillBurstExif100L();
	void TestCaptureStillBurstExif6VfL();
	void TestCaptureStillBurstExif6VfCancelL();

    void TestCaptureVgaExifL();
    void TestCaptureQqvgaExifL();
	void TestCaptureVgaSnapOneHalfExifL();
	void TestCaptureVgaSnapOneFourthExifL();
	void TestCaptureVgaSnapOneEighthExifL();
	void TestCaptureVgaSnapExifL();
	void TestCaptureVgaSnapOneEighthJpegL();
	void TestCaptureVgaSnapOneEighthExif2L();
	void TestCaptureVgaSnapColor16MExifL();
    void TestCaptureAfterPowerOffL();

    void TestPrepareVideoRecording0L();
    void TestPrepareVideoRecording1L();
    void TestPrepareVideoRecording2L();
    void TestPrepareVideoRecording0WhenPreparedL();
    void TestSpecPrepareVRSupportedFormatWhenVRInitializedL();
   
    void TestRecordQualityLevel0L();
    void TestRecordQualityLevel1L();
    void TestRecordQualityLevel2L();

    void TestStartAndStopVideoRecordingSubQcifL();
    void TestStartAndStopVideoRecordingSubQcifNoInitL();
    void TestStartAndStopVideoRecordingQcifL();

    void TestSCDigiZoomVal0L();
    void TestSCDigiZoomVal1VgaL();
    void TestSCDigiZoomVal1QqvgaL();
    void TestSCDigiZoomInitializedVal0L();
    void TestSCDigiZoomInitializedVal1VgaL();
    void TestSCDigiZoomInitializedVal1QqvgaL();
    void TestVRDigiZoomVal0L();
    void TestVRDigiZoomVal1L();
    void TestVRDigiZoomRecordingVal0L();
    void TestVRDigiZoomRecordingVal1L(); 
    
    void TestStartAndStopVRWhenTooBigSizeSubQcifL();

    void TestGetInfoL();
    void TestGetInfoPROD1MirageXL();
    void TestGetInfoPROD2MirageXL();
    void TestGetInfoPROD3MirageXPL();
    void TestGetInfoPROD4MirageXL();
    void TestGetInfoPROD6MirageXL();
    void TestGetInfoPROD7Unagi2ML();
    void TestGetInfoPROD8MirageXL();
    void TestGetInfoPROD9HWL();
    void TestGetInfoDevlonHWL();

    void TestRecordPauseResumeStopL();

    void TestSCExposureMode0L();
    void TestSCExposureMode1L();
    void TestSCExposureModeInitialized0L();
    void TestSCExposureModeInitialized1L();

    void TestVRExposureMode0L();
    void TestVRExposureMode1L();
    void TestVRExposureModeRecording0L();
    void TestVRExposureModeRecording1L();

	void TestSCEvCompensation0L();
	void TestSCEvCompensationMaxL();
	void TestSCEvCompensationMinL();
	void TestSCEvCompensationInitialized0L();
	void TestSCEvCompensationInitializedMaxL();
	void TestSCEvCompensationInitializedMinL();
	void TestSCEvCompensationInitializedInvalidL();

	void TestVREvCompensationMinL();
	void TestVREvCompensationRecordingMaxL();

    void TestSCWhiteBalanceMode0L();
    void TestSCWhiteBalanceModeInitialized0L();

    void TestVRWhiteBalanceMode0L();
    void TestVRWhiteBalanceModeRecording0L();

    void TestSCContrast50L();
    void TestSCContrastInitialized50L();

    void TestSCBrightness50L();
    void TestSCBrightnessInitialized50L();

    void TestVRContrast50L();
    void TestVRContrastRecording50L();

    void TestVRBrightness50L();
    void TestVRBrightnessRecording50L();

    void TestVideoClipMaxSize10000L();
    void TestVideoClipMaxSize10000_2L();
    
    void TestCancelCaptureStillL();

    void TestGetStillQualityIndexWhenEngineInitializedL();
    void TestGetStillQualityIndexWhenPreparedL();
    void TestGetVideoQualityIndexWhenEngineInitializedL();
    void TestGetVideoQualityIndexWhenPreparedL();

    void TestRemainingTimeWhenVideoPreparedL();

    void TestPrepareAudioBitRateVR5000L();
    void TestPrepareAudioBitRateVR128L();
    void TestPrepareAudioBitRateVR128InvVtypeL();

    void TestCaptureVga16M_NoSnapL();
    void TestCaptureQualityLevel0_NoSnapL();
    void TestCaptureVgaExif_NoSnapL();
    void TestCaptureStillBurst3_NoSnapL();
    void TestSpecPrepCaptureStillBurst3_NoSnapL();
    void TestCaptureStillBurstExif3_NoSnapL();
    
    void TestEnumerateCaptureSizesL();
    void TestSetJpegQuality50L();
    void TestSCCustomInterfaceL();    
    
    void TestFlashMode0L();
    void TestFlashMode1L();
    void TestFlashMode2L();
    
    void TestSCColorToneNormalL();
    void TestSCColorToneSepiaL();
    void TestSCColorToneGrayscaleL();
    void TestSCColorToneNegativeL();
    void TestVRColorToneL();
    void TestSCColorToneWrongParams1L();
    void TestSCColorToneWrongParams2L();
    void TestSCColorToneReleaseL();

    void TestSetDisplayIndexL();
    void TestEnumerateVideoFrameSize001L();
    void TestSecondaryCamera001L();
    void TestProcessExtViewFinderFrameReadyL();
    void TestProcessExtSnapImageL();
    void TestProcessExtCapturedImageL();
    void TestProcessExtCapturedImage2L();
    void TestProcessExtCapturedImage3L();
    void TestProcessExtCapturedImage4L();
    void TestProcessExtCancelL();
    void TestProcessExtCancel2L();
    void TestProcessExtCapturedImageBurstL();
    void TestProcessExtCapturedImageBurst2L();
    void TestInstalledExtensionListL();
    void TestSkippedExtensionListL();
    void TestSetSkippedExtensionListL();
    
    void TestRecordAsyncStop001L();
    void TestRecordAsyncStop002L();

private:
    CCaeTestStatesClient * iStatesClient;
    CCaeTestSettingsClient * iSettingsClient;
};

#endif // CAE_TEST_RELEASE_H
