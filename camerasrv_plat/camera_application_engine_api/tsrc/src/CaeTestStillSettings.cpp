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
* Description: CaeTestStillSettings.cpp
*
*/

#include "CaeTestStillSettings.h"
#include <f32file.h>


CCaeTestStillSettings::CCaeTestStillSettings ()
    {
    }


CCaeTestStillSettings::~CCaeTestStillSettings ()
    {
    }


void CCaeTestStillSettings::setUpL ()
    {
	iSettingsClient = CCaeTestSettingsClient::NewL( );

    iSettingsClient->SetFileNameBase( KFileNameBase );
    }


void CCaeTestStillSettings::tearDown ()
    {
    delete iSettingsClient;    
    iSettingsClient = 0;
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


/////////////////////////////////////////////////////////////////////
//                       Capture tests
/////////////////////////////////////////////////////////////////////

/******************************/
/*    DIGITAL ZOOM VALUE      */
/******************************/

void CCaeTestStillSettings::TestDigiZoomVal2L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETZOOMVALUEL.003
    // 
    // Action : Test out-of-bounds digital zoom value.
	//      
	// Output : Leave   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );
	
    iSettingsClient->SetDigiZoomValue( info.iMaxDigitalZoom + 1 );

    //iSettingsClient->SetStillFileName( _L("cae_digizoom_val_2.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenStillPrepared ) );
    assertL( err != KErrNone );
}

void CCaeTestStillSettings::TestDigiZoomValNeg1L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETZOOMVALUEL.004
    // 
    // Action : Test digital zoom.
	//      
	// Output : Leave   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetDigiZoomValue( -1 );

    //iSettingsClient->SetStillFileName( _L("cae_digizoom_val_-1.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenStillPrepared ) );
    assertL( err != KErrNone );
}

void CCaeTestStillSettings::TestOpticalZoomValMinL()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETZOOMVALUEL.005
    // 
    // Action : Test out-of-bounds optical zoom value.
	//      
	// Output : Leave   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetZoomMode( 1 );

    TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );
	
    iSettingsClient->SetDigiZoomValue( -1 );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenStillPrepared ) );
    assertL( err != KErrNone );
}

void CCaeTestStillSettings::TestOpticalZoomValMaxL()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETZOOMVALUEL.006
    // 
    // Action : Test out-of-bounds optical zoom value.
	//      
	// Output : Leave   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetZoomMode( 1 );

    TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );
	
    iSettingsClient->SetDigiZoomValue( 99999 );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenStillPrepared ) );
    assertL( err != KErrNone );
}

/******************************/
/*    DIGITAL ZOOM MODE      */
/******************************/

void CCaeTestStillSettings::TestDigiZoomMode0L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETZOOMMODEL.001
    //
    // Action : Test digital zoom mode.
	//      
	// Output : OK.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetZoomMode( 0 );

    //iSettingsClient->SetStillFileName( _L("cae_digizoom_mode_0.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenStillPrepared ); 
}

void CCaeTestStillSettings::TestOpticalZoomMode1L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETZOOMMODEL.002
    //
    // Action : Test optical zoom mode.
	//      
	// Output : Leave   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetZoomMode( 1 );

    //iSettingsClient->SetStillFileName( _L("cae_digizoom_mode_1.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenStillPrepared ) );
    assertL( err != KErrNone );//verify if is correct action
}

void CCaeTestStillSettings::TestOpticalDigiZoomMode2L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETZOOMMODEL.003
    //
    // Action : Test optical digital zoom mode.
	//      
	// Output : Leave   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetZoomMode( 2 );

    //iSettingsClient->SetStillFileName( _L("cae_digizoom_mode_2.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenStillPrepared ) );
    assertL( err != KErrNone );//verify if is correct action
}

void CCaeTestStillSettings::TestDigiZoomModeNeg1L()
    {
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETZOOMMODEL.004
    //
    // Action : Test digital zoom mode.
	//      
	// Output : Leave   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetZoomMode( -1 );

    //iSettingsClient->SetStillFileName( _L("cae_digizoom_mode_-1.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenStillPrepared ) );
    assertL( err != KErrNone );
    }


/******************************/
/*       BRIGHTNESS           */
/******************************/

void CCaeTestStillSettings::TestBrightness0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETBRIGHTNESSL.002
    //
    // Action : Test brightness setting with still image capture and viewfinding.
	//      
	// Output : No leave if supported. Leave if not supported.  

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetBrightness( 0 );

    iSettingsClient->SetStillFileName( _L("cae_brightness_0.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenStillPrepared ) );
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestBrightness100L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETBRIGHTNESSL.003
    //
    // Action : Test brightness setting with still image capture and viewfinding.
	//      
	// Output : No leave if supported. Leave if not supported.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetBrightness( 100 );

    iSettingsClient->SetStillFileName( _L("cae_brightness_100.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenStillPrepared ) );
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestBrightnessNeg100L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETBRIGHTNESSL.004
    //
    // Action : Test brightness setting with still image capture and viewfinding.
	//      
	// Output : No leave if supported. Leave if not supported.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetBrightness( -100 );

    iSettingsClient->SetStillFileName( _L("cae_brightness_-100.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenStillPrepared ) );
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestBrightness101L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETBRIGHTNESSL.005
    //
    // Action : Test brightness setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetBrightness( 101 );

    //iSettingsClient->SetStillFileName( _L("cae_brightness_101.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenStillPrepared ) );
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNotSupported );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestBrightnessNeg101L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETBRIGHTNESSL.006
    //
    // Action : Test brightness setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetBrightness( -101 );

    //iSettingsClient->SetStillFileName( _L("cae_brightness_-101.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenStillPrepared ) );
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNotSupported );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}


/******************************/
/*          CONTRAST          */
/******************************/

void CCaeTestStillSettings::TestContrast0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETCONTRASTL.002
    //
    // Action : Test contrast setting with still image capture and viewfinding.
	//      
	// Output : No leave if supported. Leave if not supported.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetContrast( 0 );

    iSettingsClient->SetStillFileName( _L("cae_contrast_0.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenStillPrepared ) );
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestContrast100L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETCONTRASTL.003
    //
    // Action : Test contrast setting with still image capture and viewfinding.
	//      
	// Output : No leave if supported. Leave if not supported.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetContrast( 100 );

    iSettingsClient->SetStillFileName( _L("cae_contrast_100.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenStillPrepared ) );
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestContrastNeg100L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETCONTRASTL.004
    //
    // Action : Test contrast setting with still image capture and viewfinding.
	//      
	// Output : No leave if supported. Leave if not supported.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetContrast( -100 );

    iSettingsClient->SetStillFileName( _L("cae_contrast_-100.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenStillPrepared ) );
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestContrast101L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETCONTRASTL.005
    //
    // Action : Test contrast setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetContrast( 101 );

    //iSettingsClient->SetStillFileName( _L("cae_contrast_101.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenStillPrepared ) );
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNotSupported );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestContrastNeg101L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETCONTRASTL.006
    //
    // Action : Test contrast setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetContrast( -101 );

    //iSettingsClient->SetStillFileName( _L("cae_contrast_-101.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenStillPrepared ) );
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNotSupported );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}


/******************************/
/*      EXPOSURE MODE         */
/******************************/


void CCaeTestStillSettings::TestExposureMode2L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETEXPOSUREMODEL.003
    //
    // Action : Test exposure mode 2.
	//      
	// Output : Leave.   

	const TInt KSetVal = 2;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetExposureMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_exposure_mode_2.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenStillPrepared ) );
	if ( KSetVal & info.iExposureModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestExposureMode4L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETEXPOSUREMODEL.004
    //
    // Action : Test exposure mode 4
	//      
	// Output : Leave.   

	const TInt KSetVal = 4;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetExposureMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_exposure_mode_4.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenStillPrepared ) );
	if ( KSetVal & info.iExposureModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestExposureModeNeg1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETEXPOSUREMODEL.005
    //
    // Action : Test invalid exposure mode
	//      
	// Output : Leave.   

	const TInt KSetVal = -1;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetExposureMode( KSetVal );

    //iSettingsClient->SetStillFileName( _L("cae_exposure_mode_-1.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenStillPrepared ) );
    assertL( err == KErrNotSupported );
}

void CCaeTestStillSettings::TestExposureMode5L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETEXPOSUREMODEL.006
    //
    // Action : Test combined exposure modes.
	//      
	// Output : Leave.   

	const TInt KSetVal = 5;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetExposureMode( KSetVal );

    //iSettingsClient->SetStillFileName( _L("cae_exposure_mode_5.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenStillPrepared ) );
    if ( KSetVal & info.iExposureModesSupported )
		{
		assertL( err != KErrNotSupported );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

/******************************/
/*    WHITE BALANCE MODE      */
/******************************/

void CCaeTestStillSettings::TestWhiteBalanceMode1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETWHITEBALANCEMODEL.002
    //
    // Action : Test white balance setting with still image capture and viewfinding.
	//      
	// Output : No leave, if supported. Leave, if not supported.    

	const TInt KSetVal = 1;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_white_balance_mode_1.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenStillPrepared ) );
	if ( KSetVal & info.iWhiteBalanceModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestWhiteBalanceMode2L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETWHITEBALANCEMODEL.003
    //
    // Action : Test white balance setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 2;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_white_balance_mode_2.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenStillPrepared ) );
	if ( KSetVal & info.iWhiteBalanceModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestWhiteBalanceMode4L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETWHITEBALANCEMODEL.004
    //
    // Action : Test white balance setting with still image capture and viewfinding.
	//      
	// Output : Leave.   
	
	const TInt KSetVal = 4;
	
	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_white_balance_mode_4.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenStillPrepared ) );
	if ( KSetVal & info.iWhiteBalanceModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestWhiteBalanceMode8L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETWHITEBALANCEMODEL.005
    //
    // Action : Test white balance setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 8;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_white_balance_mode_8.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenStillPrepared ) );
	if ( KSetVal & info.iWhiteBalanceModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestWhiteBalanceMode10L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETWHITEBALANCEMODEL.006
    //
    // Action : Test white balance setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 0x10;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_white_balance_mode_10.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenStillPrepared ) );
	if ( KSetVal & info.iWhiteBalanceModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestWhiteBalanceModeNeg1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETWHITEBALANCEMODEL.007
    //
    // Action : Test white balance setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = -1;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    //iSettingsClient->SetStillFileName( _L("cae_white_balance_mode_-1.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenStillPrepared ) );
    assertL( err == KErrNotSupported );
}

void CCaeTestStillSettings::TestWhiteBalanceMode5L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETWHITEBALANCEMODEL.008
    //
    // Action : Test white balance setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 5;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    //iSettingsClient->SetStillFileName( _L("cae_white_balance_mode_5.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenStillPrepared ) );
    assertL( err == KErrNotSupported || err == KErrArgument );
}


/******************************/
/*        FLASH MODE          */
/******************************/

void CCaeTestStillSettings::TestFlashMode4L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETFLASHMODEL.004
    //
    // Action : Test flash mode setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 4;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_flash_mode_4.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenStillPrepared ) );
	if ( KSetVal & info.iFlashModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestFlashMode8L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETFLASHMODEL.005
    //
    // Action : Test flash mode setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 8;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_flash_mode_8.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenStillPrepared ) );
	if ( KSetVal & info.iFlashModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestStillSettings::TestFlashModeNeg1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETFLASHMODEL.006
    //
    // Action : Test flash mode setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = -1;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    //iSettingsClient->SetStillFileName( _L("cae_flash_mode_-1.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenStillPrepared ) );
    assertL( err == KErrNotSupported );
}

void CCaeTestStillSettings::TestFlashMode5L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETFLASHMODEL.007
    //
    // Action : Test flash mode setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 5;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    //iSettingsClient->SetStillFileName( _L("cae_flash_mode_5.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenStillPrepared ) );
    assertL( err == KErrArgument || err == KErrNotSupported );
}

void CCaeTestStillSettings::TestFlashMode6L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETFLASHMODEL.008
    //
    // Action : Test flash mode setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 6;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    //iSettingsClient->SetStillFileName( _L("cae_flash_mode_6.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenStillPrepared ) );
    assertL( err == KErrArgument || err == KErrNotSupported );
}
void CCaeTestStillSettings::TestFlashMode9L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETFLASHMODEL.009
    //
    // Action : Test flash mode setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 9;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    //iSettingsClient->SetStillFileName( _L("cae_flash_mode_9.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenStillPrepared ) );
    assertL( err == KErrArgument || err == KErrNotSupported );
}
void CCaeTestStillSettings::TestFlashMode10L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETFLASHMODEL.010
    //
    // Action : Test flash mode setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 10;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    //iSettingsClient->SetStillFileName( _L("cae_flash_mode_10.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenStillPrepared ) );
    assertL( err == KErrArgument || err == KErrNotSupported );
}

/******************************/
/*        INITIALIZED         */
/******************************/

void CCaeTestStillSettings::TestExposureModeInitialized2L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETEXPOSUREMODEL.007
    //
    // Action : Test exposure mode.
	//      
	// Output : Leave.   

	const TInt KSetVal = 2;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetExposureMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_exposure_mode_initialized_2.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenInitialized ) );
	if ( KSetVal & info.iExposureModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err != KErrNone );
		}
}

void CCaeTestStillSettings::TestWhiteBalanceModeInitialized2L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETWHITEBALANCEMODEL.009
    //
    // Action : Test white balance mode setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 2;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_white_balance_mode_initialized_2.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenInitialized ) );
	if ( KSetVal & info.iWhiteBalanceModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err != KErrNone );
		}
}

void CCaeTestStillSettings::TestFlashModeInitialized2L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-SC.SETFLASHMODEL.008
    //
    // Action : Test flash mode setting with still image capture and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 2;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_flash_mode_initialized_2.jpg") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenInitialized ) );

#if (MIRAGE_X_PROD3) || (MIRAGE_X_PROD4) || (MIRAGE_X_PROD7) ||(MIRAGE_X_PROD9)
    assertL( err == KErrNone );
#else
    assertL( err != KErrNone );
#endif
}

void CCaeTestStillSettings::TestDigiZoomInitializedMode0L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-SC.SETZOOMMODEL.005
    //
    // Action : Test digital zoom mode.
	//      
	// Output : OK.   

	const TInt KSetVal = 0;

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->SetZoomMode( KSetVal );

    iSettingsClient->SetStillFileName( _L("cae_digizoom_initialized_mode_0.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenInitialized );
}

/******************************/
/*          GET INFO          */
/******************************/

void CCaeTestStillSettings::TestGetInfo()
{
	// Group:   Settings
    //
    // ID:      CAE.GEN.GETINFO.001
    // 
    // Action : Call GetInfo. Engine initialized
	//      
	// Output : OK.   

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetInfo );
}

/********************************************************/
/*                  RESET TO DEFAULTS                   */
/********************************************************/

void CCaeTestStillSettings::TestResetToDefaultsL()
    {
	// ID: CAE.GEN.RESETTODEFAULTSL.001
    //
    // Group:   General Settings
    // 
    // Action :  Resets the following settings: exposure mode, white balance mode, 
    //           zoom mode, zoom value, flash mode, brightness, and contrast.
    //
	// Output : OK.   

    iSettingsClient->SetContrast( 50 );

    iSettingsClient->SetBrightness( 50 );

#if MIRAGE_X_PROD7
    iSettingsClient->SetDigiZoomValue( 0 );
#else
    iSettingsClient->SetDigiZoomValue( 1 );
#endif

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EResetToDefaults );
    }


/********************************************************/
/*                  VIEW FINDER MIRROR                  */
/********************************************************/

void CCaeTestStillSettings::TestSetViewFinderMirrorL()
    {
	// ID:      CAE.VF.SETVIEWFINDERMIRROR.001
    //
    // Group:   View Finder
    // 
    // Action:  Test view finder mirror when initialization is done.
	//      
	// Output:  No leave if supported, leave if not supported. 
    
	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetViewFinderMirror ) );
    if ( TCameraInfo::EViewFinderMirrorSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
    }

/********************************************************/
/*               GET STILL FRAME SIZE                   */
/********************************************************/

void CCaeTestStillSettings::TestGetStillFrameSizeWhenInitializedL()
    {
	// ID: CAE.SC.GETSTILLFRAMESIZE.001
    //
    // Group:   Capture
    // 
    // Action : Get still image frame size for the specified quality level.  
    //          Initialization is done.
	//      
	// Output : No leave if supported. Leave if not supported.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetStillFrameSizeWhenInitialized );
    
    TSize size( 640, 480 );

    TSize stillFrameSize;
    iSettingsClient->GetStillFrameSize( stillFrameSize );
    if ( stillFrameSize != size )
        {
        User::Leave ( KErrGetNotEqualToValuePreviouslySet );
        }
    }

void CCaeTestStillSettings::TestGetStillFrameSizeWhenPreparedL()
    {
	// ID: CAE.SC.GETSTILLFRAMESIZE.002
    //
    // Group:   Capture
    // 
    // Action : Get still image frame size for the specified quality level,  
    //          when prepared.
	//      
	// Output : No leave.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetStillFrameSizeWhenPrepared );

    TSize size( 640, 480 );

    TSize stillFrameSize;
    iSettingsClient->GetStillFrameSize( stillFrameSize );
    if ( stillFrameSize != size )
        {
        User::Leave ( KErrGetNotEqualToValuePreviouslySet );
        }
    }

/********************************************************/
/*            ESTIMATED STILL SIZE IN BYTES             */
/********************************************************/

void CCaeTestStillSettings::TestEstimatedStillSizeInBytesWhenInitializedL()
    {
	// ID: CAE.SC.ESTIMATEDSTILLSIZEINBYTES.001
    //
    // Group:   Capture
    // 
    // Action : Get the estimated still image size in bytes 
    //          (for the specified quality level).  
    //          Initialization is done.
	//      
	// Output : No leave.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EEstimatedStillSizeInBytesWhenInitialized);

    TInt stillByteSize;
    stillByteSize = iSettingsClient->EstimatedStillSizeInBytes();
    if ( stillByteSize != 90000 )
        {
        User::Leave ( KErrGetNotEqualToValuePreviouslySet );
        }    
    }

void CCaeTestStillSettings::TestEstimatedStillSizeInBytesWhenPreparedL()
    {
	// ID: CAE.SC.ESTIMATEDSTILLSIZEINBYTES.002
    //
    // Group:   Capture
    // 
    // Action : Get the estimated still image size in bytes 
    //          (for the specified quality level),  
    //          when prepared.
	//      
	// Output : No leave.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EEstimatedStillSizeInBytesWhenPrepared);

    TInt stillByteSize;
    stillByteSize = iSettingsClient->EstimatedStillSizeInBytes();
    if ( stillByteSize != 90000 )
        {
        User::Leave ( KErrGetNotEqualToValuePreviouslySet );
        }    
    }
/********************************************************/
/*             SET SNAP IMAGE SIZE                      */
/********************************************************/

void CCaeTestStillSettings::TestSetSnapImageSizeBeforePrepareL()
    {
	// ID: CAE.SC.SETSNAPIMAGESIZE.001
    //
    // Group:   Capture
    // 
    // Action : Set the snap image size before preparing.  
	//      
	// Output : Should return KErrNotReady.  
    
    iSettingsClient->SetStillFormat( CCamera::EFormatExif );

    iSettingsClient->SetSnapFileName( _L("cae_set_snap_size1.mbm") );

    iSettingsClient->SetSnapImageSize( TSize( 44, 52 ) );

    TRAPD(err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetSnapImageSizeBeforePrepare ));
    
    assertL( err == KErrNotReady );

    }

void CCaeTestStillSettings::TestSetSnapImageSizeDuringAndAfterPrepareL()
    {
	// ID: CAE.SC.SETSNAPIMAGESIZE.003
    //
    // Group:   Capture
    // 
    // Action : Set the snap image size before preparing.  
	//      
	// Output : Ok. Visual check snap image size.  
    
    iSettingsClient->SetStillFormat( CCamera::EFormatExif );

    iSettingsClient->SetSnapFileName( _L("cae_set_snap_size2.mbm") );

    iSettingsClient->SetSnapImageSize( TSize( 44, 52 ) );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetSnapImageSizeDuringAndAfterPrepare );
    
    }


void CCaeTestStillSettings::TestInitUidL()
    {
	// ID: CAE.SC.INITUID.001
    //
    // Group:   Capture
    // 
    // Action : Get the estimated still image size in bytes 
    //          (for the specified quality level),  
    //          when prepared.
	//      
	// Output : No leave.   

    iSettingsClient->SetStillQualityLevelIndex( 0 );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EInitUid) );

    assertL( err == KErrNotSupported );

    }

void CCaeTestStillSettings::TestSetJpegQuality0L()
    {
	// ID: CAE.SET-SC.SETJPEGQUALITY.002
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture quality level 0 
    //          image when initialization is done and viewfinder is running.
    //          This test case is for boundary value testing.
	//      
	// Output : OK.   

    iSettingsClient->SetStillFormat( CCamera::EFormatJpeg );

    iSettingsClient->SetStillCompressionQuality( 0 );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iSettingsClient->SetStillFrameSize( sizeVga );

    // iSettingsClient->SetStillFileName( _L("cae_jpeg_quality_setting_0.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetJpegQualityWhenStillPrepared );
    
    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountStillPrepareComplete() == 1 );
    assertL( iSettingsClient->CountSnapImageReady() == 1 );
    assertL( iSettingsClient->CountStillImageReady() == 1 );
    }

void CCaeTestStillSettings::TestSetJpegQuality101L()
    {
	// ID: CAE.SET-SC.SETJPEGQUALITY.003
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture quality level 101 
    //          image when initialization is done and viewfinder is running.
    //          This test case is for boundary value testing.
	//      
	// Output : OK.   

    iSettingsClient->SetStillFormat( CCamera::EFormatJpeg );

    iSettingsClient->SetStillCompressionQuality( 101 );

    TSize sizeVga( KImgWidthVGA, KImgHeightVGA );

    iSettingsClient->SetStillFrameSize( sizeVga );

    // iSettingsClient->SetStillFileName( _L("cae_jpeg_quality_setting_101.jpg") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetJpegQualityWhenStillPrepared );
    
    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountStillPrepareComplete() == 1 );
    assertL( iSettingsClient->CountSnapImageReady() == 1 );
    assertL( iSettingsClient->CountStillImageReady() == 1 );
    }


void CCaeTestStillSettings::TestCaptureStillBurstIntervalM2L()
    {
	// ID: CAE.SC.CAPTURESTILLBURSTINTERVAL.M2.004
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
   
    TTimeIntervalMicroSeconds burstInterval( KMaxTUint32 );  // Test with too big value
    
    iSettingsClient->SetStillBurstCaptureInterval( burstInterval );

    TRAPD( error, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EPrepareAndSetStillBurstCaptureInterval ) );
    assertL( error == KErrArgument );

    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountStillPrepareComplete() == 1 );
    assertL( iSettingsClient->CountStillBurstCaptureMoment() == 0 );
    assertL( iSettingsClient->CountSnapImageReady() == 0 );
    assertL( iSettingsClient->CountStillImageReady() == 0 );
    assertL( iSettingsClient->CountStillBurstComplete() == 0 );
    }
    

void CCaeTestStillSettings::TestSetJpegCodecInvalidL()
    {
	// ID: CAE.SET-SC.SETJPEGCODEC.001
    //
    // Group:   Capture
    // 
    // Action : Test to prepare still capturing and capture an image 
    //          when initialization is done and viewfinder is running.
    //          This test case is for invalid codec uid testing.
	//      
	// Output : Leave.   

    iSettingsClient->SetStillFormat( CCamera::EFormatExif );

    iSettingsClient->SetImageCodecs( TUid::Uid( 0x12345 ), TUid::Uid( 0x54321 ) ); // Invalid codec uids

    TRAPD( error, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetJpegCodecWhenStillPrepared ));
    assertL( error == KErrNotFound );
    
    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountStillPrepareComplete() == 1 );
    assertL( iSettingsClient->CountSnapImageReady() == 1 );
    assertL( iSettingsClient->CountStillImageReady() == 0 );
    }

    

//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 

MTest* CCaeTestStillSettings::suiteL ()
{
	// Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL( _L8("CCaeTestStillSettings") );
    
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETZOOMVALUEL.003"), &TestDigiZoomVal2L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETZOOMVALUEL.004"), &TestDigiZoomValNeg1L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETZOOMVALUEL.005"), &TestOpticalZoomValMinL ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETZOOMVALUEL.006"), &TestOpticalZoomValMaxL ) );
    
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETZOOMMODEL.001"), &TestDigiZoomMode0L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETZOOMMODEL.002"), &TestOpticalZoomMode1L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETZOOMMODEL.003"), &TestOpticalDigiZoomMode2L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETZOOMMODEL.004"), &TestDigiZoomModeNeg1L ) );
    
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETBRIGHTNESSL.002"), &TestBrightness0L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETBRIGHTNESSL.003"), &TestBrightness100L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETBRIGHTNESSL.004"), &TestBrightnessNeg100L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETBRIGHTNESSL.005"), &TestBrightness101L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETBRIGHTNESSL.006"), &TestBrightnessNeg101L ) );
    
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETCONTRASTL.002"), &TestContrast0L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETCONTRASTL.003"), &TestContrast100L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETCONTRASTL.004"), &TestContrastNeg100L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETCONTRASTL.005"), &TestContrast101L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETCONTRASTL.006"), &TestContrastNeg101L ) );
	
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETEXPOSUREMODEL.003"), &TestExposureMode2L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETEXPOSUREMODEL.004"), &TestExposureMode4L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETEXPOSUREMODEL.005"), &TestExposureModeNeg1L ) );
    
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETEXPOSUREMODEL.006"), &TestExposureMode5L ) );
	
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETWHITEBALANCEMODEL.002"), &TestWhiteBalanceMode1L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETWHITEBALANCEMODEL.003"), &TestWhiteBalanceMode2L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETWHITEBALANCEMODEL.004"), &TestWhiteBalanceMode4L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETWHITEBALANCEMODEL.005"), &TestWhiteBalanceMode8L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETWHITEBALANCEMODEL.006"), &TestWhiteBalanceMode10L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETWHITEBALANCEMODEL.007"), &TestWhiteBalanceModeNeg1L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETWHITEBALANCEMODEL.008"), &TestWhiteBalanceMode5L ) );
	
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETFLASHMODEL.004"), &TestFlashMode4L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETFLASHMODEL.005"), &TestFlashMode8L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETFLASHMODEL.006"), &TestFlashModeNeg1L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETFLASHMODEL.007"), &TestFlashMode5L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETFLASHMODEL.008"), &TestFlashMode6L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETFLASHMODEL.009"), &TestFlashMode9L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETFLASHMODEL.010"), &TestFlashMode10L ) );
	
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETEXPOSUREMODEL.007"), &TestExposureModeInitialized2L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETWHITEBALANCEMODEL.009"), &TestWhiteBalanceModeInitialized2L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETFLASHMODEL.008"), &TestFlashModeInitialized2L ) );
	suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETZOOMMODEL.005"), &TestDigiZoomInitializedMode0L ) );

    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.GEN.GETINFO.001"), &TestGetInfo ) );
	
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.GEN.RESETTODEFAULTSL.001"), &TestResetToDefaultsL ) );
    
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.VF.SETVIEWFINDERMIRROR.001"), &TestSetViewFinderMirrorL ) );
    
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SC.GETSTILLFRAMESIZE.001"), &TestGetStillFrameSizeWhenInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SC.GETSTILLFRAMESIZE.002"), &TestGetStillFrameSizeWhenPreparedL ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SC.ESTIMATEDSTILLSIZEINBYTES.001"), &TestEstimatedStillSizeInBytesWhenInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SC.ESTIMATEDSTILLSIZEINBYTES.002"), &TestEstimatedStillSizeInBytesWhenPreparedL ) );

    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SC.SETSNAPIMAGESIZE.001"), &TestSetSnapImageSizeBeforePrepareL ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SC.SETSNAPIMAGESIZE.003"), &TestSetSnapImageSizeDuringAndAfterPrepareL ) );

    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SC.INITUID.001"), &TestInitUidL ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETJPEGQUALITY.002"), &TestSetJpegQuality0L ) );
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETJPEGQUALITY.003"), &TestSetJpegQuality101L ) );

#ifdef __CAMERA_BURST_MODE
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SC.CAPTURESTILLBURSTINTERVAL.M2.004"), &TestCaptureStillBurstIntervalM2L ) );
#endif
    
    suite->addTestL( CTestCaller<CCaeTestStillSettings>::NewL( _L8("CAE.SET-SC.SETJPEGCODEC.001"), &TestSetJpegCodecInvalidL ) );
    
    return suite;
}

