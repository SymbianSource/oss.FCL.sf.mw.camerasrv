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
* Description: CaeTestStillSettings.h
*
*/
#ifndef CAE_TEST_STILL_SETTINGS_H
#define CAE_TEST_STILL_SETTINGS_H


#include <e32base.h>

#include "CaeTestSettingsClient.h"

class CCaeTestStillSettings : public CTestCase
{
public:

	CCaeTestStillSettings();
    ~CCaeTestStillSettings();

	// Allocate the resources for one test function
    void setUpL();

	// Free the resources reserved in setUpL()
	void tearDown();

	// A function to collect and return a suite of tests
    static MTest* suiteL();

protected:

	// Own test functions that use assertions and may leave:
    void TestDigiZoomVal2L();
    void TestDigiZoomValNeg1L();
    void TestOpticalZoomValMinL();
    void TestOpticalZoomValMaxL();
    void TestDigiZoomMode0L();
    
    void TestOpticalZoomMode1L();
    void TestOpticalDigiZoomMode2L();
    
    void TestDigiZoomModeNeg1L();
    
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

    void TestFlashMode4L();
    void TestFlashMode8L();
    void TestFlashModeNeg1L();
    void TestFlashMode5L();
    void TestFlashMode6L();
    void TestFlashMode9L();
    void TestFlashMode10L();
    
    void TestExposureModeInitialized2L();
    void TestWhiteBalanceModeInitialized2L();
    void TestFlashModeInitialized2L();
    void TestDigiZoomInitializedMode0L();
    
    void TestGetInfo();
    
    void TestResetToDefaultsL();
    void TestSetViewFinderMirrorL();

    void TestGetStillFrameSizeWhenInitializedL();
    void TestGetStillFrameSizeWhenPreparedL();
    
    void TestEstimatedStillSizeInBytesWhenInitializedL();
    void TestEstimatedStillSizeInBytesWhenPreparedL();
    void TestSetSnapImageSizeBeforePrepareL();
    void TestSetSnapImageSizeDuringAndAfterPrepareL();

    void TestInitUidL();
    void TestSetJpegQuality0L();
    void TestSetJpegQuality101L();

    void TestCaptureStillBurstIntervalM2L();
    
    void TestSetJpegCodecInvalidL();

private:
    CCaeTestSettingsClient* iSettingsClient;
};

#endif // CAE_TEST_STILL_SETTINGS_H
