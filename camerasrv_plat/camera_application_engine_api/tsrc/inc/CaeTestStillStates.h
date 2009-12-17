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
* Description: CaeTestStillStates.h
*
*/
#ifndef CAE_TEST_STILL_STATES_H
#define CAE_TEST_STILL_STATES_H


#include <e32base.h>

#include "CaeTestStatesClient.h"

class CCaeTestStillStates : public CTestCase
{
public:

	CCaeTestStillStates ();
    ~CCaeTestStillStates ();

	// Allocate the resources for one test function
    void setUpL ();

	// Free the resources reserved in setUpL()
	void tearDown ();

	// A function to collect and return a suite of tests
    static MTest* suiteL ();

protected:

	// Own test functions that use assertions and may leave:
    void TestPrepareStillCaptureNeg1L();   
    void TestPrepareStillCapture3L();
    void TestSpecPrepareSCUnsupportedFormatWhenInitializedL();
    void TestSpecPrepareSCSupportedFormatWhenPreparedL();

    void TestPrepareCroppedStillCapturingL();
    void TestPrepareStillCaptureWhenPowerOffL();
    void TestCaptureAfterPowerOffOnL();

    void TestPrepareStillCaptureWhenRecordingL();
    void TestPrepareStillCaptrureWrongSizeL();

    void TestReleaseL();
    void TestReleaseReleaseL();
    void TestReserveL();
    void TestReleaseReserveCaptureL();

    void TestPowerOffPowerOffL();
    void TestStartVfStartVfL();
    void TestStartVfWhenPowerOffL();
    
    void TestCancelCaptureStillWhenInitializedL();
    void TestCancelCaptureStillAtSnapL();

    void TestCaptureCaptureStillBurst3L();
    void TestSpecPrepCaptureCaptureStillBurst3L();
    void TestCaptureStillBurst3CaptureStillL();
    void TestSpecPrepCaptureStillBurst3CaptureStillL();
    void TestCancelCaptureStillBurst3AtSnapL();
    void TestCancelCaptureStillBurst3AtMomentL();
	void TestCaptureStillBurstMp2Exif100IgnoreErrorsL();
	void TestCaptureStillBurstMp1Exif100IgnoreErrorsL();
	void TestCaptureStillBurstVgaBitmap100IgnoreErrorsL();
	void TestCaptureStillBurstVgaJpeg100IgnoreErrorsL();

    void TestDestroy1L();
    void TestDestroy2L();
    void TestDestroy3L();
    void TestDestroy4L();
    void TestDestroy5L();
    void TestDestroy6L();
    void TestDestroy7L();
    
    void TestEncodingErrorL();
    void TestPowerErrorL();
    void TestReserveErrorL();
    void TestStillCaptureErrorL();
    void TestSnapImageErrorL();
    void TestStillImageErrorL();
    void TestStillConvertError1L();
    void TestStillConvertError2L();
    void TestStillConvertError3L();
    void TestSetCameraIndexLowL();
    void TestSetCameraIndexHighL();

private:

    CCaeTestStatesClient * iStatesClient;
};

#endif // CAE_TEST_STILL_STATES_H
