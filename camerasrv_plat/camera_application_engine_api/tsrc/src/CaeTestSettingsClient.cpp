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
* Description: CaeTestSettingsClient.cpp
*
*/
#include <fbs.h>

#include "CaeTestSettingsClient.h"
//#include "mmcleanup.h"

//_LIT8(KVideoMimeType, "video/3gpp");
//_LIT(KVideoPreferredSupplier, "Nokia");
//_LIT8(KVideoType, "video/H263-2000");
//_LIT8(KVideoAudioType, " AMR");

const TUid KSdUidQltyLevels = {KNullUidValue};
const TUid KSdIllegalUidQltyLevels = {666};

CCaeTestSettingsClient::CCaeTestSettingsClient() 
    {
    }


CCaeTestSettingsClient::~CCaeTestSettingsClient()
    {
    }


CCaeTestSettingsClient* CCaeTestSettingsClient::NewL()
    {
    CCaeTestSettingsClient* self = new(ELeave) CCaeTestSettingsClient;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


void CCaeTestSettingsClient::ConstructL()
    {
    CCaeTestClient::ConstructL();

    iRunWithViewFinder = EFalse;
    }


void CCaeTestSettingsClient::RunTestActionL( TestClientActions aAction )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestVideoSettingsClient::RunTestActionL() entering" ) );
    #endif
    
    iAction = aAction;
	iSavedAction = aAction;

    // Initialize all state variables.
    InitStateVariables();

    iCamAppEngine->InitL( KSdUidQltyLevels );

	// Main part of program is a wait loop.
	// This function completes when the scheduler stops.
	CActiveScheduler::Start();
    
    if ( iError )
        {
    	#ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestSettingsClient::RunTestActionL() leaving (error detected)" ) );
        #endif
        User::Leave( iError );
        }
	
    #ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestSettingsClient::RunTestActionL() returning" ) );
    #endif
    }


void CCaeTestSettingsClient::RunTrappedL()
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestSettingsClient::RunTrappedL() entering" ) );
    #endif

    if ( iStop ) 
        {
        if ( iCamAppEngine->IsViewFinding() )
	        {
            iCamAppEngine->StopViewFinder();
            iViewFinderStarted = EFalse;
            }
        iCamAppEngine->CancelCaptureStill();
        iCamAppEngine->CloseVideoRecording();
        delete iBitmapSave;
        iBitmapSave = NULL;
        delete iDataSave;
        iDataSave = NULL;
        CActiveScheduler::Stop();
        }
    else if ( iInitReady ) 
		{
        
        if ( iAction != ESetVideoFileNameWhenNotInitialized &&
             iAction != ESetAudioWhenVideoNotInitialized &&
             iAction != ESetClipMaxSizeWhenVideoNotInitialized &&
             iAction != EGetAudioWhenVideoNotInitialized )
            {
            iCamAppEngine->InitVideoRecorderL( KSdUidQltyLevels );
            }

        User::After( 1000000 ); // Give some time for camera drivers to settle.

        switch ( iAction )
            {
            case EGetInfo:
                iCamAppEngine->GetInfo( iInfo );
                iStop = ETrue;
                break;
            case ESetVideoFileNameWhenInitialized:
            case ESetVideoFileNameWhenNotInitialized:
                iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName );
                iStop = ETrue;
                break;
            case ESetEmptyVideoFileNameWhenInitialized:
                iCamAppEngine->SetVideoRecordingFileNameL( _L("") );
                iStop = ETrue;
                break;
            case ESetDigiZoomWhenInitialized:
                iAction = EDefault;
                iCamAppEngine->SetZoomModeL( static_cast<CCaeEngine::TZoomMode>( iZoomMode ) );
                iCamAppEngine->SetZoomValueL( iDigiZoomValue );
                break;
            case ESetBrightnessWhenInitialized:
                iAction = EDefault;
                iCamAppEngine->SetBrightnessL( iBrightness );
                break;
            case ESetContrastWhenInitialized:
                {
                iAction = EDefault;

                //Test before setting, value is unknown?
                TInt contrast = iCamAppEngine->Contrast();

                iCamAppEngine->SetContrastL( iContrast );

                //Test immediately that value is right
                contrast = iCamAppEngine->Contrast();
                if ( contrast != iContrast )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetExposureModeWhenInitialized:
                iAction = EDefault;
                iCamAppEngine->SetExposureModeL( static_cast<CCamera::TExposure>( iExposureMode ) );
                break;
            case ESetEvCompensationWhenInitialized:
                {
                // Read value before setting, should be zero if not supported
                TInt evCompensation = iCamAppEngine->EvCompensation();
                if ( evCompensation != 0 )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                iCamAppEngine->SetEvCompensationL( iEvCompensation );
                }
                break;
            case ESetWhiteBalanceModeWhenInitialized:
                iAction = EDefault;
                iCamAppEngine->SetWhiteBalanceModeL( static_cast<CCamera::TWhiteBalance>( iWhiteBalanceMode ) );
                break;
            case ESetFlashModeWhenInitialized:
                iAction = EDefault;
                iCamAppEngine->SetFlashModeL( static_cast<CCamera::TFlash>( iFlashMode ) );
                break;
            case EResetToDefaults:
                // iAction = EDefault;
                iCamAppEngine->SetZoomModeL( static_cast<CCaeEngine::TZoomMode>( iZoomMode ) );
                iCamAppEngine->SetZoomValueL( iDigiZoomValue );
                if ( TCameraInfo::EBrightnessSupported & iInfo.iOptionsSupported )
		            {
                    iCamAppEngine->SetBrightnessL( iBrightness );
                    }
                if ( TCameraInfo::EContrastSupported & iInfo.iOptionsSupported )
		            {
                    iCamAppEngine->SetContrastL( iContrast );
                    }
                iCamAppEngine->SetExposureModeL( static_cast<CCamera::TExposure>( iExposureMode ) );
                iCamAppEngine->SetWhiteBalanceModeL( static_cast<CCamera::TWhiteBalance>( iWhiteBalanceMode ) );
                iCamAppEngine->SetFlashModeL( static_cast<CCamera::TFlash>( iFlashMode ) );
                break;
            case ESetViewFinderMirror:
                {
                TBool mirrorVF = iCamAppEngine->ViewFinderMirror();
                if ( mirrorVF )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                iAction = EDefault;
                iCamAppEngine->SetViewFinderMirrorL( ETrue );
                break;
            case EGetStillFrameSizeWhenInitialized:
                // Test with invalid quality level index.
                iCamAppEngine->GetStillFrameSize( -1, iStillFrameSizeSet );
                iCamAppEngine->GetStillFrameSize( 666, iStillFrameSizeSet );
                // Test with valid quality level index.
                iCamAppEngine->GetStillFrameSize( iStillQualityLevelIndex, iStillFrameSizeSet );
                iStop = ETrue;
                break;
            case EEstimatedStillSizeInBytesWhenInitialized:
                // Test with invalid quality level index.
                iEstimatedStillSizeInBytesSet = 
                    iCamAppEngine->EstimatedStillSizeInBytes( -1 );
                iEstimatedStillSizeInBytesSet = 
                    iCamAppEngine->EstimatedStillSizeInBytes( 666 );
                // Test with valid quality level index.
                iEstimatedStillSizeInBytesSet = 
                    iCamAppEngine->EstimatedStillSizeInBytes( iStillQualityLevelIndex );
                iStop = ETrue;
                break;
            case EGetVideoFrameSizeWhenInitialized:
                {
                // test first with invalid index values: negative and too big, should not modify data
                    TSize tempFrameSize1( KImgWidthQCIF, KImgHeightQCIF );
                    TSize tempFrameSize2( KImgWidthQCIF, KImgHeightQCIF );

                    iCamAppEngine->GetVideoFrameSize( -1, tempFrameSize1 );
                    if ( tempFrameSize1 != tempFrameSize2 )
                        {
                        User::Leave ( KErrArgument );
                        }
                    iCamAppEngine->GetVideoFrameSize( 666, tempFrameSize1 );
                    if ( tempFrameSize1 != tempFrameSize2 )
                        {
                        User::Leave ( KErrArgument );
                        }
                    iCamAppEngine->GetVideoFrameSize( iVideoQualityLevelIndex, iVideoFrameSizeSet );
                    iStop = ETrue;
                }
                break;
            case EVideoFrameRateWhenInitialized:
                {
                // test first with invalid index values: negative and too big, should return 0
                iVideoFrameRateSet = iCamAppEngine->VideoFrameRate( -1 );
                if ( iVideoFrameRateSet != 0 )
                    {
                    User::Leave ( KErrArgument );
                    }
                iVideoFrameRateSet = iCamAppEngine->VideoFrameRate( 666 );
                if ( iVideoFrameRateSet != 0 )
                    {
                    User::Leave ( KErrArgument );
                    }
                iVideoFrameRateSet = iCamAppEngine->VideoFrameRate( iVideoQualityLevelIndex );
                iStop = ETrue;
                }
                break;
            case EEstimatedVideoRecordingBitRateWhenInitialized:
                // test first with invalid index values: negative and too big, should return 0
                iEstimatedVideoBitRateSet = 
                    iCamAppEngine->EstimatedVideoRecordingBitRateL( -1 );
                if ( iEstimatedVideoBitRateSet != 0 )
                    {
                    User::Leave ( KErrArgument );
                    }
                iEstimatedVideoBitRateSet = 
                    iCamAppEngine->EstimatedVideoRecordingBitRateL( 666 );
                if ( iEstimatedVideoBitRateSet != 0 )
                    {
                    User::Leave ( KErrArgument );
                    }
                iEstimatedVideoBitRateSet = 
                    iCamAppEngine->EstimatedVideoRecordingBitRateL( iVideoQualityLevelIndex );
                iStop = ETrue;
                break;
            case ESetClipMaxSizeWhenVideoNotPrepared:
                iAction = EDefault;
                iCamAppEngine->SetVideoClipMaxSizeL( iVideoMaxClipSizeInBytes );
                break;
            /*
            case EPrepareAndSetStillBurstCaptureInterval:
                iAction = EDefault;
                iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
			    break;
            */
            case EInitUid:
                iCamAppEngine->InitL( KSdIllegalUidQltyLevels );// will leave
                iStop = ETrue;
                break;
            case ESetAudioWhenVideoNotInitialized:
                iAction = EDefault;
                iCamAppEngine->SetVideoAudioL( iVideoAudioEnabled );
                break;
            case ESetClipMaxSizeWhenVideoNotInitialized:
                iAction = ENone;
                iCamAppEngine->SetVideoClipMaxSizeL( iVideoMaxClipSizeInBytes );
                break;
            case EGetAudioWhenVideoNotInitialized:
                {
                iStop = ETrue;
                TBool audio = iCamAppEngine->VideoAudio();
                if ( audio != EFalse )
                    {
                    User::Leave ( KErrArgument );
                    }
                }
                break;
            case EEnumerateVideoFrameSize:
                {
                iStop = ETrue;
                TInt enumIndVGA;
                TInt enumIndCIF;
                TInt enumIndQCIF;
                TInt enumIndBIG;
                TInt enumIndZERO;
                TSize sizeVGA( KImgWidthVGA, KImgHeightVGA );
                TSize sizeCIF( KImgWidthCIF, KImgHeightCIF );
                TSize sizeQCIF( KImgWidthQCIF, KImgHeightQCIF );
                TSize sizeBIG( KImgWidthTooBig, KImgHeightTooBig );
                TSize sizeZERO( 0, 0 );

                enumIndVGA  = iCamAppEngine->EnumerateVideoFrameSizeL( sizeVGA );
                enumIndCIF  = iCamAppEngine->EnumerateVideoFrameSizeL( sizeCIF );
                enumIndQCIF = iCamAppEngine->EnumerateVideoFrameSizeL( sizeQCIF );
                enumIndBIG  = iCamAppEngine->EnumerateVideoFrameSizeL( sizeBIG );
                enumIndZERO = iCamAppEngine->EnumerateVideoFrameSizeL( sizeZERO );

                if ( ( enumIndVGA < 0 && enumIndCIF < 0 && enumIndQCIF < 0 ) ||
                    enumIndBIG >= 0 || enumIndZERO >= 0 )
                    {
                    User::Leave ( KErrNotSupported );
                    }
                }
                break;
                
            case EInstalledExtensionList:
                {
                RCaeOrderedFeatureList installedExtensions1;
                RCaeOrderedFeatureList installedExtensions2;
                TCaeOrderedFeatureListItem item;

                // call the get function
                iCamAppEngine->GetOrderL( installedExtensions1 );

                // test that cae resets the parameter before usage
                item.iFeatureUid = KSdIllegalUidQltyLevels;
                installedExtensions2.Append( item );
                iCamAppEngine->GetOrderL( installedExtensions2 );
                
                // Check that the both arrays are the same
                if ( installedExtensions1.Count() != installedExtensions2.Count() )
                    {
                    installedExtensions1.Reset();
                    installedExtensions2.Reset();
                    RDebug::Print( _L( "Cae: CCaeTestSettingsClient: CAE doesn't reset the array" ) );
                    User::Leave ( KErrArgument );
                    }
                for(TInt i=0; i < installedExtensions1.Count(); i++ )
                    {
                    if ( installedExtensions1[i].iServiceUid != installedExtensions2[i].iServiceUid ||
                        installedExtensions1[i].iFeatureUid != installedExtensions2[i].iFeatureUid )
                        {
                        installedExtensions1.Reset();
                        installedExtensions2.Reset();
                        RDebug::Print( _L( "Cae: CCaeTestSettingsClient: CAE the array is not same" ) );
                        User::Leave ( KErrArgument );
                        }
                    }
                installedExtensions1.Reset();
                installedExtensions2.Reset();
                iStop = ETrue;
                }
                break;

            case ESkippedExtensionList:
                {
                RCaeOrderedFeatureList skippedExtensions1;
                RCaeOrderedFeatureList skippedExtensions2;
                TCaeOrderedFeatureListItem item;
                // call get function twice
                iCamAppEngine->SkippedExtensionListL( skippedExtensions1 );
                // test that cae resets non-empty parameter
                item.iFeatureUid = KSdIllegalUidQltyLevels;
                skippedExtensions2.Append( item );
                iCamAppEngine->SkippedExtensionListL( skippedExtensions2 );
                // Check that the both arrays are the same
                if ( skippedExtensions1.Count() != skippedExtensions2.Count() )
                    {
                    skippedExtensions1.Reset();
                    skippedExtensions2.Reset();
                    RDebug::Print( _L( "Cae: CCaeTestSettingsClient: CAE doesn't reset the array" ) );
                    User::Leave ( KErrArgument );
                    }
                for(TInt i=0; i < skippedExtensions1.Count(); i++ )
                    {
                    if ( skippedExtensions1[i].iServiceUid != skippedExtensions2[i].iServiceUid ||
                        skippedExtensions1[i].iFeatureUid != skippedExtensions2[i].iFeatureUid )
                        {
                        skippedExtensions1.Reset();
                        skippedExtensions2.Reset();
                        RDebug::Print( _L( "Cae: CCaeTestSettingsClient: array is not same" ) );
                        User::Leave ( KErrArgument );
                        }
                    }
                skippedExtensions1.Reset();
                skippedExtensions2.Reset();
                iStop = ETrue;
                }
                break;
                
            case ESetSkippedExtensionList:
                {
                RCaeOrderedFeatureList skippedExtensions1;
                RCaeOrderedFeatureList skippedExtensions2;
                TCaeOrderedFeatureListItem item;

                // call set function twice
                iCamAppEngine->SetSkippedExtensionList( skippedExtensions1 );
                iCamAppEngine->SetSkippedExtensionList( skippedExtensions1 );
                
                // read and verify
                iCamAppEngine->SkippedExtensionListL( skippedExtensions1 );
                if ( skippedExtensions1.Count() != 0 )
                    {
                    skippedExtensions1.Reset();
                    RDebug::Print( _L( "Cae: CCaeTestSettingsClient: get/set error" ) );
                    User::Leave ( KErrArgument );
                    }

                // Add one UID
                item.iFeatureUid = KSdIllegalUidQltyLevels;
                skippedExtensions1.Append( item );
                iCamAppEngine->SetSkippedExtensionList( skippedExtensions1 );
                iCamAppEngine->SetSkippedExtensionList( skippedExtensions1 );
                
                // read and verify
                skippedExtensions1.Reset();
                iCamAppEngine->SkippedExtensionListL( skippedExtensions1 );
                if ( skippedExtensions1.Count() != 0 )
                    {
                    skippedExtensions1.Reset();
                    RDebug::Print( _L( "Cae: CCaeTestSettingsClient: set inv. error" ) );
                    User::Leave ( KErrArgument );
                    }

                // call the get function to get installed extensions
                iCamAppEngine->GetOrderL( skippedExtensions1 );

                // Add one invalid UID and set all skipped
                item.iFeatureUid = KSdIllegalUidQltyLevels;
                skippedExtensions1.Append( item );
                iCamAppEngine->SetSkippedExtensionList( skippedExtensions1 );

                // read and verify that both arrays are the same
                iCamAppEngine->SkippedExtensionListL( skippedExtensions1 );
                iCamAppEngine->GetOrderL( skippedExtensions2 );

                // Check that the both arrays are the same
                if ( skippedExtensions1.Count() != skippedExtensions2.Count() )
                    {
                    skippedExtensions1.Reset();
                    skippedExtensions2.Reset();
                    RDebug::Print( _L( "Cae: CCaeTestSettingsClient: CAE doesn't reset the array" ) );
                    User::Leave ( KErrArgument );
                    }
                for(TInt i=0; i < skippedExtensions1.Count(); i++ )
                    {
                    if ( skippedExtensions1[i].iServiceUid != skippedExtensions2[i].iServiceUid ||
                        skippedExtensions1[i].iFeatureUid != skippedExtensions2[i].iFeatureUid )
                        {
                        skippedExtensions1.Reset();
                        skippedExtensions2.Reset();
                        RDebug::Print( _L( "Cae: CCaeTestSettingsClient: array is not same" ) );
                        User::Leave ( KErrArgument );
                        }
                    }

                skippedExtensions1.Reset();
                skippedExtensions2.Reset();
                iStop = ETrue;
                }
                break;

            default:
                 //iVideoRecordingOn = EFalse;
                 //iCamAppEngine->StopVideoRecording();
                break;
            }

        User::After( 1000000 ); // Give some time for camera drivers to settle.
        
        if ( ( iAction != ENone ) && !iStop)
            {
            iInitReady = EFalse;
            iViewFinderStarted = EFalse;
            iStillPrepareReady = EFalse;
            iVideoPrepareReady = EFalse;
            switch ( iOpMode )
                {
                case EClientOpModeStill:
					switch ( iAction )
						{
						case ESetEvCompensationWhenInitialized:
						case ESetEvCompensationWhenStillPrepared:
						case ESetJpegQualityWhenStillPrepared:
						case EProcessExtCapturedImageBurst:
						case EProcessExtCapturedImageBurstTDesC8:
							if ( iAction == ESetEvCompensationWhenInitialized )
								{
								iAction = EDefault;
								}
							iCamAppEngine->PrepareStillCaptureL( iStillFrameSize, 
																 iStillDataFormat, 
																 iStillCompressionQuality );
							break;
						case ESetSnapImageSizeBeforePrepare:
							iCamAppEngine->SetSnapImageSizeL( iSnapImageSize );
							iCamAppEngine->PrepareStillCaptureL( iStillFrameSize, 
																 iStillDataFormat, 
																 iStillCompressionQuality );
							break;
						case ESetSnapImageSizeDuringAndAfterPrepare:
							iCamAppEngine->PrepareStillCaptureL( iStillFrameSize, 
																 iStillDataFormat, 
																 iStillCompressionQuality,
																 TRect(),
																 iSnapImageSize );
							break;
		    			case ESetJpegCodecWhenStillPrepared: 
                			iCamAppEngine->SetImageCodecsL( iImageDecoderUid, iImageEncoderUid );
                			iCamAppEngine->PrepareStillCaptureL( iStillFrameSize, 
                                                     			 iStillDataFormat, 
                                                     			 iStillCompressionQuality );
			    			break;
						default:
							iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
							break;
						}
			        break;
                case EClientOpModeVideo:
                    iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName );
                    iCamAppEngine->PrepareVideoRecordingL( iVideoQualityLevelIndex );
			        break;
                default:
                    User::Leave( KErrNotSupported );
                    break;
               }
            }
        else
            {
            CTimer::After( KNormalTimingDelay );
            }
        }
    else if ( iStillPrepareReady ) 
		{

        if ( iRunWithViewFinder && !iCamAppEngine->IsViewFinding() )
	        {
            iCamAppEngine->StartViewFinderBitmapsL( iViewFinderSize );
            // User::After( 1000000 ); // Give some time for camera drivers to settle.
            }

        iStillPrepareReady = EFalse;

	    TBool capture = EFalse;

        switch ( iAction )
            {
            case ESetDigiZoomWhenStillPrepared:
                {
                iAction = EDefault;
                capture = ETrue;
                iCamAppEngine->SetZoomModeL( static_cast<CCaeEngine::TZoomMode>( iZoomMode ) );
                /*TInt zoomValue1 =*/ iCamAppEngine->ZoomValue();
				iCamAppEngine->SetZoomValueL( iDigiZoomValue );
                /*TInt zoomValue2 =*/ iCamAppEngine->ZoomValue();
                }
                break;
            case ESetBrightnessWhenStillPrepared:
                iAction = EDefault;
                capture = ETrue;
                iCamAppEngine->SetBrightnessL( iBrightness );
                break;
            case ESetContrastWhenStillPrepared:
                iAction = EDefault;
                capture = ETrue;
                iCamAppEngine->SetContrastL( iContrast );
                break;
            case ESetExposureModeWhenStillPrepared:
                iAction = EDefault;
                capture = ETrue;
                iCamAppEngine->SetExposureModeL( static_cast<CCamera::TExposure>( iExposureMode ) );
                break;
            case ESetEvCompensationWhenStillPrepared:
                iAction = EDefault;
                capture = ETrue;
                iCamAppEngine->SetEvCompensationL( iEvCompensation );
                break;
            case ESetWhiteBalanceModeWhenStillPrepared:
                iAction = EDefault;
                capture = ETrue;
                iCamAppEngine->SetWhiteBalanceModeL( static_cast<CCamera::TWhiteBalance>( iWhiteBalanceMode ) );
                break;
            case ESetFlashModeWhenStillPrepared:
                iAction = EDefault;
                capture = ETrue;
                iCamAppEngine->SetFlashModeL( static_cast<CCamera::TFlash>( iFlashMode ) );
                break;
            case ESetJpegQualityWhenStillPrepared:
                iAction = EDefault;
                capture = ETrue;
// If interface after S60 2.1.
#ifndef CAE_INTERFACE_21
                iCamAppEngine->SetJpegQuality( iStillCompressionQuality );
#endif
                break;
            case EResetToDefaults:
                iAction = EDefault;
                capture = ETrue;
                iCamAppEngine->ResetToDefaultsL();
                {
                CCaeEngine::TZoomMode zoomModeR = iCamAppEngine->ZoomMode();
                if ( zoomModeR != CCaeEngine::EZoomModeDigital )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                TInt zoomValueR = iCamAppEngine->ZoomValue();
                if ( zoomValueR != 0 )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                TInt brightnessR = iCamAppEngine->Brightness();
                if ( brightnessR != 0 )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                TInt contrastR = iCamAppEngine->Contrast();
                if ( contrastR != 0 )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                TInt exposuremodeR = iCamAppEngine->ExposureMode();
                if ( exposuremodeR != CCamera::EExposureAuto )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                TInt whitebalancemodeR = iCamAppEngine->WhiteBalanceMode();
                if ( whitebalancemodeR != CCamera::EWBAuto )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                TInt flashmodeR = iCamAppEngine->FlashMode();
                if ( flashmodeR != CCamera::EFlashNone )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetViewFinderMirror:
                {
                iAction = EDefault;
                TBool mirrorVF = iCamAppEngine->ViewFinderMirror();
                capture = ETrue;
                if ( !mirrorVF )
                    {
                    User::Leave( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case EGetStillFrameSizeWhenPrepared:
                // Test with invalid quality level index.
                iCamAppEngine->GetStillFrameSize( -1, iStillFrameSizeSet );
                iCamAppEngine->GetStillFrameSize( 666, iStillFrameSizeSet );
                // Test with valid quality level index.
                iCamAppEngine->GetStillFrameSize( iStillQualityLevelIndex, iStillFrameSizeSet );
                iStop = ETrue;
                break;
            case ESetSnapImageSizeDuringAndAfterPrepare:
				iCamAppEngine->SetSnapImageSizeL( iSnapImageSize );
                capture = ETrue;
                break;
            case EEstimatedStillSizeInBytesWhenPrepared:
                // Test with invalid quality level index.
                iEstimatedStillSizeInBytesSet = 
                    iCamAppEngine->EstimatedStillSizeInBytes( -1 );
                iEstimatedStillSizeInBytesSet = 
                    iCamAppEngine->EstimatedStillSizeInBytes( 666 );
                // Test with valid quality level index.
                iEstimatedStillSizeInBytesSet = 
                    iCamAppEngine->EstimatedStillSizeInBytes( iStillQualityLevelIndex );
                iStop = ETrue;
                break;
            case EPrepareAndSetStillBurstCaptureInterval:
                {
                iImageCountToCapture = iCamAppEngine->SetStillCaptureImageCountL( iStillBurstLength );
                iCamAppEngine->SetStillBurstCaptureIntervalL( iStillBurstCaptureInterval );
                /*TInt imgCount =*/ iCamAppEngine->StillCaptureImageCount();
                capture = ETrue;
                // "prepare-version" of burst mode
                // iCamAppEngine->PrepareStillBurstL( iStillBurstLength );
                }
                break;
            default:
                capture = ETrue;
                break;
            }

        if ( capture )
			{
            iStillCaptureReady = EFalse;
            iStillCapturingOn = ETrue;
            iStillCaptured = ETrue;
	        #ifdef _DEBUG
            RDebug::Print( _L( "Cae: CCaeTestSettingsClient: calling iCamAppEngine->CaptureStill()" ) );
            #endif
            User::After( 1000000 ); // Give some time for camera drivers to settle.
			iCamAppEngine->CaptureStill();
			}
        else 
            {
            CTimer::After( KNormalTimingDelay );
            }
        
		}
    else if ( iVideoPrepareReady ) 
		{

        if ( iRunWithViewFinder && !iCamAppEngine->IsViewFinding() )
	        {
            iCamAppEngine->StartViewFinderBitmapsL( iViewFinderSize );
            // User::After( 1000000 ); // Give some time for camera drivers to settle.
            }

		TBool record( EFalse );

        iVideoPrepareReady = EFalse;

        switch ( iAction )
            {
            case ESetDigiZoomWhenVideoPrepared:
                record = ETrue;
                iCamAppEngine->SetZoomModeL( static_cast<CCaeEngine::TZoomMode>( iZoomMode ) );
                iCamAppEngine->SetZoomValueL( iDigiZoomValue );
                break;
            case ESetBrightnessWhenVideoPrepared:
                record = ETrue;
                iCamAppEngine->SetBrightnessL( iBrightness );
                break;
            case ESetContrastWhenVideoPrepared:
                record = ETrue;
                iCamAppEngine->SetContrastL( iContrast );
                break;
            case ESetExposureModeWhenVideoPrepared:
                record = ETrue;
                iCamAppEngine->SetExposureModeL( static_cast<CCamera::TExposure>( iExposureMode ) );
                break;
            case ESetEvCompensationWhenVideoPrepared:
                record = ETrue;
                iCamAppEngine->SetEvCompensationL( iEvCompensation );
                break;
            case ESetWhiteBalanceModeWhenVideoPrepared:
                record = ETrue;
                iCamAppEngine->SetWhiteBalanceModeL( static_cast<CCamera::TWhiteBalance>( iWhiteBalanceMode ) );
                break;
            case ESetFlashModeWhenVideoPrepared:
                record = ETrue;
                iCamAppEngine->SetFlashModeL( static_cast<CCamera::TFlash>( iFlashMode ) );
                break;
            case ESetVideoTimesIntervalWhenVideoPrepared:
                record = ETrue;
                iCamAppEngine->SetVideoTimesIntervalL( iVideoTimesInterval );
                break;
            case ESetVideoFileNameWhenPrepared:
                iAction = EDefault;
                iCamAppEngine->StopViewFinder();
                iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName );
			    break;
            case ESetClipMaxSizeWhenVideoPrepared:
                iAction = EDefault;
                iCamAppEngine->SetVideoClipMaxSizeL( iVideoMaxClipSizeInBytes );
                break;
            case ESetAudioWhenVideoPrepared:
                iAction = EDefault;
                iCamAppEngine->SetVideoAudioL( iVideoAudioEnabled );
                break;
            case EGetVideoFrameSizeWhenPrepared:
                iCamAppEngine->GetVideoFrameSize( iVideoQualityLevelIndex, iVideoFrameSizeSet );
                iStop = ETrue;
                break;
            case EVideoFrameRateWhenPrepared:
                iVideoFrameRateSet = iCamAppEngine->VideoFrameRate( iVideoQualityLevelIndex );
                iStop = ETrue;
                break;
            case EEstimatedVideoRecordingBitRateWhenPrepared:
                iEstimatedVideoBitRateSet = 
                    iCamAppEngine->EstimatedVideoRecordingBitRateL( iVideoQualityLevelIndex );
                iStop = ETrue;
                break;
            default:
                // iVideoPrepareReady = EFalse;
                record = ETrue;
                break;
            }
        /*
        iVideoRecordingReady = EFalse;
        iVideoRecordingOn = EFalse;
        iVideoRecorded = ETrue;
	    #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestSettingsClient: calling iCamAppEngine->StartVideoRecording()" ) );
        #endif
		iCamAppEngine->StartVideoRecording();
        */
        if ( record )
			{
            iVideoRecordingReady = EFalse;
            iVideoRecordingOn = EFalse;
            iVideoRecorded = ETrue;
	        #ifdef _DEBUG
            RDebug::Print( _L( "Cae: CCaeTestSettingsClient: calling iCamAppEngine->StartVideoRecording()" ) );
            #endif
            User::After( 1000000 ); // Give some time for camera drivers to settle.
			iCamAppEngine->StartVideoRecording();
			}
        else if ( iAction != EDefault ) 
            {
            CTimer::After( KNormalTimingDelay );
            }
		}
	else if ( iSnapImageReady )
	    {
	    iSnapImageReady = EFalse;
	    }
	else if ( iStillCaptureReady )
	    {
        // Check here if the settings were taken into use.
        // (For settings with a get-method.)
        switch ( iSavedAction )
            {
            case ESetDigiZoomWhenStillPrepared:
            case ESetDigiZoomWhenInitialized:
                {
                CCaeEngine::TZoomMode zoomMode = iCamAppEngine->ZoomMode();
                if ( zoomMode != CCaeEngine::EZoomModeDigital )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                TInt zoomValue = iCamAppEngine->ZoomValue();
                if ( zoomValue != iDigiZoomValue )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetBrightnessWhenStillPrepared:
            case ESetBrightnessWhenInitialized:
                {
                TInt brightness = iCamAppEngine->Brightness();
                if ( brightness != iBrightness )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetContrastWhenStillPrepared:
            case ESetContrastWhenInitialized:
                {
                TInt contrast = iCamAppEngine->Contrast();
                if ( contrast != iContrast )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }

                // Test poweroff case, contrast should be 0
                iCamAppEngine->PowerOff();
                contrast = iCamAppEngine->Contrast();
                if ( contrast != 0 )
                    {
                    User::Leave ( KErrArgument );
                    }
                }
                break;
            case ESetExposureModeWhenStillPrepared:
            case ESetExposureModeWhenInitialized:
                {
                TInt exposuremode = iCamAppEngine->ExposureMode();
                if ( exposuremode != iExposureMode )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetEvCompensationWhenStillPrepared:
            case ESetEvCompensationWhenInitialized:
                {
                TInt evCompensation = iCamAppEngine->EvCompensation();
                if ( evCompensation != iEvCompensation )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetJpegQualityWhenStillPrepared:
                {
// If interface after S60 2.1.
#ifndef CAE_INTERFACE_21
                TInt jpegQuality = iCamAppEngine->JpegQuality();
                if ( ( iStillCompressionQuality >= 1 ) && ( iStillCompressionQuality <= 100 ) )
                    {
                    if ( jpegQuality != iStillCompressionQuality )
                        {
                        User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                        }
                    }
                else
                    {
                    // Test clamping to the range 1-100.
                    if ( iStillCompressionQuality < 1 )
                        {
                        if ( jpegQuality != 1 )
                            {
                            User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                            }
                        }
                    else if ( iStillCompressionQuality > 100 )
                        {
                        if ( jpegQuality != 100 )
                            {
                            User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                            }
                        }
                    }
#endif
                }
                break;            
            case ESetWhiteBalanceModeWhenStillPrepared:
            case ESetWhiteBalanceModeWhenInitialized:
                {
                TInt whitebalancemode = iCamAppEngine->WhiteBalanceMode();
                if ( whitebalancemode != iWhiteBalanceMode )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetFlashModeWhenStillPrepared:
            case ESetFlashModeWhenInitialized:
                {
                TInt flashmode = iCamAppEngine->FlashMode();
                if ( flashmode != iFlashMode )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case EPrepareAndSetStillBurstCaptureInterval:
                break;
            default:
                break;
            }

        iStillCaptureReady = EFalse;
        
	    // #ifdef _DEBUG
        // RDebug::Print( _L( "Cae: CCaeTestStillSettingsClient: calling iCamAppEngine->StartViewFinderBitmapsL()" ) );
        // #endif
        // iViewFindingUsed = ETrue;
        // iCamAppEngine->StartViewFinderBitmapsL( iViewFinderSize );

        if ( iAction == EProcessExtViewFinderFrameReady ||
            iAction == EProcessExtSnapImage ||
            iAction == EProcessExtCapturedImage ||
            iAction == EProcessExtCapturedImageTDesC8 ||
            iAction == EProcessExtCapturedImageCFbsBitmap ||
            iAction == EProcessExtCapturedImageBurst ||
            iAction == EProcessExtCapturedImageBurstTDesC8 ||
            iAction == EProcessExtCancel ||
            iAction == EProcessExtCancel2 )
            {
            if ( iAction == EProcessExtCapturedImageTDesC8 )
                {
                // Test also that release should not stop the extension mode operation
                iCamAppEngine->Release();
                }
            iCamAppEngine->SetCaeExtensionModeL( ETrue, EFalse );
            iExtensionMode = ETrue;
            CTimer::After( iTimeout );
            }
        else if ( iAction == EProcessExtCapturedImageNoInit )
            {
            // test images are captured now, delete observer1 engine and create a new
            delete iCamAppEngine;
            iCamAppEngine = NULL;
            iCamAppEngine = CCaeEngine::NewL( 0 );
            iCamAppEngine->SetCamAppEngineObserver( *this );
            iCamAppEngine->SetCaeStillBurstObserver( *this );
        	iCamAppEngine->GetInfo( iInfo );
        
            // Move cae to extension mode without initl or prepare call    
            iCamAppEngine->SetCaeExtensionModeL( ETrue, EFalse );
            iExtensionMode = ETrue;
            CTimer::After( iTimeout );
            }
        else if ( iStillBurstLength == 1 )
            {
            iStop = ETrue;
            CTimer::After( iTimeout );
            }
	    }

	else if ( iStillBurstReady )
	    {
        iStillBurstReady = EFalse;

		switch ( iAction )
		    {
            case EPrepareAndSetStillBurstCaptureInterval:
		    default:
                iAction = ENone;
                {
                TTimeIntervalMicroSeconds stillburstcaptureinterval = 
                iCamAppEngine->StillBurstCaptureInterval();
                if ( stillburstcaptureinterval != iStillBurstCaptureInterval )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }

                iImageCountToCapture = iCamAppEngine->SetStillCaptureImageCountL( 1 );
                if ( iImageCountToCapture != 1 )
                    {
                    User::Leave( KErrGeneral );
                    }
                }
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
			    break;	
		    }
        }

	else if ( iVideoRecordingOn )
	    {
        switch ( iAction )
            {
            case ESetDigiZoomWhenVideoRecording:
                iAction = EDefault;
                iCamAppEngine->SetZoomModeL( static_cast<CCaeEngine::TZoomMode>( iZoomMode ) );
                iCamAppEngine->SetZoomValueL( iDigiZoomValue );
                break;
            case ESetBrightnessWhenVideoRecording:
                iAction = EDefault;
                iCamAppEngine->SetBrightnessL( iBrightness );
                break;
            case ESetContrastWhenVideoRecording:
                iAction = EDefault;
                iCamAppEngine->SetContrastL( iContrast );
                break;
            case ESetExposureModeWhenVideoRecording:
                iAction = EDefault;
                iCamAppEngine->SetExposureModeL( static_cast<CCamera::TExposure>( iExposureMode ) );
                break;
            case ESetEvCompensationWhenVideoRecording:
                iAction = EDefault;
                iCamAppEngine->SetEvCompensationL( iEvCompensation );
                break;
            case ESetWhiteBalanceModeWhenVideoRecording:
                iAction = EDefault;
                iCamAppEngine->SetWhiteBalanceModeL( static_cast<CCamera::TWhiteBalance>( iWhiteBalanceMode ) );
                break;
            case ESetFlashModeWhenVideoRecording:
                iAction = EDefault;
                iCamAppEngine->SetFlashModeL( static_cast<CCamera::TFlash>( iFlashMode ) );
                break;
            case ESetVideoTimesIntervalWhenVideoRecording:
                iAction = EDefault;
                iCamAppEngine->SetVideoTimesIntervalL( iVideoTimesInterval );
                break;
            case ESetClipMaxSizeWhenVideoRecording:
                iAction = ENone;
                iCamAppEngine->SetVideoClipMaxSizeL( iVideoMaxClipSizeInBytes );
                break;
            case ESetAudioWhenVideoRecording:
                iAction = ENone;
                iCamAppEngine->SetVideoAudioL( iVideoAudioEnabled );
                break;
            case ESetVideoFileNameWhenRecording:
                iAction = EDefault;
                iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName );
                iStop = ETrue;
                break;
            case ESetVideoTimesIntervalWhenVideoPrepared:
                iAction = ENone;
                iVideoRecordingOn = EFalse;
                iCamAppEngine->PauseVideoRecording();
                break;

            default:
                iAction = ENone;
                iVideoRecordingOn = EFalse;
                iCamAppEngine->StopVideoRecording();
                break;
            }
            
        if ( iAction != ENone )
            {
            CTimer::After( iTimeout );
            }
	    }
	else if ( iVideoRecordingPaused )
	    {
        iAction = ENone;
        iVideoRecordingPaused = EFalse;
        iCamAppEngine->ResumeVideoRecording();
	    }
	else if ( iVideoRecordingReady )
	    {
        // Check here if the settings were taken into use.
        // (For settings with a get-method.)
        switch ( iSavedAction )
            {
            case ESetDigiZoomWhenVideoPrepared:
            case ESetDigiZoomWhenVideoRecording:
                {
                CCaeEngine::TZoomMode zoomMode = iCamAppEngine->ZoomMode();
                if ( zoomMode != CCaeEngine::EZoomModeDigital )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                TInt zoomValue = iCamAppEngine->ZoomValue();
                if ( zoomValue != iDigiZoomValue )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetBrightnessWhenVideoPrepared:
            case ESetBrightnessWhenVideoRecording:
                {
                TInt brightness = iCamAppEngine->Brightness();
                if ( brightness != iBrightness )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }

                // Test poweroff case, brightness should be 0
                iCamAppEngine->PowerOff();
                brightness = iCamAppEngine->Brightness();
                if ( brightness != 0 )
                    {
                    User::Leave ( KErrArgument );
                    }
                }
                break;
            case ESetContrastWhenVideoPrepared:
            case ESetContrastWhenVideoRecording:
                {
                TInt contrast = iCamAppEngine->Contrast();
                if ( contrast != iContrast )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetExposureModeWhenVideoPrepared:
            case ESetExposureModeWhenVideoRecording:
                {
                TInt exposuremode = iCamAppEngine->ExposureMode();
                if ( exposuremode != iExposureMode )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetEvCompensationWhenVideoPrepared:
            case ESetEvCompensationWhenVideoRecording:
                {
                TInt evCompensation = iCamAppEngine->EvCompensation();
                if ( evCompensation != iEvCompensation )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetWhiteBalanceModeWhenVideoPrepared:
            case ESetWhiteBalanceModeWhenVideoRecording:
                {
                TInt whitebalancemode = iCamAppEngine->WhiteBalanceMode();
                if ( whitebalancemode != iWhiteBalanceMode )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetFlashModeWhenVideoPrepared:
            case ESetFlashModeWhenVideoRecording:
                {
                TInt flashmode = iCamAppEngine->FlashMode();
                if ( flashmode != iFlashMode )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetClipMaxSizeWhenVideoNotPrepared:
            case ESetClipMaxSizeWhenVideoPrepared:
                {
                TInt clipmaxsize = iCamAppEngine->VideoClipMaxSize();
                if ( iVideoMaxClipSizeInBytes >= 0 )
                    {
                    if ( clipmaxsize != iVideoMaxClipSizeInBytes )
                        {
                        User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                        }
                    }
                else
                    {
                    if ( clipmaxsize != 0 )
                        {
                        User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                        }
                    }
                }
                break;
            case ESetAudioWhenVideoPrepared:
                {
                TBool audio = iCamAppEngine->VideoAudio();
                if ( audio != iVideoAudioEnabled )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            case ESetVideoTimesIntervalWhenVideoPrepared:
                {
                TTimeIntervalMicroSeconds timesinterval = 
                    iCamAppEngine->VideoTimesInterval();
                if ( timesinterval != iVideoTimesInterval )
                    {
                    User::Leave ( KErrGetNotEqualToValuePreviouslySet );
                    }
                }
                break;
            default:
                break;
            }

		iAction = ENone;
        iVideoRecordingReady = EFalse;
        iStopViewFinder = ETrue;
        CTimer::After( KNormalTimingDelay );

	    }
    else if ( !iStillCaptured && !iVideoRecorded && 
              !iVideoRecordingOn && !iStopViewFinder ) 
        {
        switch ( iAction )
		    {
            case EStartVfPrepareCaptureStillStartAndStopVf:
                if ( !iStillPrepareReady ) 
                    {
	                #ifdef _DEBUG
                    RDebug::Print( _L( "Cae: CCaeTestSettingsClient: calling iCamAppEngine->PrepareStillCaptureL()" ) );
                    #endif
                    iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
                    }
                break;
            case EStartVfPrepareRecordVideoStartAndStopVf:
                if ( !iVideoPrepareReady ) 
                    {
	                #ifdef _DEBUG
                    RDebug::Print( _L( "Cae: CCaeTestSettingsClient: calling iCamAppEngine->PrepareVideoRecordingL()" ) );
                    #endif
                    iCamAppEngine->PrepareVideoRecordingL( iVideoQualityLevelIndex );
                    }
                break;
            case ESwitchPowerOffWhenVfRunning:
                iCamAppEngine->PowerOff();
                iStopViewFinder = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            default:
			    break;
		    }
        }
	else if ( iStopViewFinder ) 
        {
	    #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestSettingsClient: calling iCamAppEngine->StopViewFinder()" ) );
        #endif
        iCamAppEngine->StopViewFinder();
        iViewFinderStarted = EFalse;
        iStop = ETrue;
        CTimer::After( KNormalTimingDelay );
        }
	else if ( iExtensionMode ) 
        {
        switch ( iAction )
		    {
            case EProcessExtViewFinderFrameReady:
            if ( !iBitmapSendDone && iBitmapSave )
                {
                // send the original bitmap to ext handling
                // before sending the iBitmapSave bust be cleared because callback
                // writes the handled bitmap back there
                iBitmapSendDone = ETrue;
                CFbsBitmap* tempBitmap = iBitmapSave;
                iBitmapSave = NULL;
                iCamAppEngine->ProcessExtViewFinderFrameReadyL( *tempBitmap );
                // delete tempBitmap;
                delete tempBitmap;
                tempBitmap = NULL;
                }
            else if ( iBitmapSendDone )
                {
                // check that there is new handled bitmap
                if ( !iBitmapSave )
                    {
            	    #ifdef _DEBUG
                    RDebug::Print( _L( "Cae: CCaeTestSettingsClient: iBitmapSave not found after ProcessExtViewFinderFrameReadyL" ) );
                    #endif
                    User::Leave ( KErrNotFound );
                    }
                // all ok, set iAction to default to stop new VF frame copy and delete
                // bitmap, return to normal mode and stop test case
                iAction = EDefault;
                delete iBitmapSave;
                iBitmapSave = NULL;
                iCamAppEngine->SetCaeExtensionModeL( EFalse, ETrue );
                iExtensionMode = EFalse;
                iBitmapSendDone = EFalse;
                iStop = ETrue;
                }

                CTimer::After( KLongTimingDelay );
                break;
                
            case EProcessExtSnapImage:
            if ( !iBitmapSendDone && iBitmapSave )
                {
                // send the original snap bitmap to ext handling
                // before sending the iBitmapSave bust be cleared because callback
                // writes the handled bitmap back there
                iBitmapSendDone = ETrue;
                CFbsBitmap* tempBitmap = iBitmapSave;
                iBitmapSave = NULL;
                iCamAppEngine->ProcessExtSnapImageL( *tempBitmap );
                // delete tempBitmap;
                delete tempBitmap;
                tempBitmap = NULL;
                }
            else if ( iBitmapSendDone )
                {
                // check that there is new handled bitmap
                if ( !iBitmapSave )
                    {
            	    #ifdef _DEBUG
                    RDebug::Print( _L( "Cae: CCaeTestSettingsClient: iBitmapSave not found after EProcessExtSnapImage" ) );
                    #endif
                    User::Leave ( KErrNotFound );
                    }
                // all ok, set iAction to default to stop new VF frame copy and delete
                // bitmap, return to normal mode and stop test case
                iAction = EDefault;
                delete iBitmapSave;
                iBitmapSave = NULL;
                iCamAppEngine->SetCaeExtensionModeL( EFalse, ETrue );
                iExtensionMode = EFalse;
                iBitmapSendDone = EFalse;
                iStop = ETrue;
                CTimer::After( KLongTimingDelay );
                }
                break;
                
            case EProcessExtCapturedImage:
            case EProcessExtCapturedImageTDesC8:
		    case EProcessExtCapturedImageCFbsBitmap:
            case EProcessExtCapturedImageNoInit:
            if ( !iBitmapSendDone && iDataSave )
                {
                // send the original image to ext handling
                // before sending the iDataSave must be cleared because callback
                // writes the handled bitmap back there
                // Note that iBitmapSendDone flag is used even though this there is no bitmap involved
                iBitmapSendDone = ETrue;
                HBufC8* tempData = iDataSave;
                CleanupStack::PushL( tempData );
                iDataSave = NULL;
                if ( iAction == EProcessExtCapturedImage || iAction == EProcessExtCapturedImageNoInit)
                    {
                    // Test interface with HBufC8, CAE takes ownership of data
                    iCamAppEngine->ProcessExtCapturedImageL( tempData, EFalse );
                    }
                else if ( iAction == EProcessExtCapturedImageTDesC8 )
                    {
                    // Test interface with TDesC8
                    // Create new pointer
                    TDesC8* tempDataDes = tempData;
                    tempData = NULL;
                    iCamAppEngine->ProcessExtCapturedImageL( *tempDataDes, EFalse );
                    delete tempDataDes; // CAE makes own copy of this type
                    tempDataDes = NULL;
                    }
                else if ( iAction == EProcessExtCapturedImageCFbsBitmap )
                    {
                    // Test interface with CFbsBitmap
                    // Create new pointer

                    CFbsBitmap* tempBitmap = iBitmapSave;
                    CleanupStack::PushL( tempBitmap );
                    iBitmapSave = NULL;
                    delete tempData; // not used in this test case
                    tempData = NULL;
                    // CAE makes own copy of bitmap data (duplicate)
                    iCamAppEngine->ProcessExtCapturedImageL( *tempBitmap, EFalse );
                    CleanupStack::PopAndDestroy( tempBitmap );
                    }
                CleanupStack::Pop( /*tempData*/ );
                }
            else if ( iBitmapSendDone )
                {
                // check that there is new handled image
                if ( !iDataSave )
                    {
            	    #ifdef _DEBUG
                    RDebug::Print( _L( "Cae: CCaeTestSettingsClient: iDataSave not found after EProcessExtCapturedImage" ) );
                    #endif
                    User::Leave ( KErrNotFound );
                    }
                // all ok, set iAction to default to stop new VF frame copy and delete
                // image data, return to normal mode and stop test case
                iAction = EDefault;
                delete iDataSave;
                iDataSave = NULL;
                iCamAppEngine->SetCaeExtensionModeL( EFalse, ETrue );
                iExtensionMode = EFalse;
                iBitmapSendDone = EFalse;
                iStop = ETrue;
                CTimer::After( KLongTimingDelay );
                }
                break;
                
            case EProcessExtCancel:
                iCamAppEngine->ProcessExtCancel();
                iAction = EDefault;
                iCamAppEngine->SetCaeExtensionModeL( EFalse, ETrue );
                iExtensionMode = EFalse;
                iBitmapSendDone = EFalse;
                iStop = ETrue;
                CTimer::After( KLongTimingDelay );
                break;
            case EProcessExtCancel2:

                if ( iDataSave )
                    {
                    // send the original image to ext handling
                    iBitmapSendDone = ETrue;
                    HBufC8* tempData = iDataSave;
                    iDataSave = NULL;
                    // Test interface with TDesC8
                    TDesC8* tempDataDes = tempData;
                    tempData = NULL;
                    iCamAppEngine->ProcessExtCapturedImageL( *tempDataDes, EFalse );
                    delete tempDataDes; // CAE makes own copy of this type
                    tempDataDes = NULL;
                    }
                else
                    {
            	    #ifdef _DEBUG
                    RDebug::Print( _L( "Cae: CCaeTestSettingsClient: iDataSave not found after EProcessExtCancel2" ) );
                    #endif
                    User::Leave ( KErrNotFound );
                    }
                
                // Cancel image processing
                iCamAppEngine->ProcessExtCancel();
                
                // return back to normal mode
                iAction = EDefault;
                delete iDataSave;
                iDataSave = NULL;
                iCamAppEngine->SetCaeExtensionModeL( EFalse, ETrue );
                iExtensionMode = EFalse;
                iBitmapSendDone = EFalse;
                iStop = ETrue;
                
                // start timer as all activities are stopped
                CTimer::After( KNormalTimingDelay );
                break;

            case EProcessExtCapturedImageBurst:
            case EProcessExtCapturedImageBurstTDesC8:
            if ( !iBitmapSendDone && iDataSave && iBitmapSave )
                {
                // send the original snap bitmap and image to ext handling
                // before sending the iBitmapSave and iDataSave must be cleared because callback
                // writes the handled bitmap back there

                iBitmapSendDone = ETrue;
                CFbsBitmap* tempBitmap = iBitmapSave;
                iBitmapSave = NULL;
                HBufC8* tempData = iDataSave;
                iDataSave = NULL;
              
                // Change CAE state to burst mode
                iImageCountToCapture = iCamAppEngine->SetStillCaptureImageCountL( iStillBurstLength );
                iCountSnapImageReady = 0;  // Reset image counters
                iCountStillImageReady = 0;
                
                // Send all burst snapimages and main images at once

                for ( TInt i=0; i < iStillBurstLength; i++ )              
                    {
                    // Send bitmap
                    iCamAppEngine->ProcessExtSnapImageL( *tempBitmap );
                   
                    // Copy main image
                    HBufC8* msg =  HBufC8::NewL( tempData->Size());
                    *msg = *tempData;

                    // Send main image copy
                    if ( iAction == EProcessExtCapturedImageBurst )
                        {
                        // Test interface with HBufC8
                        iCamAppEngine->ProcessExtCapturedImageL( msg, i >= (iStillBurstLength - 1) );
                        }
                  else
                        {
                        // Test interface with TDesC8
                        // Create new pointer
                        TDesC8* tempDataDes = msg;
                        msg = NULL;
                        iCamAppEngine->ProcessExtCapturedImageL( *tempDataDes, i >= (iStillBurstLength - 1) );
                        delete tempDataDes; // CAE makes own copy of this type
                        tempDataDes = NULL;
                        }
                    }

                delete tempBitmap;
                tempBitmap = NULL;
                delete tempData;
                tempData = NULL;
                }
            else if ( iBitmapSendDone )
                {
                // check that there is at least one new handled image (snap + main)
                //
                if ( !iBitmapSave )
                    {
            	    #ifdef _DEBUG
                    RDebug::Print( _L( "Cae: CCaeTestSettingsClient: iBitmapSave not found after EProcessExtCapturedImageBurst" ) );
                    #endif
                    User::Leave ( KErrNotFound );
                    }
                if ( !iDataSave )
                    {
            	    #ifdef _DEBUG
                    RDebug::Print( _L( "Cae: CCaeTestSettingsClient: iDataSave not found after EProcessExtCapturedImageBurst" ) );
                    #endif
                    User::Leave ( KErrNotFound );
                    }
                // all ok, set iAction to default to stop new VF frame copy and delete
                // image data, return to normal mode and stop test case
                iAction = EDefault;
                delete iBitmapSave;
                iBitmapSave = NULL;
                delete iDataSave;
                iDataSave = NULL;
                iCamAppEngine->SetCaeExtensionModeL( EFalse, ETrue );
                iExtensionMode = EFalse;
                iBitmapSendDone = EFalse;
                iStop = ETrue;
                CTimer::After( KLongTimingDelay );
                }
                break;

            default:
                break;
		    }

        }
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestSettingsClient::RunTrappedL() returning" ) );
    #endif
    }


