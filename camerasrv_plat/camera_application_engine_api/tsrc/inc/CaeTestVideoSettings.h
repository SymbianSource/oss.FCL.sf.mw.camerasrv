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
* Description: CaeTestVideoSettings.h
*
*/
#ifndef CAE_TEST_VIDEO_SETTINGS_H
#define CAE_TEST_VIDEO_SETTINGS_H


#include <e32base.h>

#include "CaeTestSettingsClient.h"

class CCaeTestVideoSettings : public CTestCase
{
public:

	CCaeTestVideoSettings ();
    ~CCaeTestVideoSettings ();

	// Allocate the resources for one test function
    void setUpL ();

	// Free the resources reserved in setUpL()
	void tearDown ();

	// A function to collect and return a suite of tests
    static MTest* suiteL ();

protected:

	// Own test functions that use assertions and may leave:
    void TestDigiZoomVal2L();
    void TestDigiZoomNegVal1L();
    void TestDigiZoomMode0L();
    void TestOpticalZoomMode1L();
    void TestOpticalDigiZoomMode2L();
    void TestDigiZoomNegMode1L();
    void TestBrightness0L();
    void TestBrightness100L();
    void TestBrightnessNeg100L();
    void TestBrightness101L();
    void TestBrightnessNeg101L();
    void TestContrast0L();
    void TestContrast100L();
    void TestContrastNeg100L();
    void TestContrast101L();
    void TestContrastNeg101L();
    void TestExposureMode2L();
    void TestExposureMode4L();
    void TestExposureModeNeg1L();
    void TestExposureMode5L();
    void TestWhiteBalanceMode1L();
    void TestWhiteBalanceMode2L();
    void TestWhiteBalanceMode4L();
    void TestWhiteBalanceMode8L();
    void TestWhiteBalanceMode10L();
    void TestWhiteBalanceModeNeg1L();
    void TestWhiteBalanceMode5L();
    void TestWhiteBalanceMode12L();
    void TestFlashMode0L();
    void TestFlashMode1L();
    void TestFlashMode2L();
    void TestFlashMode4L();
    void TestFlashMode8L();
    void TestFlashModeNeg1L();
    void TestFlashMode5L();
    void TestVideoClipMaxSize0L();
    void TestVideoClipMaxSizeNeg1L();
    void TestVideoClipMaxSize10001L(); 
    void TestVideoClipMaxSizeWhenVideoNotPrepared0L();
    void TestVideoClipMaxSizeWhenVideoNotPrepared5000L();
    void TestVideoAudioETrueL();
    void TestVideoAudioEFalseL();
    void TestExposureModeRecording2L();
    void TestWhiteBalanceModeRecording2L();
    void TestFlashModeRecording2L();
    void TestDigiZoomRecordingMode0L();
    void TestVideoClipMaxSizeRecording10000L();
    void TestVideoClipMaxSizeNotInitialized10000L();
    void TestVideoAudioRecordingETrueL();
    void TestVideoAudioNotInitializedL();
    void TestGetVideoAudioNotInitializedL();

    void TestSetVideoFileNameL();
    void TestSetEmptyVideoFileNameL();
    void TestSetVideoFileNameWhenPreparedL();
    void TestSetVideoFileNameWhenNotInitializedL();
    void TestSetVideoFileNameWhenRecordingL();

    void TestSetVideoTimeIntervalSubQcifL();
    void TestSetVideoTimeIntervalSubQcifWhenPreparedL();
    void TestSetVideoTimeIntervalSubQcifWhenRecordingL();
    void TestSetZeroVideoTimeIntervalSubQcifWhenPreparedL();
    void TestSetVideoTimeIntervalNegativeL();
    void TestSetVideoTimeIntervalHugeL();
    
    void TestGetVideoFrameSizeWhenInitializedL();
    void TestGetVideoFrameSizeWhenPreparedL();
    void TestVideoFrameRateWhenInitializedL();
    void TestVideoFrameRateWhenPreparedL();
    void TestEstimatedVideoRecordingBitRateWhenInitializedL();
    void TestEstimatedVideoRecordingBitRateWhenPreparedL();


private:
    CCaeTestSettingsClient * iSettingsClient;
};

#endif // CAE_TEST_VIDEO_SETTINGS_H
