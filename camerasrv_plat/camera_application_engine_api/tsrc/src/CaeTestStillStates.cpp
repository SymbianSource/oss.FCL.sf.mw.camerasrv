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
* Description: CaeTestStillStates.cpp
*
*/

#include "CaeTestStillStates.h"
#include <f32file.h>

/*
#if ( defined (__WINS__) || defined (__WINSCW) )
_LIT(KProcessFileName, "c:\\stillstates.dat");
#else
_LIT(KProcessFileName, "e:\\stillstates.dat");
#endif
*/

//const TReal32 KViewFinderFrameFrequencyMinimum = 13.0;



CCaeTestStillStates::CCaeTestStillStates ()
    {
    }



CCaeTestStillStates::~CCaeTestStillStates ()
    {
    }


void CCaeTestStillStates::setUpL ()
    {
	iStatesClient = CCaeTestStatesClient::NewL();

    iStatesClient->SetFileNameBase( KFileNameBase );
    }


void CCaeTestStillStates::tearDown ()
    {
    delete iStatesClient;    
    iStatesClient = 0;
    }


//
// Own test functions.
//
// The framework will do for a test function:
//
// setUpL();
// TRAPD(leaveCode, 'myTestFunction()');
// "Report leave error or assertion failure for 'myTestFunction()'".
// tearDown();
//

/********************************************************/
/*               PREPARE STILL CAPTURE                  */
/********************************************************/

void CCaeTestStillStates::TestPrepareStillCaptureNeg1L()
    {
	// ID: CAE.SC.PREPARESTILLCAPTUREL.005
    //
    // Group:   Capture
    // 
    // Action : Try to prepare the engine for still image capture using invalid quality level index. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetStillQualityLevelIndex( -1 );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareStillCapturing ) );
    assertL( error != KErrNone );
    }

void CCaeTestStillStates::TestPrepareStillCapture3L()
    {
	// ID: CAE.SC.PREPARESTILLCAPTUREL.006
    //
    // Group:   Capture
    // 
    // Action : Try to prepare the engine for still image capture using invalid quality level index. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetStillQualityLevelIndex( 3 );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareStillCapturing ) );
    assertL( error != KErrNone );
    }

void CCaeTestStillStates::TestSpecPrepareSCUnsupportedFormatWhenInitializedL()
    {
	// ID: CAE.SC.PREPARESTILLCAPTUREL.008
    //
    // Group:   Capture
    // 
    // Action : Set still image capturing format to a format unsupported by Camera API implementation. 
	//          Initialization is done.
    //
	// Output : Leave.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizeVga( KImgWidthTooBig, KImgHeightTooBig );

    iStatesClient->SetStillFrameSize( sizeVga );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareStill ) );//verified if is correct action
    assertL( error != KErrNone );
    }

void CCaeTestStillStates::TestSpecPrepareSCSupportedFormatWhenPreparedL()
    {
	// ID: CAE.SC.PREPARESTILLCAPTUREL.009
    //
    // Group:   Capture
    // 
    // Action : Set still image capturing format to a format supported by Camera API implementation. 
	//          Still Capture prepared.
    //
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareStillWhenPrepared );//verified if is correct action
    }

void CCaeTestStillStates::TestPrepareCroppedStillCapturingL()
    {
	// ID: CAE.SC.PREPARESTILLCAPTUREL.011
    //
    // Group:   Capture
    // 
    // Action : 
	//      
	// Output : Leave.   

    iStatesClient->SetStillQualityLevelIndex( 1 );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareCroppedStillCapturing ) );
    assertL( error == KErrNotSupported );
    }

void CCaeTestStillStates::TestPrepareStillCaptureWhenPowerOffL()
    {
	// ID: CAE.SC.PREPARESTILLCAPTUREL.011
    //
    // Group:   Capture
    // 
    // Action : Prepare still capture when init is not ready
	//      
	// Output : Leave.   

    iStatesClient->SetStillQualityLevelIndex( 1 );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareStillCaptureWhenPowerOff ) );
    assertL( error == KErrNotReady );
    }

void CCaeTestStillStates::TestPrepareStillCaptureWhenRecordingL()
    {
	// ID: CAE.SC.PREPARESTILLCAPTUREL.012
    //
    // Group:   Capture
    // 
    // Action : Prepare still capture when recording
	//      
	// Output : Leave.   

    iStatesClient->SetStillQualityLevelIndex( 1 );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareStillCaptureWhenRecording ) );
    assertL( error == KErrNotReady );
    }

void CCaeTestStillStates::TestPrepareStillCaptrureWrongSizeL()
    {
	// ID: CAE.SC.PREPARESTILLCAPTUREL.013
    //
    // Group:   Capture
    // 
    // Action : Set still image capturing format to a format unsupported by Camera API implementation. 
	//          Initialization is done.
    //
	// Output : Leave.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize wrongSize( 600, 400 );

    iStatesClient->SetStillFrameSize( wrongSize );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareStill ) );//verified if is correct action
    assertL( error == KErrNotSupported );
    }

/********************************************************/
/*                 STATE CHANGE                         */
/********************************************************/

void CCaeTestStillStates::TestReleaseL()
    {
    // ID: CAE.GEN.RELESEANDRESERVE.002
	//
    // General
    // 
    // Action : Try to release the camera module.
	//      
	// Output : OK.   

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERelease );
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

void CCaeTestStillStates::TestReleaseReleaseL()
    {
    // ID: CAE.GEN.RELESEANDRESERVE.003
	//
    // General
    // 
    // Action : Try to release the camera module twice.
	//      
	// Output : OK.   

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EReleaseRelease);
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

void CCaeTestStillStates::TestReserveL()
    {
    // ID: CAE.GEN.RELESEANDRESERVE.004
	//
    // General
    // 
    // Action : Try to re-reserve the camera module after initial reservation.  
	//      
	// Output : OK.   

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EReserveReserve );
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

void CCaeTestStillStates::TestReleaseReserveCaptureL()
    {
    // ID: CAE.GEN.RELESEANDRESERVE.005
	//
    // General
    // 
    // Action : Try to re-reserve the camera module after initial reservation
    //          and releasing. Then capture still image.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetStillFileName( _L("cae_relres_ql_2.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EReleaseReserveCapture );
    assertL( iStatesClient->CountInitComplete() == 2 );
    }


/********************************************************/
/*                GENERAL TESTS                         */
/********************************************************/

void CCaeTestStillStates::TestPowerOffPowerOffL()
    {
    // ID: CAE.GEN.POWEROFFPOWEROFF.001
	//
    // General
    // 
    // Action : Test to turn camera power off two time, when initialization is done.
	//      
	// Output : OK.   

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPowerOffPowerOff );
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

void CCaeTestStillStates::TestCaptureAfterPowerOffOnL()
    {
	// ID: CAE.SC.CAPTURESTILL.POWEROFF.002
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture Vga Exif image 
    //          image after poweroff + poweron.
	//      
	// Output : No leave, image cae_snap_1_8_vga_poweroffon captured

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );
    
    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    //iStatesClient->SetSnapFileName( _L("cae_snap_1_8_vga_poweroff.mbm") );

    iStatesClient->SetSnapImageSize( TSize( 44, 52 ) );

    iStatesClient->SetStillFileName( _L("cae_snap_1_8_vga_poweroffon.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ECaptureStillAfterPowerOffOn );

    assertL( iStatesClient->CountInitComplete() == 2 );
    assertL( iStatesClient->CountStillPrepareComplete() == 2 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestStillStates::TestStartVfStartVfL()
    {
	// CAE.VF.STARTVIEWFINDERSTARTVIEWFINDER.001
    //
    // View Finder
    // 
    // Action : Test start two time view finder, when initialization is done.
	//      
	// Output : OK.   

    TSize sizeQqvga( KImgWidthQQVGA, KImgHeightQQVGA );

    iStatesClient->SetViewFinderSize( sizeQqvga );

    iStatesClient->SetTimeout( 100000 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfStartVf );
    }

void CCaeTestStillStates::TestStartVfWhenPowerOffL()
    {
	// CAE.VF.STARTVIEWFINDERWHENPOWEROFF.001
    //
    // View Finder
    // 
    // Action : Test to start view finder when power is off, initialization is done.
	//      
	// Output : Leave.  
    
    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfWhenPowerOff ) );
    assertL( error == KErrNotReady );
    }


void CCaeTestStillStates::TestCancelCaptureStillWhenInitializedL()
    {
	// CAE.SC.CANCEL.001
    //
    // Capture (actually just cancelling)
    // 
    // Action : Test to cancel still capturing when just initialization is done.
	//      
	// Output : OK.  
    
    iStatesClient->RunTestActionL( CCaeTestStatesClient::ECancelCaptureStillWhenInitialized );
    }


void CCaeTestStillStates::TestCancelCaptureStillAtSnapL()
    {
	// CAE.SC.CANCEL.002
    //
    // Capture
    // 
    // Action : Test to cancel still capturing after snap image callback. Captures an image after cancelling.
	//      
	// Output : OK.  
    
    iStatesClient->SetStillQualityLevelIndex( 0 );
    
    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureAndCancelStillAtSnap );
    
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 2 ); // == 2 because calling PrepareStillCaptureL after cancelling
    assertL( iStatesClient->CountSnapImageReady() == 3 );
    // Changed this from 2 to 3. Before it was timing dependent, giving either value. Now the case is timed better.
    assertL( iStatesClient->CountStillImageReady() == 3 ); 
    }


/********************************************************/
/*                STILL BURST TESTS                     */
/********************************************************/


void CCaeTestStillStates::TestCaptureCaptureStillBurst3L()
    {
	// ID: CAE.SC.CAPTURECAPTURESTILLBURST.3.001
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          images in a burst when initialization is done. Then repeat the burst capture.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetSnapFileName( _L("cae_snap_vga16M_burstburst3.mbm") );

    iStatesClient->SetStillFileName( _L("cae_vga16M_burstburst3.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureCaptureStillBurst );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == ( 2 * KStillBurstLength ) );
    assertL( iStatesClient->CountSnapImageReady() == ( 2 * KStillBurstLength ) );
    assertL( iStatesClient->CountStillImageReady() == ( 2 * KStillBurstLength ) );
    assertL( iStatesClient->CountStillBurstComplete() == 2 );
    }

void CCaeTestStillStates::TestSpecPrepCaptureCaptureStillBurst3L()
    {
	// ID: CAE.SC.CAPTURECAPTURESTILLBURST.3.002
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          images in a burst when initialization is done. Then repeat the burst capture.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizevga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizevga );

    iStatesClient->SetSnapFileName( _L("cae_snap_vga16M_spburstburst3.mbm") );

    iStatesClient->SetStillFileName( _L("cae_vga16M_spburstburst3.mbm") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecPrepAndCaptureCaptureStillBurst );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == ( 2 * KStillBurstLength ) );
    assertL( iStatesClient->CountSnapImageReady() == ( 2 * KStillBurstLength ) );
    assertL( iStatesClient->CountStillImageReady() == ( 2 * KStillBurstLength ) );
    assertL( iStatesClient->CountStillBurstComplete() == 2 );
    }

void CCaeTestStillStates::TestCaptureStillBurst3CaptureStillL()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTCAPTURESTILL.3.001
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          images in a burst when initialization is done. Then repeat the burst capture.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetSnapFileName( _L("cae_snap_vga16M_burst3capture.mbm") );

    iStatesClient->SetStillFileName( _L("cae_vga16M_burst3capture.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurstCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountSnapImageReady() == ( KStillBurstLength + 1 ) );
    assertL( iStatesClient->CountStillImageReady() == ( KStillBurstLength + 1 ) );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }

void CCaeTestStillStates::TestSpecPrepCaptureStillBurst3CaptureStillL()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTCAPTURESTILL.3.002
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          images in a burst when initialization is done. Then repeat the burst capture.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizevga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizevga );

    iStatesClient->SetSnapFileName( _L("cae_snap_qqvga16M_spburst3capture.mbm") );

    iStatesClient->SetStillFileName( _L("cae_qqvga16M_spburst3capture.mbm") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecPrepAndCaptureStillBurstCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountSnapImageReady() == ( KStillBurstLength + 1 ) );
    assertL( iStatesClient->CountStillImageReady() == ( KStillBurstLength + 1 ) );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }


void CCaeTestStillStates::TestCancelCaptureStillBurst3AtSnapL()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.CANCEL.007
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          images in a burst and cancel still burst after snap image callback, 
    //          initialization is done. After cancelling captures a burst.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetSnapFileName( _L("cae_snap_vga16M_burst_cancel.mbm") );

    iStatesClient->SetStillFileName( _L("cae_vga16M_burst_cancel.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureAndCancelStillBurstAtSnap );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 2 ); // == 2 because calling PrepareStillCaptureL after cancel
    assertL( iStatesClient->CountStillBurstCaptureMoment() == 3 * KStillBurstLength );
    assertL( iStatesClient->CountSnapImageReady() >= KStillBurstLength + 1 );
    assertL( iStatesClient->CountStillImageReady() >= KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }


void CCaeTestStillStates::TestCancelCaptureStillBurst3AtMomentL()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.CANCEL.008
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          images in a burst and cancel still burst in the second capture moment callback.
    //          Initialization is done. After cancelling prepares, captures burst, and cancels.
    //          Then captures a burst that is not cancelled.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetSnapFileName( _L("cae_snap_vga16M_burst_cancelmom.mbm") );

    iStatesClient->SetStillFileName( _L("cae_vga16M_burst_cancelmom.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureAndCancelStillBurstAtMoment );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 2 ); // == 2 because calling PrepareStillCaptureL after cancel
    assertL( iStatesClient->CountStillBurstCaptureMoment() == ( 2 + 1 + KStillBurstLength ) );
    assertL( iStatesClient->CountSnapImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }


void CCaeTestStillStates::TestCaptureStillBurstMp2Exif100IgnoreErrorsL()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.MP2EXIF.IGNORE.100.001
    //
    // Group: Capture
    // 
    // Action : Test OOM case using too long burst with 2 megapixel Exif images. OOM error is ignored in callbacks 
	//			and the test case tries to get all snap and still images from the engine.  
	//      
	// Output : Leave

    const TInt KStillBurstLength = 100;
    const TSize sizeMp2( KImgWidthMP2, KImgHeightMP2 );

	iStatesClient->SetStopOnErrorInMcaeoSnapImageReady( EFalse );
	iStatesClient->SetStopOnErrorInMcaeoStillImageReady( EFalse );

    iStatesClient->SetStillFrameSize( sizeMp2 );
    iStatesClient->SetStillFormat( CCamera::EFormatExif );
    iStatesClient->SetStillBurstLength( KStillBurstLength );

	TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurstExif ) );

    assertL( error == KErrNoMemory );
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == iStatesClient->CountSnapImageReady() + 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == iStatesClient->CountStillImageReady() + 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == iStatesClient->CountImageDeliveredStillBurstComplete() + 1 );	
    }

void CCaeTestStillStates::TestCaptureStillBurstMp1Exif100IgnoreErrorsL()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.MP1EXIF.IGNORE.100.001
    //
    // Group: Capture
    // 
    // Action : Test OOM case using too long burst with one megapixel Exif images. OOM error is ignored in callbacks 
	//			and the test case tries to get all snap and still images from the engine.  
	//      
	// Output : Leave

    const TInt KStillBurstLength = 100;
    const TSize sizeMp1M3( KImgWidthMP1M3, KImgHeightMP1M3 );

	iStatesClient->SetStopOnErrorInMcaeoSnapImageReady( EFalse );
	iStatesClient->SetStopOnErrorInMcaeoStillImageReady( EFalse );

    iStatesClient->SetStillFrameSize( sizeMp1M3 );
    iStatesClient->SetStillFormat( CCamera::EFormatExif );
    iStatesClient->SetStillBurstLength( KStillBurstLength );

	TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurstExif ) );

    assertL( error == KErrNoMemory );
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == iStatesClient->CountSnapImageReady() + 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == iStatesClient->CountStillImageReady() + 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == iStatesClient->CountImageDeliveredStillBurstComplete() + 1 );	
    }



void CCaeTestStillStates::TestCaptureStillBurstVgaBitmap100IgnoreErrorsL()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.VGABITMAP.IGNORE.100.002
    //
    // Group: Capture
    // 
    // Action : Test OOM case using too long burst with VGA bitmaps. OOM error is ignored in callbacks 
	//			and the test case tries to get all snap and still images from the engine.  
	//      
	// Output : Leave    

    const TInt KStillBurstLength = 100;
    const TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

	iStatesClient->SetStopOnErrorInMcaeoSnapImageReady( EFalse );
	iStatesClient->SetStopOnErrorInMcaeoStillImageReady( EFalse );

    iStatesClient->SetStillFrameSize( sizeVga );
    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );
    iStatesClient->SetStillBurstLength( KStillBurstLength );

	TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurst ) );

    assertL( error == KErrNoMemory );
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == iStatesClient->CountSnapImageReady() + 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == iStatesClient->CountStillImageReady() + 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == iStatesClient->CountImageDeliveredStillBurstComplete() + 1 );	
    }


void CCaeTestStillStates::TestCaptureStillBurstVgaJpeg100IgnoreErrorsL()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.VGAJPEG.IGNORE.100.003
    //
    // Group: Capture
    // 
    // Action : Test OOM case using too long burst with VGA Jpeg images. OOM error is ignored in callbacks 
	//			and the test case tries to get all snap and still images from the engine.  
	//      
	// Output : Leave    

    const TInt KStillBurstLength = 100;
    const TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

	iStatesClient->SetStopOnErrorInMcaeoSnapImageReady( EFalse );
	iStatesClient->SetStopOnErrorInMcaeoStillImageReady( EFalse );

    iStatesClient->SetStillFrameSize( sizeVga );
    iStatesClient->SetStillFormat( CCamera::EFormatJpeg );
    iStatesClient->SetStillBurstLength( KStillBurstLength );

	TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurstExif ) );

    assertL( error == KErrNoMemory );
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == iStatesClient->CountSnapImageReady() + 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == iStatesClient->CountStillImageReady() + 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == iStatesClient->CountImageDeliveredStillBurstComplete() + 1 );	
    }



/********************************************************/
/*                "DESTROY ENGINE"-TESTS                */
/********************************************************/


void CCaeTestStillStates::TestDestroy1L()
    {
	// ID: CAE.SC.DESTROY.001
    //
    // Group:   General
    // 
    // Action : Test to destroy the engine when still capture prepare is ready. 
	//      
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenStillPrepareReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == 0 );
    }


void CCaeTestStillStates::TestDestroy2L()
    {
	// ID: CAE.SC.DESTROY.002
    //
    // Group:   General
    // 
    // Action : Test to destroy the engine just after snap image callback. 
	//      
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatJpeg );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenSnapImageReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 0 );
    }


void CCaeTestStillStates::TestDestroy3L()
    {
	// ID: CAE.SC.DESTROY.003
    //
    // Group:   General
    // 
    // Action : Test to destroy the engine just after snap image callback. 
	//      
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenSnapImageReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestStillStates::TestDestroy4L()
    {
	// ID: CAE.SC.DESTROY.004
    //
    // Group:   General
    // 
    // Action : Test to destroy the engine just after still image callback. 
	//      
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatJpeg );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenStillCaptureReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestStillStates::TestDestroy5L()
    {
	// ID: CAE.SC.DESTROY.005
    //
    // Group:   General
    // 
    // Action : Test to destroy the engine just after still burst ready callback. 
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetStillFormat( CCamera::EFormatJpeg );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenStillBurstReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountSnapImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }


void CCaeTestStillStates::TestDestroy6L()
    {
	// ID: CAE.SC.DESTROY.006
    //
    // Group:   General
    // 
    // Action : Test to destroy the engine just after the first snap image callback in still burst. 
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetStillFormat( CCamera::EFormatJpeg );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenSnapImageReadyInStillBurst );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountSnapImageReady() >= 1 );
    assertL( iStatesClient->CountStillImageReady() == 0 );
    assertL( iStatesClient->CountStillBurstComplete() == 0 );
    }


void CCaeTestStillStates::TestDestroy7L()
    {
	// ID: CAE.SC.DESTROY.007
    //
    // Group:   General
    // 
    // Action : Test to destroy the engine just after the first still image callback in still burst. 
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetStillFormat( CCamera::EFormatJpeg );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenStillImageReadyInStillBurst );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountSnapImageReady() >= 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    assertL( iStatesClient->CountStillBurstComplete() == 0 );
    }


/********************************************************/
/*                ERROR SIMULATIONS                     */
/********************************************************/


void CCaeTestStillStates::TestEncodingErrorL()
    {
	// ID: CAE.SC.ERRORS.001
    //
    // Group:   Errors.
    // 
    // Action : Test still image capture with simulated encoding error.
	//      
	// Output : Leave with code KErrAbort.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EEncodingError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestStillStates::TestPowerErrorL()
    {
	// ID: CAE.SC.ERRORS.002
    //
    // Group:   Errors.
    // 
    // Action : Test still image capture with simulated camera power error.
	//      
	// Output : Leave with code KErrAbort.   

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPowerError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 0 );
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == 0 );
    }


void CCaeTestStillStates::TestReserveErrorL()
    {
	// ID: CAE.SC.ERRORS.003
    //
    // Group:   Errors.
    // 
    // Action : Test still image capture with simulated camera reservation error.
	//      
	// Output : Leave with code KErrAbort.   

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EReserveError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 0 );
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == 0 );
    }


void CCaeTestStillStates::TestStillCaptureErrorL()
    {
	// ID: CAE.SC.ERRORS.004
    //
    // Group:   Errors.
    // 
    // Action : Test still image capture with simulated capture error.
	//      
	// Output : Leave with code KErrAbort.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStillCaptureError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 0 );
    }


void CCaeTestStillStates::TestSnapImageErrorL()
    {
	// ID: CAE.SC.ERRORS.005
    //
    // Group:   Errors.
    // 
    // Action : Test still image capture with simulated snap image error.
	//      
	// Output : Leave with code KErrAbort.   

    const TSize sizeMp1M3( KImgWidthMP1M3, KImgHeightMP1M3 );
    iStatesClient->SetStillFrameSize( sizeMp1M3 );
    iStatesClient->SetStillFormat( CCamera::EFormatExif );


    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESnapImageError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 0 );
    }


void CCaeTestStillStates::TestStillImageErrorL()
    {
	// ID: CAE.SC.ERRORS.006
    //
    // Group:   Errors.
    // 
    // Action : Test still image capture with simulated still image error.
	//      
	// Output : Leave with code KErrAbort.   

    const TSize sizeMp1M3( KImgWidthMP1M3, KImgHeightMP1M3 );
    iStatesClient->SetStillFrameSize( sizeMp1M3 );
    iStatesClient->SetStillFormat( CCamera::EFormatJpeg );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStillImageError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestStillStates::TestStillConvertError1L()
    {
	// ID: CAE.SC.ERRORS.007
    //
    // Group:   Errors.
    // 
    // Action : Test still image capture with simulated still image conversion error.
	//      
	// Output : Leave with code KErrAbort.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStillConvertError1 ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestStillStates::TestStillConvertError2L()
    {
	// ID: CAE.SC.ERRORS.008
    //
    // Group:   Errors.
    // 
    // Action : Test still image capture with simulated still image conversion error.
	//      
	// Output : Leave with code KErrAbort.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStillConvertError2 ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestStillStates::TestStillConvertError3L()
    {
	// ID: CAE.SC.ERRORS.009
    //
    // Group:   Errors.
    // 
    // Action : Test still image capture with simulated still image conversion error.
	//      
	// Output : Leave with code KErrAbort.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStillConvertError3 ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestStillStates::TestSetCameraIndexLowL()
    {
	// ID: CAE.SC.SETCAMERAINDEX.001
    //
    // Group:   Capture
    // 
    // Action : Set camera index to -1 and capture a still image.
	//      
	// Output : leave

    iStatesClient->SetDisplayIndex( 1 );
    iStatesClient->SetSnapFileName( _L("cae_snap_camera_index_1.mbm") );
    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESetCameraIndexLow ) );
    assertL( error == KErrHardwareNotAvailable );
    }

void CCaeTestStillStates::TestSetCameraIndexHighL()
    {
	// ID: CAE.SC.SETCAMERAINDEX.002
    //
    // Group:   Capture
    // 
    // Action : Set camera index to too big value and capture a still image.
	//      
	// Output : leave

    iStatesClient->SetDisplayIndex( 1 );
    iStatesClient->SetSnapFileName( _L("cae_snap_camera_index_1.mbm") );
    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESetCameraIndexHigh ) );
    assertL( error == KErrHardwareNotAvailable );
    }

//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 

MTest* CCaeTestStillStates::suiteL ()
    {
	// Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL( _L8("CCaeTestStillStates") );

    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.PREPARESTILLCAPTUREL.005"), &TestPrepareStillCaptureNeg1L ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.PREPARESTILLCAPTUREL.006"), &TestPrepareStillCapture3L ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.PREPARESTILLCAPTUREL.008"), &TestSpecPrepareSCUnsupportedFormatWhenInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.PREPARESTILLCAPTUREL.009"), &TestSpecPrepareSCSupportedFormatWhenPreparedL ) );
   
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.PREPARESTILLCAPTUREL.010"), &TestPrepareCroppedStillCapturingL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.PREPARESTILLCAPTUREL.011"), &TestPrepareStillCaptureWhenPowerOffL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.PREPARESTILLCAPTUREL.012"), &TestPrepareStillCaptureWhenRecordingL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.PREPARESTILLCAPTUREL.013"), &TestPrepareStillCaptrureWrongSizeL ) );
    
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.GEN.RELESEANDRESERVE.002"), &TestReleaseL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.GEN.RELESEANDRESERVE.003"), &TestReleaseReleaseL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.GEN.RELESEANDRESERVE.004"), &TestReserveL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.GEN.RELESEANDRESERVE.005"), &TestReleaseReserveCaptureL ) );
    
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.GEN.POWEROFFPOWEROFF.001"), &TestPowerOffPowerOffL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.CAPTURESTILL.POWEROFF.002"), &TestCaptureAfterPowerOffOnL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.VF.STARTVIEWFINDERSTARTVIEWFINDER.001"), &TestStartVfStartVfL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.VF.STARTVIEWFINDERWHENPOWEROFF.001"), &TestStartVfWhenPowerOffL ) );
        
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.CANCEL.001"), &TestCancelCaptureStillWhenInitializedL ) );
    
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.CANCEL.002"), &TestCancelCaptureStillAtSnapL ) );

#ifdef __CAMERA_BURST_MODE
    // Only if Still BURST is supported (built in).
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.CAPTURECAPTURESTILLBURST.3.001"), &TestCaptureCaptureStillBurst3L ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.CAPTURECAPTURESTILLBURST.3.002"), &TestSpecPrepCaptureCaptureStillBurst3L ) );
    
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTCAPTURESTILL.3.001"), &TestCaptureStillBurst3CaptureStillL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTCAPTURESTILL.3.002"), &TestSpecPrepCaptureStillBurst3CaptureStillL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.CANCEL.007"), &TestCancelCaptureStillBurst3AtSnapL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.CANCEL.008"), &TestCancelCaptureStillBurst3AtMomentL ) );

#if (MIRAGE_X_PROD2) || (MIRAGE_X_PROD4)
	suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.MP1EXIF.IGNORE.100.001"), &TestCaptureStillBurstMp1Exif100IgnoreErrorsL ) );
#endif
#if (MIRAGE_X_PROD7)
	suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.MP2EXIF.IGNORE.100.001"), &TestCaptureStillBurstMp2Exif100IgnoreErrorsL ) );
#endif
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.VGABITMAP.IGNORE.100.002"), &TestCaptureStillBurstVgaBitmap100IgnoreErrorsL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.VGAJPEG.IGNORE.100.003"), &TestCaptureStillBurstVgaJpeg100IgnoreErrorsL ) );

#endif // __CAMERA_BURST_MODE

    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.DESTROY.001"), &TestDestroy1L ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.DESTROY.002"), &TestDestroy2L ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.DESTROY.003"), &TestDestroy3L ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.DESTROY.004"), &TestDestroy4L ) );

#ifdef __CAMERA_BURST_MODE
    // Only if Still BURST is supported (built in).
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.DESTROY.005"), &TestDestroy5L ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.DESTROY.006"), &TestDestroy6L ) );    
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.DESTROY.007"), &TestDestroy7L ) );
#endif
    
    #ifdef CAE_TEST_VERSION
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.ERRORS.001"), &TestEncodingErrorL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.ERRORS.002"), &TestPowerErrorL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.ERRORS.003"), &TestReserveErrorL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.ERRORS.004"), &TestStillCaptureErrorL ) );
#if (MIRAGE_X_PROD2) || (MIRAGE_X_PROD4)
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.ERRORS.005"), &TestSnapImageErrorL ) );
#endif
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.ERRORS.006"), &TestStillImageErrorL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.ERRORS.007"), &TestStillConvertError1L ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.ERRORS.008"), &TestStillConvertError2L ) );
    // suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.ERRORS.009"), &TestStillConvertError3L ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.SETCAMERAINDEX.001"), &TestSetCameraIndexLowL ) );
    suite->addTestL( CTestCaller<CCaeTestStillStates>::NewL( _L8("CAE.SC.SETCAMERAINDEX.002"), &TestSetCameraIndexHighL ) );
#endif
    return suite;
    }




