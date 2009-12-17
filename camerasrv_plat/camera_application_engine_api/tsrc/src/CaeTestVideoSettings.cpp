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
* Description: CaeTestVideoSettings.cpp
*
*/

#include "CaeTestVideoSettings.h"
#include <f32file.h>



CCaeTestVideoSettings::CCaeTestVideoSettings()
    {
    }


CCaeTestVideoSettings::~CCaeTestVideoSettings()
    {
    }


void CCaeTestVideoSettings::setUpL()
    {
	iSettingsClient = CCaeTestSettingsClient::NewL();

    iSettingsClient->SetFileNameBase( KFileNameBase );
    }

//_LIT(KVideoFileName, "videoclip.3gp");

void CCaeTestVideoSettings::tearDown()
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


/********************************************************/
/*             VIDEO RECORDING DIGITAL ZOOM VALUE       */
/********************************************************/

void CCaeTestVideoSettings::TestDigiZoomVal2L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETZOOMVALUEL.008
    //
    // Action : Test out-of-bounds digital zoom value.
	//      
	// Output : OK.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );
	
    iSettingsClient->SetDigiZoomValue( info.iMaxDigitalZoom + 1 );

    //iSettingsClient->SetVideoFileName( _L("cae_digizoom_val_2.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenVideoPrepared ) );
    assertL( err != KErrNone );
}

void CCaeTestVideoSettings::TestDigiZoomNegVal1L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETZOOMVALUEL.009
    //
    // Action : Test digital zoom.
	//      
	// Output : OK.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetDigiZoomValue( -1 );

    //iSettingsClient->SetVideoFileName( _L("cae_digizoom_val_-1.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenVideoPrepared ) );
    assertL( err != KErrNone );
}

/********************************************************/
/*             VIDEO RECORDING DIGITAL ZOOM MODE        */
/********************************************************/

void CCaeTestVideoSettings::TestDigiZoomMode0L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETZOOMMODEL.006
    //
    // Action : Test digital zoom mode.
	//      
	// Output : OK.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetZoomMode( 0 );

    iSettingsClient->SetVideoFileName( _L("cae_digizoom_mode_0.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenVideoPrepared );//verify if is correct action
}

void CCaeTestVideoSettings::TestOpticalZoomMode1L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETZOOMMODEL.007
    //
    // Action : Test optical zoom mode.
	//      
	// Output : OK.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetZoomMode( 1 );

    //iSettingsClient->SetVideoFileName( _L("cae_digizoom_mode_1.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenVideoPrepared ) );//verify if is correct action
    assertL( err != KErrNone );//verify if is correct action
}

void CCaeTestVideoSettings::TestOpticalDigiZoomMode2L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETZOOMMODEL.008
    //
    // Action : Test optical digital zoom mode.
	//      
	// Output : OK.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetZoomMode( 2 );

    //iSettingsClient->SetVideoFileName( _L("cae_digizoom_mode_2.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenVideoPrepared ) );//verify if is correct action
    assertL( err != KErrNone );//verify if is correct action
}

void CCaeTestVideoSettings::TestDigiZoomNegMode1L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETZOOMMODEL.009
    //
    // Action : Test digital zoom mode.
	//      
	// Output : OK.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetZoomMode( -1 );

    //iSettingsClient->SetVideoFileName( _L("cae_digizoom_mode_-1.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenVideoPrepared ) );//verify if is correct action
    assertL( err != KErrNone );
}

/********************************************************/
/*             VIDEO RECORDING BRIGHTNESS               */
/********************************************************/

void CCaeTestVideoSettings::TestBrightness0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETBRIGHTNESSL.009
    //
    // Action : Test brightness setting with video recording and viewfinding.
	//      
	// Output : No leave.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetBrightness( 0 );

    iSettingsClient->SetVideoFileName( _L("cae_brightness_0.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenVideoPrepared ) );
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestBrightness100L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETBRIGHTNESSL.010
    //
    // Action : Test brightness setting with video recording and viewfinding.
	//      
	// Output : No leave.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetBrightness( 100 );

    //iSettingsClient->SetVideoFileName( _L("cae_brightness_100.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenVideoPrepared ) );
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestBrightnessNeg100L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETBRIGHTNESSL.011
    //
    // Action : Test brightness setting with video recording and viewfinding.
	//      
	// Output : No leave.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetBrightness( -100 );

    //iSettingsClient->SetVideoFileName( _L("cae_brightness_-100.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenVideoPrepared ) );
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestBrightness101L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETBRIGHTNESSL.012
    //
    // Action : Test brightness setting with video recording and viewfinding.
	//      
	// Output : Leave.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetBrightness( 101 );

    //iSettingsClient->SetVideoFileName( _L("cae_brightness_101.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenVideoPrepared ) );
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNotSupported );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestBrightnessNeg101L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETBRIGHTNESSL.013
    //
    // Action : Test brightness setting with video recording and viewfinding.
	//      
	// Output : Leave.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetBrightness( -101 );

    //iSettingsClient->SetVideoFileName( _L("cae_brightness_-101.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetBrightnessWhenVideoPrepared ) );
    if ( TCameraInfo::EBrightnessSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNotSupported );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

/********************************************************/
/*             VIDEO RECORDING CONTRAST                 */
/********************************************************/

void CCaeTestVideoSettings::TestContrast0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETCONTRASTL.009
    //
    // Action : Test contrast setting with video recording and viewfinding.
	//      
	// Output : No leave if supported. Leave if not supported.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetContrast( 0 );

    iSettingsClient->SetVideoFileName( _L("cae_contrast_0.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenVideoPrepared ) );
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestContrast100L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETCONTRASTL.010
    //
    // Action : Test contrast setting with video recording and viewfinding.
	//      
	// Output : No leave if supported. Leave if not supported.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetContrast( 100 );

    //iSettingsClient->SetVideoFileName( _L("cae_contrast_100.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenVideoPrepared ) );
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestContrastNeg100L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETCONTRASTL.011
    //
    // Action : Test contrast setting with video recording and viewfinding.
	//      
	// Output : No leave if supported. Leave if not supported.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetContrast( -100 );

    //iSettingsClient->SetVideoFileName( _L("cae_contrast_-100.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenVideoPrepared ) );
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestContrast101L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETCONTRASTL.012
    //
    // Action : Test contrast setting with video recording and viewfinding.
	//      
	// Output : Leave.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetContrast( 101 );

    //iSettingsClient->SetVideoFileName( _L("cae_contrast_101.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenVideoPrepared ) );
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNotSupported );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestContrastNeg101L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETCONTRASTL.013
    //
    // Action : Test contrast setting with video recording and viewfinding.
	//      
	// Output : Leave.   

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetContrast( -101 );

    //iSettingsClient->SetVideoFileName( _L("cae_contrast_-101.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetContrastWhenVideoPrepared ) );
    if ( TCameraInfo::EContrastSupported & info.iOptionsSupported )
		{
		assertL( err == KErrNotSupported );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}


/********************************************************/
/*             VIDEO RECORDING EXPOSURE MODE            */
/********************************************************/


void CCaeTestVideoSettings::TestExposureMode2L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETEXPOSUREMODEL.010
    //
    // Action : Test exposure mode 2.
	//      
	// Output : No leave if the mode is supported. Leave if not supported.   

	const TInt KSetVal = 2;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetExposureMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_exposure_mode_2.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenVideoPrepared ) );
	if ( KSetVal & info.iExposureModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err != KErrNone );
		}
}

void CCaeTestVideoSettings::TestExposureMode4L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETEXPOSUREMODEL.011
    //
    // Action : Test exposure mode 4
	//      
	// Output : No leave if the mode is supported. Leave if not supported.   

	const TInt KSetVal = 4;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetExposureMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_exposure_mode_4.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenVideoPrepared ) );
	if ( KSetVal & info.iExposureModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err != KErrNone );
		}
}

void CCaeTestVideoSettings::TestExposureModeNeg1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETEXPOSUREMODEL.012
    //
    // Action : Test invalid exposure mode
	//      
	// Output : Leave.   

	const TInt KSetVal = -1;

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetExposureMode( KSetVal );

    //iSettingsClient->SetVideoFileName( _L("cae_exposure_mode_-1.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenVideoPrepared ) );
    assertL( err == KErrNotSupported );
}

void CCaeTestVideoSettings::TestExposureMode5L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETEXPOSUREMODEL.013
    //
    // Action : Test combined exposure modes.
	//      
	// Output : Leave. 

	const TInt KSetVal = 5;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetExposureMode( KSetVal );

    // iSettingsClient->SetVideoFileName( _L("cae_exposure_mode_5.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenVideoPrepared ) );
	if ( KSetVal & info.iExposureModesSupported )
		{
		assertL( err != KErrNotSupported );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

/********************************************************/
/*           VIDEO RECORDING WHITE BALANCE MODE         */
/********************************************************/

void CCaeTestVideoSettings::TestWhiteBalanceMode1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETWHITEBALANCEMODEL.011
    //
    // Action : Test white balance setting with video recording and viewfinding.
	//      
	// Output : No leave if the mode is supported. Leave if not supported.   

	const TInt KSetVal = 1;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_white_balance_mode_1.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenVideoPrepared ) );
	if ( KSetVal & info.iWhiteBalanceModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestWhiteBalanceMode2L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETWHITEBALANCEMODEL.012
    //
    // Action : Test white balance setting with video recording and viewfinding.
	//      
	// Output : No leave if the mode is supported. Leave if not supported.   

	const TInt KSetVal = 2;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_white_balance_mode_2.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenVideoPrepared ) );
	if ( KSetVal & info.iWhiteBalanceModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestWhiteBalanceMode4L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETWHITEBALANCEMODEL.013
    //
    // Action : Test white balance setting with video recording and viewfinding.
	//      
	// Output : No leave if the mode is supported. Leave if not supported.   

	const TInt KSetVal = 4;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_white_balance_mode_4.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenVideoPrepared ) );
	if ( KSetVal & info.iWhiteBalanceModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestWhiteBalanceMode8L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETWHITEBALANCEMODEL.014
    //
    // Action : Test white balance setting with video recording and viewfinding.
	//      
	// Output : No leave if the mode is supported. Leave if not supported.   

	const TInt KSetVal = 8;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_white_balance_mode_8.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenVideoPrepared ) );
	if ( KSetVal & info.iWhiteBalanceModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestWhiteBalanceMode10L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETWHITEBALANCEMODEL.015
    //
    // Action : Test white balance setting with video recording and viewfinding.
	//      
	// Output : No leave if the mode is supported. Leave if not supported.   

	const TInt KSetVal = 0x10;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_white_balance_mode_10.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenVideoPrepared ) );
	if ( KSetVal & info.iWhiteBalanceModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestWhiteBalanceModeNeg1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETWHITEBALANCEMODEL.016
    //
    // Action : Test white balance setting with video recording and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = -1;

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    //iSettingsClient->SetVideoFileName( _L("cae_white_balance_mode_-1.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenVideoPrepared ) );
    assertL( err != KErrNone );
}

void CCaeTestVideoSettings::TestWhiteBalanceMode5L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETWHITEBALANCEMODEL.017
    //
    // Action : Test white balance setting with video recording and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 5;

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    //iSettingsClient->SetVideoFileName( _L("cae_white_balance_mode_5.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenVideoPrepared ) );
    assertL( err != KErrNone );
}

void CCaeTestVideoSettings::TestWhiteBalanceMode12L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETWHITEBALANCEMODEL.018
    //
    // Action : Test white balance setting with video recording and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 0x12; // illegal combination of cloudy & flash

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    //iSettingsClient->SetVideoFileName( _L("cae_white_balance_mode_12.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenVideoPrepared ) );
    assertL( err != KErrNone );
}

/********************************************************/
/*             VIDEO RECORDING FLASH MODE               */
/********************************************************/

void CCaeTestVideoSettings::TestFlashMode0L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETFLASHMODEL.009
    //
    // Action : Test flash mode setting with video recording and viewfinding.
	//      
	// Output : No leave.   

	const TInt KSetVal = 0;

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_flash_mode_0.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenVideoPrepared );
}

void CCaeTestVideoSettings::TestFlashMode1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETFLASHMODEL.010
    //
    // Action : Test flash mode setting with video recording and viewfinding.
	//      
	// Output : No leave if the mode is supported. Leave if not supported.   

	const TInt KSetVal = 1;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_flash_mode_1.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenVideoPrepared ) );
	if ( KSetVal & info.iFlashModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestFlashMode2L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETFLASHMODEL.011
    //
    // Action : Test flash mode setting with video recording and viewfinding.
	//      
	// Output : No leave if the mode is supported. Leave if not supported.   

	const TInt KSetVal = 2;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_flash_mode_2.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenVideoPrepared ) );
	if ( KSetVal & info.iFlashModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestFlashMode4L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETFLASHMODEL.012
    //
    // Action : Test flash mode setting with video recording and viewfinding.
	//      
	// Output : No leave if the mode is supported. Leave if not supported.   

	const TInt KSetVal = 4;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_flash_mode_4.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenVideoPrepared ) );
	if ( KSetVal & info.iFlashModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestFlashMode8L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETFLASHMODEL.013
    //
    // Action : Test flash mode setting with video recording and viewfinding.
	//      
	// Output : No leave if the mode is supported. Leave if not supported.   

	const TInt KSetVal = 8;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_flash_mode_8.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenVideoPrepared ) );
	if ( KSetVal & info.iFlashModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestFlashModeNeg1L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETFLASHMODEL.014
    //
    // Action : Test flash mode setting with video recording and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = -1;

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    //iSettingsClient->SetVideoFileName( _L("cae_flash_mode_-1.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenVideoPrepared ) );
    assertL( err != KErrNone );
}

void CCaeTestVideoSettings::TestFlashMode5L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETFLASHMODEL.015
    //
    // Action : Test flash mode setting with video recording and viewfinding.
	//      
	// Output : Leave.   

	const TInt KSetVal = 5;

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    //iSettingsClient->SetVideoFileName( _L("cae_flash_mode_5.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenVideoPrepared ) );
    assertL( err != KErrNone );
}

/********************************************************/
/*             VIDEO RECORDING CLIP MAX SIZE            */
/********************************************************/

void CCaeTestVideoSettings::TestVideoClipMaxSize0L()
{
	// Group:   Settings.
    // 
    //ID:       CAE.SET-VR.SETVIDEOCLIPMAXSIZEL.001
    //
    // Action : Test video clip max size setting with video recording.
	//      
	// Output : No leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetTimeout( 5000000 );

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoClipMaxSize( 0 );

    iSettingsClient->SetVideoFileName( _L("cae_clip_max_size_0.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetClipMaxSizeWhenVideoPrepared );
}

void CCaeTestVideoSettings::TestVideoClipMaxSizeNeg1L()
{
	// Group:   Settings.
    // 
    //ID:       CAE.SET-VR.SETVIDEOCLIPMAXSIZEL.003
    //
    // Action : Test video clip max size setting with video recording.
	//      
	// Output : No leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetTimeout( 5000000 );

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoClipMaxSize( -1 );

    iSettingsClient->SetVideoFileName( _L("cae_clip_max_size_-1.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetClipMaxSizeWhenVideoPrepared );
}

/*
void CCaeTestVideoSettings::TestVideoClipMaxSize10001L()
{
	// Group:   Settings.
    // 
    //ID:       CAE.SET-VR.SETVIDEOCLIPMAXSIZEL.004
    //
    // Action : Test video clip max size setting with video recording.
	//      
	// Output : No leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetTimeout( 10000000 );

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoClipMaxSize( 10001 );

    iSettingsClient->SetVideoFileName( _L("cae_clip_max_size_10001.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetClipMaxSizeWhenVideoPrepared ) );
    assertL( err == KErrCompletion );
}
*/

/********************************************************/
/*               VIDEO RECORDING AUDIO                  */
/********************************************************/

void CCaeTestVideoSettings::TestVideoAudioETrueL()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETVIDEOAUDIO.001
    //
    // Action : Test audio enable setting with video recording and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoAudio( ETrue );

    iSettingsClient->SetVideoFileName( _L("cae_video_audio_true.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetAudioWhenVideoPrepared );
}

void CCaeTestVideoSettings::TestVideoAudioEFalseL()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETVIDEOAUDIO.002
    //
    // Action : Test audio enable setting with video recording and viewfinding.
	//      
	// Output : No leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoAudio( EFalse );

    iSettingsClient->SetVideoFileName( _L("cae_video_audio_false.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetAudioWhenVideoPrepared );
}


/********************************************************/
/*                    VIDEO RECORDING                   */
/********************************************************/

void CCaeTestVideoSettings::TestExposureModeRecording2L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETEXPOSUREMODEL.014
    //
    // Action : Test exposure mode
	//      
	// Output : No leave if mode is supported. Leave if not supported.  

	const TInt KSetVal = 2;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetExposureMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_exposure_mode_recording_2.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetExposureModeWhenVideoRecording ) );
	if ( KSetVal & info.iExposureModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestWhiteBalanceModeRecording2L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETWHITEBALANCEMODEL.018
    //
    // Action : Test white balance setting with Video image capture and viewfinding.
	//      
	// Output : No leave if the mode is supported. Leave if not supported.   

	const TInt KSetVal = 2;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetWhiteBalanceMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_white_balance_mode_recording_2.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetWhiteBalanceModeWhenVideoRecording ) );
	if ( KSetVal & info.iWhiteBalanceModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestFlashModeRecording2L()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETFLASHMODEL.016
    //
    // Action : Test flash mode setting with Video image capture and viewfinding.
	//      
	// Output : No leave if the mode is supported. Leave if not supported.   

	const TInt KSetVal = 2;

	TCamAppEngineInfo info;
	iSettingsClient->GetInfo( info );

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetFlashMode( KSetVal );

    iSettingsClient->SetVideoFileName( _L("cae_flash_mode_recording_2.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetFlashModeWhenVideoRecording ) );
	if ( KSetVal & info.iFlashModesSupported )
		{
		assertL( err == KErrNone );
		}
	else
		{
		assertL( err == KErrNotSupported );
		}
}

void CCaeTestVideoSettings::TestDigiZoomRecordingMode0L()
{
	// Group:   Settings
    // 
    // ID:      CAE.SET-VR.SETZOOMMODEL.010    // ID:      CAE.SET-VR.SETZOOMMODEL.006
    //
    //
    // Action : Test digital zoom mode.
	//      
	// Output : OK.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetZoomMode( 0 );

    iSettingsClient->SetVideoFileName( _L("cae_digizoom_recording_mode_0.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetDigiZoomWhenVideoRecording );//verify if is correct action
}

void CCaeTestVideoSettings::TestVideoClipMaxSizeRecording10000L()
{
	// Group:   Settings.
    // 
    //ID:       CAE.SET-VR.SETVIDEOCLIPMAXSIZEL.005
    //
    // Action : Test video clip max size setting with video recording and viewfinding.
	//      
	// Output : Leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoClipMaxSize( 10000 );

    iSettingsClient->SetVideoFileName( _L("cae_clip_max_size_recording_10000.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetClipMaxSizeWhenVideoRecording ) );
    assertL( err == KErrNotReady );
}

void CCaeTestVideoSettings::TestVideoClipMaxSizeNotInitialized10000L()
{
	// Group:   Settings.
    // 
    //ID:       CAE.SET-VR.SETVIDEOCLIPMAXSIZEL.006
    //
    // Action : Test video clip max size setting with video recording and viewfinding.
	//      
	// Output : Leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoClipMaxSize( 10000 );

    iSettingsClient->SetVideoFileName( _L("cae_clip_max_size_recording_10000.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetClipMaxSizeWhenVideoNotInitialized ) );
    assertL( err == KErrNotReady );
}

void CCaeTestVideoSettings::TestVideoAudioRecordingETrueL()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETVIDEOAUDIO.003
    //
    // Action : Test audio enable setting with video recording and viewfinding.
	//      
	// Output : Leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoAudio( ETrue );

    iSettingsClient->SetVideoFileName( _L("cae_video_audio_recording_true.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetAudioWhenVideoRecording ) );
    assertL( err == KErrNotReady );
}

void CCaeTestVideoSettings::TestVideoAudioNotInitializedL()
{
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETVIDEOAUDIO.004
    //
    // Action : Test audio enable setting with video not initialized.
	//      
	// Output : Leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoAudio( ETrue );

    iSettingsClient->SetVideoFileName( _L("cae_video_audio_recording_notinit.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetAudioWhenVideoNotInitialized ) );
    assertL( err == KErrNotReady );
}
void CCaeTestVideoSettings::TestGetVideoAudioNotInitializedL()
{
	// Group:   Settings.
    // 
    // ID:      CAE.GET-VR.GETVIDEOAUDIO.005
    //
    // Action : Test video audio
	//      
	// Output : No leave.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoAudio( EFalse );

    iSettingsClient->SetVideoFileName( _L("cae_video_audio_notinit.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetAudioWhenVideoNotInitialized );
}

/********************************************************/
/*             VIDEO RECORDING FILE NAME       */
/********************************************************/

void CCaeTestVideoSettings::TestSetVideoFileNameL()
    {
	// Group:   Settings.
    //
    // ID:      CAE.SET-VR.SETVIDEORECORDINGFILENAMEL.001
    // 
    // Action : Try to set valid file name for the video clip to be recorded. 
    //          Video recorder initialization is done .
	//      
	// Output : No leave. File KVideoFileName, "videoclip.3gp".   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoFileName( _L("videoclip.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetVideoFileNameWhenInitialized );

    assertL( iSettingsClient->CountInitComplete() == 1 );
    }


void CCaeTestVideoSettings::TestSetEmptyVideoFileNameL()
    {
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETVIDEORECORDINGFILENAMEL.002
    // 
    // Action : Try to set invalid (empty) file name for the video clip to be recorded. 
    //          Video recorder initialization is done .
	//      
	// Output : Leave.    

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    //iSettingsClient->SetVideoFileName( _L("") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetEmptyVideoFileNameWhenInitialized ) );
    assertL( err != KErrNone );

    assertL( iSettingsClient->CountInitComplete() == 1 );
    }


void CCaeTestVideoSettings::TestSetVideoFileNameWhenPreparedL()
    {
	// Group:   Settings.
    //
    // ID:      CAE.SET-VR.SETVIDEORECORDINGFILENAMEL.003
    // 
    // Action : Set Video File Name when prepared
	//      
	// Output : No leave. File KVideoFileName, "videoclip.3gp".   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetTimeout( 5000000 );

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoFileName( _L("videoclip.3gp") );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetVideoFileNameWhenPrepared );

    assertL( iSettingsClient->CountInitComplete() == 1 );
    }

void CCaeTestVideoSettings::TestSetVideoFileNameWhenNotInitializedL()
    {
	// Group:   Settings.
    // 
    // ID:      CAE.SET-VR.SETVIDEORECORDINGFILENAMEL.004
    // 
    // Action : Try to set file name for the video clip to be recorded. 
    //          Video recorder initialization is not done .
	//      
	// Output : Leave.    

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoFileName( _L("videoclipnoi.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetVideoFileNameWhenNotInitialized ) );
    assertL( err != KErrNone );

    assertL( iSettingsClient->CountInitComplete() == 1 ); // This is not videoinit counter
    }

void CCaeTestVideoSettings::TestSetVideoFileNameWhenRecordingL()
    {
	// Group:   Settings.
    //
    // ID:      CAE.SET-VR.SETVIDEORECORDINGFILENAMEL.005
    // 
    // Action : Set Video File Name when recording
	//      
	// Output : Leave. 

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );   

    iSettingsClient->SetTimeout( 5000000 );

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->SetVideoFileName( _L("videocliprec.3gp") );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetVideoFileNameWhenRecording ) );
    assertL( err != KErrNone );

    assertL( iSettingsClient->CountInitComplete() == 1 );
    }

/********************************************************/
/*             VIDEO RECORDING TIME INTERVAL            */
/********************************************************/

void CCaeTestVideoSettings::TestSetVideoTimeIntervalSubQcifWhenPreparedL()
    {
	// ID: CAE.SET-VR.SETVIDEOTIMEINTERVAL.SUBQCIF.001
    //
    // Group:   Settings.
    // 
    // Action : Mcaeo VideoRecordingTime when video prepared. 
    //          Inform about video recording time elapsed.
	//      
	// Output : OK.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );

    iSettingsClient->SetTimeout( 3000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iSettingsClient->SetVideoFrameSize( sizeSubQcif );
    iSettingsClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iSettingsClient->SetVideoBitRate( TInt( 25000 ) );

    iSettingsClient->SetVideoFileName( _L("cae_time_interval_prepared_subqcif.3gp") );

    TTimeIntervalMicroSeconds videoTimeInterval( 400000 );
    
    iSettingsClient->SetVideoTimesInterval( videoTimeInterval );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetVideoTimesIntervalWhenVideoPrepared );
    
    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountVideoPrepareComplete() == 1 );
    assertL( iSettingsClient->CountVideoRecordingOn() == 2 );
    assertL( iSettingsClient->CountVideoRecordingPaused() == 1 );
    assertL( iSettingsClient->CountVideoRecordingComplete() == 1 );
    assertL( iSettingsClient->CountVideoRecordingTimes() >= 3 );
    }

void CCaeTestVideoSettings::TestSetVideoTimeIntervalSubQcifWhenRecordingL()
    {
	// ID: CAE.SET-VR.SETVIDEOTIMEINTERVAL.SUBQCIF.002
    //
    // Group:   Settings.
    // 
    // Action : Mcaeo VideoRecordingTime when video recording. 
    //          Inform about video recording time elapsed.
	//      
	// Output : OK.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );

    iSettingsClient->SetTimeout( 5000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iSettingsClient->SetVideoFrameSize( sizeSubQcif );
    iSettingsClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iSettingsClient->SetVideoBitRate( TInt( 25000 ) );

    iSettingsClient->SetVideoFileName( _L("cae_time_interval_recording_subqcif.3gp") );

    TTimeIntervalMicroSeconds videoTimeInterval( 400000 );
    
    iSettingsClient->SetVideoTimesInterval( videoTimeInterval );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetVideoTimesIntervalWhenVideoRecording ) );
    assertL( err == KErrNotReady );
    
    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountVideoPrepareComplete() == 1 );
    assertL( iSettingsClient->CountVideoRecordingOn() == 1 );
    assertL( iSettingsClient->CountVideoRecordingComplete() == 1 );
    assertL( iSettingsClient->CountVideoRecordingTimes() >= 2 );

    }

void CCaeTestVideoSettings::TestSetZeroVideoTimeIntervalSubQcifWhenPreparedL()
    {
	// ID: CAE.SET-VR.SETVIDEOTIMEINTERVAL.SUBQCIF.003
    //
    // Group:   Settings.
    // 
    // Action : Mcaeo VideoRecordingTime when video prepared. 
    //          Inform about video recording time elapsed.
	//      
	// Output : OK.   

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );

    iSettingsClient->SetTimeout( 3000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iSettingsClient->SetVideoFrameSize( sizeSubQcif );
    iSettingsClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iSettingsClient->SetVideoBitRate( TInt( 25000 ) );

    iSettingsClient->SetVideoFileName( _L("cae_time_interval_zero_prepared_subqcif.3gp") );

    TTimeIntervalMicroSeconds videoTimeInterval( 0 );
    
    iSettingsClient->SetVideoTimesInterval( videoTimeInterval );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetVideoTimesIntervalWhenVideoPrepared );
    
    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountVideoPrepareComplete() == 1 );
    assertL( iSettingsClient->CountVideoRecordingOn() == 2 );
    assertL( iSettingsClient->CountVideoRecordingPaused() == 1 );
    assertL( iSettingsClient->CountVideoRecordingComplete() == 1 );
    assertL( iSettingsClient->CountVideoRecordingTimes() == 0 );
    }

void CCaeTestVideoSettings::TestSetVideoTimeIntervalNegativeL()
    {
	// ID: CAE.SET-VR.SETVIDEOTIMEINTERVAL.SUBQCIF.004
    //
    // Group:   Settings.
    // 
    // Action : Mcaeo VideoRecordingTime when video prepared. 
    //          Set illegal interval
	//      
	// Output : Leave.

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );

    iSettingsClient->SetTimeout( 3000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iSettingsClient->SetVideoFrameSize( sizeSubQcif );
    iSettingsClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iSettingsClient->SetVideoBitRate( TInt( 25000 ) );

    iSettingsClient->SetVideoFileName( _L("cae_time_interval_prepared_subqcif_neg.3gp") );

    TTimeIntervalMicroSeconds videoTimeInterval( -1 );
    
    iSettingsClient->SetVideoTimesInterval( videoTimeInterval );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetVideoTimesIntervalWhenVideoPrepared ) );
    assertL( err == KErrArgument );
    
    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountVideoPrepareComplete() == 1 );
    }

void CCaeTestVideoSettings::TestSetVideoTimeIntervalHugeL()
    {
	// ID: CAE.SET-VR.SETVIDEOTIMEINTERVAL.SUBQCIF.005
    //
    // Group:   Settings.
    // 
    // Action : Mcaeo VideoRecordingTime when video prepared. 
    //          Set illegal interval
	//      
	// Output : Leave.

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );

    iSettingsClient->SetTimeout( 3000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iSettingsClient->SetVideoFrameSize( sizeSubQcif );
    iSettingsClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iSettingsClient->SetVideoBitRate( TInt( 25000 ) );

    iSettingsClient->SetVideoFileName( _L("cae_time_interval_prepared_subqcif_neg.3gp") );

    TTimeIntervalMicroSeconds videoTimeInterval( KMaxTUint32 );
    
    iSettingsClient->SetVideoTimesInterval( videoTimeInterval );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::ESetVideoTimesIntervalWhenVideoPrepared ) );
    assertL( err == KErrArgument );
    
    assertL( iSettingsClient->CountInitComplete() == 1 );
    assertL( iSettingsClient->CountVideoPrepareComplete() == 1 );
    }

/********************************************************/
/*                 GET VIDEO FRAME SIZE                 */
/********************************************************/

void CCaeTestVideoSettings::TestGetVideoFrameSizeWhenInitializedL()
    {
	// ID:      CAE.VR.GETVIDEOFRAMESIZE.001
    //
    // Group:   Record.
    // 
    // Action : Get the video frame size of the specified 
    //          quality level, when initialization is done.
	//      
	// Output : Compare.  

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetVideoFrameSizeWhenInitialized ) );
    assertL( err == KErrNone );

    TSize size( KImgWidthQCIF, KImgHeightQCIF );

    TSize videoFrameSize;
    iSettingsClient->GetVideoFrameSize( videoFrameSize );
    if ( videoFrameSize != size )
        {
        User::Leave ( KErrGetNotEqualToValuePreviouslySet );
        }
    }

void CCaeTestVideoSettings::TestGetVideoFrameSizeWhenPreparedL()
    {
	// ID:      CAE.VR.GETVIDEOFRAMESIZE.002
    //
    // Group:   Record.
    // 
    // Action : Get the video frame size of the specified 
    //          quality level, when prepared.
	//      
	// Output : Compare.  

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EGetVideoFrameSizeWhenPrepared );

    TSize size( KImgWidthQCIF, KImgHeightQCIF );

    TSize videoFrameSize;
    iSettingsClient->GetVideoFrameSize( videoFrameSize );
    if ( videoFrameSize != size )
        {
        User::Leave ( KErrGetNotEqualToValuePreviouslySet );
        }
    }

/********************************************************/
/*                  VIDEO FRAME RATE                    */
/********************************************************/

void CCaeTestVideoSettings::TestVideoFrameRateWhenInitializedL()
    {
	// ID:      CAE.VR.VIDEOFRAMERATE.001
    //
    // Group:   Record.
    // 
    // Action : Get the video frame rate of the specified 
    //          quality level, when initialization is done.
	//      
	// Output : Compare.  

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EVideoFrameRateWhenInitialized ));
    assertL( err == KErrNone );

    TReal32 expectedResult( 15.0 );

    TReal32 videoFrameRate = iSettingsClient->VideoFrameRate();
    if ( videoFrameRate != expectedResult )
        {
        User::Leave ( KErrGetNotEqualToValuePreviouslySet );
        }
    }

void CCaeTestVideoSettings::TestVideoFrameRateWhenPreparedL()
    {
	// ID:      CAE.VR.VIDEOFRAMERATE.002
    //
    // Group:   Record.
    // 
    // Action : Get the video frame rate of the specified 
    //          quality level, when prepared.
	//      
	// Output : Compare.  

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EVideoFrameRateWhenPrepared );

    TReal32 expectedResult( 15.0 );

    TReal32 videoFrameRate = iSettingsClient->VideoFrameRate();
    if ( videoFrameRate != expectedResult )
        {
        User::Leave ( KErrGetNotEqualToValuePreviouslySet );
        }
    }

/********************************************************/
/*          ESTIMATED VIDEO RECORDING BIT RATE           */
/********************************************************/

void CCaeTestVideoSettings::TestEstimatedVideoRecordingBitRateWhenInitializedL()
    {
	// ID:      CAE.VR.ESTIMATEDVIDEORECORDINGBITRATE.001
    //
    // Group:   Record.
    // 
    // Action : Get the estimated video recording bit rate including audio bit rate 
    //          (for specified video quality level) when initialization is done.
	//      
	// Output : Compare.  

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    TRAPD( err, iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EEstimatedVideoRecordingBitRateWhenInitialized ) );
    assertL( err == KErrNone );

    TInt videoBitRate;
    videoBitRate = iSettingsClient->EstimatedVideoRecordingBitRate();
    if ( videoBitRate != 78000 )
        {
        User::Leave ( KErrGetNotEqualToValuePreviouslySet );
        }
    }

void CCaeTestVideoSettings::TestEstimatedVideoRecordingBitRateWhenPreparedL()
    {
	// ID:      CAE.VR.ESTIMATEDVIDEORECORDINGBITRATE.002
    //
    // Group:   Record.
    // 
    // Action : Get the estimated video recording bit rate including audio bit rate 
    //          (for specified video quality level) when prepared.
	//      
	// Output : Compare.  

    iSettingsClient->SetOpMode( CCaeTestSettingsClient::EClientOpModeVideo );

    iSettingsClient->SetVideoQualityLevelIndex( 0 );

    iSettingsClient->RunTestActionL( CCaeTestSettingsClient::EEstimatedVideoRecordingBitRateWhenPrepared );

    TInt videoBitRate;
    videoBitRate = iSettingsClient->EstimatedVideoRecordingBitRate();
    if ( videoBitRate != 78000 )
        {
        User::Leave ( KErrGetNotEqualToValuePreviouslySet );
        }
    }

//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 

MTest* CCaeTestVideoSettings::suiteL ()
{
	// Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL( _L8("CCaeTestVideoSettings") );
    
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOCLIPMAXSIZEL.001"), &TestVideoClipMaxSize0L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOCLIPMAXSIZEL.003"), &TestVideoClipMaxSizeNeg1L ) );	

// NOT IN USE:    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOCLIPMAXSIZEL.004"), &TestVideoClipMaxSize10001L ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETZOOMVALUEL.008"), &TestDigiZoomVal2L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETZOOMVALUEL.009"), &TestDigiZoomNegVal1L ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETZOOMMODEL.006"), &TestDigiZoomMode0L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETZOOMMODEL.007"), &TestOpticalZoomMode1L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETZOOMMODEL.008"), &TestOpticalDigiZoomMode2L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETZOOMMODEL.009"), &TestDigiZoomNegMode1L ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETBRIGHTNESSL.009"), &TestBrightness0L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETBRIGHTNESSL.010"), &TestBrightness100L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETBRIGHTNESSL.011"), &TestBrightnessNeg100L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETBRIGHTNESSL.012"), &TestBrightness101L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETBRIGHTNESSL.013"), &TestBrightnessNeg101L ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETCONTRASTL.009"), &TestContrast0L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETCONTRASTL.010"), &TestContrast100L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETCONTRASTL.011"), &TestContrastNeg100L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETCONTRASTL.012"), &TestContrast101L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETCONTRASTL.013"), &TestContrastNeg101L ) );
	
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETEXPOSUREMODEL.010"), &TestExposureMode2L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETEXPOSUREMODEL.011"), &TestExposureMode4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETEXPOSUREMODEL.012"), &TestExposureModeNeg1L ) );
	
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETEXPOSUREMODEL.013"), &TestExposureMode5L ) );
	
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETWHITEBALANCEMODEL.011"), &TestWhiteBalanceMode1L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETWHITEBALANCEMODEL.012"), &TestWhiteBalanceMode2L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETWHITEBALANCEMODEL.013"), &TestWhiteBalanceMode4L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETWHITEBALANCEMODEL.014"), &TestWhiteBalanceMode8L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETWHITEBALANCEMODEL.015"), &TestWhiteBalanceMode10L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETWHITEBALANCEMODEL.016"), &TestWhiteBalanceModeNeg1L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETWHITEBALANCEMODEL.017"), &TestWhiteBalanceMode5L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETWHITEBALANCEMODEL.018"), &TestWhiteBalanceMode12L ) );
	
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETFLASHMODEL.009"), &TestFlashMode0L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETFLASHMODEL.010"), &TestFlashMode1L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETFLASHMODEL.011"), &TestFlashMode2L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETFLASHMODEL.012"), &TestFlashMode4L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETFLASHMODEL.013"), &TestFlashMode8L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETFLASHMODEL.014"), &TestFlashModeNeg1L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETFLASHMODEL.015"), &TestFlashMode5L ) );	
    
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOAUDIO.001"), &TestVideoAudioETrueL ) );
       
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOAUDIO.002"), &TestVideoAudioEFalseL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETEXPOSUREMODEL.014"), &TestExposureModeRecording2L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETWHITEBALANCEMODEL.018"), &TestWhiteBalanceModeRecording2L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETFLASHMODEL.016"), &TestFlashModeRecording2L ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETZOOMMODEL.010"), &TestDigiZoomRecordingMode0L ) );
	
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOCLIPMAXSIZEL.005"), &TestVideoClipMaxSizeRecording10000L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOCLIPMAXSIZEL.006"), &TestVideoClipMaxSizeNotInitialized10000L ) );
    
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOAUDIO.003"), &TestVideoAudioRecordingETrueL ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOAUDIO.004"), &TestVideoAudioNotInitializedL ) );
	suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.GET-VR.GETVIDEOAUDIO.005"), &TestGetVideoAudioNotInitializedL ) );

    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEORECORDINGFILENAMEL.001"), &TestSetVideoFileNameL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEORECORDINGFILENAMEL.002"), &TestSetEmptyVideoFileNameL ) );  
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEORECORDINGFILENAMEL.003"), &TestSetVideoFileNameWhenPreparedL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEORECORDINGFILENAMEL.004"), &TestSetVideoFileNameWhenNotInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEORECORDINGFILENAMEL.005"), &TestSetVideoFileNameWhenRecordingL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOTIMEINTERVAL.SUBQCIF.001"), &TestSetVideoTimeIntervalSubQcifWhenPreparedL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOTIMEINTERVAL.SUBQCIF.002"), &TestSetVideoTimeIntervalSubQcifWhenRecordingL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOTIMEINTERVAL.SUBQCIF.003"), &TestSetZeroVideoTimeIntervalSubQcifWhenPreparedL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOTIMEINTERVAL.SUBQCIF.004"), &TestSetVideoTimeIntervalNegativeL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.SET-VR.SETVIDEOTIMEINTERVAL.SUBQCIF.005"), &TestSetVideoTimeIntervalHugeL ) );

    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.VR.GETVIDEOFRAMESIZE.001"), &TestGetVideoFrameSizeWhenInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.VR.GETVIDEOFRAMESIZE.002"), &TestGetVideoFrameSizeWhenPreparedL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.VR.VIDEOFRAMERATE.001"), &TestVideoFrameRateWhenInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.VR.VIDEOFRAMERATE.002"), &TestVideoFrameRateWhenPreparedL ) );

    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.VR.ESTIMATEDVIDEORECORDINGBITRATE.001"), &TestEstimatedVideoRecordingBitRateWhenInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoSettings>::NewL( _L8("CAE.VR.ESTIMATEDVIDEORECORDINGBITRATE.002"), &TestEstimatedVideoRecordingBitRateWhenPreparedL ) );
    
    return suite;
}

