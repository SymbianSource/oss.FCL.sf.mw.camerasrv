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
* Description: CaeTestVideoStates.h
*
*/
#ifndef CAE_TEST_VIDEO_STATES_H
#define CAE_TEST_VIDEO_STATES_H


#include <e32base.h>

#include "CaeTestStatesClient.h"

class CCaeTestVideoStates : public CTestCase
{
public:

	CCaeTestVideoStates ();
    ~CCaeTestVideoStates ();

	// Allocate the resources for one test function
    void setUpL ();

	// Free the resources reserved in setUpL()
	void tearDown ();

	// A function to collect and return a suite of tests
    static MTest* suiteL ();

protected:

	// Own test functions that use assertions and may leave:

    void TestPrepareAndRecordQualityLevel0L();
    void TestPrepareAndRecordQualityLevel1L();
    void TestPrepareAndRecordQualityLevel2L();

    void TestVideoRecordingQcif60000Fr10NoAudioNoVfL();
    void TestVideoRecordingQcif60000Fr10WithAudioNoVfL();
    void TestVideoRecordingQcif60000Fr10NoAudioWithVfL();
    void TestVideoRecordingQcif60000Fr10WithAudioWithVfL();

    void TestRecordPauseStopL();
    void TestPauseWhenPreparedL();
    void TestRecordPausePauseStopL();

    void TestResumeWhenPreparedL();
    void TestResumeWhenNotPausedL();

    void TestPrepareVideoRecordingNeg1L();   
    void TestPrepareVideoRecording3L();
    void TestSpecPrepareVRUnsupportedFormatWhenVRInitializedL();
    void TestSpecPrepareVRSupportedFormatWhenPreparedL();

    void TestStartVideoRecordingWhenVRInitializedL();
    void TestStartVideoRecordingWhenRecordingL();

    void TestStopVideoRecordingWhenVRInitializedL();
    void TestStopVideoRecordingWhenPreparedL();

    void TestCaptureRecordL();
    void TestRecordCaptureL();
    void TestRecordRecordL();

    void TestRecordCaptureComplexSequence1L();
    void TestRecordCaptureComplexSequence2L();

    void TestReleaseReserveRecordL();

    void TestDestroy1L();
    void TestDestroy2L();
    void TestDestroy3L();
    void TestDestroy4L();

    void TestVideoRecorderOpenErrorL();
    void TestVideoRecorderPrepareErrorL();
    void TestVideoRecordingCompleteErrorL();
    void TestPrepareVideoSettingsErrorL();

    void TestPrepareAudioBitRateVR0L();
    void TestPrepareAudioBitRateVR5150L();
    void TestPrepareAudioBitRateVR12200L();
    void TestPrepareAudioBitRateVR20000L();
    void TestPrepareAudioBitRateVR12200AudioType0L();
    void TestPrepareAudioBitRateVR12200AudioTypeInvL();

    void TestInitVideoRecorderWithUidL();
    void TestRemainingTimeWhenVideoNotInitializedL();

    	// Own test functions that use assertions and may leave (SET 1)

    void TestPrepareAndRecordQualityLevel0QL();
    void TestPrepareAndRecordQualityLevel1QL();
    void TestPrepareAndRecordQualityLevel2QL();

    void TestVideoRecordingQcif60000Fr10NoAudioNoVfQL();
    void TestVideoRecordingQcif60000Fr10WithAudioNoVfQL();
    void TestVideoRecordingQcif60000Fr10NoAudioWithVfQL();
    void TestVideoRecordingQcif60000Fr10WithAudioWithVfQL();

    void TestRecordPauseStopQL();
    void TestPauseWhenPreparedQL();
    void TestRecordPausePauseStopQL();

    void TestResumeWhenPreparedQL();
    void TestResumeWhenNotPausedQL();

    void TestPrepareVideoRecordingNeg1QL();   
    void TestPrepareVideoRecording3QL();
    void TestSpecPrepareVRUnsupportedFormatWhenVRInitializedQL();
    void TestSpecPrepareVRSupportedFormatWhenPreparedQL();

    void TestStartVideoRecordingWhenVRInitializedQL();
    void TestStartVideoRecordingWhenRecordingQL();

    void TestStopVideoRecordingWhenVRInitializedQL();
    void TestStopVideoRecordingWhenPreparedQL();

    void TestCaptureRecordQL();
    void TestRecordCaptureQL();
    void TestRecordRecordQL();

    void TestRecordCaptureComplexSequence1QL();
    void TestRecordCaptureComplexSequence2QL();

    void TestReleaseReserveRecordQL();

    void TestDestroy1QL();
    void TestDestroy2QL();
    void TestDestroy3QL();
    void TestDestroy4QL();

    void TestVideoRecorderOpenErrorQL();
    void TestVideoRecorderPrepareErrorQL();
    void TestVideoRecordingCompleteErrorQL();
    void TestPrepareVideoSettingsErrorQL();

    void TestPrepareAudioBitRateVR0QL();
    void TestPrepareAudioBitRateVR5150QL();
    void TestPrepareAudioBitRateVR12200QL();
    void TestPrepareAudioBitRateVR20000QL();
    void TestPrepareAudioBitRateVR12200AudioType0QL();
    void TestPrepareAudioBitRateVR12200AudioTypeInvQL();

    void TestInitVideoRecorderWithUidQL();


    	// Own test functions that use assertions and may leave (SET 2)

    void TestPrepareAndRecordQualityLevel0Q4L();
    void TestPrepareAndRecordQualityLevel1Q4L();
    void TestPrepareAndRecordQualityLevel2Q4L();

    void TestVideoRecordingQcif60000Fr10NoAudioNoVfQ4L();
    void TestVideoRecordingQcif60000Fr10WithAudioNoVfQ4L();
    void TestVideoRecordingQcif60000Fr10NoAudioWithVfQ4L();
    void TestVideoRecordingQcif60000Fr10WithAudioWithVfQ4L();

    void TestRecordPauseStopQ4L();
    void TestPauseWhenPreparedQ4L();
    void TestRecordPausePauseStopQ4L();

    void TestResumeWhenPreparedQ4L();
    void TestResumeWhenNotPausedQ4L();

    void TestPrepareVideoRecordingNeg1Q4L();   
    void TestPrepareVideoRecording3Q4L();
    void TestSpecPrepareVRUnsupportedFormatWhenVRInitializedQ4L();
    void TestSpecPrepareVRSupportedFormatWhenPreparedQ4L();

    void TestStartVideoRecordingWhenVRInitializedQ4L();
    void TestStartVideoRecordingWhenRecordingQ4L();

    void TestStopVideoRecordingWhenVRInitializedQ4L();
    void TestStopVideoRecordingWhenPreparedQ4L();

    void TestCaptureRecordQ4L();
    void TestRecordCaptureQ4L();
    void TestRecordRecordQ4L();

    void TestRecordCaptureComplexSequence1Q4L();
    void TestRecordCaptureComplexSequence2Q4L();

    void TestReleaseReserveRecordQ4L();

    void TestDestroy1Q4L();
    void TestDestroy2Q4L();
    void TestDestroy3Q4L();
    void TestDestroy4Q4L();

    void TestVideoRecorderOpenErrorQ4L();
    void TestVideoRecorderPrepareErrorQ4L();
    void TestVideoRecordingCompleteErrorQ4L();
    void TestPrepareVideoSettingsErrorQ4L();

    void TestPrepareAudioBitRateVR0Q4L();
    void TestPrepareAudioBitRateVR5150Q4L();
    void TestPrepareAudioBitRateVR12200Q4L();
    void TestPrepareAudioBitRateVR20000Q4L();
    void TestPrepareAudioBitRateVR12200AudioType0Q4L();
    void TestPrepareAudioBitRateVR12200AudioTypeInvQ4L();

    void TestInitVideoRecorderWithUidQ4L();


private:
    CCaeTestStatesClient * iStatesClient;
};

#endif // CAE_TEST_VIDEO_STATES_H
