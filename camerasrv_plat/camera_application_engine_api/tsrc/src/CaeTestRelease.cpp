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
* Description: CaeTestRelease.cpp
*
*/

#include "CaeTestRelease.h"
#include <f32file.h>
#include <e32base.h>
#include <e32svr.h>


const TReal32 KViewFinderFrameFrequencyMinimum = 15.0;



CCaeTestRelease::CCaeTestRelease ()
    {
    }



CCaeTestRelease::~CCaeTestRelease ()
    {
    }


void CCaeTestRelease::setUpL ()
    {
    iStatesClient = CCaeTestStatesClient::NewL( );

    iStatesClient->SetFileNameBase( KFileNameBase );

    iSettingsClient = CCaeTestSettingsClient::NewL();

    iSettingsClient->SetFileNameBase( KFileNameBase );
    }


void CCaeTestRelease::tearDown ()
    {
    // pointers in other test e.g. still states tests etc. are not cleared
    delete iStatesClient; 
    iStatesClient = NULL;
    delete iSettingsClient;   
    iSettingsClient = NULL;
//    User::After(30000000);
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
/*                   GENERAL TESTS                      */
/********************************************************/

void CCaeTestRelease::TestInitL()
    {
    // ID: CAE.GEN.INITL.001
	//
    // General
    // 
    // Action : Test call InitL() when instatiation is done.
	//      
	// Output : OK.   

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EInit );// here we have to put the correct action
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

void CCaeTestRelease::TestInitVideoRecorderL()
    {
    // ID: CAE.GEN.INITVIDEORECORDERL.001
	//
    // General
    // 
    // Action : Test Call InitVideoRecorderL() when initialization is done.
	//      
	// Output : OK.   

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EInitVideoRecorder );// here we have to put the correct action
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

void CCaeTestRelease::TestPowerOffAndOnL()
    {
    // ID: CAE.GEN.POWEROFFANDON.001
	//
    // General
    // 
    // Action : Test to turn camera power off and on when initialization is done.
	//      
	// Output : OK.   

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESwitchPowerOffAndOn );
    assertL( iStatesClient->CountInitComplete() == 2 );
    }

void CCaeTestRelease::TestReleaseAndReserveL()
    {
    // ID: CAE.GEN.RELESEANDRESERVE.001
	//
    // General
    // 
    // Action : Release vapauttaa kameran muiden sovellusten käyttöön.
    //          Reserve varaa kameran (uudestaan) ja palauttaa entiset asetukset (kontrasti,....) voimaan. 
	//      
	// Output : OK.   

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EReleaseReserve );
    assertL( iStatesClient->CountInitComplete() == 2 );
    }

void CCaeTestRelease::TestCCameraHandleL()
    {
    // ID: CAE.GEN.CCAMERAHANDLE.001
	//
    // General
    // 
    // Action : Test call CCameraHandle when instatiation is done.
	//      
	// Output : OK.   

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ECCameraHandle );
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

/********************************************************/
/*            STILL CAPTURE GENERAL TESTS               */
/********************************************************/

void CCaeTestRelease::TestPowerOnWhenInitializedL()
    {
    // ID: CAE.GEN-SC.POWERON.001
	//
    // General
    // 
    // Action : Test to turn camera power on when initialization is done.
	//      
	// Output : OK.   

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESwitchPowerOnWhenInitialized );// here we have to put the correct action
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

void CCaeTestRelease::TestPowerOnWhenStillCapturePreparedL()
    {
    // ID: CAE.GEN-SC.POWERON.002
	//
    // General
    // 
    // Action : Test to turn camera power on when still capture prepared.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESwitchPowerOnWhenStillPrepared );// here we have to put the correct action
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

void CCaeTestRelease::TestPowerOffWhenStillCapturePreparedL()
    {
    // ID: CAE.GEN-SC.POWEROFF.001
	//
    // General
    // 
    // Action : Test to turn camera power off when still capture prepared.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESwitchPowerOffWhenStillPrepared );// here we have to put the correct action
    
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

void CCaeTestRelease::TestPowerOffWhenViewfinderRunningL()
    {
    // ID: CAE.GEN-SC.POWEROFF.002
	//
    // General
    // 
    // Action : Test to turn camera power off when viewfinder running.
	//      
	// Output : OK.   

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESwitchPowerOffWhenVfRunning );// here we have to put the correct action
    
    assertL( iStatesClient->CountInitComplete() == 1 );
    }


/********************************************************/
/*           VIDEO RECORDING GENERAL TESTS              */
/********************************************************/

void CCaeTestRelease::TestPowerOnWhenVideoRecordingPreparedL()
    {
    // ID: CAE.GEN-VR.POWERON.003
	//
    // General
    // 
    // Action : Test to turn camera power on when video recording prepared.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESwitchPowerOnWhenVideoPrepared );// here we have to put the correct action
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

void CCaeTestRelease::TestPowerOnWhenRecordingL()
    {
    // ID: CAE.GEN.VR.POWERON.004
	//
    // General
    // 
    // Action : Test to turn camera power on when recording.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 1000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESwitchPowerOnWhenRecordingVideo );// here we have to put the correct action
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

void CCaeTestRelease::TestPowerOffWhenVideoRecordingPreparedL()
    {
    // ID: CAE.GEN-VR.POWEROFF.003
	//
    // General
    // 
    // Action : Test to turn camera power off video recording prepared.
	//      
	// Output : OK.   

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESwitchPowerOffWhenVideoPrepared );// here we have to put the correct action
    }

void CCaeTestRelease::TestPowerOffWhenRecordingL()
    {
    // ID: CAE.GEN-VR.POWEROFF.004
	//
    // General
    // 
    // Action : Test to turn camera power off when recording.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 1000000 );

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESwitchPowerOffWhenRecordingVideo );// here we have to put the correct action
    }

/********************************************************/
/*                    VIEW FINDER                       */
/********************************************************/

void CCaeTestRelease::TestViewFinderStartAndStopQqvgaL()
    {
	// CAE.VF.STARTSTOPVIEWFINDER.QQVGA.001
    //
    // View Finder
    // 
    // Action : Test to start and stop QQVGA view finder when initialization is done.
	//      
	// Output : OK.   

    TSize sizeQqvga( KImgWidthQQVGA, KImgHeightQQVGA );

    iStatesClient->SetViewFinderSize( sizeQqvga );
    iStatesClient->SetTimeout( 10000000 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartAndStopVf );

    TReal32 viewFinderFrameFrequency = iStatesClient->ViewFinderFrameFrequency();

    assertL( viewFinderFrameFrequency >= KViewFinderFrameFrequencyMinimum );
    }

void CCaeTestRelease::TestViewFinderStartAndStopSubqcifL()
    {
	// CAE.VF.STARTSTOPVIEWFINDER.SUBQCIF.002
    //
    // View Finder
    // 
    // Action : Test to start and stop SUBQCIF view finder when initialization is done.
	//      
	// Output : OK.   

    TSize sizeSubqcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetViewFinderSize( sizeSubqcif );
    iStatesClient->SetTimeout( 10000000 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartAndStopVf );

    TReal32 viewFinderFrameFrequency = iStatesClient->ViewFinderFrameFrequency();

    assertL( viewFinderFrameFrequency >= KViewFinderFrameFrequencyMinimum );
    }

void CCaeTestRelease::TestCroppedViewFinderStartAndStopL()
    {
	// CAE.VF.STARTSTOPVIEWFINDER.CROPPED.003
    //
    // View Finder
    // 
    // Action : Test to start and stop cropped view finder when initialization is done.
	//      
	// Output : OK.   

    TSize sizeQqvga( KImgWidthQQVGA, KImgHeightQQVGA );

    iStatesClient->SetViewFinderSize( sizeQqvga );
    iStatesClient->SetTimeout( 100000 );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartAndStopCroppedVf ) );
    if( error == KErrNoMemory )
        {
        User::Leave( error ); // For memory test
        }
    assertL( error == KErrNotSupported );

    TReal32 viewFinderFrameFrequency = iStatesClient->ViewFinderFrameFrequency();

    assertL( viewFinderFrameFrequency >= KViewFinderFrameFrequencyMinimum );
    }

void CCaeTestRelease::TestViewFinderDirectStartAndStopL()
    {
	// CAE.VF.STARTSTOPVIEWFINDERDIRECT.001
    //
    // View Finder
    // 
    // Action : Test to start and stop view finder direct when initialization is done.
	//      
	// Output : Leave.  
    
    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartAndStopDirectVf ) );
    if( error == KErrNoMemory )
        {
        User::Leave( error ); // For memory test
        }
#if (HW_DEVLON)
    assertL( error != KErrNotSupported );  // Returns error because illegal parameters (NULL)
#else
    assertL( error == KErrNotSupported );
#endif
    }

void CCaeTestRelease::TestCroppedViewFinderDirectStartAndStopL()
    {
	// CAE.VF.STARTSTOPVIEWFINDERDIRECT.002
    //
    // View Finder
    // 
    // Action : Test to start and stop cropped view finder direct when initialization is done.
	//      
	// Output : Leave.  
    
    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartAndStopCroppedDirectVf ) );
    if( error == KErrNoMemory )
        {
        User::Leave( error ); // For memory test
        }
#if (HW_DEVLON)
    assertL( error == KErrNotSupported );  // Not supported
#else
    assertL( error == KErrNotSupported );
#endif
    }

/********************************************************/
/*              STILL CAPTURE QUALITY LEVEL             */
/********************************************************/

void CCaeTestRelease::TestCaptureQualityLevel0L()
    {
	// ID: CAE.SC.CAPTURESTILL.JPEGVGA90.008
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture quality level 0 
    //          image when initialization is done and viewfinder is running.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetSnapFileName( _L("cae_snap_vga16M.mbm") );

    iStatesClient->SetStillFileName( _L("cae_ql_0_vga90.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareCaptureStillStartAndStopVf );
    
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureQualityLevel1L()
    {
	// ID: CAE.SC.CAPTURESTILL.JPEGVGA50.009

    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture quality level 1 
    //          image when initialization is done and viewfinder is running.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 1 );
    
    iStatesClient->SetStillFileName( _L("cae_ql_1_vga50.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareCaptureStillStartAndStopVf );
    }


void CCaeTestRelease::TestCaptureQualityLevel2L()
    {
	// ID: CAE.SC.CAPTURESTILL.JPEGQQVGA90.010

    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture quality level 2 
    //          image when initialization is done and viewfinder is running.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 2 );
    
    iStatesClient->SetStillFileName( _L("cae_ql_2_qqvga90.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareCaptureStillStartAndStopVf );
    }


/********************************************************/
/*                    STILL CAPTURE                     */
/********************************************************/

void CCaeTestRelease::TestCaptureVga16ML()
    {
	// ID: CAE.SC.CAPTURESTILL.VGA16M.002
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture VGA 16M  
    //          image when initialization is done.
	//      
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );

    iStatesClient->SetStillFileName( _L("cae_vga16M.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureVga64KL()
    {
	// ID: CAE.SC.CAPTURESTILL.VGA64K.003
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture VGA 64K  
    //          image when initialization is done.
	//      
	// Output : OK.   

    RDebug::Print(_L("XYZ:TestCaptureVga64KL start"));

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor64K );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );

    iStatesClient->SetStillFileName( _L("cae_vga64K.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    
    RDebug::Print(_L("XYZ:TestCaptureVga64KL ending"));
    }


void CCaeTestRelease::TestCaptureVga4KL()
    {
	// ID: CAE.SC.CAPTURESTILL.VGA4K.004
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture VGA 4K 
    //          image when initialization is done.
	//      
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor4K );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );

    iStatesClient->SetStillFileName( _L("cae_vga4K.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestRelease::TestCaptureQqvga16ML()
    {
	// ID: CAE.SC.CAPTURESTILL.QQVGA16M.005
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture QQVGA 16M  
    //          image when initialization is done.
	//      
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizeQqvga( KImgWidthQQVGA, KImgHeightQQVGA );

    iStatesClient->SetStillFrameSize( sizeQqvga );

    iStatesClient->SetSnapFileName( _L("cae_snap_qqvga16M.mbm") );

    iStatesClient->SetStillFileName( _L("cae_qqvga16M.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );
    }


void CCaeTestRelease::TestCaptureQqvga64KL()
    {
	// ID: CAE.SC.CAPTURESTILL.QQVGA64K.006
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture QQVGA 64K  
    //          image when initialization is done.
	//      
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor64K );

    TSize sizeQqvga( KImgWidthQQVGA, KImgHeightQQVGA );

    iStatesClient->SetStillFrameSize( sizeQqvga );

    iStatesClient->SetStillFileName( _L("cae_qqvga64K.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );
    }


void CCaeTestRelease::TestCaptureQqvga4KL()
    {
	// ID: CAE.SC.CAPTURESTILL.QQVGA4K.007
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture QQVGA 4K 
    //          image when initialization is done.
	//      
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor4K );

    TSize sizeQqvga( KImgWidthQQVGA, KImgHeightQQVGA );

    iStatesClient->SetStillFrameSize( sizeQqvga );

    iStatesClient->SetStillFileName( _L("cae_qqvga4K.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );
    }


void CCaeTestRelease::TestCaptureMp1_16ML()
    {
	// ID: CAE.SC.CAPTURESTILL.MP1.16M.011
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture 1MegaPixel 16M  
    //          image when initialization is done.
	//      
    //       ** For Mirage X PROD1**
    //
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizeMp1( KImgWidthMP1, KImgHeightMP1 );

    iStatesClient->SetStillFrameSize( sizeMp1 );

    iStatesClient->SetStillFileName( _L("cae_mp1_16M.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureMp1_64KL()
    {
	// ID: CAE.SC.CAPTURESTILL.MP1.64K.012
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture 1MegaPixel 64K  
    //          image when initialization is done.
	//      
    //       ** For Mirage X PROD1**
    //
	// Output : OK.   

    RDebug::Print(_L("XYZ:TestCaptureMp1_64KL start"));

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor64K );

    TSize sizeMp1( KImgWidthMP1, KImgHeightMP1 );

    iStatesClient->SetStillFrameSize( sizeMp1 );

    iStatesClient->SetStillFileName( _L("cae_mp1_64K.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    
    RDebug::Print(_L("XYZ:TestCaptureMp1_64KL ending"));
    }


void CCaeTestRelease::TestCaptureMp1_4KL()
    {
	// ID: CAE.SC.CAPTURESTILL.MP1.4K.013
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture 1Mega Pixel 4K 
    //          image when initialization is done.
	//      
    //       ** For Mirage X PROD1**
    //
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor4K );

    TSize sizeMp1( KImgWidthMP1, KImgHeightMP1 );

    iStatesClient->SetStillFrameSize( sizeMp1 );

    iStatesClient->SetStillFileName( _L("cae_mp1_4K.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestRelease::TestCaptureJpegMp1L()
    {
	// ID: CAE.SC.CAPTURESTILL.JPEGMP1.014
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture 1MegaPixel Jpeg 
    //          image when initialization is done.
	//      
    //       ** For Mirage X PROD1**
    //
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatJpeg );

    iStatesClient->SetStillCompressionQuality( 50 );

    TSize sizeMp1( KImgWidthMP1, KImgHeightMP1 );

    iStatesClient->SetStillFrameSize( sizeMp1 );

    iStatesClient->SetStillFileName( _L("cae_jpegmp1_50.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureMp1M3_16ML()
    {
	// ID: CAE.SC.CAPTURESTILL.MP1M3.16M.018
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture 1,3MegaPixel 16M  
    //          image when initialization is done.
	//      
    //       ** For Mirage X PROD2**
    //
    // Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizeMp1M3( KImgWidthMP1M3, KImgHeightMP1M3 );

    iStatesClient->SetStillFrameSize( sizeMp1M3 );

    iStatesClient->SetStillFileName( _L("cae_mp1m3_16M.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureMp1M3_64KL()
    {
	// ID: CAE.SC.CAPTURESTILL.MP1M3.64K.019
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture 1,3MegaPixel 64K  
    //          image when initialization is done.
	//      
    //       ** For Mirage X PROD2**
    //
	// Output : OK.   

    RDebug::Print(_L("XYZ:TestCaptureMp1_64KL start"));

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor64K );

    TSize sizeMp1M3( KImgWidthMP1M3, KImgHeightMP1M3 );

    iStatesClient->SetStillFrameSize( sizeMp1M3 );

    iStatesClient->SetStillFileName( _L("cae_mp1m3_64K.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    
    RDebug::Print(_L("XYZ:TestCaptureMp1_64KL ending"));
    }


void CCaeTestRelease::TestCaptureMp1M3_4KL()
    {
	// ID: CAE.SC.CAPTURESTILL.MP1M3.4K.020
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture 1,3Mega Pixel 4K 
    //          image when initialization is done.
	//      
    //       ** For Mirage X PROD2**
    //
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor4K );

    TSize sizeMp1M3( KImgWidthMP1M3, KImgHeightMP1M3 );

    iStatesClient->SetStillFrameSize( sizeMp1M3 );

    iStatesClient->SetStillFileName( _L("cae_mp1m3_4K.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestRelease::TestCaptureJpegMp1M3L()
    {
	// ID: CAE.SC.CAPTURESTILL.JPEGMP1M3.021
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture 1,3MegaPixel Jpeg 
    //          image when initialization is done.
	//      
    //       ** For Mirage X PROD2**
    //
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatJpeg );

    iStatesClient->SetStillCompressionQuality( 50 );

    TSize sizeMp1M3( KImgWidthMP1M3, KImgHeightMP1M3 );

    iStatesClient->SetStillFrameSize( sizeMp1M3 );

    iStatesClient->SetStillFileName( _L("cae_jpegmp1m3_50.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureMp2_16ML()
    {
	// ID: CAE.SC.CAPTURESTILL.MP2.16M.025
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture 2 MegaPixel 16M  
    //          image when initialization is done.
	//      
    //       ** For UNAGI 2M PROD7 **
    //
    // Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizeMp2( KImgWidthMP2, KImgHeightMP2 );

    iStatesClient->SetStillFrameSize( sizeMp2 );

    iStatesClient->SetStillFileName( _L("cae_mp2_16M.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureMp2_64KL()
    {
	// ID: CAE.SC.CAPTURESTILL.MP2.64K.026
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture 2 MegaPixel 64K  
    //          image when initialization is done.
	//      
    //       ** For UNAGI 2M PROD7 **
    //
	// Output : OK.   

    RDebug::Print(_L("XYZ:TestCaptureMp2_64KL start"));

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor64K );

    TSize sizeMp2( KImgWidthMP2, KImgHeightMP2 );

    iStatesClient->SetStillFrameSize( sizeMp2 );

    iStatesClient->SetStillFileName( _L("cae_mp2_64K.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    
    RDebug::Print(_L("XYZ:TestCaptureMp2_64KL ending"));
    }


void CCaeTestRelease::TestCaptureMp2_4KL()
    {
	// ID: CAE.SC.CAPTURESTILL.MP2.4K.027
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture 2 Mega Pixel 4K 
    //          image when initialization is done.
	//      
    //       ** For UNAGI 2M PROD7 **
    //
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor4K );

    TSize sizeMp2( KImgWidthMP2, KImgHeightMP2 );

    iStatesClient->SetStillFrameSize( sizeMp2 );

    iStatesClient->SetStillFileName( _L("cae_mp2_4K.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestRelease::TestCaptureJpegMp2L()
    {
	// ID: CAE.SC.CAPTURESTILL.JPEG.MP2.028
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture 2 MegaPixel Jpeg 
    //          image when initialization is done.
	//      
    //       ** For UNAGI 2M PROD7 **
    //
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatJpeg );

    iStatesClient->SetStillCompressionQuality( 50 );

    TSize sizeMp2( KImgWidthMP2, KImgHeightMP2 );

    iStatesClient->SetStillFrameSize( sizeMp2 );

    iStatesClient->SetStillFileName( _L("cae_jpegmp2_50.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestRelease::TestCaptureExifMp2L()
    {
	// ID: CAE.SC.CAPTURESTILL.EXIF.MP2.029
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture 2 MegaPixel Exif 
    //          image when initialization is done.
	//      
    //       ** For UNAGI 2M PROD7 **
    //
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetStillCompressionQuality( 95 );

    TSize sizeMp2( KImgWidthMP2, KImgHeightMP2 );

    iStatesClient->SetStillFrameSize( sizeMp2 );

    iStatesClient->SetStillFileName( _L("cae_exifmp2_95.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestRelease::TestCaptureExifSVGAL()
    {
	// ID: CAE.SC.CAPTURESTILL.EXIF.SVGA.030
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture SVGA (800x600) Exif 
    //          image when initialization is done.
	//      
    //       ** For UNAGI 2M PROD7 **
    //
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetStillCompressionQuality( 30 );

    TSize sizeSVGA( KImgWidthSVGA, KImgHeightSVGA );

    iStatesClient->SetStillFrameSize( sizeSVGA );

    iStatesClient->SetStillFileName( _L("cae_exif_svga_30.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestRelease::TestCaptureCodecL()
    {
	// ID: CAE.SC.CAPTURESTILL.CODEC.031
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture an image 
    //          when codec has been set to default.
	//      
    //
	// Output : OK.   

    iSettingsClient->SetStillFormat( CCamera::EFormatExif );

    iSettingsClient->SetStillFileName( _L("cae_exif_codec.jpg") );
   
    iSettingsClient->SetImageCodecs( KNullUid, KNullUid );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetJpegCodecWhenStillPrepared );

    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountStillPrepareComplete() == 1 );
    assertL( iSettingsClient->CountSnapImageReady() == 1 );
    assertL( iSettingsClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureCodec2L()
    {
	// ID: CAE.SC.CAPTURESTILL.CODEC.032
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture an image 
    //          when codec has been set to specific uid.
	//      
    //
	// Output : OK.   

    iSettingsClient->SetStillFormat( CCamera::EFormatExif );

    iSettingsClient->SetStillFileName( _L("cae_exif_codec.jpg") );
    
    #if ( SYMBIAN_CODECS )
    iSettingsClient->SetImageCodecs( TUid::Uid( KSymbianJpegDecoderValue ), TUid::Uid( KSymbianJpegEncoderValue ) );
    #else
    iSettingsClient->SetImageCodecs( TUid::Uid( KOptimizedJpegDecoderValue ), TUid::Uid( KOptimizedJpegEncoderValue ) );
    #endif
    
    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetJpegCodecWhenStillPrepared );

    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountStillPrepareComplete() == 1 );
    assertL( iSettingsClient->CountSnapImageReady() == 1 );
    assertL( iSettingsClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureCaptureL()
    {
	// ID:      CAE.SC.CAPTURECAPTURE.001
    //
    // Group:   Capture
    // 
    // Action : Capture photo two times successively
	//      
	// Output : No leave.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetStillFileName( _L("cae_capturecapture_ql0.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ECaptureCapture );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 2 );
    assertL( iStatesClient->CountStillImageReady() == 2 );
    }


void CCaeTestRelease::TestCaptureCaptureSpecPrepL()
    {
	// ID:      CAE.SC.CAPTURECAPTURE.002
    //
    // Group:   Capture
    // 
    // Action : Capture photo two times successively
	//      
	// Output : No leave.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizeQqvga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeQqvga );

    iStatesClient->SetStillFileName( _L("cae_capturecapture_vga16M.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecPrepCaptureCapture );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 2 );
    assertL( iStatesClient->CountStillImageReady() == 2 );
    }

void CCaeTestRelease::TestCaptureAfterPowerOffL()
    {
	// ID: CAE.SC.CAPTURESTILL.POWEROFF.001
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture Vga Exif image 
    //          image after poweroff.
	//      
	// Output : No leave

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );
    
    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    //iStatesClient->SetSnapFileName( _L("cae_snap_1_8_vga_poweroff.mbm") );

    iStatesClient->SetSnapImageSize( TSize( 44, 52 ) );

    iStatesClient->SetStillFileName( _L("cae_snap_1_8_vga_poweroff.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ECaptureStillAfterPowerOff );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == 0 );
    }


/********************************************************/
/*              PREPARE STILL CAPTURE                   */
/********************************************************/

void CCaeTestRelease::TestPrepareStillCapture0L()
    {
	// ID: CAE.SC.PREPARESTILLCAPTUREL.001
    //
    // Group:   Capture
    // 
    // Action : Prepare the engine for still image capture on specified quality level. 
    //          Initialization is done.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareStillCapturing );
    }

void CCaeTestRelease::TestPrepareStillCapture1L()
    {
	// ID: CAE.SC.PREPARESTILLCAPTUREL.002
    //
    // Group:   Capture
    // 
    // Action : Prepare the engine for still image capture on specified quality level. 
    //          Initialization is done.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 1 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareStillCapturing );
    }

void CCaeTestRelease::TestPrepareStillCapture2L()
    {
	// ID: CAE.SC.PREPARESTILLCAPTUREL.003
    //
    // Group:   Capture
    // 
    // Action : Prepare the engine for still image capture on specified quality level. 
    //          Initialization is done.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareStillCapturing );
    }

void CCaeTestRelease::TestPrepareStillCapture0WhenPreparedL()
    {
	// ID: CAE.SC.PREPARESTILLCAPTUREL.004
    //
    // Group:   Capture
    // 
    // Action : Prepare the engine for still image capture on specified quality 
    //          level when already prepared.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareStillCapturingWhenPrepared );//we have to put correct action
    }

void CCaeTestRelease::TestSpecPrepareSCSupportedFormatWhenInitializedL()
    {
	// ID: CAE.SC.PREPARESTILLCAPTUREL.007
    //
    // Group:   Capture
    // 
    // Action : Set still image capturing format to a format supported by Camera API implementation. 
	//          Initialization is done.
    //
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareStill );//verified if is correct action
    }

/********************************************************/
/*           STILL CAPTURE ENGINE INITIALIZED           */
/********************************************************/

void CCaeTestRelease::TestCaptureStillWhenEngineInitializedL()
    {
	// ID: CAE.SC.CAPTURESTILL.001
    //
    // Group:   Capture
    // 
    // Action : Capture still image. Engine initialization is done.
	//      
	// Output : Leave.   

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::ECaptureStillWhenEngineInitialized ) );// verified if is correct action
    if( error == KErrNoMemory )
        {
        User::Leave( error ); // For memory test
        }
    assertL( error != KErrNone );
    }


/********************************************************/
/*                    STILL BURST CAPTURE               */
/********************************************************/

void CCaeTestRelease::TestCaptureStillBurst3L()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.3.001
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          images in a burst when initialization is done.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetSnapFileName( _L("cae_snap_vga16M_burst3.mbm") );

    iStatesClient->SetStillFileName( _L("cae_vga16M_burst3.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurst );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountSnapImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }

void CCaeTestRelease::TestCaptureStillBurst0L()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.0.002
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture  
    //          images in a burst when BurstLength has to small value, 
    //          when initialization is done.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 0;

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurst ) );
    if( error == KErrNoMemory )
        {
        User::Leave( error ); // For memory test
        }
    assertL( error == KErrArgument );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );    
    assertL( iStatesClient->CountSnapImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 0 );
    }

void CCaeTestRelease::TestCaptureStillBurst100L()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.100.003
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture  
    //          images in a burst when BurstLength has to big value, 
    //          when initialization is done.
	//      
	// Output : Leave.   

    const TInt KStillBurstLength = 100;

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

	TInt error = KErrNone;
	TRAP( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurst ) );
    assertL( error == KErrNoMemory );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );

    // assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }

void CCaeTestRelease::TestCancelCaptureStillBurst3L()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.CANCEL.004
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          images in a burst and cancel still burst, initialization is done.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetSnapFileName( _L("cae_snap_vga16M_burst_cancel.mbm") );

    iStatesClient->SetStillFileName( _L("cae_vga16M_burst_cancel.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureAndCancelStillBurst );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == 1 ); // 3.1 and later
    //assertL( iStatesClient->CountStillBurstCaptureMoment() == 0 ); // 3.0 and earlier 
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == 0 );
    assertL( iStatesClient->CountStillBurstComplete() == 0 );
    }

void CCaeTestRelease::TestStopCaptureStillBurstAfter2L()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.STOP2.005
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          images in a burst and stop still burst, when initialization is done.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 5;
    const TInt KStillBurstStoppedLength = 2;

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetSnapFileName( _L("cae_snap_vga16M_burst_stop.mbm") );

    iStatesClient->SetStillFileName( _L("cae_vga16M_burst_stop.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureAndStopStillBurst );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstStoppedLength + 1 );
    assertL( iStatesClient->CountSnapImageReady() == KStillBurstStoppedLength );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstStoppedLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }

void CCaeTestRelease::TestSpecPrepCaptureStillBurst3L()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.3.006
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          images in a burst when initialization is done.
    //          Preparation is done using explicitly specified image format parameters.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizevga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizevga );

    iStatesClient->SetSnapFileName( _L("cae_snap_vga16M_spburst3.mbm") );

    iStatesClient->SetStillFileName( _L("cae_vga16M_spburst3.mbm") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecPrepAndCaptureStillBurst );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountSnapImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }

void CCaeTestRelease::TestCaptureStillBurstInterval0L()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTINTERVAL.0.001
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode, 
    //          SetStillBurstCaptureIntervalL, StillBurstCaptureInterval 
    //          and capture three images in a burst when initialization is done.
	//      
	// Output : Leave.   

    const TInt KStillBurstLength = 3;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetSnapFileName( _L("cae_snap_vga16M_burst_intvl_0.mbm") );

    iSettingsClient->SetStillFileName( _L("cae_vga16M_burst_intvl_0.jpg") );

    iSettingsClient->SetStillBurstLength( KStillBurstLength );
    
    TTimeIntervalMicroSeconds burstInterval( 0 );
    
    iSettingsClient->SetStillBurstCaptureInterval( burstInterval );

    iSettingsClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndSetStillBurstCaptureInterval );

    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountStillPrepareComplete() == 1 );
    assertL( iSettingsClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iSettingsClient->CountSnapImageReady() == KStillBurstLength );
    assertL( iSettingsClient->CountStillImageReady() == KStillBurstLength );
    assertL( iSettingsClient->CountStillBurstComplete() == 1 );
    }

void CCaeTestRelease::TestCaptureStillBurstInterval1000000L()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTINTERVAL.1000000.002
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode,
    //          SetStillBurstCaptureIntervalL, StillBurstCaptureInterval  
    //          and capture three images in a burst, when initialization is done.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetSnapFileName( _L("cae_snap_vga16M_burst_intvl_1000000.mbm") );

    iSettingsClient->SetStillFileName( _L("cae_vga16M_burst_intvl_1000000.jpg") );

    iSettingsClient->SetStillBurstLength( KStillBurstLength );
   
    TTimeIntervalMicroSeconds burstInterval( 1000000 );
    
    iSettingsClient->SetStillBurstCaptureInterval( burstInterval );

    iSettingsClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndSetStillBurstCaptureInterval );

    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountStillPrepareComplete() == 1 );
    assertL( iSettingsClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iSettingsClient->CountSnapImageReady() == KStillBurstLength );
    assertL( iSettingsClient->CountStillImageReady() == KStillBurstLength );
    assertL( iSettingsClient->CountStillBurstComplete() == 1 );
    }

void CCaeTestRelease::TestCaptureStillBurstIntervalM1L()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTINTERVAL.M1.003
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode,
    //          SetStillBurstCaptureIntervalL, StillBurstCaptureInterval  
    //          and capture three images in a burst, when initialization is done.
	//      
	// Output : Leave.   

    const TInt KStillBurstLength = 3;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetStillBurstLength( KStillBurstLength );
   
    TTimeIntervalMicroSeconds burstInterval( -1 );
    
    iSettingsClient->SetStillBurstCaptureInterval( burstInterval );

    TRAPD( error, iSettingsClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndSetStillBurstCaptureInterval ) );
    if( error == KErrNoMemory )
        {
        User::Leave( error ); // For memory test
        }
    assertL( error == KErrArgument );

    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountStillPrepareComplete() == 1 );
    assertL( iSettingsClient->CountStillBurstCaptureMoment() == 0 );
    assertL( iSettingsClient->CountSnapImageReady() == 0 );
    assertL( iSettingsClient->CountStillImageReady() == 0 );
    assertL( iSettingsClient->CountStillBurstComplete() == 0 );
    }

void CCaeTestRelease::TestCaptureStillBurstExif3L()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTEXIF.3.001
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          VGA Exif images in a burst when initialization is done.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

     TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );
    
    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_snap_burst3_exif.mbm") );

    iStatesClient->SetStillFileName( _L("cae_burst3_exif.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurstExif );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstLength );

    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }


void CCaeTestRelease::TestCaptureStillBurstExif6L()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTEXIF.6.002
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture six  
    //          Exif images in a burst when initialization is done. Uses default
    //          image resolution.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 6;

    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_snap_burst6_exif.mbm") );

    iStatesClient->SetStillFileName( _L("cae_burst6_exif.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurstExif );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountViewFinderFrameReady() == 0 );

    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }

void CCaeTestRelease::TestCaptureStillBurstExif6NSL()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTEXIF.6.006
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture six  
    //          Exif images in a burst when initialization is done. Uses default
    //          image resolution. Test will not stop on errors in call-backs
    //          and this will test the completeStillBurst() call if run out of memory.
	//          in cppUnit memory tests.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 6;

    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_snap_burst6_exif.mbm") );

    iStatesClient->SetStillFileName( _L("cae_burst6_exif.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->SetStopOnErrorInMcaeoSnapImageReady( EFalse );

	iStatesClient->SetStopOnErrorInMcaeoStillImageReady( EFalse );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurstExif );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountViewFinderFrameReady() == 0 );

    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }


void CCaeTestRelease::TestCaptureStillBurstExif6_ThumbnailL()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTEXIF.6.007
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture six  
    //          Exif images in a burst when initialization is done. Uses default
    //          image resolution. Exif image thumbnails are decoded to snap images.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 6;

    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_snap_burst6_7_exif.mbm") );

    iStatesClient->SetStillFileName( _L("cae_burst6_7_exif.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->SetSnapImageSource( CCaeEngine::ESnapImageSourceThumbnail );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurstExif );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountViewFinderFrameReady() == 0 );

    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }


void CCaeTestRelease::TestCaptureStillBurstExif100L()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTEXIF.100.003
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture  
    //          exif images in a burst when BurstLength has to big value, 
    //          when initialization is done.
	//      
	// Output : Leave.   

    const TInt KStillBurstLength = 100;

    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

	TInt error = KErrNone;
	TRAP( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurstExif ) );
    assertL( error == KErrNoMemory );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );

    assertL( iStatesClient->CountStillBurstComplete() == 0 );
    }


void CCaeTestRelease::TestCaptureStillBurstExif6VfL()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTEXIF.6.VF.004
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture six  
    //          Exif images in a burst when initialization is done. Uses default
    //          image resolution. The view finder is running during the burst.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 6;

    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_snap_burst6_exif.mbm") );

    iStatesClient->SetStillFileName( _L("cae_burst6_exif.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

	iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurstExif );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstLength );

    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }


void CCaeTestRelease::TestCaptureStillBurstExif6VfCancelL()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTEXIF.6.VF.CANCEL.005
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and cancel the burst after   
    //          the first capture. Intention is to test the situation when the burst is pending
	//			the view finder to occur before it continues and cancel the burst at that 
	//			point. Uses default image resolution. The view finder is running 
	//          during the burst.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 6;

    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_snap_burst6_exif.mbm") );

    iStatesClient->SetStillFileName( _L("cae_burst6_exif.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

	iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureAndCancelStillBurstExif );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == 0 );

    assertL( iStatesClient->CountStillBurstCaptureMoment() == 2 );
    assertL( iStatesClient->CountStillBurstComplete() == 0 );
    }


/********************************************************/
/*              STILL CAPTURE EXIF                      */
/********************************************************/

void CCaeTestRelease::TestCaptureVgaExifL()
    {
	// ID: CAE.SC.CAPTURESTILL.VGA.EXIF.001
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture Vga Exif 
    //          image when initialization is done and viewfinder is running.
	//      
	// Output : OK.   

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );
    
    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_snap_vga_exif.mbm") );

    iStatesClient->SetStillFileName( _L("cae_vga_exif.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareCaptureStillStartAndStopVfExif );
    
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureQqvgaExifL()
    {
	// ID: CAE.SC.CAPTURESTILL.QQVGA.EXIF.002

    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture Qqvga Exif 
    //          image when initialization is done and viewfinder is running.
	//      
	// Output : OK.   

    TSize sizeQqvga( KImgWidthQQVGA, KImgHeightQQVGA );

    iStatesClient->SetStillFrameSize( sizeQqvga );

    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_qqvga_exif.mbm") );

    iStatesClient->SetStillFileName( _L("cae_qqvga_exif.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareCaptureStillStartAndStopVfExif );
    }


void CCaeTestRelease::TestCaptureVgaSnapOneHalfExifL()
    {
	// ID: CAE.SC.CAPTURESTILL.VGA.EXIF.003
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture Vga Exif image 
    //          image when initialization is done and viewfinder is running.
	//			The snap image is downscaled to 1/2 on Exif decoding.
	//      
	// Output : OK.   

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );
    
    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_snap_1_2_vga_exif.mbm") );

    iStatesClient->SetSnapImageSize( TSize( 176, 208 ) );

    iStatesClient->SetStillFileName( _L("cae_snap_1_2_vga_exif.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareCaptureStillStartAndStopVfSnapExif );
    
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureVgaSnapOneFourthExifL()
    {
	// ID: CAE.SC.CAPTURESTILL.VGA.EXIF.004
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture Vga Exif image 
    //          image when initialization is done and viewfinder is running.
	//			The snap image is downscaled 1/4 on Exif decoding.
	//      
	// Output : OK.   

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );
    
    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_snap_1_4_vga_exif.mbm") );

    iStatesClient->SetSnapImageSize( TSize( 88, 104 ) );

    iStatesClient->SetStillFileName( _L("cae_snap_1_4_vga_exif.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareCaptureStillStartAndStopVfSnapExif );
    }


void CCaeTestRelease::TestCaptureVgaSnapOneEighthExifL()
    {
	// ID: CAE.SC.CAPTURESTILL.VGA.EXIF.005
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture Vga Exif image 
    //          image when initialization is done and viewfinder is running.
	//			The snap image is downscaled 1/8 on Exif decoding.
	//      
	// Output : OK.   

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );
    
    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_snap_1_8_vga_exif.mbm") );

    iStatesClient->SetSnapImageSize( TSize( 44, 52 ) );

    iStatesClient->SetStillFileName( _L("cae_snap_1_8_vga_exif.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareCaptureStillStartAndStopVfSnapExif );
    }


void CCaeTestRelease::TestCaptureVgaSnapExifL()
    {
	// ID: CAE.SC.CAPTURESTILL.VGA.EXIF.006
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture Vga Exif image 
    //          image when initialization is done and viewfinder is running.
	//			The snap image is given but not downscaled on Exif decoding.
	//      
	// Output : OK.   

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );
    
    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_snap_vga_exif.mbm") );

    iStatesClient->SetSnapImageSize( TSize( 176, 250 ) );

    iStatesClient->SetStillFileName( _L("cae_snap_vga_exif.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareCaptureStillStartAndStopVfSnapExif );
    }


void CCaeTestRelease::TestCaptureVgaSnapOneEighthJpegL()
    {
	// ID: CAE.SC.CAPTURESTILL.VGA.JPEG.007
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture Vga Jpeg 
    //          image when initialization is done and viewfinder is running.
	//			The snap image is downscaled 1/8 on Jpeg decoding.
    //          This is same as CAE.SC.CAPTURESTILL.VGA.EXIF.005 but format is Jpeg.
	//      
	// Output : OK.   

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );
    
    iStatesClient->SetStillFormat( CCamera::EFormatJpeg ); // test Jpeg format too

    iStatesClient->SetSnapFileName( _L("cae_snap_1_8_vga_jpeg.mbm") );

    iStatesClient->SetSnapImageSize( TSize( 44, 52 ) );

    iStatesClient->SetStillFileName( _L("cae_snap_1_8_vga_jpeg.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareCaptureStillStartAndStopVfSnapExif );
    }

void CCaeTestRelease::TestCaptureVgaSnapOneEighthExif2L()
    {
	// ID: CAE.SC.CAPTURESTILL.VGA.EXIF.008
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture Vga Exif image 
    //          image when initialization is done and viewfinder is running.
	//			The snap image is downscaled 1/8 on Exif decoding. The snap 
	//			image size is set only after prepare.
	//      
	// Output : OK. Visual check of snap file size.

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );
    
    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_snap_1_8_vga_exif2.mbm") );

    iStatesClient->SetSnapImageSize( TSize( 44, 52 ) );

    iStatesClient->SetStillFileName( _L("cae_snap_1_8_vga_exif2.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ECaptureStillSetSnapSizeExif );
    }


void CCaeTestRelease::TestCaptureVgaSnapColor16MExifL()
    {
	// ID: CAE.SC.CAPTURESTILL.VGA.EXIF.009
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture Vga Exif image 
    //          when initialization is done and viewfinder is running.
	//			The snap image color mode is set to EColor16M.
	//      
	// Output : OK. Visual check of snap file color mode.

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );
    
    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    iStatesClient->SetSnapFileName( _L("cae_snap_color_mode_vga_exif.mbm") );

    iStatesClient->SetSnapImageColorMode( EColor16M );

    iStatesClient->SetStillFileName( _L("cae_snap_color_mode_vga_exif.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ECaptureStillSetSnapColorModeExif );
    }


/********************************************************/
/*              PREPARE VIDEO RECORDING                 */
/********************************************************/

void CCaeTestRelease::TestPrepareVideoRecording0L()
    {
	// ID: CAE.VR.PREPAREVIDEORECORDINGL.001
    //
    // Group:   Record
    // 
    // Action : Prepare the engine for video recording on specified quality level. 
    //          Initialization is done.
	//      
	// Output : OK.   

    iStatesClient->SetVideoQualityLevelIndex( 0 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iStatesClient->SetVideoFileName( _L("cae_prepare_vr_0.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareVideoRecording );
        
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    }

void CCaeTestRelease::TestPrepareVideoRecording1L()
    {
	// ID: CAE.VR.PREPAREVIDEORECORDINGL.002
    //
    // Group:   Record
    // 
    // Action : Prepare the engine for video recording on specified quality level. 
    //          Initialization is done.
	//      
	// Output : OK.   

    iStatesClient->SetVideoQualityLevelIndex( 1 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iStatesClient->SetVideoFileName( _L("cae_prepare_vr_1.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareVideoRecording );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    }

void CCaeTestRelease::TestPrepareVideoRecording2L()
    {
	// ID: CAE.VR.PREPAREVIDEORECORDINGL.003
    //
    // Group:   Record
    // 
    // Action : Prepare the engine for video recording on specified quality level. 
    //          Initialization is done.
	//      
	// Output : OK.   

    iStatesClient->SetVideoQualityLevelIndex( 2 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iStatesClient->SetVideoFileName( _L("cae_prepare_vr_2.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareVideoRecording );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    }

void CCaeTestRelease::TestPrepareVideoRecording0WhenPreparedL()
    {
	// ID: CAE.VR.PREPAREVIDEORECORDINGL.004
    //
    // Group:   Record
    // 
    // Action : Prepare the engine for video recording on specified quality 
    //          level when already prepared.
	//      
	// Output : OK.   

    iStatesClient->SetVideoQualityLevelIndex( 0 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iStatesClient->SetVideoFileName( _L("cae_prepared_vr_0.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareVideoRecordingWhenPrepared );//we have to put correct action

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 2 );
    }

void CCaeTestRelease::TestSpecPrepareVRSupportedFormatWhenVRInitializedL()
{
    // ID: CAE.VR.PREPAREVIDEORECORDINGL.007
    //
    // Group:   Record.
    // 
    // Action : Set video recording format to a format supported by MMF Camcorder plug-in 
    //          and Camera API implementation. 
    //          Video recorder initialization is done.
	//      
	// Output : OK.    

    iStatesClient->SetTimeout( 1000000 );

    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );
    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( 10.0 );
    iStatesClient->SetVideoBitRate( 50000 );
    iStatesClient->SetVideoAudio( ETrue );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    //iStatesClient->SetVideoFileName( _L("cae_video_format_initialized.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareVideo );//verified if is correct action

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    }

/********************************************************/
/*             VIDEO RECORDING QUALITY LEVEL            */
/********************************************************/

void CCaeTestRelease::TestRecordQualityLevel0L()
    {
	// ID:      CAE.VR.VIDEORECORDING.QCIF64000FR15.001
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 0 
    //          video clip when initialization is done and viewfinder is running.
	//      
	// Output : No leave. File cae_ql_0.3gp.   

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetViewFinderSize( sizeSubQcif );
    
    iStatesClient->SetVfFileName( _L("cae_video_ql_0_vfimg.mbm") ); 

    // iStatesClient->SetVfFileCountToSave( 150 );
        
    iStatesClient->SetTimeout( 30000000 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iStatesClient->SetVideoFileName( _L("cae_ql_0.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareRecordVideoStartAndStopVf );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


void CCaeTestRelease::TestRecordQualityLevel1L()
    {
	// ID:      CAE.VR.VIDEORECORDING.SUBQCIF60000FR15.002
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 1 
    //          video clip when initialization is done and viewfinder is running.
	//      
	// Output : No leave. File cae_ql_1.3gp.   

    iStatesClient->SetVideoQualityLevelIndex( 1 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetViewFinderSize( sizeSubQcif );
    
    iStatesClient->SetTimeout( 20000000 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iStatesClient->SetVideoFileName( _L("cae_ql_1.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareRecordVideoStartAndStopVf );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


void CCaeTestRelease::TestPrepareAudioBitRateVR128L()
    {
	// ID:      CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.002
    //
    // Group:   Record.
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          Use high video bitrate. This is for PROD7.
	//      
	// Output : No leave. File cae_subqcif_VideoBitRate_128000.3gp.   

    TSize sizeSubQcif( 176, 144 );

    iStatesClient->SetViewFinderSize( sizeSubQcif );
    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 15.0 ) );

    iStatesClient->SetVideoBitRate( TInt( 128000 ) );
    iStatesClient->SetAudioBitRate( TInt( 12200 ) );
    
    iStatesClient->SetTimeout( 20000000 );

    // 3: AMR audio, H.263 video, video type video/H263-2000; profile=0; level=45
    // 0: AMR audio, H.263 video, video type video/H263-2000
    iStatesClient->SetVideoCodecSet( 3 ); 

    iStatesClient->SetVideoFileName( _L("cae_subqcif_VideoBitRate_128000.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestRelease::TestPrepareAudioBitRateVR128InvVtypeL()
    {
	// ID:      CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.003
    //
    // Group:   Record.
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          Use high video bitrate and invalid videotype.
	//      
	// Output : Leave   

    TSize sizeSubQcif( 176, 144 );

    iStatesClient->SetViewFinderSize( sizeSubQcif );
    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 15.0 ) );

    iStatesClient->SetVideoBitRate( TInt( 128000 ) );
    iStatesClient->SetAudioBitRate( TInt( 12200 ) );
    
    iStatesClient->SetTimeout( 20000000 );

    // 3: AMR audio, H.263 video, video type video/H263-2000; profile=0; level=45
    // 0: AMR audio, H.263 video, video type video/H263-2000
    iStatesClient->SetVideoCodecSet( 0 ); 

    iStatesClient->SetVideoFileName( _L("cae_subqcif_VideoBitRate_128000inv.3gp") );

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }
    assertL( err != KErrNone );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestRelease::TestRecordQualityLevel2L()
    {
	// ID:      CAE.VR.VIDEORECORDING.SUBQCIF40000FR5.003
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 2 
    //          video clip when initialization is done and viewfinder is running.
	//      
	// Output : No leave. File cae_q2_0.3gp.   

    iStatesClient->SetVideoQualityLevelIndex( 2 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetViewFinderSize( sizeSubQcif );
    
    iStatesClient->SetTimeout( 20000000 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iStatesClient->SetVideoFileName( _L("cae_ql_2.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareRecordVideoStartAndStopVf );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*            START AND STOP VIDEO RECORDING            */
/********************************************************/

void CCaeTestRelease::TestStartAndStopVideoRecordingSubQcifL()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.SUBQCIF.001
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording subQCIF sized video works. 
    //          Video recording prepared for subQCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iStatesClient->SetVideoFileName( _L("cae_subqcif.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestRelease::TestStartAndStopVideoRecordingQcifL()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.QCIF.002
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording QCIF sized video works. 
    //          Video recording prepared for QCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 50000 ) );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iStatesClient->SetVideoFileName( _L("cae_qcif.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*             STILL CAPTURE DIGITAL ZOOM VALUE         */
/********************************************************/

void CCaeTestRelease::TestSCDigiZoomVal0L()
{
	// Group:   Settings
    //
    // ID:      CAE.SET-SC.SETZOOMVALUEL.001
    // 
    // Action : Test digital zoom.
	//      
	// Output : OK.  
 
    RDebug::Print(_L("XYZ:TestSCDigiZoomVal0L start"));

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetDigiZoomValue( 0 );

    iSettingsClient->SetSnapFileName( _L("cae_digizoom_val_0_vga.mbm") );

    iSettingsClient->SetStillFileName( _L("cae_digizoom_val_0_vga.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenStillPrepared );
 
    RDebug::Print(_L("XYZ:TestSCDigiZoomVal0L ending"));
}

void CCaeTestRelease::TestSCDigiZoomVal1VgaL()
{
	// Group:   Settings
    //
    // ID:      CAE.SET-SC.SETZOOMVALUEL.002_vga
    // 
    // Action : Test digital zoom.
	//      
	// Output : OK.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetDigiZoomValue( info.iMaxDigitalZoom );

    iSettingsClient->SetSnapFileName( _L("cae_digizoom_val_1_vga.mbm") );

    iSettingsClient->SetStillFileName( _L("cae_digizoom_val_1_vga.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenStillPrepared );
}

void CCaeTestRelease::TestSCDigiZoomVal1QqvgaL()
{
	// Group:   Settings
    //
    // ID:      CAE.SET-SC.SETZOOMVALUEL.002_qqvga
    // 
    // Action : Test digital zoom.
	//      
	// Output : OK.   

    iSettingsClient->SetStillQualityLevelIndex( 2 );

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetDigiZoomValue( info.iMaxDigitalZoom );

    iSettingsClient->SetSnapFileName( _L("cae_digizoom_val_1_qqvga.mbm") );
    
    iSettingsClient->SetStillFileName( _L("cae_digizoom_val_1_qqvga.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenStillPrepared );
}

void CCaeTestRelease::TestSCDigiZoomInitializedVal0L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETZOOMVALUEL.005
    //
    // Action : Test digital zoom.
	//      
	// Output : OK.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetDigiZoomValue( 0 );

    iSettingsClient->SetSnapFileName( _L("cae_digizoom_initialized_val_0_vga.mbm") );

    iSettingsClient->SetStillFileName( _L("cae_digizoom_initialized_val_0_vga.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenInitialized );
}

void CCaeTestRelease::TestSCDigiZoomInitializedVal1VgaL()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETZOOMVALUEL.012_vga
    //
    // Action : Test digital zoom.
	//      
	// Output : OK.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetDigiZoomValue( info.iMaxDigitalZoom );

    iSettingsClient->SetSnapFileName( _L("cae_digizoom_initialized_val_1_vga.mbm") );

    iSettingsClient->SetStillFileName( _L("cae_digizoom_initialized_val_1_vga.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenInitialized );
}

void CCaeTestRelease::TestSCDigiZoomInitializedVal1QqvgaL()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETZOOMVALUEL.012_qqvga
    //
    // Action : Test digital zoom.
	//      
	// Output : OK.   

    iSettingsClient->SetStillQualityLevelIndex( 2 );

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetDigiZoomValue( info.iMaxDigitalZoom );

    iSettingsClient->SetSnapFileName( _L("cae_digizoom_initialized_val_1_qqvga.mbm") );

    iSettingsClient->SetStillFileName( _L("cae_digizoom_initialized_val_1_qqvga.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenInitialized );
}

/********************************************************/
/*             VIDEO RECORDING DIGITAL ZOOM VALUE       */
/********************************************************/

void CCaeTestRelease::TestVRDigiZoomVal0L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETZOOMVALUEL.006
    //
    // Action : Test digital zoom.
	//      
	// Output : OK.

    iSettingsClient->SetTimeout( 4000000 );
    
    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetDigiZoomValue( 0 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_digizoom_val_0.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenVideoPrepared );
}

void CCaeTestRelease::TestVRDigiZoomVal1L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETZOOMVALUEL.007
    //
    // Action : Test digital zoom.
	//      
	// Output : OK.   

    iSettingsClient->SetTimeout( 4000000 );
    
    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetDigiZoomValue( info.iMaxDigitalZoom );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_digizoom_val_1.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenVideoPrepared );
}

void CCaeTestRelease::TestVRDigiZoomRecordingVal0L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETZOOMVALUEL.010
    //
    // Action : Test digital zoom.
	//      
	// Output : OK.   

    iSettingsClient->SetTimeout( 4000000 );
    
    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetDigiZoomValue( 0 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_digizoom_recording_val_0.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenVideoRecording );
}

void CCaeTestRelease::TestVRDigiZoomRecordingVal1L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETZOOMVALUEL.011
    //
    // Action : Test digital zoom.
	//      
	// Output : OK.   

    iSettingsClient->SetTimeout( 4000000 );
    
    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );
    iSettingsClient->SetDigiZoomValue( info.iMaxDigitalZoom );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_digizoom_recording_val_1.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenVideoRecording );
}

/********************************************************/
/*        START AND STOP VIDEO RECORDING DISK FULL      */
/********************************************************/

void CCaeTestRelease::TestStartAndStopVRWhenTooBigSizeSubQcifL()
    {
	// ID: CAE.VR.STARTSTOPVRTOOBIGSIZE.SUBQCIF.001
    //
    // Group:   Record
    // 
    // Action : 
	//      
	// Output : Leave.   

    iStatesClient->SetTimeout( 100000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    //iStatesClient->SetVideoFileName( _L("cae_too_big_subqcif.3gp") );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo ) );//we have to put correct action
    if( error == KErrNoMemory )
        {
        User::Leave( error ); // For memory test
        }
    assertL( error == KErrDiskFull );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*                     GET INFO                         */
/********************************************************/

void CCaeTestRelease::TestGetInfoL()
    {
    // Group:   Settings
    //
    // ID:      CAE.GEN.GETINFO.001
    // 
    // Action : Call GetInfo. Engine initialized
    //      
    // Output : OK.   
    
    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetInfo );
    
    // Get Camera Application Engine info object.
    TCamAppEngineInfo info;
    iSettingsClient->GetInfo( info );
    
    // Do comparisons.

    /*
    TVersion hardwareVersion = TVersion(0,0,0);
    assertL( ( info.iHardwareVersion.iMajor == hardwareVersion.iMajor ) &&
             ( info.iHardwareVersion.iMinor == hardwareVersion.iMinor ) &&
             ( info.iHardwareVersion.iBuild == hardwareVersion.iBuild ) );

    TVersion softwareVersion = TVersion(0,0,0);
    assertL( ( info.iSoftwareVersion.iMajor == softwareVersion.iMajor ) &&
             ( info.iSoftwareVersion.iMinor == softwareVersion.iMinor ) &&
             ( info.iSoftwareVersion.iBuild == softwareVersion.iBuild ) );
    */

    TCameraInfo::TCameraOrientation orientation = TCameraInfo::EOrientationOutwards;
    assertL( info.iOrientation == orientation );

    TUint32 optionsSupported = TCameraInfo::EViewFinderBitmapsSupported |
                               TCameraInfo::EImageCaptureSupported |
                               TCameraInfo::EVideoCaptureSupported |
                               TCameraInfo::EContrastSupported |
                               TCameraInfo::EBrightnessSupported;
    assertL( info.iOptionsSupported == optionsSupported );
    
    TUint32 flashModesSupported = CCamera::EFlashNone;
    assertL( info.iFlashModesSupported == flashModesSupported );

    TUint32 exposureModesSupported = CCamera::EExposureAuto | 
                                     CCamera::EExposureNight;
    assertL( info.iExposureModesSupported == exposureModesSupported );
    
    TUint32 whiteBalanceModesSupported = CCamera::EWBAuto;
    assertL( info.iWhiteBalanceModesSupported == whiteBalanceModesSupported );

    TInt minzoom = 0;
    assertL( info.iMinZoom == minzoom );

    TInt maxZoom = 0;
    assertL( info.iMaxZoom == maxZoom );

    TInt maxDigitalZoom = 1;
    assertL( info.iMaxDigitalZoom == maxDigitalZoom );
    
    TReal32 minZoomFactor = 0.0;
    assertL( info.iMinZoomFactor == minZoomFactor );

    TReal32 maxZoomFactor = 0.0;
    assertL( info.iMaxZoomFactor == maxZoomFactor );

    TReal32 maxDigitalZoomFactor = 2.0;
    assertL( info.iMaxDigitalZoomFactor == maxDigitalZoomFactor );

    TInt numImageSizesSupported = 2;
    assertL( info.iNumImageSizesSupported == numImageSizesSupported );

    TUint32 imageFormatsSupported = CCamera::EFormatFbsBitmapColor4K |
                                    CCamera::EFormatFbsBitmapColor64K |
                                    CCamera::EFormatFbsBitmapColor16M;
    assertL( info.iImageFormatsSupported == imageFormatsSupported );

    TInt numStillQualityLevelsSupported = 3;
    assertL( info.iNumStillQualityLevelsSupported == numStillQualityLevelsSupported );

    TInt numVideoQualityLevelsSupported = 3;
    assertL( info.iNumVideoQualityLevelsSupported == numVideoQualityLevelsSupported );

    }

void CCaeTestRelease::TestGetInfoPROD1MirageXL()
    {
    // Group:   Settings
    //
    // ID:      CAE.GEN.GETINFO.002
    // 
    // Action : Call GetInfo. Engine initialized
    //          ** For PROD1 Mirage X**
    //      
    // Output : OK.   
    
    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetInfo );
    
    // Get Camera Application Engine info object.
    TCamAppEngineInfo info;
    iSettingsClient->GetInfo( info );
    
    // Do comparisons.

    /*
    TVersion hardwareVersion = TVersion(0,0,0);
    assertL( ( info.iHardwareVersion.iMajor == hardwareVersion.iMajor ) &&
             ( info.iHardwareVersion.iMinor == hardwareVersion.iMinor ) &&
             ( info.iHardwareVersion.iBuild == hardwareVersion.iBuild ) );

    TVersion softwareVersion = TVersion(0,0,0);
    assertL( ( info.iSoftwareVersion.iMajor == softwareVersion.iMajor ) &&
             ( info.iSoftwareVersion.iMinor == softwareVersion.iMinor ) &&
             ( info.iSoftwareVersion.iBuild == softwareVersion.iBuild ) );
    */

    TCameraInfo::TCameraOrientation orientation = TCameraInfo::EOrientationOutwards;
    assertL( info.iOrientation == orientation );

    TUint32 optionsSupported = TCameraInfo::EViewFinderBitmapsSupported |
                               TCameraInfo::EImageCaptureSupported |
                               TCameraInfo::EVideoCaptureSupported |
                               TCameraInfo::EContrastSupported |
                               TCameraInfo::EBrightnessSupported;
    assertL( info.iOptionsSupported == optionsSupported );
    
    TUint32 flashModesSupported = CCamera::EFlashNone;
    assertL( info.iFlashModesSupported == flashModesSupported );

    TUint32 exposureModesSupported = CCamera::EExposureAuto | 
                                     CCamera::EExposureNight;
    assertL( info.iExposureModesSupported == exposureModesSupported );
    
    TUint32 whiteBalanceModesSupported = CCamera::EWBAuto;
    assertL( info.iWhiteBalanceModesSupported == whiteBalanceModesSupported );

    TInt minzoom = 0;
    assertL( info.iMinZoom == minzoom );

    TInt maxZoom = 0;
    assertL( info.iMaxZoom == maxZoom );

    TInt maxDigitalZoom = 3;
    assertL( info.iMaxDigitalZoom == maxDigitalZoom );
    
    TReal32 minZoomFactor = 0.0;
    assertL( info.iMinZoomFactor == minZoomFactor );

    TReal32 maxZoomFactor = 0.0;
    assertL( info.iMaxZoomFactor == maxZoomFactor );

    TReal32 maxDigitalZoomFactor = 4.0;
    assertL( info.iMaxDigitalZoomFactor == maxDigitalZoomFactor );

    TInt numImageSizesSupported = 4;
    assertL( info.iNumImageSizesSupported == numImageSizesSupported );

    TUint32 imageFormatsSupported = CCamera::EFormatFbsBitmapColor4K |
                                    CCamera::EFormatFbsBitmapColor64K |
                                    CCamera::EFormatFbsBitmapColor16M;
    assertL( info.iImageFormatsSupported == imageFormatsSupported );

    TInt numStillQualityLevelsSupported = 3;
    assertL( info.iNumStillQualityLevelsSupported == numStillQualityLevelsSupported );

    TInt numVideoQualityLevelsSupported = 3;
    assertL( info.iNumVideoQualityLevelsSupported == numVideoQualityLevelsSupported );

    }


void CCaeTestRelease::TestGetInfoPROD2MirageXL()
    {
    // Group:   Settings
    //
    // ID:      CAE.GEN.GETINFO.003
    // 
    // Action : Call GetInfo. Engine initialized
    //          ** For PROD2 Mirage X**
    //      
    // Output : OK.   
    
    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetInfo );
    
    // Get Camera Application Engine info object.
    TCamAppEngineInfo info;
    iSettingsClient->GetInfo( info );
    
    // Do comparisons.

    /*
    TVersion hardwareVersion = TVersion(0,0,0);
    assertL( ( info.iHardwareVersion.iMajor == hardwareVersion.iMajor ) &&
             ( info.iHardwareVersion.iMinor == hardwareVersion.iMinor ) &&
             ( info.iHardwareVersion.iBuild == hardwareVersion.iBuild ) );

    TVersion softwareVersion = TVersion(0,0,0);
    assertL( ( info.iSoftwareVersion.iMajor == softwareVersion.iMajor ) &&
             ( info.iSoftwareVersion.iMinor == softwareVersion.iMinor ) &&
             ( info.iSoftwareVersion.iBuild == softwareVersion.iBuild ) );
    */

    TCameraInfo::TCameraOrientation orientation = TCameraInfo::EOrientationOutwards;
    assertL( info.iOrientation == orientation );

    TUint32 optionsSupported = TCameraInfo::EViewFinderBitmapsSupported |
                               TCameraInfo::EImageCaptureSupported |
                               TCameraInfo::EVideoCaptureSupported |
                               TCameraInfo::EContrastSupported |
                               TCameraInfo::EBrightnessSupported;
    assertL( info.iOptionsSupported == optionsSupported );
    
    TUint32 flashModesSupported = CCamera::EFlashNone;
    assertL( info.iFlashModesSupported == flashModesSupported );

    TUint32 exposureModesSupported = CCamera::EExposureAuto | 
                                     CCamera::EExposureNight |
                                     CCamera::EExposureCenter |
                                     CCamera::EExposureBacklight;
    assertL( info.iExposureModesSupported == exposureModesSupported );
    
    TUint32 whiteBalanceModesSupported = CCamera::EWBAuto;
    assertL( info.iWhiteBalanceModesSupported == whiteBalanceModesSupported );

    TInt minzoom = 0;
    assertL( info.iMinZoom == minzoom );

    TInt maxZoom = 0;
    assertL( info.iMaxZoom == maxZoom );

    TInt maxDigitalZoom = 52;
    assertL( info.iMaxDigitalZoom == maxDigitalZoom );
    
    TReal32 minZoomFactor = 0.0;
    assertL( info.iMinZoomFactor == minZoomFactor );

    TReal32 maxZoomFactor = 0.0;
    assertL( info.iMaxZoomFactor == maxZoomFactor );

    TReal32 maxDigitalZoomFactor = 6.0;
    assertL( info.iMaxDigitalZoomFactor == maxDigitalZoomFactor );

    TInt numImageSizesSupported = 3;
    assertL( info.iNumImageSizesSupported == numImageSizesSupported );

    TUint32 imageFormatsSupported = CCamera::EFormatFbsBitmapColor4K |
                                    CCamera::EFormatFbsBitmapColor64K |
                                    CCamera::EFormatFbsBitmapColor16M |
                                    CCamera::EFormatExif;
    assertL( info.iImageFormatsSupported == imageFormatsSupported );

    TInt numStillQualityLevelsSupported = 3;
    assertL( info.iNumStillQualityLevelsSupported == numStillQualityLevelsSupported );

    TInt numVideoQualityLevelsSupported = 3;
    assertL( info.iNumVideoQualityLevelsSupported == numVideoQualityLevelsSupported );

    }


void CCaeTestRelease::TestGetInfoPROD3MirageXPL()
    {
    // Group:   Settings
    //
    // ID:      CAE.GEN.GETINFO.004
    // 
    // Action : Call GetInfo. Engine initialized
    //          ** For PROD3 Mirage XP**
    //      
    // Output : OK.   
    
    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetInfo );
    
    // Get Camera Application Engine info object.
    TCamAppEngineInfo info;
    iSettingsClient->GetInfo( info );
    
    // Do comparisons.

    /*
    TVersion hardwareVersion = TVersion(0,0,0);
    assertL( ( info.iHardwareVersion.iMajor == hardwareVersion.iMajor ) &&
             ( info.iHardwareVersion.iMinor == hardwareVersion.iMinor ) &&
             ( info.iHardwareVersion.iBuild == hardwareVersion.iBuild ) );

    TVersion softwareVersion = TVersion(0,0,0);
    assertL( ( info.iSoftwareVersion.iMajor == softwareVersion.iMajor ) &&
             ( info.iSoftwareVersion.iMinor == softwareVersion.iMinor ) &&
             ( info.iSoftwareVersion.iBuild == softwareVersion.iBuild ) );
    */

    TCameraInfo::TCameraOrientation orientation = TCameraInfo::EOrientationOutwards;
    assertL( info.iOrientation == orientation );

    TUint32 optionsSupported = TCameraInfo::EViewFinderBitmapsSupported |
                               TCameraInfo::EImageCaptureSupported |
                               TCameraInfo::EVideoCaptureSupported |
                               TCameraInfo::EContrastSupported |
                               TCameraInfo::EBrightnessSupported;
    assertL( info.iOptionsSupported == optionsSupported );
    
    TUint32 flashModesSupported = CCamera::EFlashNone |
                                  CCamera::EFlashAuto | 
                                  CCamera::EFlashForced;
    assertL( info.iFlashModesSupported == flashModesSupported );

    TUint32 exposureModesSupported = CCamera::EExposureAuto | 
                                     CCamera::EExposureNight;
    assertL( info.iExposureModesSupported == exposureModesSupported );
    
    TUint32 whiteBalanceModesSupported = CCamera::EWBAuto |
                                         CCamera::EWBDaylight |
                                         CCamera::EWBCloudy | 
                                         CCamera::EWBTungsten |
                                         CCamera::EWBFluorescent;
    assertL( info.iWhiteBalanceModesSupported == whiteBalanceModesSupported );

    TInt minzoom = 0;
    assertL( info.iMinZoom == minzoom );

    TInt maxZoom = 0;
    assertL( info.iMaxZoom == maxZoom );

    TInt maxDigitalZoom = 3;
    assertL( info.iMaxDigitalZoom == maxDigitalZoom );
    
    TReal32 minZoomFactor = 0.0;
    assertL( info.iMinZoomFactor == minZoomFactor );

    TReal32 maxZoomFactor = 0.0;
    assertL( info.iMaxZoomFactor == maxZoomFactor );

    TReal32 maxDigitalZoomFactor = 4.0;
    assertL( info.iMaxDigitalZoomFactor == maxDigitalZoomFactor );

    TInt numImageSizesSupported = 3; // 4 with HW version of Camera API !
    assertL( info.iNumImageSizesSupported == numImageSizesSupported );

    TUint32 imageFormatsSupported = CCamera::EFormatFbsBitmapColor4K |
                                    CCamera::EFormatFbsBitmapColor64K |
                                    CCamera::EFormatFbsBitmapColor16M |
                                    CCamera::EFormatExif;
    assertL( info.iImageFormatsSupported == imageFormatsSupported );

    TInt numStillQualityLevelsSupported = 3;
    assertL( info.iNumStillQualityLevelsSupported == numStillQualityLevelsSupported );

    TInt numVideoQualityLevelsSupported = 3;
    assertL( info.iNumVideoQualityLevelsSupported == numVideoQualityLevelsSupported );

    }

void CCaeTestRelease::TestGetInfoPROD4MirageXL()
    {
    // Group:   Settings
    //
    // ID:      CAE.GEN.GETINFO.005
    // 
    // Action : Call GetInfo. Engine initialized
    //          ** For PROD4 Mirage X**
    //      
    // Output : OK.   
    
    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetInfo );
    
    // Get Camera Application Engine info object.
    TCamAppEngineInfo info;
    iSettingsClient->GetInfo( info );
    
    // Do comparisons.

    /*
    TVersion hardwareVersion = TVersion(0,0,0);
    assertL( ( info.iHardwareVersion.iMajor == hardwareVersion.iMajor ) &&
             ( info.iHardwareVersion.iMinor == hardwareVersion.iMinor ) &&
             ( info.iHardwareVersion.iBuild == hardwareVersion.iBuild ) );

    TVersion softwareVersion = TVersion(0,0,0);
    assertL( ( info.iSoftwareVersion.iMajor == softwareVersion.iMajor ) &&
             ( info.iSoftwareVersion.iMinor == softwareVersion.iMinor ) &&
             ( info.iSoftwareVersion.iBuild == softwareVersion.iBuild ) );
    */

    TCameraInfo::TCameraOrientation orientation = TCameraInfo::EOrientationOutwards;
    assertL( info.iOrientation == orientation );

    TUint32 optionsSupported = TCameraInfo::EViewFinderBitmapsSupported |
                               TCameraInfo::EImageCaptureSupported |
                               TCameraInfo::EVideoCaptureSupported |
                               TCameraInfo::EContrastSupported |
                               TCameraInfo::EBrightnessSupported;
    assertL( info.iOptionsSupported == optionsSupported );
    
    TUint32 flashModesSupported = CCamera::EFlashNone | 
                                           CCamera::EFlashAuto |
                                           CCamera::EFlashForced;
    assertL( info.iFlashModesSupported == flashModesSupported );

    TUint32 exposureModesSupported = CCamera::EExposureAuto | 
                                     CCamera::EExposureNight |
                                     CCamera::EExposureCenter |
                                     CCamera::EExposureBacklight;
    assertL( info.iExposureModesSupported == exposureModesSupported );
    
    TUint32 whiteBalanceModesSupported = CCamera::EWBAuto;
    assertL( info.iWhiteBalanceModesSupported == whiteBalanceModesSupported );

    TInt minzoom = 0;
    assertL( info.iMinZoom == minzoom );

    TInt maxZoom = 0;
    assertL( info.iMaxZoom == maxZoom );

    TInt maxDigitalZoom = 52;
    assertL( info.iMaxDigitalZoom == maxDigitalZoom );
    
    TReal32 minZoomFactor = 0.0;
    assertL( info.iMinZoomFactor == minZoomFactor );

    TReal32 maxZoomFactor = 0.0;
    assertL( info.iMaxZoomFactor == maxZoomFactor );

    TReal32 maxDigitalZoomFactor = 6.0;
    assertL( info.iMaxDigitalZoomFactor == maxDigitalZoomFactor );

    TInt numImageSizesSupported = 3;
    assertL( info.iNumImageSizesSupported == numImageSizesSupported );

    TUint32 imageFormatsSupported = CCamera::EFormatFbsBitmapColor4K |
                                    CCamera::EFormatFbsBitmapColor64K |
                                    CCamera::EFormatFbsBitmapColor16M |
                                    CCamera::EFormatExif;
    assertL( info.iImageFormatsSupported == imageFormatsSupported );

    TInt numStillQualityLevelsSupported = 3;
    assertL( info.iNumStillQualityLevelsSupported == numStillQualityLevelsSupported );

    TInt numVideoQualityLevelsSupported = 3;
    assertL( info.iNumVideoQualityLevelsSupported == numVideoQualityLevelsSupported );

    }

void CCaeTestRelease::TestGetInfoPROD6MirageXL()
    {
    // Group:   Settings
    //
    // ID:      CAE.GEN.GETINFO.006
    // 
    // Action : Call GetInfo. Engine initialized
    //          ** For PROD6 Mirage X**
    //      
    // Output : OK.   
    
    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetInfo );
    
    // Get Camera Application Engine info object.
    TCamAppEngineInfo info;
    iSettingsClient->GetInfo( info );
    
    // Do comparisons.

    /*
    TVersion hardwareVersion = TVersion(0,0,0);
    assertL( ( info.iHardwareVersion.iMajor == hardwareVersion.iMajor ) &&
             ( info.iHardwareVersion.iMinor == hardwareVersion.iMinor ) &&
             ( info.iHardwareVersion.iBuild == hardwareVersion.iBuild ) );

    TVersion softwareVersion = TVersion(0,0,0);
    assertL( ( info.iSoftwareVersion.iMajor == softwareVersion.iMajor ) &&
             ( info.iSoftwareVersion.iMinor == softwareVersion.iMinor ) &&
             ( info.iSoftwareVersion.iBuild == softwareVersion.iBuild ) );
    */

    TCameraInfo::TCameraOrientation orientation = TCameraInfo::EOrientationOutwards;
    assertL( info.iOrientation == orientation );

    TUint32 optionsSupported = TCameraInfo::EViewFinderBitmapsSupported |
                               TCameraInfo::EImageCaptureSupported |
                               TCameraInfo::EVideoCaptureSupported |
                               TCameraInfo::EViewFinderMirrorSupported;

    assertL( info.iOptionsSupported == optionsSupported );
    
    TUint32 flashModesSupported = CCamera::EFlashNone;
    assertL( info.iFlashModesSupported == flashModesSupported );

    TUint32 exposureModesSupported = CCamera::EExposureAuto |
    								 CCamera::EExposureNight; 
    assertL( info.iExposureModesSupported == exposureModesSupported );
    
    TUint32 whiteBalanceModesSupported = CCamera::EWBAuto;
    assertL( info.iWhiteBalanceModesSupported == whiteBalanceModesSupported );

    TInt minzoom = 0;
    assertL( info.iMinZoom == minzoom );

    TInt maxZoom = 0;
    assertL( info.iMaxZoom == maxZoom );

    TInt maxDigitalZoom = 52;
    assertL( info.iMaxDigitalZoom == maxDigitalZoom );
    
    TReal32 minZoomFactor = 0.0;
    assertL( info.iMinZoomFactor == minZoomFactor );

    TReal32 maxZoomFactor = 0.0;
    assertL( info.iMaxZoomFactor == maxZoomFactor );

    TReal32 maxDigitalZoomFactor = 6.0;
    assertL( info.iMaxDigitalZoomFactor == maxDigitalZoomFactor );

    TInt numImageSizesSupported = 3;
    assertL( info.iNumImageSizesSupported == numImageSizesSupported );

    TUint32 imageFormatsSupported = CCamera::EFormatFbsBitmapColor4K |
                                    CCamera::EFormatFbsBitmapColor64K |
                                    CCamera::EFormatFbsBitmapColor16M |
                                    CCamera::EFormatExif;
    assertL( info.iImageFormatsSupported == imageFormatsSupported );

    TInt numStillQualityLevelsSupported = 3;
    assertL( info.iNumStillQualityLevelsSupported == numStillQualityLevelsSupported );

    TInt numVideoQualityLevelsSupported = 3;
    assertL( info.iNumVideoQualityLevelsSupported == numVideoQualityLevelsSupported );

    }


void CCaeTestRelease::TestGetInfoPROD7Unagi2ML()
    {
    // Group:   Settings
    //
    // ID:      CAE.GEN.GETINFO.007
    // 
    // Action : Call GetInfo. Engine initialized
    //          ** For PROD7 Unagi 2M **
    //      
    // Output : OK.   
    
    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetInfo );
    
    // Get Camera Application Engine info object.
    TCamAppEngineInfo info;
    iSettingsClient->GetInfo( info );
    
    // Do comparisons.

    /*
    TVersion hardwareVersion = TVersion(0,0,0);
    assertL( ( info.iHardwareVersion.iMajor == hardwareVersion.iMajor ) &&
             ( info.iHardwareVersion.iMinor == hardwareVersion.iMinor ) &&
             ( info.iHardwareVersion.iBuild == hardwareVersion.iBuild ) );

    TVersion softwareVersion = TVersion(0,0,0);
    assertL( ( info.iSoftwareVersion.iMajor == softwareVersion.iMajor ) &&
             ( info.iSoftwareVersion.iMinor == softwareVersion.iMinor ) &&
             ( info.iSoftwareVersion.iBuild == softwareVersion.iBuild ) );
    */

    TCameraInfo::TCameraOrientation orientation = TCameraInfo::EOrientationOutwards;
    assertL( info.iOrientation == orientation );

    TUint32 optionsSupported = TCameraInfo::EViewFinderBitmapsSupported |
                               TCameraInfo::EImageCaptureSupported |
                               TCameraInfo::EVideoCaptureSupported |
                               TCameraInfo::EViewFinderMirrorSupported;
                               //TCameraInfo::EContrastSupported |
                               //TCameraInfo::EBrightnessSupported;
    assertL( info.iOptionsSupported == optionsSupported );
    
    TUint32 flashModesSupported = CCamera::EFlashNone | 
                                  CCamera::EFlashAuto |
                                  CCamera::EFlashForced |
                                  CCamera::EFlashRedEyeReduce;
    assertL( info.iFlashModesSupported == flashModesSupported );

    TUint32 exposureModesSupported = CCamera::EExposureAuto | 
                                     CCamera::EExposureNight |
                                     CCamera::EExposureCenter |
                                     CCamera::EExposureBacklight |
                                     CCamera::EExposureSport;
    assertL( info.iExposureModesSupported == exposureModesSupported );
    
    TUint32 whiteBalanceModesSupported = CCamera::EWBAuto |
                                         CCamera::EWBDaylight |
                                         CCamera::EWBCloudy |
                                         CCamera::EWBTungsten |
                                         CCamera::EWBFluorescent;
    assertL( info.iWhiteBalanceModesSupported == whiteBalanceModesSupported );

    TInt minzoom = 0;
    assertL( info.iMinZoom == minzoom );

    TInt maxZoom = 0;
    assertL( info.iMaxZoom == maxZoom );

    TInt maxDigitalZoom = 71;
    assertL( info.iMaxDigitalZoom == maxDigitalZoom );
    
    TReal32 minZoomFactor = 0.0;
    assertL( info.iMinZoomFactor == minZoomFactor );

    TReal32 maxZoomFactor = 0.0;
    assertL( info.iMaxZoomFactor == maxZoomFactor );

    TReal32 maxDigitalZoomFactor = 20.0;
    assertL( info.iMaxDigitalZoomFactor == maxDigitalZoomFactor );

    TInt numImageSizesSupported = 4;
    assertL( info.iNumImageSizesSupported == numImageSizesSupported );

    TUint32 imageFormatsSupported = CCamera::EFormatFbsBitmapColor4K |
                                    CCamera::EFormatFbsBitmapColor64K |
                                    CCamera::EFormatFbsBitmapColor16M |
                                    CCamera::EFormatExif;
    assertL( info.iImageFormatsSupported == imageFormatsSupported );

    TInt numStillQualityLevelsSupported = 3;
    assertL( info.iNumStillQualityLevelsSupported == numStillQualityLevelsSupported );

    TInt numVideoQualityLevelsSupported = 3;
    assertL( info.iNumVideoQualityLevelsSupported == numVideoQualityLevelsSupported );

    }



void CCaeTestRelease::TestGetInfoPROD8MirageXL()

    {

    // Group:   Settings
    //
    // ID:      CAE.GEN.GETINFO.008
    // 
    // Action : Call GetInfo. Engine initialized
    //          ** For PROD8 Mirage X**
    //      
    // Output : OK.   
    
    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetInfo );   

    // Get Camera Application Engine info object.
    TCamAppEngineInfo info;
    iSettingsClient->GetInfo( info );  

    // Do comparisons.

    /*
    TVersion hardwareVersion = TVersion(0,0,0);
    assertL( ( info.iHardwareVersion.iMajor == hardwareVersion.iMajor ) &&
             ( info.iHardwareVersion.iMinor == hardwareVersion.iMinor ) &&
             ( info.iHardwareVersion.iBuild == hardwareVersion.iBuild ) );

    TVersion softwareVersion = TVersion(0,0,0);
    assertL( ( info.iSoftwareVersion.iMajor == softwareVersion.iMajor ) &&
             ( info.iSoftwareVersion.iMinor == softwareVersion.iMinor ) &&
             ( info.iSoftwareVersion.iBuild == softwareVersion.iBuild ) );
    */

    TCameraInfo::TCameraOrientation orientation = TCameraInfo::EOrientationOutwards;
    assertL( info.iOrientation == orientation );

    TUint32 optionsSupported = TCameraInfo::EViewFinderBitmapsSupported |
                               TCameraInfo::EImageCaptureSupported |
                               TCameraInfo::EVideoCaptureSupported;                               
    assertL( info.iOptionsSupported == optionsSupported );
  
    TUint32 flashModesSupported = CCamera::EFlashNone;
    assertL( info.iFlashModesSupported == flashModesSupported );

    TUint32 exposureModesSupported = CCamera::EExposureAuto |
    								 CCamera::EExposureNight |
    								 CCamera::EExposureSport |
                                     CCamera::EExposureCenter;
                                     
    assertL( info.iExposureModesSupported == exposureModesSupported );

    TUint32 whiteBalanceModesSupported = CCamera::EWBAuto |
                                         CCamera::EWBDaylight |
                                         CCamera::EWBCloudy |
                                         CCamera::EWBTungsten |
                                         CCamera::EWBFluorescent;

    assertL( info.iWhiteBalanceModesSupported == whiteBalanceModesSupported );

    TInt minzoom = 0;
    assertL( info.iMinZoom == minzoom );

    TInt maxZoom = 0;
    assertL( info.iMaxZoom == maxZoom );

    TInt maxDigitalZoom = 71;
    assertL( info.iMaxDigitalZoom == maxDigitalZoom );

    TReal32 minZoomFactor = 0.0;
    assertL( info.iMinZoomFactor == minZoomFactor );

    TReal32 maxZoomFactor = 0.0;
    assertL( info.iMaxZoomFactor == maxZoomFactor );

    TReal32 maxDigitalZoomFactor = 20.0;
    assertL( info.iMaxDigitalZoomFactor == maxDigitalZoomFactor );

    TInt numImageSizesSupported = 5;
    assertL( info.iNumImageSizesSupported == numImageSizesSupported );

    TUint32 imageFormatsSupported = CCamera::EFormatFbsBitmapColor4K |
                                    CCamera::EFormatFbsBitmapColor64K |
                                    CCamera::EFormatFbsBitmapColor16M |
                                    CCamera::EFormatExif;

    assertL( info.iImageFormatsSupported == imageFormatsSupported );

    TInt numStillQualityLevelsSupported = 3;
    assertL( info.iNumStillQualityLevelsSupported == numStillQualityLevelsSupported );

    TInt numVideoQualityLevelsSupported = 3;
    assertL( info.iNumVideoQualityLevelsSupported == numVideoQualityLevelsSupported );

    }
    

void CCaeTestRelease::TestGetInfoPROD9HWL()

    {

    // Group:   Settings
    //
    // ID:      CAE.GEN.GETINFO.009
    // 
    // Action : Call GetInfo. Engine initialized
    //          ** For PROD9 HW **
    //      
    // Output : OK.   
    
    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetInfo );   

    // Get Camera Application Engine info object.
    TCamAppEngineInfo info;
    iSettingsClient->GetInfo( info );  

    // Do comparisons.

    /*
    TVersion hardwareVersion = TVersion(0,0,0);
    assertL( ( info.iHardwareVersion.iMajor == hardwareVersion.iMajor ) &&
             ( info.iHardwareVersion.iMinor == hardwareVersion.iMinor ) &&
             ( info.iHardwareVersion.iBuild == hardwareVersion.iBuild ) );

    TVersion softwareVersion = TVersion(0,0,0);
    assertL( ( info.iSoftwareVersion.iMajor == softwareVersion.iMajor ) &&
             ( info.iSoftwareVersion.iMinor == softwareVersion.iMinor ) &&
             ( info.iSoftwareVersion.iBuild == softwareVersion.iBuild ) );
    */

    TCameraInfo::TCameraOrientation orientation = TCameraInfo::EOrientationOutwards;
    assertL( info.iOrientation == orientation );

    TUint32 optionsSupported = TCameraInfo::EViewFinderBitmapsSupported |
                               TCameraInfo::EImageCaptureSupported |
                               TCameraInfo::EVideoCaptureSupported;                               
    assertL( info.iOptionsSupported == optionsSupported );
  
    TUint32 flashModesSupported =           CCamera::EFlashAuto     |
                                            CCamera::EFlashForced   |
                                            CCamera::EFlashRedEyeReduce;
    assertL( info.iFlashModesSupported == flashModesSupported );

    TUint32 exposureModesSupported = CCamera::EExposureAuto |
    								 CCamera::EExposureNight |
    								 CCamera::EExposureBacklight |
                                     CCamera::EExposureCenter |
                                     CCamera::EExposureSuperNight;
                                     
    RDebug::Print(_L("info.iExposureModesSupported = 0x%x / 0x%x"), info.iExposureModesSupported, exposureModesSupported);
    assertL( info.iExposureModesSupported == exposureModesSupported );

    TUint32 whiteBalanceModesSupported = CCamera::EWBAuto |
                                         CCamera::EWBDaylight |
                                         CCamera::EWBTungsten |
                                         CCamera::EWBFluorescent;

    assertL( info.iWhiteBalanceModesSupported == whiteBalanceModesSupported );

    TInt minzoom = 0;
    assertL( info.iMinZoom == minzoom );

    TInt maxZoom = 0;
    assertL( info.iMaxZoom == maxZoom );

    TInt maxDigitalZoom = 40;
    assertL( info.iMaxDigitalZoom == maxDigitalZoom );

    TReal32 minZoomFactor = 0.0;
    assertL( info.iMinZoomFactor == minZoomFactor );

    TReal32 maxZoomFactor = 0.0;
    assertL( info.iMaxZoomFactor == maxZoomFactor );

    TReal32 maxDigitalZoomFactor = 4.0;
    assertL( info.iMaxDigitalZoomFactor == maxDigitalZoomFactor );

#if ( defined (__WINS__) || defined (__WINSCW) ) // this different on hw
    TInt numImageSizesSupported = 3;
#else
    TInt numImageSizesSupported = 4;
#endif
    RDebug::Print(_L("info.iNumImageSizesSupported = %d"), info.iNumImageSizesSupported);

    assertL( info.iNumImageSizesSupported == numImageSizesSupported );

    TUint32 imageFormatsSupported = CCamera::EFormatFbsBitmapColor4K |
                                    CCamera::EFormatFbsBitmapColor64K |
                                    CCamera::EFormatFbsBitmapColor16M |
                                    CCamera::EFormatExif;

    assertL( info.iImageFormatsSupported == imageFormatsSupported );

    TInt numStillQualityLevelsSupported = 3;
    assertL( info.iNumStillQualityLevelsSupported == numStillQualityLevelsSupported );

    TInt numVideoQualityLevelsSupported = 3;
    assertL( info.iNumVideoQualityLevelsSupported == numVideoQualityLevelsSupported );

    }
    

void CCaeTestRelease::TestGetInfoDevlonHWL()
    {

    // Group:   Settings
    //
    // ID:      CAE.GEN.GETINFO.010
    // 
    // Action : Call GetInfo. Engine initialized
    //          ** For Devlon HW **
    //      
    // Output : OK. Note only interfaces are tested, not the content itself anymore
    
    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetInfo );   

    // Get Camera Application Engine info object.
    TCamAppEngineInfo info;
    iSettingsClient->GetInfo( info );  

//    TCameraInfo::TCameraOrientation orientation = TCameraInfo::EOrientationOutwards;
//    assertL( info.iOrientation == orientation );
    RDebug::Print(_L("info.iOrientation = 0x%x"), info.iOrientation);

//    TUint32 optionsSupported = TCameraInfo::EViewFinderBitmapsSupported |
//                               TCameraInfo::EImageCaptureSupported |
//                               TCameraInfo::EVideoCaptureSupported;                               
//    assertL( info.iOptionsSupported == optionsSupported );
    RDebug::Print(_L("info.iOptionsSupported = 0x%x"), info.iOptionsSupported);
  
//    TUint32 flashModesSupported =           CCamera::EFlashAuto     |
//                                            CCamera::EFlashForced   |
//                                            CCamera::EFlashRedEyeReduce;
//    assertL( info.iFlashModesSupported == flashModesSupported );
    RDebug::Print(_L("info.iFlashModesSupported = 0x%x"), info.iFlashModesSupported);

//    TUint32 exposureModesSupported = CCamera::EExposureAuto |
//    								 CCamera::EExposureNight |
//    								 CCamera::EExposureBacklight |
//                                     CCamera::EExposureCenter;
                                     
//    assertL( info.iExposureModesSupported == exposureModesSupported );
    RDebug::Print(_L("info.iExposureModesSupported = 0x%x"), info.iExposureModesSupported);

//    TUint32 whiteBalanceModesSupported = CCamera::EWBAuto |
//                                         CCamera::EWBDaylight |
//                                         CCamera::EWBTungsten |
//                                         CCamera::EWBFluorescent;

//    assertL( info.iWhiteBalanceModesSupported == whiteBalanceModesSupported );
    RDebug::Print(_L("info.iWhiteBalanceModesSupported = 0x%x"), info.iWhiteBalanceModesSupported);

//    TInt minzoom = 0;
//    assertL( info.iMinZoom == minzoom );
    RDebug::Print(_L("info.iMinZoom = 0x%x"), info.iMinZoom);

//    TInt maxZoom = 0;
//    assertL( info.iMaxZoom == maxZoom );
    RDebug::Print(_L("info.iMaxZoom = 0x%x"), info.iMaxZoom);

//    TInt maxDigitalZoom = 40;
//    assertL( info.iMaxDigitalZoom == maxDigitalZoom );
    RDebug::Print(_L("info.iMaxDigitalZoom = 0x%x"), info.iMaxDigitalZoom);

//    TReal32 minZoomFactor = 0.0;
//    assertL( info.iMinZoomFactor == minZoomFactor );
    RDebug::Print(_L("info.minZoomFactor = 0x%x"), info.iMinZoomFactor);

//    TReal32 maxZoomFactor = 0.0;
//    assertL( info.iMaxZoomFactor == maxZoomFactor );
    RDebug::Print(_L("info.iMaxZoomFactor = 0x%x"), info.iMaxZoomFactor);

//    TReal32 maxDigitalZoomFactor = 4.0;
//    assertL( info.iMaxDigitalZoomFactor == maxDigitalZoomFactor );
    RDebug::Print(_L("info.iMaxDigitalZoomFactor = 0x%x"), info.iMaxDigitalZoomFactor);

//#if ( defined (__WINS__) || defined (__WINSCW) ) // this different on hw
//    TInt numImageSizesSupported = 3;
//#else
//    TInt numImageSizesSupported = 4;
//#endif
//    RDebug::Print(_L("info.iNumImageSizesSupported = %d"), info.iNumImageSizesSupported);

//    assertL( info.iNumImageSizesSupported == numImageSizesSupported );
    RDebug::Print(_L("info.iNumImageSizesSupported = 0x%x"), info.iNumImageSizesSupported);

//    TUint32 imageFormatsSupported = CCamera::EFormatFbsBitmapColor4K |
//                                    CCamera::EFormatFbsBitmapColor64K |
//                                    CCamera::EFormatFbsBitmapColor16M |
//                                    CCamera::EFormatExif;

//    assertL( info.iImageFormatsSupported == imageFormatsSupported );
    RDebug::Print(_L("info.iImageFormatsSupported = 0x%x"), info.iImageFormatsSupported);

//    TInt numStillQualityLevelsSupported = 3;
//    assertL( info.iNumStillQualityLevelsSupported == numStillQualityLevelsSupported );
    RDebug::Print(_L("info.iNumStillQualityLevelsSupported = 0x%x"), info.iNumStillQualityLevelsSupported);

//    TInt numVideoQualityLevelsSupported = 3;
//    assertL( info.iNumVideoQualityLevelsSupported == numVideoQualityLevelsSupported );
    RDebug::Print(_L("info.iNumVideoQualityLevelsSupported = 0x%x"), info.iNumVideoQualityLevelsSupported);

    }
    

/********************************************************/
/*               RESUME VIDEO RECORDING                 */
/********************************************************/

void CCaeTestRelease::TestRecordPauseResumeStopL()
    {
	// ID:      CAE.VR.RESUMEVIDEORECORDING.001
    //
    // Group:   Record.
    // 
    // Action : Resume video clip recording after pause. 
    //          Video recording.
	//      
	// Output : No leave. File KVideoFileName, "videoclip.3gp".   

    iStatesClient->SetTimeout( 4000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iStatesClient->SetVideoFileName( _L("cae_pause_resume.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareRecordPauseResumeStopVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 2 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*             STILL CAPTURE EXPOSURE MODE              */
/********************************************************/

void CCaeTestRelease::TestSCExposureMode0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETEXPOSUREMODEL.001
    //
    // Action : Test auto exposure mode setting with still image capture and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetExposureMode( 0 ); // auto mode should be supported

    iSettingsClient->SetStillFileName( _L("cae_exposure_mode_0.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenStillPrepared );
}

void CCaeTestRelease::TestSCExposureMode1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETEXPOSUREMODEL.002
    //
    // Action : Test night exposure mode setting with still image capture and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetExposureMode( 1 ); // night mode should be supported

    iSettingsClient->SetStillFileName( _L("cae_exposure_mode_1.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenStillPrepared );
}

void CCaeTestRelease::TestSCExposureModeInitialized0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETEXPOSUREMODEL.015
    //
    // Action : Test auto exposure mode setting with still image capture and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetExposureMode( 0 ); // auto mode should be supported

    iSettingsClient->SetStillFileName( _L("cae_exposure_mode_initialized_0.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenInitialized );
}

void CCaeTestRelease::TestSCExposureModeInitialized1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETEXPOSUREMODEL.016
    //
    // Action : Test night exposure mode setting with still image capture and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetExposureMode( 1 ); // night mode should be supported

    iSettingsClient->SetStillFileName( _L("cae_exposure_mode_initialized_1.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenInitialized );
}

/********************************************************/
/*             VIDEO RECORDING EXPOSURE MODE            */
/********************************************************/

void CCaeTestRelease::TestVRExposureMode0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETEXPOSUREMODEL.008
    //
    // Action : Test auto exposure mode setting with video recording and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetTimeout( 4000000 );

    iSettingsClient->SetExposureMode( 0 ); // auto mode should be supported

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_exposure_mode_0.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenVideoPrepared );
}

void CCaeTestRelease::TestVRExposureMode1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETEXPOSUREMODEL.009
    //
    // Action : Test night exposure mode setting with video recording and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetTimeout( 4000000 );

    iSettingsClient->SetExposureMode( 1 ); // night mode should be supported

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_exposure_mode_1.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenVideoPrepared );
}

void CCaeTestRelease::TestVRExposureModeRecording0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETEXPOSUREMODEL.017
    //
    // Action : Test auto exposure mode setting with Video image capture and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetTimeout( 4000000 );

    iSettingsClient->SetExposureMode( 0 ); // auto mode should be supported

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_exposure_mode_recording_0.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenVideoRecording );
}

void CCaeTestRelease::TestVRExposureModeRecording1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETEXPOSUREMODEL.018
    //
    // Action : Test night exposure mode setting with Video image capture and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetTimeout( 4000000 );

    iSettingsClient->SetExposureMode( 1 ); // night mode should be supported

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_exposure_mode_recording_1.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenVideoRecording );
}

/********************************************************/
/*             STILL CAPTURE EV COMPENSATION            */
/********************************************************/

void CCaeTestRelease::TestSCEvCompensation0L()
{
	// Group:   Settings
    //
    // ID:      CAE.SET-SC.SETEVCOMP.001
    // 
    // Action : Test Ev compensation with still image capture and viewfinding.
	//      
	// Output : KErrNotSupported.  
 
    iSettingsClient->SetEvCompensation( 0 );

    iSettingsClient->SetSnapFileName( _L("cae_evcomp_0.mbm") );

    iSettingsClient->SetStillFileName( _L("cae_evcomp_0.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenStillPrepared ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }

	assertL(err == KErrNotSupported);
}

void CCaeTestRelease::TestSCEvCompensationMaxL()
{
	// Group:   Settings
    //
    // ID:      CAE.SET-SC.SETEVCOMP.002
    // 
    // Action : Test Ev compensation with still image capture and viewfinding.
	//      
	// Output : KErrNotSupported.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetEvCompensation( info.iMaxEvCompensation );

    iSettingsClient->SetSnapFileName( _L("cae_evcomp_max.mbm") );

    iSettingsClient->SetStillFileName( _L("cae_evcomp_max.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenStillPrepared ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }

	assertL(err == KErrNotSupported);
}

void CCaeTestRelease::TestSCEvCompensationMinL()
{
	// Group:   Settings
    //
    // ID:      CAE.SET-SC.SETEVCOMP.003
    // 
    // Action : Test Ev compensation with still image capture and viewfinding.
	//      
	// Output : KErrNotSupported.   

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetEvCompensation( info.iMinEvCompensation );

    iSettingsClient->SetSnapFileName( _L("cae_evcomp_min.mbm") );

    iSettingsClient->SetStillFileName( _L("cae_evcomp_min.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenStillPrepared ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }

	assertL(err == KErrNotSupported);
}


void CCaeTestRelease::TestSCEvCompensationInitialized0L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETEVCOMP.004
    //
    // Action : Test Ev compensation with still image capture and viewfinding.
	//      
	// Output : KErrNotSupported.   

    iSettingsClient->SetEvCompensation( 0 );

    iSettingsClient->SetSnapFileName( _L("cae_evcomp_initialized_0.mbm") );

    iSettingsClient->SetStillFileName( _L("cae_evcomp_initialized_0.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenInitialized ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }

	assertL(err == KErrNotSupported);
}

void CCaeTestRelease::TestSCEvCompensationInitializedMaxL()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETEVCOMP.005
    //
    // Action : Test Ev compensation with still image capture and viewfinding.
	//      
	// Output : KErrNotSupported.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetEvCompensation( info.iMaxEvCompensation );

    iSettingsClient->SetSnapFileName( _L("cae_evcomp_initilized_max.mbm") );

    iSettingsClient->SetStillFileName( _L("cae_evcomp_initilized_max.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenInitialized ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }

	assertL(err == KErrNotSupported);
}

void CCaeTestRelease::TestSCEvCompensationInitializedMinL()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETEVCOMP.006
    //
    // Action : Test Ev compensation with still image capture and viewfinding.
	//      
	// Output : KErrNotSupported.   

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetEvCompensation( info.iMinEvCompensation );

    iSettingsClient->SetSnapFileName( _L("cae_evcomp_initialized_min.mbm") );

    iSettingsClient->SetStillFileName( _L("cae_evcomp_initialized_min.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenInitialized ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }

	assertL(err == KErrNotSupported);
}

void CCaeTestRelease::TestSCEvCompensationInitializedInvalidL()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETEVCOMP.007
    //
    // Action : Test Ev compensation with still image capture and viewfinding.
	//      
	// Output : KErrNotSupported.   

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetEvCompensation( info.iMaxEvCompensation + 1 );

    TRAPD( error, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenInitialized ) );
    if( error == KErrNoMemory )
        {
        User::Leave( error ); // For memory test
        }
	assertL(error == KErrNotSupported);
}



/********************************************************/
/*             VIDEO RECORDING EV COMPENSATION          */
/********************************************************/

void CCaeTestRelease::TestVREvCompensationMinL()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETEVCOMP.001
    //
    // Action : Test Ev compensation with Video image capture and viewfinding.
	//      
	// Output : KErrNotSupported.   

    iSettingsClient->SetTimeout( 4000000 );
    
    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetEvCompensation( info.iMinEvCompensation );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_evcomp_min.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenVideoPrepared ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }

	assertL(err == KErrNotSupported);
}

void CCaeTestRelease::TestVREvCompensationRecordingMaxL()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETEVCOMP.002
    //
    // Action : Test Ev compensation with Video image capture and viewfinding.
	//      
	// Output : KErrNotSupported.   

    iSettingsClient->SetTimeout( 4000000 );
    
    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetEvCompensation( info.iMaxEvCompensation );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_evcomp_recording_max.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenVideoRecording ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }

	assertL(err == KErrNotSupported);
}


/* UNUSED

void CCaeTestRelease::TestVREvCompensation0L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETEVCOMP.001
    //
    // Action : Test Ev compensation with Video image capture and viewfinding.
	//      
	// Output : OK.

    iSettingsClient->SetTimeout( 4000000 );
    
    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetEvCompensation( 0 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_evcomp_0.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenVideoPrepared );
}

void CCaeTestRelease::TestVREvCompensationMaxL()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETEVCOMP.002
    //
    // Action : Test Ev compensation with Video image capture and viewfinding.
	//      
	// Output : OK.   

    iSettingsClient->SetTimeout( 4000000 );
    
    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetEvCompensation( info.iMaxEvCompensation );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_evcomp_max.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenVideoPrepared );
}

void CCaeTestRelease::TestVREvCompensationRecording0L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETEVCOMP.004
    //
    // Action : Test Ev compensation with Video image capture and viewfinding.
	//      
	// Output : OK.   

    iSettingsClient->SetTimeout( 4000000 );
    
    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetEvCompensation( 0 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_evcomp_recording_0.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenVideoRecording );
}

void CCaeTestRelease::TestVREvCompensationRecordingMinL()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETEVCOMP.006
    //
    // Action : Test Ev compensation with Video image capture and viewfinding.
	//      
	// Output : OK.   

    iSettingsClient->SetTimeout( 4000000 );
    
    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetEvCompensation( info.iMinEvCompensation );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_evcomp_recording_min.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenVideoRecording );
}

void CCaeTestRelease::TestVREvCompensationRecordingInvalidL()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETEVCOMP.007
    //
    // Action : Test Ev compensation with Video image capture and viewfinding.
	//      
	// Output : OK.   

    iSettingsClient->SetTimeout( 4000000 );
    
    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);
    iSettingsClient->SetEvCompensation( info.iMinEvCompensation - 1 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_evcomp_recording_invalid.3gp") );
    
    TRAPD( error, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEvCompensationWhenVideoRecording ) );
	assertL(error == KErrNotSupported);
}

*/

/********************************************************/
/*           STILL CAPTURE WHITE BALANCE MODE           */
/********************************************************/

void CCaeTestRelease::TestSCWhiteBalanceMode0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETWHITEBALANCEMODEL.001
    //
    // Action : Test white balance setting with still image capture and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( 0 ); // auto mode should always be supported

    iSettingsClient->SetStillFileName( _L("cae_white_balance_mode_0.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenStillPrepared );
}

void CCaeTestRelease::TestSCWhiteBalanceModeInitialized0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETWHITEBALANCEMODEL.019
    //
    // Action : Test white balance setting with still image capture and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( 0 ); // auto mode should always be supported

    iSettingsClient->SetStillFileName( _L("cae_white_balance_mode_initialized_0.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenInitialized );
}

/********************************************************/
/*           VIDEO RECORDING WHITE BALANCE MODE         */
/********************************************************/

void CCaeTestRelease::TestVRWhiteBalanceMode0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETWHITEBALANCEMODEL.010
    //
    // Action : Test white balance setting with video recording and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetTimeout( 4000000 );

    iSettingsClient->SetWhiteBalanceMode( 0 ); // auto mode should always be supported

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_white_balance_mode_0.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenVideoPrepared );
}

void CCaeTestRelease::TestVRWhiteBalanceModeRecording0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETWHITEBALANCEMODEL.020
    //
    // Action : Test white balance setting with Video image capture and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetTimeout( 4000000 );

    iSettingsClient->SetWhiteBalanceMode( 0 ); // auto mode should always be supported

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_white_balance_mode_recording_0.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenVideoRecording );
}

/********************************************************/
/*             STILL CAPTURE CONTRAST                   */
/********************************************************/

void CCaeTestRelease::TestSCContrast50L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETCONTRASTL.001
    //
    // Action : Test contrast setting with still image capture and viewfinding.
	//      
	// Output : No leave.   

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetContrast( 50 );

    iSettingsClient->SetStillFileName( _L("cae_contrast_50.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenStillPrepared ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestRelease::TestSCContrastInitialized50L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETCONTRASTL.007
    //
    // Action : Test contrast setting with still image capture and viewfinding.
	//      
	// Output : No leave.   

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetContrast( 50 );

    iSettingsClient->SetStillFileName( _L("cae_contrast_initialized_50.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenInitialized ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

/********************************************************/
/*             STILL CAPTURE BRIGHTNESS                 */
/********************************************************/

void CCaeTestRelease::TestSCBrightness50L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETBRIGHTNESSL.001
    //
    // Action : Test brightness setting with still image capture and viewfinding.
	//      
	// Output : No leave.   

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetBrightness( 50 );

    iSettingsClient->SetStillFileName( _L("cae_brightness_50.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenStillPrepared ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestRelease::TestSCBrightnessInitialized50L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETBRIGHTNESSL.007
    //
    // Action : Test brightness setting with still image capture and viewfinding.
	//      
	// Output : No leave.   

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetBrightness( 50 );

    iSettingsClient->SetStillFileName( _L("cae_brightness_initialized_50.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenInitialized ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

/********************************************************/
/*             VIDEO RECORDING CONTRAST                 */
/********************************************************/

void CCaeTestRelease::TestVRContrast50L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETCONTRASTL.008
    //
    // Action : Test contrast setting with video recording and viewfinding.
	//      
	// Output : No leave.   

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetTimeout( 4000000 );

    iSettingsClient->SetContrast( 50 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_contrast_50.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenVideoPrepared ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestRelease::TestVRContrastRecording50L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETCONTRASTL.014
    //
    // Action : Test contrast setting with Video image capture and viewfinding.
	//      
	// Output : No leave.   

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetTimeout( 4000000 );

    iSettingsClient->SetContrast( 50 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_contrast_recording_50.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenVideoRecording ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

/********************************************************/
/*             VIDEO RECORDING BRIGHTNESS               */
/********************************************************/

void CCaeTestRelease::TestVRBrightness50L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETBRIGHTNESSL.008
    //
    // Action : Test brightness setting with video recording and viewfinding.
	//      
	// Output : No leave.   

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetTimeout( 4000000 );

    iSettingsClient->SetBrightness( 50 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_brightness_50.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenVideoPrepared ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestRelease::TestVRBrightnessRecording50L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETBRIGHTNESSL.014
    //
    // Action : Test brightness setting with Video image capture and viewfinding.
	//      
	// Output : No leave.   

 	TCamAppEngineInfo info;
	iSettingsClient->GetInfo(info);

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetTimeout( 4000000 );

    iSettingsClient->SetBrightness( 50 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_brightness_recording_50.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenVideoRecording ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

/********************************************************/
/*             VIDEO RECORDING CLIP MAX SIZE            */
/********************************************************/

void CCaeTestRelease::TestVideoClipMaxSize10000L()
    {
	// Group:   Settings.
    // 
    //ID:       CAE.SET-VR.SETVIDEOCLIPMAXSIZEL.002
    //
    // Action : Test video clip max size setting with recording.
	//      
	// Output : Leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetTimeout( 20000000 );

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoClipMaxSize( 10000 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iSettingsClient->SetVideoFileName( _L("cae_clip_max_size_10000.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetClipMaxSizeWhenVideoPrepared ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }
    assertL( err == KErrCompletion );
    }

/********************************************************/
/*               CANCEL CAPTURE STILL                   */
/********************************************************/

void CCaeTestRelease::TestCancelCaptureStillL()
    {
	// ID: CAE.SC.CANCELCAPTURESTILL.001
    //
    // Group:   Capture
    // 
    // Action : Cancel on-going still image capture request, 
    //          when already prepared.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureAndCancelStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == 0 );
    }

/********************************************************/
/*               GET QUALITY INDEX                      */
/********************************************************/

void CCaeTestRelease::TestGetStillQualityIndexWhenEngineInitializedL()
    {
	// ID: CAE.SC.GETSTILLQUALITYINDEX.001
    //
    // Group:   Capture
    // 
    // Action : Get still quality index, when engine is initialized.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 1 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EGetStillQualityIndexWhenEngineInitialized );
    }

void CCaeTestRelease::TestGetStillQualityIndexWhenPreparedL()
    {
	// ID: CAE.SC.GETSTILLQUALITYINDEX.002
    //
    // Group:   Capture
    // 
    // Action : Get still quality index, when still image capture is prepared.
	//      
	// Output : OK.   

    iStatesClient->SetStillQualityLevelIndex( 1 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EGetStillQualityIndexWhenPrepared );
    }

void CCaeTestRelease::TestGetVideoQualityIndexWhenEngineInitializedL()
{
	// Group:   Video.
    // 
    // ID:      CAE.VR.GETVIDEOQUALITYINDEX.001
    //
    // Action : Get video quality index, when engine is initialized.
	//      
	// Output : No leave.   

    iStatesClient->SetTimeout( 1000000 );

    iStatesClient->SetVideoQualityLevelIndex( 1 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EGetVideoQualityIndexWhenEngineInitialized );
}

void CCaeTestRelease::TestGetVideoQualityIndexWhenPreparedL()
{
	// Group:   Video.
    // 
    // ID:      CAE.VR.GETVIDEOQUALITYINDEX.002
    //
    // Action : Get video quality index, when video recording is prepared.
	//      
	// Output : No leave.   

    iStatesClient->SetTimeout( 1000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EGetVideoQualityIndexWhenPrepared );
}

/********************************************************/
/*            REMAINING VIDEO RECORDING TIME            */
/********************************************************/

void CCaeTestRelease::TestRemainingTimeWhenVideoPreparedL()
{
	// Group:   Video.
    // 
    // ID:      CAE.VR.REMAININGVIDEORECORDINGTIME.001
    //
    // Action : Get RemainingVideoRecordingTime, when video recording is prepared.
	//      
	// Output : No leave.   

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERemainingTimeWhenVideoPrepared );
}

/********************************************************/
/*      PREPARE AUDIO BIT RATE AND RECORD VIDEO         */
/********************************************************/

void CCaeTestRelease::TestPrepareAudioBitRateVR5000L()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.001
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    iStatesClient->SetAudioBitRate( TInt( 5000 ) );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_5000.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


/************************************************************************/
/*   STILL AND STILL BURST CAPTURE - snap-image creation switched off   */
/************************************************************************/

void CCaeTestRelease::TestCaptureVga16M_NoSnapL()
    {
	// ID: CAE.SC.CAPTURESTILL.VGA16M.015
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing and capture VGA 16M  
    //          image when initialization is done. 
    //          Snap image creation switched off.
	//      
	// Output : OK.   

    iStatesClient->SetSnapImageCreation( EFalse );

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );

    // iStatesClient->SetStillFileName( _L("cae_vga16M_NoSnap.mbm") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureQualityLevel0_NoSnapL()
    {
	// ID: CAE.SC.CAPTURESTILL.JPEGVGA90.016
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture quality level 0 
    //          image when initialization is done and viewfinder is running.
    //          Snap image creation switched off.
	//      
	// Output : OK.   

    iStatesClient->SetSnapImageCreation( EFalse );

    iStatesClient->SetStillQualityLevelIndex( 0 );

    // iStatesClient->SetStillFileName( _L("cae_ql_0_vga90_NoSnap.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareCaptureStillStartAndStopVf );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureVgaExif_NoSnapL()
    {
	// ID: CAE.SC.CAPTURESTILL.VGA.EXIF.017
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture Vga Exif 
    //          image when initialization is done and viewfinder is running.
    //          Snap image creation switched off.
	//      
	// Output : OK.   

    iStatesClient->SetSnapImageCreation( EFalse );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );
    
    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    // iStatesClient->SetStillFileName( _L("cae_vga_exif_NoSnap.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareCaptureStillStartAndStopVfExif );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }


void CCaeTestRelease::TestCaptureStillBurst3_NoSnapL()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.3.007
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          images in a burst when initialization is done.
    //          Snap image creation switched off.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetSnapImageCreation( EFalse );

    iStatesClient->SetStillQualityLevelIndex( 0 );

    // iStatesClient->SetStillFileName( _L("cae_vga16M_burst3_NoSnap.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurst );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }


void CCaeTestRelease::TestSpecPrepCaptureStillBurst3_NoSnapL()
    {
	// ID: CAE.SC.CAPTURESTILLBURST.3.008
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          images in a burst when initialization is done.
    //          Preparation is done using explicitly specified image format parameters.
    //          Snap image creation switched off.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetSnapImageCreation( EFalse );

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

    TSize sizevga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizevga );

    // iStatesClient->SetStillFileName( _L("cae_qqvga16M_spburst3_NoSnap.mbm") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecPrepAndCaptureStillBurst );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }


void CCaeTestRelease::TestCaptureStillBurstExif3_NoSnapL()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTEXIF.3.004
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing burst mode and capture three  
    //          VGA Exif images in a burst when initialization is done.
    //          Snap image creation switched off.
	//      
	// Output : OK.   

    const TInt KStillBurstLength = 3;

    iStatesClient->SetSnapImageCreation( EFalse );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iStatesClient->SetStillFrameSize( sizeVga );
    
    iStatesClient->SetStillFormat( CCamera::EFormatExif );

    // iStatesClient->SetStillFileName( _L("cae_burst3_exif_NoSnap.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndCaptureStillBurstExif );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == KStillBurstLength );

    assertL( iStatesClient->CountStillBurstCaptureMoment() == KStillBurstLength );
    assertL( iStatesClient->CountStillBurstComplete() == 1 );
    }


void CCaeTestRelease::TestEnumerateCaptureSizesL()
    {
	// ID: CAE.GEN.ENUMERATE.001
    //
    // Group:   General
    // 
    // Action : Test to enumerate still capture sizes. Tests the largest image size only.
	//      
	// Output : OK.   

    iStatesClient->SetStillFormat( CCamera::EFormatFbsBitmapColor16M );

#if (MIRAGE_X_PROD1)
    TSize sizeMp1( KImgWidthMP1, KImgHeightMP1 );
    iStatesClient->SetStillFrameSize( sizeMp1 );
#elif (MIRAGE_X_PROD2) || (MIRAGE_X_PROD4) || (MIRAGE_X_PROD3)
    TSize sizeMp1M3( KImgWidthMP1M3, KImgHeightMP1M3 );
    iStatesClient->SetStillFrameSize( sizeMp1M3 );
#elif (MIRAGE_X_PROD7)
    TSize sizeMp2( KImgWidthMP2, KImgHeightMP2 );
    iStatesClient->SetStillFrameSize( sizeMp2 );
#elif (MIRAGE_X_PROD6) || (MIRAGE_X_PROD8) || (MIRAGE_X_PROD9)
    TSize sizeMp2( KImgWidthMP2, KImgHeightMP2 );
    iStatesClient->SetStillFrameSize( sizeMp2 );
#elif (HW_DEVLON)
    iStatesClient->SetStillFormat( CCamera::EFormatExif );
    TSize sizeMp3( KImgWidthMP3, KImgHeightMP3 );
    iStatesClient->SetStillFrameSize( sizeMp3 );
#else
    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );
    iStatesClient->SetStillFrameSize( sizeVga ); 
#endif

		// ignore errors if correct resolution was not found
    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::EEnumerateCaptureSizes ) );

		if ( err )
        {
        RDebug::Print(_L("CCaeTestRelease::TestEnumerateCaptureSizesL() failed = %d, ignored"), err);
        }
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 0 );
    assertL( iStatesClient->CountSnapImageReady() == 0 );
    assertL( iStatesClient->CountStillImageReady() == 0 );
    }


void CCaeTestRelease::TestSetJpegQuality50L()
    {
	// ID: CAE.SET-SC.SETJPEGQUALITY.001
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture quality level 0 
    //          image when initialization is done and viewfinder is running.
	//      
	// Output : OK.   

    iSettingsClient->SetStillFormat( CCamera::EFormatJpeg );

    iSettingsClient->SetStillCompressionQuality( 50 );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iSettingsClient->SetStillFrameSize( sizeVga );

    // iSettingsClient->SetStillFileName( _L("cae_jpeg_quality_setting_50.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestStatesClient::ESetJpegQualityWhenStillPrepared );
    
    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountStillPrepareComplete() == 1 );
    assertL( iSettingsClient->CountSnapImageReady() == 1 );
    assertL( iSettingsClient->CountStillImageReady() == 1 );
    }

void CCaeTestRelease::TestSCCustomInterfaceL()
    {
	// ID: CAE.GEN.CUSTOMINTERFACE.001
    //
    // Group:   General
    // 
    // Action : Test custom interface with illegal UID values -1, 0 and 1.
	//      
	// Output : Returns 0 with illegal UID values.   

    TUid interfaceUid;

    interfaceUid.iUid = -1;
    iStatesClient->SetCustomInterfaceUid( interfaceUid );
    iStatesClient->RunTestActionL( CCaeTestStatesClient::EGetCustomInterface );
    assertL( iStatesClient->CustomInterfacePointer() == 0 );

    interfaceUid.iUid = 0;
    iStatesClient->SetCustomInterfaceUid( interfaceUid );
    iStatesClient->RunTestActionL( CCaeTestStatesClient::EGetCustomInterface );
    assertL( iStatesClient->CustomInterfacePointer() == 0 );

    interfaceUid.iUid = 1;
    iStatesClient->SetCustomInterfaceUid( interfaceUid );
    iStatesClient->RunTestActionL( CCaeTestStatesClient::EGetCustomInterface );
    assertL( iStatesClient->CustomInterfacePointer() == 0 );
    }

/******************************/
/*        FLASH MODE          */
/******************************/

void CCaeTestRelease::TestFlashMode0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETFLASHMODEL.001
    //
    // Action : Test flash mode setting with still image capture and viewfinding.
	//      
	// Output : No leave.   

	const TInt KSetVal = 0;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_flash_mode_0.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenStillPrepared );
}

void CCaeTestRelease::TestFlashMode1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETFLASHMODEL.002
    //
    // Action : Test flash mode setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 1;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_flash_mode_1.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenStillPrepared ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }
	if ( KSetVal & info.iFlashModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestRelease::TestFlashMode2L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETFLASHMODEL.003
    //
    // Action : Test flash mode setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 2;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_flash_mode_2.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenStillPrepared ) );
    if( err == KErrNoMemory )
        {
        User::Leave( err ); // For memory test
        }
	if ( KSetVal & info.iFlashModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestRelease::TestSCColorToneNormalL()
    {
	// ID: CAE.SET-SC.COLORTONE.001
    //
    // Group:   General
    // 
    // Action : Test Color Tone interface with ENormal parameter.
	//      
	// Output : OK.   

    iStatesClient->SetColorToneValue( MCameraColorToneSettings::ENormal );
    iStatesClient->SetCustomInterfaceUid( KCameraColorToneSettingsUid );
    iStatesClient->SetSnapFileName( _L("cae_snap_colortone_normal1.mbm") );
    iStatesClient->SetStillFileName( _L("cae_colortone_normal.jpg") );
    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESetColorTone );
    assertL( iStatesClient->CustomInterfacePointer() != 0 );
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestRelease::TestSCColorToneSepiaL()
    {
	// ID: CAE.SET-SC.COLORTONE.002
    //
    // Group:   General
    // 
    // Action : Test Color Tone interface with ESepia parameter.
	//      
	// Output : OK.   

    iStatesClient->SetColorToneValue( MCameraColorToneSettings::ESepia );
    iStatesClient->SetCustomInterfaceUid( KCameraColorToneSettingsUid );
    iStatesClient->SetSnapFileName( _L("cae_snap_colortone_sepia.mbm") );
    iStatesClient->SetStillFileName( _L("cae_colortone_sepia.jpg") );
    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESetColorTone );
    assertL( iStatesClient->CustomInterfacePointer() != 0 );
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestRelease::TestSCColorToneGrayscaleL()
    {
	// ID: CAE.SET-SC.COLORTONE.003
    //
    // Group:   General
    // 
    // Action : Test Color Tone interface with EGrayscale parameter.
	//      
	// Output : OK.   

    iStatesClient->SetColorToneValue( MCameraColorToneSettings::EGrayscale );
    iStatesClient->SetCustomInterfaceUid( KCameraColorToneSettingsUid );
    iStatesClient->SetSnapFileName( _L("cae_snap_colortone_grayscale.mbm") );
    iStatesClient->SetStillFileName( _L("cae_colortone_grayscale.jpg") );
    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESetColorTone );
    assertL( iStatesClient->CustomInterfacePointer() != 0 );
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestRelease::TestSCColorToneNegativeL()
    {
	// ID: CAE.SET-SC.COLORTONE.004
    //
    // Group:   General
    // 
    // Action : Test Color Tone interface with ENegative parameter.
	//      
	// Output : OK.   

    iStatesClient->SetColorToneValue( MCameraColorToneSettings::ENegative );
    iStatesClient->SetCustomInterfaceUid( KCameraColorToneSettingsUid );
    iStatesClient->SetSnapFileName( _L("cae_snap_colortone_negative.mbm") );
    iStatesClient->SetStillFileName( _L("cae_colortone_negative.jpg") );
    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESetColorTone );
    assertL( iStatesClient->CustomInterfacePointer() != 0 );
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestRelease::TestVRColorToneL()
    {
	// ID: CAE.SET-VR.COLORTONE.005
    //
    // Group:   General
    // 
    // Action : Test Color Tone interface with video recording. Test case will start video recording
    //          and then sets every supported color tone with 5 second interval. Total execution time is 
    //          then with 4 color tones 20 seconds. Test case will report interface failures. Color tone 
    //          fuctionality must be checked manually from .3gp file.
	//      
	// Output : OK.   

    iStatesClient->SetVideoQualityLevelIndex( 0 );

#if (MIRAGE_X_PROD3)
    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video
#else
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
#endif

    iStatesClient->SetVideoFileName( _L("cae_colortone.3gp") );
    iStatesClient->SetTimeout( 5000000 ); // time for one color, then change
    
    // this walue is set before exiting
    iStatesClient->SetColorToneValue( MCameraColorToneSettings::ENormal );
    iStatesClient->SetCustomInterfaceUid( KCameraColorToneSettingsUid );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESetColorToneVideoRecording );

    assertL( iStatesClient->CustomInterfacePointer() != 0 );
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestRelease::TestSCColorToneWrongParams1L()
    {
	// ID: CAE.SET-SC.COLORTONE.006
    //
    // Group:   General
    // 
    // Action : Test Color Tone interface with illegal Color Tone parameter 2^31.
    //          Should leave with code KErrNotSupported.
	//      
	// Output : OK.   

    iStatesClient->SetColorToneValue( (MCameraColorToneSettings::TColorTone) 0x80000000 );
    iStatesClient->SetCustomInterfaceUid( KCameraColorToneSettingsUid );
    iStatesClient->SetSnapFileName( _L("cae_snap_colortone_wrong_params.mbm") );
    iStatesClient->SetStillFileName( _L("cae_colortone_wrong_params.jpg") );
    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESetColorToneWrongParams ) );
    if( error == KErrNoMemory )
        {
        User::Leave( error ); // For memory test
        }
    assertL( error == KErrNotSupported );
    }

void CCaeTestRelease::TestSCColorToneWrongParams2L()
    {
	// ID: CAE.SET-SC.COLORTONE.007
    //
    // Group:   General
    // 
    // Action : Test Color Tone interface with illegal Color Tone parameter 0x07.
    //          Should leave with code KErrArgument.
	//      
	// Output : OK.   

    iStatesClient->SetColorToneValue( (MCameraColorToneSettings::TColorTone) 0x07 );
    iStatesClient->SetCustomInterfaceUid( KCameraColorToneSettingsUid );
    iStatesClient->SetSnapFileName( _L("cae_snap_colortone_wrong_params.mbm") );
    iStatesClient->SetStillFileName( _L("cae_colortone_wrong_params.jpg") );
    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESetColorToneWrongParams ) );
    if( error == KErrNoMemory )
        {
        User::Leave( error ); // For memory test
        }
    assertL( error == KErrArgument );
    }

void CCaeTestRelease::TestSCColorToneReleaseL()
    {
	// ID: CAE.SET-SC.COLORTONE.008
    //
    // Group:   General
    // 
    // Action : Test Color Tone interface with ENegative parameter when
    //          Release() and Reserve() has been used. After Reserve() the default 
    //          values should be set (ENormal) by CameraAPI
	//      
	// Output : OK.   

    iStatesClient->SetColorToneValue( MCameraColorToneSettings::ENegative );
    iStatesClient->SetCustomInterfaceUid( KCameraColorToneSettingsUid );
    iStatesClient->SetSnapFileName( _L("cae_snap_colortone_release.mbm") );
    iStatesClient->SetStillFileName( _L("cae_colortone_release.jpg") );
    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESetColorToneRelease );
    assertL( iStatesClient->CustomInterfacePointer() != 0 );
    assertL( iStatesClient->CountInitComplete() == 2 ); // reserve() calls once McaeoInitComplete
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

/********************************************************/
/*           DISPLAY INDEX                              */
/********************************************************/

void CCaeTestRelease::TestSetDisplayIndexL()
    {
	// ID: CAE.SC.SETDISPLAYINDEX.001
    //
    // Group:   Capture
    // 
    // Action : Set display index to 1 and capture a still image.
	//      
	// Output : No errors. Snap image should be enhanced.  

    iStatesClient->SetDisplayIndex( 1 );
    iStatesClient->SetSnapFileName( _L("cae_snap_disp_index_1.mbm") );
    //iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill );
    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESetDisplayIndex );
    }

/********************************************************/
/*             VIDEO RECORDING ENUMERATE FRAME SIZES    */
/********************************************************/

void CCaeTestRelease::TestEnumerateVideoFrameSize001L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.ENUMERATEVIDEOFRAMESIZE.001
    //
    // Action : Test that enumerate can be called with correct and incorrect frame sizes
	//      
	// Output : No leave.   

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EEnumerateVideoFrameSize );
}


/********************************************************/
/*             Extension mode test cases                */
/********************************************************/

void CCaeTestRelease::TestProcessExtViewFinderFrameReadyL()
{
	// Group:   still capture/extension mode.
    // 
    // ID:      CAE.SC.ProcessExtViewFinderFrameReadyL.001
    //
    // Action : Test extension mode ProcessExtViewFinderFrameReadyL normal operation
	//      
	// Output : No leave.   

    TSize sizeQqvga( KImgWidthQQVGA, KImgHeightQQVGA );
    iSettingsClient->SetViewFinderSize( sizeQqvga );
	iSettingsClient->SetRunningWithViewFinder( ETrue );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EProcessExtViewFinderFrameReady );
}

void CCaeTestRelease::TestProcessExtSnapImageL()
{
	// Group:   still capture/extension mode.
    // 
    // ID:      CAE.SC.ProcessExtSnapImageL.001
    //
    // Action : Test extension mode ProcessExtSnapImageL normal operation
	//      
	// Output : No leave.   

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EProcessExtSnapImage );
}

void CCaeTestRelease::TestProcessExtCapturedImageL()
{
	// Group:   still capture/extension mode.
    // 
    // ID:      CAE.SC.ProcessExtCapturedImageL.001
    //
    // Action : Test extension mode ProcessExtCapturedImageL normal operation with HBufC8 param 
	//      
	// Output : No leave.   

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EProcessExtCapturedImage );
}


void CCaeTestRelease::TestProcessExtCapturedImage2L()
{
	// Group:   still capture/extension mode.
    // 
    // ID:      CAE.SC.ProcessExtCapturedImageL.002
    //
    // Action : Test extension mode ProcessExtCapturedImageL normal operation with TDesC8 param
    //          Test that Relese() can be called
	//      
	// Output : No leave.   

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EProcessExtCapturedImageTDesC8 );
}

void CCaeTestRelease::TestProcessExtCapturedImage3L()
{
	// Group:   still capture/extension mode.
    // 
    // ID:      CAE.SC.ProcessExtCapturedImageL.003
    //
    // Action : Test extension mode ProcessExtCapturedImageL normal operation with CFbsBitmap param
	//      
	// Output : No leave.   

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EProcessExtCapturedImageCFbsBitmap );
}

void CCaeTestRelease::TestProcessExtCapturedImage4L()
{
	// Group:   still capture/extension mode.
    // 
    // ID:      CAE.SC.ProcessExtCapturedImageL.004
    //
    // Action : Test extension mode ProcessExtCapturedImageL normal operation with HBufC8 param 
	//      
	// Output : No leave.   

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EProcessExtCapturedImageNoInit );
}

void CCaeTestRelease::TestProcessExtCancelL()
{
	// Group:   still capture/extension mode.
    // 
    // ID:      CAE.SC.ProcessExtCancel.001
    //
    // Action : Test extension mode ProcessExtCancel call before extension interface is used
	//      
	// Output : No leave.   

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EProcessExtCancel );
}


void CCaeTestRelease::TestProcessExtCancel2L()
{
	// Group:   still capture/extension mode.
    // 
    // ID:      CAE.SC.ProcessExtCancel.002
    //
    // Action : Test extension mode ProcessExtCancel normal operation. Cancell is called when extension
    //          processing is in progress
	//      
	// Output : No leave.   

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EProcessExtCancel2 );
}

void CCaeTestRelease::TestProcessExtCapturedImageBurstL()
{
	// Group:   still capture/extension mode.
    // 
    // ID:      CAE.SC.ProcessExtCapturedImageBurstL.001
    //
    // Action : Test extension mode ProcessExtCapturedImageL normal burst operation with HBufC8 param
    //          One image is captured and then it is sent as 6 burst images to extension handling.
    //          Also snap images are sent 6 times.
	//      
	// Output : No leave.   

    iSettingsClient->SetStillBurstLength( 6 );
    iSettingsClient->SetStillFormat( CCamera::EFormatExif );
    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EProcessExtCapturedImageBurst );
    assertL( iSettingsClient->CountStillImageReady() == 6 );
    assertL( iSettingsClient->CountSnapImageReady() == 6 );
    assertL( iSettingsClient->CountStillBurstComplete() == 1 );
}

void CCaeTestRelease::TestProcessExtCapturedImageBurst2L()
{
	// Group:   still capture/extension mode.
    // 
    // ID:      CAE.SC.ProcessExtCapturedImageBurstL.002
    //
    // Action : Test extension mode ProcessExtCapturedImageL normal burst operation with TDesC8 param 
    //          One image is captured and then it is sent as 6 burst images to extension handling
    //          Also snap images are sent 6 times.
	//      
	// Output : No leave.   

    iSettingsClient->SetStillBurstLength( 6 );
    iSettingsClient->SetStillFormat( CCamera::EFormatExif );
    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EProcessExtCapturedImageBurstTDesC8 );
    assertL( iSettingsClient->CountSnapImageReady() == 6 );
    assertL( iSettingsClient->CountStillImageReady() == 6 );
    assertL( iSettingsClient->CountStillBurstComplete() == 1 );
}

void CCaeTestRelease::TestInstalledExtensionListL()
{
	// Group:   still capture/extension mode.
    // 
    // ID:      CAE.SC.InstalledExtensionList.001
    //
    // Action : Test extension mode InstalledExtensionList normal operation
	//      
	// Output : No leave.   

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EInstalledExtensionList );
}

void CCaeTestRelease::TestSkippedExtensionListL()
{
	// Group:   still capture/extension mode.
    // 
    // ID:      CAE.SC.SkippedExtensionListL.001
    //
    // Action : Test extension mode SkippedExtensionList normal operation
	//      
	// Output : No leave.   

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESkippedExtensionList );
}

void CCaeTestRelease::TestSetSkippedExtensionListL()
{
	// Group:   still capture/extension mode.
    // 
    // ID:      CAE.SC.SetSkippedExtensionList.001
    //
    // Action : Test extension mode SetSkippedExtensionList normal operation
	//      
	// Output : No leave.   

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetSkippedExtensionList );
}


void CCaeTestRelease::TestStartAndStopVideoRecordingSubQcifNoInitL()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.SUBQCIF.NOINIT.001
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording subQCIF sized video works. 
    //          Video recording prepared for subQCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );

    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoFileName( _L("cae_subqcif_noinit.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideoNoInit );

    assertL( iStatesClient->CountInitComplete() == 0 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }



void CCaeTestRelease::TestSecondaryCamera001L()
    {
	// ID: CAE.SC.SecondaryCamera.001
    //
    // Group:   Capture
    // 
    // Action : Test to setup still capturing for secondary camera and capture Jpeg 
    //          image when initialization is done.
	//      
    //       ** For PROD9 320x240 camera **
    //
	// Output : OK.   

    TSize sizeQVGA( KImgWidthQVGA, KImgHeightQVGA );

    iStatesClient->SetStillFormat( CCamera::EFormatJpeg );
    iStatesClient->SetStillCompressionQuality( 80 );
    iStatesClient->SetStillFrameSize( sizeQVGA ); // only supported size
    iStatesClient->SetSnapImageSize( TSize( 240, 180 ) ); // no action, cae returns qvga if no adis
    iStatesClient->SetSnapImageSource( CCaeEngine::ESnapImageSourceThumbnail ); // no action as C-API returns bitmap
    iStatesClient->SetStillFileName( _L("cae_jpegsecond_001.jpg") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndCaptureStill2nd );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestRelease::TestRecordAsyncStop001L()
    {
	// ID:      CAE.VR.VIDEORECORDING.AsyncStop.001
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 0 
    //          video clip when async stop is enabled.
	//      
	// Output : No leave.

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetViewFinderSize( sizeSubQcif );
    
    iStatesClient->SetVfFileName( _L("cae_video_asyncstop_001.mbm") ); 

    iStatesClient->SetVideoStopMode( ETrue ); 

    iStatesClient->SetTimeout( 30000000 );

    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoFileName( _L("cae_video_asyncstop_001.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareRecordVideoStartAndStopVf );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingStopped() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestRelease::TestRecordAsyncStop002L()
    {
	// ID:      CAE.VR.VIDEORECORDING.AsyncStop.002
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 0 
    //          video clip when async stop is disabled.
	//      
	// Output : No leave.

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetViewFinderSize( sizeSubQcif );
    
    iStatesClient->SetVfFileName( _L("cae_video_asyncstop_001.mbm") ); 

    iStatesClient->SetVideoStopMode( EFalse ); 

    iStatesClient->SetTimeout( 30000000 );

    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoFileName( _L("cae_video_asyncstop_001.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVfPrepareRecordVideoStartAndStopVf );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingStopped() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 

#if 1
    
MTest* CCaeTestRelease::suiteL ()
    {
	// Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL( _L8("CCaeTestRelease") );

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.INITL.001"), &TestInitL ) );
    
#if VIDEO_RECORDING_TEST_CASES
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.INITVIDEORECORDERL.001"), &TestInitVideoRecorderL ) );
#endif
    
#if (!MIRAGE_X) && (!UNAGI_2M) 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.GETINFO.001"), &TestGetInfoL ) );
#endif

#if MIRAGE_X_PROD1 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.GETINFO.002"), &TestGetInfoPROD1MirageXL ) );
#endif
    
#if MIRAGE_X_PROD2 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.GETINFO.003"), &TestGetInfoPROD2MirageXL ) );
#endif
    
#if MIRAGE_X_PROD3 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.GETINFO.004"), &TestGetInfoPROD3MirageXPL ) );
#endif
    
#if MIRAGE_X_PROD4 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.GETINFO.005"), &TestGetInfoPROD4MirageXL ) );
#endif

#if MIRAGE_X_PROD6 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.GETINFO.006"), &TestGetInfoPROD6MirageXL ) );
#endif

#if MIRAGE_X_PROD7 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.GETINFO.007"), &TestGetInfoPROD7Unagi2ML ) );
#endif

#if MIRAGE_X_PROD8 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.GETINFO.008"), &TestGetInfoPROD8MirageXL ) );
#endif

#if MIRAGE_X_PROD9 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.GETINFO.009"), &TestGetInfoPROD9HWL ) );
#endif

#if HW_DEVLON 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.GETINFO.010"), &TestGetInfoDevlonHWL ) );
#endif

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.POWEROFFANDON.001"), &TestPowerOffAndOnL ) );
     
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.RELESEANDRESERVE.001"), &TestReleaseAndReserveL ) );
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN-SC.POWERON.001"), &TestPowerOnWhenInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN-SC.POWERON.002"), &TestPowerOnWhenStillCapturePreparedL ) );

#if VIDEO_RECORDING_TEST_CASES
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN-VR.POWERON.003"), &TestPowerOnWhenVideoRecordingPreparedL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN-VR.POWERON.004"), &TestPowerOnWhenRecordingL ) );
#endif
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN-SC.POWEROFF.001"), &TestPowerOffWhenStillCapturePreparedL ) );    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN-SC.POWEROFF.002"), &TestPowerOffWhenViewfinderRunningL ) );

#if VIDEO_RECORDING_TEST_CASES
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN-VR.POWEROFF.003"), &TestPowerOffWhenVideoRecordingPreparedL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN-VR.POWEROFF.004"), &TestPowerOffWhenRecordingL ) );
#endif

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VF.STARTSTOPVIEWFINDER.QQVGA.001"), &TestViewFinderStartAndStopQqvgaL ) );
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VF.STARTSTOPVIEWFINDER.SUBQCIF.002"), &TestViewFinderStartAndStopSubqcifL ) );
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VF.STARTSTOPVIEWFINDER.CROPPED.003"), &TestCroppedViewFinderStartAndStopL ) );
#if !( (HW_DEVLON) )
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VF.STARTSTOPVIEWFINDERDIRECT.001"), &TestViewFinderDirectStartAndStopL ) );
#endif
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VF.STARTSTOPVIEWFINDERDIRECT.002"), &TestCroppedViewFinderDirectStartAndStopL ) );
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.001"), &TestCaptureStillWhenEngineInitializedL ) );
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.VGA16M.002"), &TestCaptureVga16ML ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.VGA64K.003"), &TestCaptureVga64KL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.VGA4K.004"), &TestCaptureVga4KL ) );

#if !( (MIRAGE_X_PROD9) || (HW_DEVLON) )
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.QQVGA16M.005"), &TestCaptureQqvga16ML ) ); 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.QQVGA64K.006"), &TestCaptureQqvga64KL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.QQVGA4K.007"), &TestCaptureQqvga4KL ) );
#endif
    
#if MIRAGE_X_PROD1 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.MP1.16M.011"), &TestCaptureMp1_16ML ) );

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.MP1.64K.012"), &TestCaptureMp1_64KL ) );

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.MP1.4K.013"), &TestCaptureMp1_4KL ) );

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.JPEGMP1.014"), &TestCaptureJpegMp1L ) );    

#endif

#if (MIRAGE_X_PROD2) || (MIRAGE_X_PROD4) || (MIRAGE_X_PROD3)
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.MP1M3.16M.018"), &TestCaptureMp1M3_16ML ) );

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.MP1M3.64K.019"), &TestCaptureMp1M3_64KL ) );

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.MP1M3.4K.020"), &TestCaptureMp1M3_4KL ) );

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.JPEGMP1M3.021"), &TestCaptureJpegMp1M3L ) );    

#endif // (MIRAGE_X_PROD2) || (MIRAGE_X_PROD4) || (MIRAGE_X_PROD3)

#if MIRAGE_X_PROD7
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.MP2.16M.025"), &TestCaptureMp2_16ML ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.MP2.64K.026"), &TestCaptureMp2_64KL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.MP2.4K.027"), &TestCaptureMp2_4KL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.JPEG.MP2.028"), &TestCaptureJpegMp2L ) );    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.EXIF.MP2.029"), &TestCaptureExifMp2L ) );    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.EXIF.SVGA.030"), &TestCaptureExifSVGAL ) );    
#endif // MIRAGE_X_PROD7

#if MIRAGE_X_PROD8
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.CODEC.031"), &TestCaptureCodecL ) );    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.CODEC.032"), &TestCaptureCodec2L ) );
#endif // MIRAGE_X_PROD8
        
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.JPEGVGA90.008"), &TestCaptureQualityLevel0L ) );
   
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.JPEGVGA50.009"), &TestCaptureQualityLevel1L ) );

#if !( (MIRAGE_X_PROD9) || (HW_DEVLON) )
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.JPEGQQVGA90.010"), &TestCaptureQualityLevel2L ) );
#endif

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURECAPTURE.001"), &TestCaptureCaptureL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURECAPTURE.002"), &TestCaptureCaptureSpecPrepL ) );
    
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.PREPARESTILLCAPTUREL.001"), &TestPrepareStillCapture0L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.PREPARESTILLCAPTUREL.002"), &TestPrepareStillCapture1L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.PREPARESTILLCAPTUREL.003"), &TestPrepareStillCapture2L ) );    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.PREPARESTILLCAPTUREL.004"), &TestPrepareStillCapture0WhenPreparedL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.PREPARESTILLCAPTUREL.007"), &TestSpecPrepareSCSupportedFormatWhenInitializedL ) );
    
#if VIDEO_RECORDING_TEST_CASES
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.001"), &TestPrepareVideoRecording0L ) );

#if !( (HW_DEVLON) )
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.002"), &TestPrepareVideoRecording1L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.003"), &TestPrepareVideoRecording2L ) );    
#endif
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.004"), &TestPrepareVideoRecording0WhenPreparedL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.007"), &TestSpecPrepareVRSupportedFormatWhenVRInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.VIDEORECORDING.QCIF64000FR15.001"), &TestRecordQualityLevel0L ) );  

#if !( (HW_DEVLON) )
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.VIDEORECORDING.SUBQCIF60000FR15.002"), &TestRecordQualityLevel1L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.VIDEORECORDING.SUBQCIF40000FR5.003"), &TestRecordQualityLevel2L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.SUBQCIF.001"), &TestStartAndStopVideoRecordingSubQcifL ) );
#endif
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.QCIF.002"), &TestStartAndStopVideoRecordingQcifL ) );
#endif
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETZOOMVALUEL.001"), &TestSCDigiZoomVal0L ) );    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETZOOMVALUEL.002_vga"), &TestSCDigiZoomVal1VgaL ) );
#if !( (MIRAGE_X_PROD9) || (HW_DEVLON) )
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETZOOMVALUEL.002_qqvga"), &TestSCDigiZoomVal1QqvgaL ) );
#endif
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETZOOMVALUEL.005"), &TestSCDigiZoomInitializedVal0L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETZOOMVALUEL.012_vga"), &TestSCDigiZoomInitializedVal1VgaL ) );

#if !( (MIRAGE_X_PROD9) || (HW_DEVLON) )
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETZOOMVALUEL.012_qqvga"), &TestSCDigiZoomInitializedVal1QqvgaL ) );
#endif

#if VIDEO_RECORDING_TEST_CASES
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETZOOMVALUEL.006"), &TestVRDigiZoomVal0L ) );    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETZOOMVALUEL.007"), &TestVRDigiZoomVal1L ) );    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETZOOMVALUEL.010"), &TestVRDigiZoomRecordingVal0L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETZOOMVALUEL.011"), &TestVRDigiZoomRecordingVal1L ) );

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.RESUMEVIDEORECORDING.001"), &TestRecordPauseResumeStopL ) );
#endif
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETEXPOSUREMODEL.001"), &TestSCExposureMode0L ) );

#if !MIRAGE_X_PROD6 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETEXPOSUREMODEL.002"), &TestSCExposureMode1L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETEXPOSUREMODEL.016"), &TestSCExposureModeInitialized1L ) );
#endif
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETEXPOSUREMODEL.015"), &TestSCExposureModeInitialized0L ) );

#if VIDEO_RECORDING_TEST_CASES
#if !MIRAGE_X_PROD6 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETEXPOSUREMODEL.008"), &TestVRExposureMode0L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETEXPOSUREMODEL.009"), &TestVRExposureMode1L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETEXPOSUREMODEL.018"), &TestVRExposureModeRecording1L ) );
#endif
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETEXPOSUREMODEL.017"), &TestVRExposureModeRecording0L ) );
#endif

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETEVCOMP.001"), &TestSCEvCompensation0L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETEVCOMP.002"), &TestSCEvCompensationMaxL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETEVCOMP.003"), &TestSCEvCompensationMinL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETEVCOMP.004"), &TestSCEvCompensationInitialized0L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETEVCOMP.005"), &TestSCEvCompensationInitializedMaxL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETEVCOMP.006"), &TestSCEvCompensationInitializedMinL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETEVCOMP.007"), &TestSCEvCompensationInitializedInvalidL ) );

#if VIDEO_RECORDING_TEST_CASES
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETEVCOMP.001"), &TestVREvCompensationMinL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETEVCOMP.002"), &TestVREvCompensationRecordingMaxL ) );
#endif

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETWHITEBALANCEMODEL.001"), &TestSCWhiteBalanceMode0L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETWHITEBALANCEMODEL.019"), &TestSCWhiteBalanceModeInitialized0L ) );

#if VIDEO_RECORDING_TEST_CASES
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETWHITEBALANCEMODEL.010"), &TestVRWhiteBalanceMode0L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETWHITEBALANCEMODEL.020"), &TestVRWhiteBalanceModeRecording0L ) );
#endif

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETBRIGHTNESSL.001"), &TestSCBrightness50L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETBRIGHTNESSL.007"), &TestSCBrightnessInitialized50L ) );

#if VIDEO_RECORDING_TEST_CASES
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETBRIGHTNESSL.008"), &TestVRBrightness50L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETBRIGHTNESSL.014"), &TestVRBrightnessRecording50L ) );
#endif

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETCONTRASTL.001"), &TestSCContrast50L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETCONTRASTL.007"), &TestSCContrastInitialized50L ) );

#if VIDEO_RECORDING_TEST_CASES
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETCONTRASTL.008"), &TestVRContrast50L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETCONTRASTL.014"), &TestVRContrastRecording50L ) );
	
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.SETVIDEOCLIPMAXSIZEL.002"), &TestVideoClipMaxSize10000L ) );
#endif
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CANCELCAPTURESTILL.001"), &TestCancelCaptureStillL ) );

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.GETSTILLQUALITYINDEX.001"), &TestGetStillQualityIndexWhenEngineInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.GETSTILLQUALITYINDEX.002"), &TestGetStillQualityIndexWhenPreparedL ) );
    
#if VIDEO_RECORDING_TEST_CASES
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.GETVIDEOQUALITYINDEX.001"), &TestGetVideoQualityIndexWhenEngineInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.GETVIDEOQUALITYINDEX.002"), &TestGetVideoQualityIndexWhenPreparedL ) );
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.REMAININGVIDEORECORDINGTIME.001"), &TestRemainingTimeWhenVideoPreparedL ) );
    
//    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.STARTSTOPVRTOOBIGSIZE.SUBQCIF.001"), &TestStartAndStopVRWhenTooBigSizeSubQcifL ) );
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.001"), &TestPrepareAudioBitRateVR5000L ) );

#if (MIRAGE_X_PROD7)
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.002"), &TestPrepareAudioBitRateVR128L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.003"), &TestPrepareAudioBitRateVR128InvVtypeL ) );
#endif

#endif

#ifdef __CAMERA_BURST_MODE
    // Only if Still BURST is supported (built in).
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.3.001"), &TestCaptureStillBurst3L ) );   
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.0.002"), &TestCaptureStillBurst0L ) );

//    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.100.003"), &TestCaptureStillBurst100L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.CANCEL.004"), &TestCancelCaptureStillBurst3L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.STOP2.005"), &TestStopCaptureStillBurstAfter2L ) );
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.3.006"), &TestSpecPrepCaptureStillBurst3L ) );
    
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTINTERVAL.0.001"), &TestCaptureStillBurstInterval0L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTINTERVAL.1000000.002"), &TestCaptureStillBurstInterval1000000L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTINTERVAL.M1.003"), &TestCaptureStillBurstIntervalM1L ) );
#endif


#if (MIRAGE_X_PROD2) || (MIRAGE_X_PROD4) || (MIRAGE_X_PROD3) || (MIRAGE_X_PROD7)

#ifdef __CAMERA_BURST_MODE
	// Exif burst test
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTEXIF.3.001"), &TestCaptureStillBurstExif3L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTEXIF.6.002"), &TestCaptureStillBurstExif6L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTEXIF.100.003"), &TestCaptureStillBurstExif100L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTEXIF.6.VF.004"), &TestCaptureStillBurstExif6VfL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTEXIF.6.VF.CANCEL.005"), &TestCaptureStillBurstExif6VfCancelL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTEXIF.6.006"), &TestCaptureStillBurstExif6NSL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTEXIF.6.007"), &TestCaptureStillBurstExif6_ThumbnailL ) );
#endif

	// Exif tests
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.VGA.EXIF.001"), &TestCaptureVgaExifL ) );	
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.QQVGA.EXIF.002"), &TestCaptureQqvgaExifL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.VGA.EXIF.003"), &TestCaptureVgaSnapOneHalfExifL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.VGA.EXIF.004"), &TestCaptureVgaSnapOneFourthExifL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.VGA.EXIF.005"), &TestCaptureVgaSnapOneEighthExifL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.VGA.EXIF.006"), &TestCaptureVgaSnapExifL ) );

    // Jpeg tests
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.VGA.JPEG.007"), &TestCaptureVgaSnapOneEighthJpegL ) );

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.VGA.EXIF.008"), &TestCaptureVgaSnapOneEighthExif2L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.VGA.EXIF.009"), &TestCaptureVgaSnapColor16MExifL ) );
#endif // (MIRAGE_X_PROD2) || (MIRAGE_X_PROD4) || (MIRAGE_X_PROD3) || (MIRAGE_X_PROD7)

    // Test cases for snap-image creation switched off.
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.VGA16M.015"), &TestCaptureVga16M_NoSnapL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.JPEGVGA90.016"), &TestCaptureQualityLevel0_NoSnapL ) );
#if (MIRAGE_X_PROD2) || (MIRAGE_X_PROD4) || (MIRAGE_X_PROD3) || (MIRAGE_X_PROD7)
    // Exif tests
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.VGA.EXIF.017"), &TestCaptureVgaExif_NoSnapL ) );	
#endif

#ifdef __CAMERA_BURST_MODE
    // Only if Still BURST is supported (built in).
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.3.007"), &TestCaptureStillBurst3_NoSnapL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURST.3.008"), &TestSpecPrepCaptureStillBurst3_NoSnapL ) );

#if (MIRAGE_X_PROD2) || (MIRAGE_X_PROD4) || (MIRAGE_X_PROD3) || (MIRAGE_X_PROD7)
    // Exif tests
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTEXIF.3.004"), &TestCaptureStillBurstExif3_NoSnapL ) );
#endif
#endif // __CAMERA_BURST_MODE

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.ENUMERATE.001"), &TestEnumerateCaptureSizesL ) );

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETJPEGQUALITY.001"), &TestSetJpegQuality50L ) );

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.CUSTOMINTERFACE.001"), &TestSCCustomInterfaceL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETFLASHMODEL.001"), &TestFlashMode0L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETFLASHMODEL.002"), &TestFlashMode1L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.SETFLASHMODEL.003"), &TestFlashMode2L ) );

#if COLOR_TONE_TEST_CASES
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.COLORTONE.001"), &TestSCColorToneNormalL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.COLORTONE.002"), &TestSCColorToneSepiaL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.COLORTONE.003"), &TestSCColorToneGrayscaleL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.COLORTONE.004"), &TestSCColorToneNegativeL ) );
#if VIDEO_RECORDING_TEST_CASES
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.COLORTONE.005"), &TestVRColorToneL ) );
#endif
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.COLORTONE.006"), &TestSCColorToneWrongParams1L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.COLORTONE.007"), &TestSCColorToneWrongParams2L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-SC.COLORTONE.008"), &TestSCColorToneReleaseL ) );
#endif // COLOR_TONE_TEST_CASES

//#if (MIRAGE_X_PROD7)
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.SETDISPLAYINDEX.001"), &TestSetDisplayIndexL ) );
//#endif
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.CAPTURESTILL.POWEROFF.001"), &TestCaptureAfterPowerOffL ) );

    // new tests after 3.2
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SET-VR.ENUMERATEVIDEOFRAMESIZE.001"), &TestEnumerateVideoFrameSize001L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.GEN.CCAMERAHANDLE.001"), &TestCCameraHandleL ) );

    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.ProcessExtViewFinderFrameReadyL.001"), &TestProcessExtViewFinderFrameReadyL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.ProcessExtSnapImageL.001"), &TestProcessExtSnapImageL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.ProcessExtCapturedImageL.001"), &TestProcessExtCapturedImageL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.ProcessExtCapturedImageL.002"), &TestProcessExtCapturedImage2L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.ProcessExtCapturedImageL.003"), &TestProcessExtCapturedImage3L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.ProcessExtCapturedImageL.004"), &TestProcessExtCapturedImage4L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.ProcessExtCancel.001"), &TestProcessExtCancelL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.ProcessExtCancel.002"), &TestProcessExtCancel2L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.ProcessExtCapturedImageBurstL.001"), &TestProcessExtCapturedImageBurstL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.ProcessExtCapturedImageBurstL.002"), &TestProcessExtCapturedImageBurst2L ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.InstalledExtensionList.001"), &TestInstalledExtensionListL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.SkippedExtensionListL.001"), &TestSkippedExtensionListL ) );
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.SetSkippedExtensionList.001"), &TestSetSkippedExtensionListL ) );

#if VIDEO_RECORDING_TEST_CASES
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.SUBQCIF.NOINIT.001"), &TestStartAndStopVideoRecordingSubQcifNoInitL ) );
#endif

#if ( ((MIRAGE_X_PROD9) || (HW_DEVLON)) && (SECONDARY_CAMERA_TEST_CASES) ) 
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.SC.SecondaryCamera.001"), &TestSecondaryCamera001L ) );
#endif

    // Asynchronous video stop test cases
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.VIDEORECORDING.AsyncStop.001"), &TestRecordAsyncStop001L ) );  
    suite->addTestL( CTestCaller<CCaeTestRelease>::NewL( _L8("CAE.VR.VIDEORECORDING.AsyncStop.002"), &TestRecordAsyncStop002L ) );  

    return suite;
    }

#else


MTest* CCaeTestRelease::suiteL ()
    {
	// Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL( _L8("CCaeTestRelease") );

    
    return suite;
    }

#endif
