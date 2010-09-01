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
* Description: CaeTestStatesClient.cpp
*
*/
#include <fbs.h>
#include <w32std.h>

#include "CaeTestStatesClient.h"

//_LIT8(KVideoMimeType, "video/3gpp");
//_LIT(KVideoPreferredSupplier, "Nokia");
//_LIT8(KVideoType, "video/H263-2000");
//_LIT8(KVideoAudioType, " AMR");
_LIT8(KVideoAudioTypeInv, " TOYOTA");
const TUid KIllegalUidQltyLevels = {666};


CCaeTestStatesClient::CCaeTestStatesClient() 
    {
    }


CCaeTestStatesClient::~CCaeTestStatesClient()
    {
    }


CCaeTestStatesClient* CCaeTestStatesClient::NewL()
    {
    CCaeTestStatesClient* self = new(ELeave) CCaeTestStatesClient;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


void CCaeTestStatesClient::ConstructL()
    {
    CCaeTestClient::ConstructL();

    iRunWithViewFinder = EFalse;
    }


void CCaeTestStatesClient::RunTestActionL( TestClientActions aAction )
    {

    iAction = aAction;

    iActionCycle = 0;

    // Initialize all state variables.
    InitStateVariables();

    #ifdef CAE_TEST_VERSION
    switch ( iAction )
		{
        case EEncodingError:
            CaeSetMcaeseoHBufC8ImageReadyError( KErrAbort );
            break;
        case EDecodingError:
            CaeSetMcaesdoCFbsBitmapImageReadyError( KErrAbort );
            break;
        case EPowerError:
            CaeSetPowerOnCompleteError( KErrAbort );
            break;
        case EReserveError:
            CaeSetReserveCompleteError( KErrAbort );
            break;
        case EStillCaptureError:
            CaeSetImageReadyError( KErrAbort );
            break;
        case EVideoRecorderOpenError:
            CaeSetMvruoOpenCompleteError( KErrAbort );
            break;
        case EVideoRecorderPrepareError:
            CaeSetMvruoPrepareCompleteError( KErrAbort );
            break;
        case EVideoRecordingCompleteError:
            CaeSetMvruoRecordCompleteError( KErrAbort );
            break;
        case EPrepareVideoSettingsError:
            CaeSetPrepareVideoSettingsError( KErrAbort );
            break;
        case ESnapImageError:
            CaeSetCreateAndDeliverSnapImageError( KErrAbort );
            break;
        case EStillImageError:
            CaeSetCreateAndDeliverStillImageError( KErrAbort );
            break;
        case EStillConvertError1:
            CaeSetConvertCFbsBitmapToHBufC8Error( KErrAbort );
            break;
        case EStillConvertError3:
            CaeSetConvertHBufC8ToCFbsBitmapError( KErrAbort );
            break;
        case EStillConvertError2:
            CaeSetStillConvertError( KErrAbort );
            break;
        default:
            break;
        }
    #endif // CAE_TEST_VERSION


    switch (iAction)
        {
    case ESetDisplayIndex:
        {
        delete iCamAppEngine;
        iCamAppEngine = NULL;
        iCamAppEngine = CCaeEngine::NewL( 0, iDisplayIndex );
        iCamAppEngine->SetCamAppEngineObserver( *this );
        iCamAppEngine->SetCaeStillBurstObserver( *this );
	    iCamAppEngine->GetInfo( iInfo );
        }
        break;
    case ESetCameraIndexLow:
        {
        delete iCamAppEngine;
        iCamAppEngine = NULL;
        iCamAppEngine = CCaeEngine::NewL( -1, iDisplayIndex );
        iCamAppEngine->SetCamAppEngineObserver( *this );
        iCamAppEngine->SetCaeStillBurstObserver( *this );
	    iCamAppEngine->GetInfo( iInfo );
        }
        break;
    case ESetCameraIndexHigh:
        {
        delete iCamAppEngine;
        iCamAppEngine = NULL;
        iCamAppEngine = CCaeEngine::NewL( 123456, iDisplayIndex );
        iCamAppEngine->SetCamAppEngineObserver( *this );
        iCamAppEngine->SetCaeStillBurstObserver( *this );
	    iCamAppEngine->GetInfo( iInfo );
        }
        break;
    case ESpecifiedPrepareAndCaptureStill2nd:
        {
        delete iCamAppEngine;
        iCamAppEngine = NULL;
        iCamAppEngine = CCaeEngine::NewL( 1 );
        iCamAppEngine->SetCamAppEngineObserver( *this );
        iCamAppEngine->SetCaeStillBurstObserver( *this );
	    iCamAppEngine->GetInfo( iInfo );
        }
 
    default:
        break;
        }

    
// If interface after S60 2.1.
#ifndef CAE_INTERFACE_21
    if ( iAction != ESpecifiedPrepareAndCaptureStill &&
        iAction != ESpecifiedPrepareAndRecordVideoNoInit )
        {
        // Normal case
        // This starts operations and state changes.
        if ( iCreateSnapImage ) 
            {
            iCamAppEngine->InitL();
            }
        else 
            {
            iCamAppEngine->InitL();
            iCamAppEngine->SetSnapImageCreation( EFalse );
            }
        }
    else if ( iAction == ESpecifiedPrepareAndCaptureStill ||
            iAction == ESpecifiedPrepareAndCaptureStill2nd )
        {
        // 
        // This starts operations and state changes.
        if ( iCreateSnapImage ) 
            {
            iCamAppEngine->InitL( EFalse );
            iCamAppEngine->SetSnapImageCreation( ETrue );
            }
        else 
            {
            iCamAppEngine->InitL( ETrue );
            iCamAppEngine->SetSnapImageCreation( EFalse );
            }
        }
    else if ( iAction == ESpecifiedPrepareAndRecordVideoNoInit )
        {
        // No CAE init here, used in 3.2 in case only CAE video features are used

    	#ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestStatesClient::RunTestActionL() reading cameraHandle" ) );
        #endif

        TInt cameraHandle = iCamAppEngine->CCameraHandle();

    	#ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestStatesClient::RunTestActionL() creating obs2 NewDuplicate2L cameraHandle=%d" ),cameraHandle );
        #endif

        // Create a new Camera API duplicate object, if supported
        TRAPD( err, iCameraDup = CCamera::NewDuplicate2L( static_cast<MCameraObserver2&>(*this), cameraHandle ) );
        if ( err )
            {
        	#ifdef _DEBUG
            RDebug::Print( _L( "Cae: CCaeTestStatesClient::RunTestActionL() CCamera::NewDuplicate2L return code=%d" ), err ); 
            #endif

            // Create old Camera API duplicate object.
            iCameraDup = CCamera::NewDuplicateL( static_cast<MCameraObserver&>(*this), cameraHandle );
            }
        else
            {
        	#ifdef _DEBUG
            RDebug::Print( ( _L( "Cae: CCaeTestStatesClient::RunTestActionL() using duplicated MCameraObserver2" )) ); 
            #endif
            }

    	#ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestStatesClient::RunTestActionL() calling reserve iCameraDup=%x" ),iCameraDup );
        #endif

        iCameraDup->Reserve(); // Poweron called after this in reservecomplete

        // Give C-API time to complete operation
        User::After( 500000 );
        
    	#ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestStatesClient::RunTestActionL() calling cae disable/enable videorecording" ) );
        #endif

        // Let CAE know that camera HW reservation is done,
        // this will initialize some CAE flags so that video mode can be used
        iCamAppEngine->DisableVideoRecording(); // Just to test release before reserve
        iCamAppEngine->EnableVideoRecording(); // 
        iCamAppEngine->DisableVideoRecording(); // test release 
        iCamAppEngine->EnableVideoRecording(); // reserve again
        iCamAppEngine->EnableVideoRecording(); // double reserve

        } 
#else   // CAE_INTERFACE_21
        // This starts operations and state changes.
        if ( iCreateSnapImage ) 
            {
            iCamAppEngine->InitL();
            }
        else 
            {
            iCamAppEngine->InitL( EFalse );
            }
#endif
        
	// Main part of program is a wait loop.
	// This function completes when the scheduler stops.
	CActiveScheduler::Start();
    
    if ( iError )
        {
    	#ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestStatesClient::RunTestActionL() leaving (error detected)" ) );
        #endif
        User::Leave( iError );
        }
	
    #ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestStatesClient::RunTestActionL() returning" ) );
    #endif
    }


void CCaeTestStatesClient::RunTrappedL()
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestStatesClient::RunTrappedL() entering, iAction=%d" ),iAction );
    #endif

    // This is for timing critical cases
    TTime currentTime; 
    currentTime.HomeTime();
    if ( iNextRunLTime.MicroSecondsFrom( currentTime ) > TTimeIntervalMicroSeconds(0) )
        {
        CTimer::At( iNextRunLTime );
        return;
        }

    if ( iAction == ENone || iStop ) 
        {
        if ( iCamAppEngine->IsViewFinding() )
	        {
            iCamAppEngine->StopViewFinder();
            iViewFinderStarted = EFalse;
            }
        iCamAppEngine->CancelCaptureStill();
        iCancelCount++;
        iCamAppEngine->CloseVideoRecording();
        CActiveScheduler::Stop();
        }
    else if ( iInitReady ) 
		{
        iInitReady = EFalse;
        iViewFinderStarted = EFalse;        

        switch ( iAction )
		    {
            case EInit:
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            case ECCameraHandle:
                iStop = ETrue;
                iCameraHandle = iCamAppEngine->CCameraHandle();
                CTimer::After( KNormalTimingDelay );
                break;
            case EInitVideoRecorder:
                iCamAppEngine->InitVideoRecorderL();
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            case EInitVideoRecorderWithUid:
                iCamAppEngine->InitVideoRecorderL(KIllegalUidQltyLevels);
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            case ESwitchPowerOnWhenInitialized:
                if ( iStopWhenCompleted ) 
                    {
                    iStop = ETrue;
                    CTimer::After( KNormalTimingDelay );
                    }
                else 
                    {
                    iStopWhenCompleted = ETrue;
                    iInitReady = ETrue;
                    iCamAppEngine->PowerOn();
                    CTimer::After( KNormalTimingDelay );
                    }
                break;
            case ESwitchPowerOffAndOn:
                if ( iStopWhenCompleted ) 
                    {
                    iStop = ETrue;
                    CTimer::After( KNormalTimingDelay );
                    }
                else if ( iPowerOn ) 
                    {
                    iCamAppEngine->PowerOff();
                    iPowerOn = EFalse;
                    iInitReady = ETrue;
                    CTimer::After( KNormalTimingDelay );
                    }
                else 
                    {
                    iStopWhenCompleted = ETrue;
                    iCamAppEngine->PowerOn();
                    }
                break;
            case EPowerOffPowerOff:
                iCamAppEngine->PowerOff();
                iCamAppEngine->PowerOff();
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            case EStartVfWhenPowerOff:
                iCamAppEngine->PowerOff();
                iViewFindingUsed = ETrue;
                iCamAppEngine->StartViewFinderBitmapsL( iViewFinderSize );
                if( iCamAppEngine->IsViewFinding() )
                    {
                    User::Leave ( KErrGeneral );
                    }
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay ); // Give some time for all Active objects to react to this command.
                break;
            case ERelease:
                iCamAppEngine->Release();
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            case EReleaseRelease:
                iCamAppEngine->Release();
                iCamAppEngine->Release();
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            case EReserveReserve:
                iCamAppEngine->Reserve();
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay ); // needed because this Reserve after InitL does do nothing (no call-back)
                break;
            case EReleaseReserve:
                if ( iStopWhenCompleted ) 
                    {
                    iStop = ETrue;
                    CTimer::After( KNormalTimingDelay );
                    }
                else if ( iPowerOn ) 
                    {
                    iCamAppEngine->Release();
                    iPowerOn = EFalse;
                    iInitReady = ETrue;
                    CTimer::After( KNormalTimingDelay );
                    }
                else 
                    {
                    iStopWhenCompleted = ETrue;
                    iCamAppEngine->Reserve();
                    }
                break;
            case EReleaseReserveCapture:
                if ( iContinueWhenCompleted ) 
                    {
                    iStillPrepareReady = EFalse;
                    iVideoPrepareReady = EFalse;
                    iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
                    }
                else if ( iPowerOn ) 
                    {
                    iCamAppEngine->Release();
                    iPowerOn = EFalse;
                    iInitReady = ETrue;
                    CTimer::After( KNormalTimingDelay );
                    }
                else 
                    {
                    iContinueWhenCompleted = ETrue;
                    iCamAppEngine->Reserve();
                    }
                break;
            case EReleaseReserveRecord:
                if ( iContinueWhenCompleted ) 
                    {
                    iStillPrepareReady = EFalse;
                    iVideoPrepareReady = EFalse;
                    iCamAppEngine->InitVideoRecorderL();
                    iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName );
                    iCamAppEngine->PrepareVideoRecordingL( iVideoQualityLevelIndex );
                    }
                else if ( iPowerOn ) 
                    {
                    iCamAppEngine->Release();
                    iPowerOn = EFalse;
                    iInitReady = ETrue;
                    CTimer::After( KNormalTimingDelay );
                    }
                else 
                    {
                    iContinueWhenCompleted = ETrue;
                    iCamAppEngine->Reserve();
                    }
                break;
            case EStartVideoRecordingWhenInitialized:
                iCamAppEngine->StartVideoRecording();
                break;
            case EStopVideoRecordingWhenInitialized:
                iCamAppEngine->StopVideoRecording();
                break;
            case EGetStillQualityIndexWhenEngineInitialized:
                {
                TInt stillQualityIndex = iCamAppEngine->StillQualityIndex();
                if ( stillQualityIndex != -1 ) 
                    {
                    User::Leave( KErrGeneral );
                    }
                }
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            case EGetVideoQualityIndexWhenEngineInitialized:
                {
                TInt videoQualityIndex = iCamAppEngine->VideoQualityIndex();
                if ( videoQualityIndex != -1 ) 
                    {
                    User::Leave( KErrGeneral );
                    }
                }
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            case EEnumerateCaptureSizes:
                {
                TSize stillFrameSize;
                // Tests to get the largest image size (index value 0).
                iCamAppEngine->EnumerateStillCaptureSizes( stillFrameSize, 0, iStillDataFormat );
                if ( stillFrameSize != iStillFrameSize )
                    {
                    #ifdef _DEBUG
                    RDebug::Print( _L( "Cae: CCaeTestStatesClient::RunTrappedL() got stillFrameSize w=%d, h=%d, leaving -2" ), stillFrameSize.iWidth, stillFrameSize.iHeight );
                    #endif
                    
                    User::Leave( KErrGeneral );
                    }
                }
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            case EPrepareStillCapturing:
            case EPrepareStillCapturingWhenPrepared:
            case EPrepareAndCaptureStill:
            case EPrepareAndCaptureAndCancelStill:
            case EPrepareAndCaptureAndCancelStillAtSnap:
            // Burst
            case EPrepareAndCaptureStillBurst:
            case EPrepareAndCaptureStillBurstCaptureStill:
            case EPrepareAndCaptureAndCancelStillBurst:
            case EPrepareAndCaptureAndCancelStillBurstAtMoment:
            case EPrepareAndCaptureAndCancelStillBurstAtSnap:
            case EPrepareAndCaptureAndStopStillBurst:
            case EPrepareAndCaptureCaptureStillBurst:
            // 
            case EStartAndStopVfWhenStillPreparedAndCaptured:
            case ECaptureCapture:
            case ECaptureRecord:
            case ESwitchPowerOnWhenStillPrepared:
            case ESwitchPowerOffWhenStillPrepared:
            case EGetStillQualityIndexWhenPrepared:
            case EEncodingError:
            case EStillCaptureError:
            case EStillConvertError1:
            case EStillConvertError2:
            case EStillConvertError3:
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
			    break;
            case EPrepareCroppedStillCapturing:
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                {
                TRect cropRect; 
                iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex, cropRect );
                }
			    break;
            case EPrepareStillCaptureWhenPowerOff:
                iCamAppEngine->PowerOff();
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
			    break;
            case EPrepareVideoRecording:
            case EPrepareVideoRecordingWhenPrepared:
            case EPrepareAndRecordVideo:
            case EStartAndStopVfWhenVideoPreparedAndRecorded:
            case ERecordRecord:
            case ERecordCapture:
            case ERecordCaptureComplexSequence1:
            case ERecordCaptureComplexSequence2:
            case ESwitchPowerOnWhenVideoPrepared:
            case ESwitchPowerOffWhenVideoPrepared:
            case ESwitchPowerOnWhenRecordingVideo:
            case ESwitchPowerOffWhenRecordingVideo:
            case EStartVideoRecordingWhenRecording:
            case EStopVideoRecordingWhenPrepared:
            case EPauseVideoWhenPrepared:
            case EResumeVideoWhenPrepared:
            case EResumeVideoWhenNotPaused:
            case EPrepareRecordPauseStopVideo:
            case EPrepareRecordPauseResumeStopVideo:
            case EPrepareRecordPausePauseStopVideo:
            case EGetVideoQualityIndexWhenPrepared:
            case ERemainingTimeWhenVideoPrepared:
            case EVideoRecorderOpenError:
            case EVideoRecorderPrepareError:
            case EVideoRecordingCompleteError:
            case EPrepareVideoSettingsError:
            case EPrepareStillCaptureWhenRecording:
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                iCamAppEngine->InitVideoRecorderL();
                iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName );
                iCamAppEngine->PrepareVideoRecordingL( iVideoQualityLevelIndex );
			    break;
            case ESpecifiedPrepareStill:
            case ESpecPrepCaptureCapture:
            case ESpecifiedPrepareStillWhenPrepared:
            case ESpecifiedPrepareAndCaptureStill:
            case ESpecifiedPrepareAndCaptureStill2nd:
            case EPrepareAndCaptureStillBurstExif:
            case EPrepareAndCaptureAndCancelStillBurstExif:
            case EStartVfPrepareCaptureStillStartAndStopVfExif:
            case EStillImageError:
		    case ESetDisplayIndex:
            // Burst
            case ESpecPrepAndCaptureStillBurst:
            case ESpecPrepAndCaptureCaptureStillBurst:
            case ESpecPrepAndCaptureStillBurstCaptureStill:
            // "Destroy Engine"
            case EDestroyEngineWhenStillPrepareReady:
            case EDestroyEngineWhenSnapImageReady:
            case EDestroyEngineWhenStillCaptureReady:
            case EDestroyEngineWhenStillBurstReady:
            case EDestroyEngineWhenSnapImageReadyInStillBurst:
            case EDestroyEngineWhenStillImageReadyInStillBurst:
            case ECaptureStillSetSnapSizeExif:
            case ECaptureStillAfterPowerOff:
            case ECaptureStillAfterPowerOffOn:
            case ESnapImageError:
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                iCamAppEngine->PrepareStillCaptureL( iStillFrameSize, 
                                                     iStillDataFormat, 
                                                     iStillCompressionQuality );
			    break;
			case EStartVfPrepareCaptureStillStartAndStopVfSnapExif:
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                iCamAppEngine->PrepareStillCaptureL( iStillFrameSize, 
                                                     iStillDataFormat, 
                                                     iStillCompressionQuality,
													 iCropRect,
													 iSnapImageSize);
			    break;
            case ECaptureStillSetSnapColorModeExif:
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                iCamAppEngine->PrepareStillCaptureL( iStillFrameSize, 
                                                     iStillDataFormat, 
                                                     iStillCompressionQuality );
				iCamAppEngine->SetSnapImageColorMode( EColor16M );
			    break;
            case ESpecifiedPrepareVideo: 
            case ESpecifiedPrepareVideoWhenPrepared: 
            case ESpecifiedPrepareAndRecordVideo: 
            case ESpecifiedPrepareAndRecordVideoNoInit: 
            // "Destroy Engine"
            case EDestroyEngineWhenVideoPrepareReady:
            case EDestroyEngineWhenVideoRecordingOn:
            case EDestroyEngineWhenVideoRecordingPaused:
            case EDestroyEngineWhenVideoRecordingReady:
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                iCamAppEngine->InitVideoRecorderL();
                iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName );
                {
                iCamAppEngine->PrepareVideoRecordingL( iVideoFrameSize, 
                                                       iVideoFrameRate, 
                                                       iVideoBitRate, 
                                                       iVideoAudioEnabled, 
                                                       *iMimeTypeSet[iVideoCodecSet], 
                                                       *iPreferredSupplierSet[iVideoCodecSet],
                                                       *iVideoTypeSet[iVideoCodecSet], 
                                                       *iAudioTypeSet[iVideoCodecSet] );
                }
			    break;
            case ESpecifiedPrepareAudioBrAndRecordVideo:
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                iCamAppEngine->InitVideoRecorderL();
                iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName );
                {
                iCamAppEngine->PrepareVideoRecordingL( iVideoFrameSize, 
                                                       iVideoFrameRate, 
                                                       iVideoBitRate, 
                                                       iVideoAudioEnabled,
                                                       iAudioBitRate,
                                                       *iMimeTypeSet[iVideoCodecSet], 
                                                       *iPreferredSupplierSet[iVideoCodecSet],
                                                       *iVideoTypeSet[iVideoCodecSet], 
                                                       *iAudioTypeSet[iVideoCodecSet] );
                }
			    break;
            case ESpecifiedPrepareAudioBrAndRecordVideoAT0:
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                iCamAppEngine->InitVideoRecorderL();
                iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName );
                {
                const TDesC8& audioType = KNullDesC8;
                iCamAppEngine->PrepareVideoRecordingL( iVideoFrameSize, 
                                                       iVideoFrameRate, 
                                                       iVideoBitRate, 
                                                       iVideoAudioEnabled,
                                                       iAudioBitRate,
                                                       *iMimeTypeSet[iVideoCodecSet], 
                                                       *iPreferredSupplierSet[iVideoCodecSet],
                                                       *iVideoTypeSet[iVideoCodecSet], 
                                                       audioType );
                }
			    break;
            case ESpecifiedPrepareAudioBrAndRecordVideoATInv:
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                iCamAppEngine->InitVideoRecorderL();
                iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName );
                {
                const TDesC8& audioType = KVideoAudioTypeInv;
                iCamAppEngine->PrepareVideoRecordingL( iVideoFrameSize, 
                                                       iVideoFrameRate, 
                                                       iVideoBitRate, 
                                                       iVideoAudioEnabled,
                                                       iAudioBitRate,
                                                       *iMimeTypeSet[iVideoCodecSet], 
                                                       *iPreferredSupplierSet[iVideoCodecSet],
                                                       *iVideoTypeSet[iVideoCodecSet], 
                                                       audioType );
                }
			    break;
            case EStartAndStopVf:  
                iViewFindingUsed = ETrue;
                iCamAppEngine->StartViewFinderBitmapsL( iViewFinderSize );
                // Test IsViewFinding()
                if ( !iCamAppEngine->IsViewFinding() )
                    {
                    User::Leave( KErrGeneral );
                    }
                iStopViewFinder = ETrue;
                CTimer::After( iTimeout );
                break;
            case EStartAndStopCroppedVf:  
                iViewFindingUsed = ETrue;
                {
                TRect emptyRect;
                iCamAppEngine->StartViewFinderBitmapsL( iViewFinderSize, emptyRect );
                }
                // Test IsViewFinding()
                if ( !iCamAppEngine->IsViewFinding() )
                    {
                    User::Leave( KErrGeneral );
                    }
                iStopViewFinder = ETrue;
                CTimer::After( iTimeout );
                break;
            case EStartAndStopDirectVf:  
                iViewFindingUsed = ETrue;
                {
                RWsSession*      wsSession = NULL;
                CWsScreenDevice* wsScreenDevice = NULL;
                RWindowBase*     windowBase = NULL;
                TRect*           tRect = NULL;
                iCamAppEngine->StartViewFinderDirectL( *wsSession,
                                                       *wsScreenDevice,
                                                       *windowBase,
                                                       *tRect);
                }
                // Test IsViewFinding()
                if ( !iCamAppEngine->IsViewFinding() )
                    {
                    User::Leave( KErrGeneral );
                    }
                iStopViewFinder = ETrue;
                CTimer::After( iTimeout );
                break;
            case EStartAndStopCroppedDirectVf:  
                iViewFindingUsed = ETrue;
                {
                RWsSession*      wsSession = NULL;
                CWsScreenDevice* wsScreenDevice = NULL;
                RWindowBase*     windowBase = NULL;
                TRect*           screenRect = NULL;
                TRect*           cropRect = NULL;
                iCamAppEngine->StartViewFinderDirectL( *wsSession,
                                                       *wsScreenDevice,
                                                       *windowBase,
                                                       *screenRect,
                                                       *cropRect );
                }
                // Test IsViewFinding()
                if ( !iCamAppEngine->IsViewFinding() )
                    {
                    User::Leave( KErrGeneral );
                    }
                iStopViewFinder = ETrue;
                CTimer::After( iTimeout );
                break;
            case ESwitchPowerOffWhenVfRunning:
                iViewFindingUsed = ETrue;
                iCamAppEngine->StartViewFinderBitmapsL( iViewFinderSize );
                CTimer::After( iTimeout );
                break;
            case EStartVfPrepareCaptureStillStartAndStopVf:
            case EStartVfPrepareRecordVideoStartAndStopVf:
            case ESetColorToneVideoRecording:
                iViewFindingUsed = ETrue;
                iCamAppEngine->InitVideoRecorderL();
                iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName );
                iCamAppEngine->StartViewFinderBitmapsL( iViewFinderSize );
                CTimer::After( 500000 );
                break;
            case EIsVideoRecordingWhenPrepared:
                break;
            case ECaptureStillWhenEngineInitialized:
                iCamAppEngine->CaptureStill();
                break;
            case ECancelCaptureStillWhenInitialized:
                iCamAppEngine->CancelCaptureStill();
                iCancelCount++;
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;            
            case EStartVfStartVf:
                iViewFindingUsed = ETrue;
                iCamAppEngine->StartViewFinderBitmapsL( iViewFinderSize );
                iCamAppEngine->StartViewFinderBitmapsL( iViewFinderSize );
                iStopViewFinder = ETrue;
                CTimer::After( iTimeout );
                break;
                
// If interface after S60 2.1.
#ifndef CAE_INTERFACE_21
            case EGetCustomInterface:
                iCustomInterfacePointer = iCamAppEngine->CustomInterface( iCustomInterfaceUid );
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            case ESetColorTone:
            case ESetColorToneWrongParams:
            case ESetColorToneRelease:
                if ( iAction == ESetColorToneRelease && iReleaseDone )
                    {
                    // Do the Reserve, note that engine will call
                    // CCaeTestClient::McaeoInitComplete when camera is ready
                    iReleaseDone = EFalse;
                    iReserveDone = ETrue;
                    iInitReady   = ETrue; // continue in this case on next run
                    iCamAppEngine->Reserve();
                    // No timer call here, McaeoInitComplete does it
                    }
                else if ( iAction == ESetColorToneRelease && iReserveDone )
                    {
                    // state flags to defaults, release/reserve is done now
                    iReleaseDone = EFalse;
                    iReserveDone = EFalse;

                    // Verify that settings are reset to default after reserve
                    MCameraColorToneSettings* CTptr = NULL;
                    CTptr = static_cast<MCameraColorToneSettings*> ( iCustomInterfacePointer );
                    if ( CTptr->ColorTone() != MCameraColorToneSettings::ENormal )
                        {
                    	    #ifdef _DEBUG
                            RDebug::Print( _L( "Cae: CCaeTestStatesClient::RunTrappedL() Error:ColorTone settings not reset after release()" ));
                            #endif
                            iStop = ETrue;
                            CTimer::After( KNormalTimingDelay );
                        }
                    else
                        {
                        // all ok, capture a picture and verify manually normal settings
                        iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
                        }
                    }
                else
                    {
                    iCustomInterfacePointer = iCamAppEngine->CustomInterface( iCustomInterfaceUid );
                    if ( !iCustomInterfacePointer )
                        {               // Interface not supported, exit test case
                	    #ifdef _DEBUG
                        RDebug::Print( _L( "Cae: CCaeTestStatesClient::RunTrappedL() CustomInterface returns 0 for uid %x" ), iCustomInterfaceUid.iUid );
                        #endif
                        iStop = ETrue;
                        CTimer::After( KNormalTimingDelay );
                        }
                    else
                        {
                        MCameraColorToneSettings* CTptr = NULL;
                        TUint32 supportedCT = 0 ;

                        // cast the TAny pointer to the real color tone interface pointer and
                        // read the bitmap of supported color tones
                        CTptr = static_cast<MCameraColorToneSettings*> ( iCustomInterfacePointer );
                        supportedCT = CTptr->SupportedColorTones();

                        // Check that given parameter is supported by the interface
                        if ( (iColorToneValue & supportedCT) ||
                            iColorToneValue == MCameraColorToneSettings::ENormal ||
                            iAction == ESetColorToneWrongParams)
                            {
                            // then set the new color tone and verify that it has been set
                            CTptr->SetColorToneL( iColorToneValue );
                            if ( CTptr->ColorTone() == iColorToneValue )
                                {
                                if ( iAction == ESetColorToneRelease )
                                    {
                                    // Do now release and then on next run the reserve
                                    iCamAppEngine->Release();
                                    iReleaseDone = ETrue;
                                    iReserveDone = EFalse;
                                    iInitReady   = ETrue; // this keeps the same switch active
                                    CTimer::After( KNormalTimingDelay );
                                    }
                                else
                                    {
                                    // all ok, capture a picture and verify manually changes
                                    iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
                                    }
                                }
                            else // Value not set
                                {
                            	#ifdef _DEBUG
                                RDebug::Print( _L( "Cae: ESetColorTone: color iColorToneValue not set to: %d" ), iColorToneValue );
                                #endif
                                iStop = ETrue;
                                CTimer::After( KNormalTimingDelay );
                                }
                            }
                        else // Value not supported
                            {
                       	    #ifdef _DEBUG
                            RDebug::Print( _L( "Cae: ESetColorTone: color iColorToneValue not supported: %d" ), iColorToneValue );
                            #endif
                            iStop = ETrue;
                            CTimer::After( KNormalTimingDelay );
                            }
                        }
                    }
                break;
#endif
            case ERemainingTimeWhenVideoNotInitialized:
                {
                iAction = ENone;
                TTimeIntervalMicroSeconds zeroRemainingTime( 0 );
                TTimeIntervalMicroSeconds remainingTime( 1 );
                remainingTime = iCamAppEngine->RemainingVideoRecordingTime();
                if ( remainingTime != zeroRemainingTime )
                    {
                    User::Leave( KErrArgument );
                    }
                CTimer::After( KNormalTimingDelay );
                }
                break;
            default:
			    break;
		    }
		}
    else if ( iStillPrepareReady ) 
		{
        
        if ( iRunWithViewFinder && !iCamAppEngine->IsViewFinding() )
	        {
            iCamAppEngine->StartViewFinderBitmapsL( iViewFinderSize );
            User::After( 1000000 ); // Give some time for camera drivers to settle.
            }

        iStillPrepareReady = EFalse;

		TBool capture = EFalse;

		switch ( iAction )
			{
            case ESwitchPowerOnWhenStillPrepared:
                iAction = ENone;
                iCamAppEngine->PowerOn();
                CTimer::After( KNormalTimingDelay );
                break;
            case ESwitchPowerOffWhenStillPrepared:
                iAction = ENone;
                iCamAppEngine->PowerOff();
                CTimer::After( KNormalTimingDelay );
                break;
            case EGetStillQualityIndexWhenPrepared:
                {
                TInt stillQualityIndex = iCamAppEngine->StillQualityIndex();
                if ( stillQualityIndex != iStillQualityLevelIndex ) 
                    {
                    User::Leave( KErrGeneral );
                    }
                }
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            case EStartStillOpModeStopVf:
                iStopViewFinder = ETrue;
                CTimer::After( iTimeout );
                break;
            case ESpecifiedPrepareStill:
            case EPrepareStillCapturing:
                iAction = ENone;
                CTimer::After( KNormalTimingDelay );
                break;
            case EPrepareStillCapturingWhenPrepared:
                if ( iStopWhenCompleted ) 
                    {
                    iStop = ETrue;
                    CTimer::After( KNormalTimingDelay );
                    }
                else 
                    {
                    iStopWhenCompleted = ETrue;
                    iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
                    }
                break;
            case ESpecifiedPrepareStillWhenPrepared:
                if ( iStopWhenCompleted ) 
                    {
                    iStop = ETrue;
                    CTimer::After( KNormalTimingDelay );
                    }
                else 
                    {
                    iStopWhenCompleted = ETrue;
                    iCamAppEngine->PrepareStillCaptureL( iStillFrameSize, 
                                                         iStillDataFormat, 
                                                         iStillCompressionQuality );
                    }
                break;
            // Burst
            case EPrepareAndCaptureStillBurst:
            case EPrepareAndCaptureStillBurstExif:
            case EPrepareAndCaptureAndCancelStillBurstExif:
            case ESpecPrepAndCaptureStillBurst:
            case EPrepareAndCaptureAndCancelStillBurst:
            case EPrepareAndCaptureAndCancelStillBurstAtMoment:
            case EPrepareAndCaptureAndCancelStillBurstAtSnap:
            case EPrepareAndCaptureAndStopStillBurst:
            case EPrepareAndCaptureCaptureStillBurst:
            case ESpecPrepAndCaptureCaptureStillBurst:
            case EPrepareAndCaptureStillBurstCaptureStill:
            case ESpecPrepAndCaptureStillBurstCaptureStill:
            case EDestroyEngineWhenStillBurstReady:
            case EDestroyEngineWhenSnapImageReadyInStillBurst:
            case EDestroyEngineWhenStillImageReadyInStillBurst:
                if ( iImageCountToCapture == 1 ) 
                    {
                    iImageCountToCapture = iCamAppEngine->SetStillCaptureImageCountL( iStillBurstLength );
                    }
                if ( iSnapImageSource != CCaeEngine::ESnapImageSourceOriginal )
                    {
                    iCamAppEngine->SetSnapImageSourceL( iSnapImageSource );
                    }
                capture = ETrue;
                break;
			case ECaptureStillSetSnapSizeExif:
				iCamAppEngine->SetSnapImageSizeL( iSnapImageSize );
                capture = ETrue;
			    break;
            case ECaptureStillAfterPowerOff:
				iCamAppEngine->PowerOff();
                capture = ETrue;
                iStop = ETrue;  // Engine cancels capture if power is not on
                CTimer::After( KNormalTimingDelay );
			    break;
            case ECaptureStillAfterPowerOffOn:
                if ( iStopWhenCompleted ) 
                    {
                    if ( iCountStillPrepareComplete <= 1 )
                        {
                        iCamAppEngine->PrepareStillCaptureL( iStillFrameSize, 
                                                             iStillDataFormat, 
                                                             iStillCompressionQuality );
                        }
                    else
                        {
                        capture = ETrue;
                        }
                    //CTimer::After( KNormalTimingDelay );
                    }
                else if ( iPowerOn ) 
                    {
                    iCamAppEngine->PowerOff();
                    iPowerOn = EFalse;
                    iStillPrepareReady = ETrue; // come back here on next run
                    CTimer::After( KLongTimingDelay );
                    }
                else 
                    {
                    iStopWhenCompleted = ETrue;
                    iCamAppEngine->PowerOn();
                    iStillPrepareReady = ETrue; // come back here on next run
//                    CTimer::After( KLongTimingDelay*100 );
                    }
			    break;
			case EDestroyEngineWhenStillPrepareReady:
                delete iCamAppEngine;
                iCamAppEngine = NULL;
                CActiveScheduler::Stop();
			    break;
			default:
                capture = ETrue;
                break;
			}
        
        // Special treatments for some specific test actions:
        switch ( iAction ) 
            {
            case EPrepareAndCaptureAndStopStillBurst:
                iStopStillBurst = ETrue;
                break;
            case EPrepareAndCaptureAndCancelStillBurstAtMoment:
                iCancelStillBurst = ETrue;
                break;
            case EPrepareAndCaptureAndCancelStillBurstExif:
				// Cancel still burst after next view finder frame
                iCancelStillBurstAfterVfFrame = ETrue; 
                break;
            case EPrepareAndCaptureAndCancelStillAtSnap:
                iRunlForSnapImage = ETrue;
                // Added delay so that the encoder has always time to finish before the next RunTrappedL.
                // Before is was not so deterministic.
                iNextRunLTime.HomeTime();
                iNextRunLTime += TTimeIntervalMicroSeconds32(KLongTimingDelay*1000);
                break;  
            case EPrepareAndCaptureAndCancelStillBurstAtSnap:
            case EDestroyEngineWhenSnapImageReady:
            case EDestroyEngineWhenSnapImageReadyInStillBurst:
                iRunlForSnapImage = ETrue;
                break;
            case EDestroyEngineWhenStillImageReadyInStillBurst:
                iRunlForStillImage = ETrue; 
                break;
            default:
                break;               
            }
        
        // Capture still if requested for the test action.
        if ( capture )
			{
            iStillCaptureReady = EFalse;
            iStillCapturingOn = ETrue;
            iStillCaptured = ETrue;
	        #ifdef _DEBUG
            RDebug::Print( _L( "Cae: CCaeTestStatesClient: calling iCamAppEngine->CaptureStill()" ) );
            #endif
			iCamAppEngine->CaptureStill();
			}

        if ( iAction == EPrepareAndCaptureAndCancelStill || 
             iAction == EPrepareAndCaptureAndCancelStillBurst )
            {
            iCamAppEngine->CancelCaptureStill();
            iCancelCount++;
            iStop = ETrue;
            CTimer::After( KNormalTimingDelay );
            }

		}
    else if ( iVideoPrepareReady ) 
		{

        if ( iRunWithViewFinder && !iCamAppEngine->IsViewFinding() )
	        {
            iCamAppEngine->StartViewFinderBitmapsL( iViewFinderSize );
            User::After( 1000000 ); // Give some time for camera drivers to settle.
            }

        iVideoPrepareReady = EFalse;

        TBool record = EFalse;

		switch ( iAction )
			{
            case ESwitchPowerOnWhenVideoPrepared:
                iAction = ENone;
                iCamAppEngine->PowerOn();
                CTimer::After( KNormalTimingDelay );
                break;
            case ESwitchPowerOffWhenVideoPrepared:
                iAction = ENone;
                iCamAppEngine->PowerOff();
                CTimer::After( KNormalTimingDelay );
                break;
            case EGetVideoQualityIndexWhenPrepared:
                {
                TInt videoQualityIndex = iCamAppEngine->VideoQualityIndex();
                if ( videoQualityIndex != iVideoQualityLevelIndex ) 
                    {
                    User::Leave( KErrGeneral );
                    }
                }
                iStop = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            case EPrepareVideoRecording:
            case ESpecifiedPrepareVideo:
                iAction = ENone;
                CTimer::After( KNormalTimingDelay );
                break;
            case EPrepareVideoRecordingWhenPrepared:
                if ( iStopWhenCompleted ) 
                    {
                    iStop = ETrue;
                    CTimer::After( KNormalTimingDelay );
                    }
                else 
                    {
                    iStopWhenCompleted = ETrue;
                    iCamAppEngine->PrepareVideoRecordingL( iVideoQualityLevelIndex );
                    }
                break;
            case ESpecifiedPrepareVideoWhenPrepared:
                if ( iStopWhenCompleted ) 
                    {
                    iStop = ETrue;
                    CTimer::After( KNormalTimingDelay );
                    }
                else 
                    {
                    iStopWhenCompleted = ETrue;
                    iCamAppEngine->PrepareVideoRecordingL( iVideoFrameSize, 
                                                           iVideoFrameRate, 
                                                           iVideoBitRate, 
                                                           iVideoAudioEnabled, 
                                                           *iMimeTypeSet[iVideoCodecSet], 
                                                           *iPreferredSupplierSet[iVideoCodecSet],
                                                           *iVideoTypeSet[iVideoCodecSet], 
                                                           *iAudioTypeSet[iVideoCodecSet] );
                    }
                break;
            case EPauseVideoWhenPrepared:
                iCamAppEngine->PauseVideoRecording();
                break;
            case EResumeVideoWhenPrepared:
                iCamAppEngine->ResumeVideoRecording();
                break;
            case EStopVideoRecordingWhenPrepared:
                iCamAppEngine->StopVideoRecording();
                break;
            case EStartVideoOpModeStopVf:
                iStopViewFinder = ETrue;
                CTimer::After( iTimeout );
                break;
            case EIsVideoRecordingWhenPrepared:
                iAction = ENone;
                {
                TBool isRecording = iCamAppEngine->IsVideoRecording();
                if ( isRecording ) 
                    {
                    User::Leave ( KErrGeneral );
                    }
                }
                break;
            case ERemainingTimeWhenVideoPrepared:
                {
                iAction = ENone;
                TTimeIntervalMicroSeconds remainingTime( 0 );
                remainingTime = iCamAppEngine->RemainingVideoRecordingTime();
                CTimer::After( KNormalTimingDelay );
                }
                break;
			case EDestroyEngineWhenVideoPrepareReady:
                delete iCamAppEngine;
                iCamAppEngine = NULL;
                CActiveScheduler::Stop();
			    break;
			    
// If interface after S60 2.1.
#ifndef CAE_INTERFACE_21
            case ESetColorToneVideoRecording:
                iCustomInterfacePointer = iCamAppEngine->CustomInterface( iCustomInterfaceUid );
                if ( !iCustomInterfacePointer )
                    {               // Interface not supported, exit test case
            	    #ifdef _DEBUG
                    RDebug::Print( _L( "Cae: CCaeTestStatesClient::RunTrappedL() CustomInterface returns 0 for uid %x" ), iCustomInterfaceUid.iUid );
                    #endif
                    iStop = ETrue;
                    CTimer::After( KNormalTimingDelay );
                    }
                else
                    {
                    MCameraColorToneSettings* CTptr = NULL;

                    // cast the TAny pointer to the real color tone interface pointer and
                    // read the bitfield of supported color tones. All of them are tested once.
                    CTptr = static_cast<MCameraColorToneSettings*> ( iCustomInterfacePointer );
                    iTestedColorTones = CTptr->SupportedColorTones();
                    record = ETrue;
                    }
				break;
#endif
            default:
                record = ETrue;
				break;
			}
        
        if ( record )
			{
            iVideoRecordingReady = EFalse;
            iVideoRecordingOn = EFalse;
            iVideoRecorded = ETrue;
	        #ifdef _DEBUG
            RDebug::Print( _L( "Cae: CCaeTestStatesClient: calling iCamAppEngine->StartVideoRecording()" ) );
            #endif
			iCamAppEngine->StartVideoRecording();
			}
		}
	else if ( iSnapImageReady )
	    {
	    iSnapImageReady = EFalse;

		switch ( iAction )
		    {
            case EPrepareAndCaptureAndCancelStillAtSnap: 
            case EPrepareAndCaptureAndCancelStillBurstAtSnap:
                iCamAppEngine->CancelCaptureStill();
                iCancelCount++;
                if ( iCancelCount == 1 )
                    {
                    iCountSnapImageReady--;
                    // To test that preparing does not ruin cancelling.
                    iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex ); 
                    }
                else 
                    {
                    iCountSnapImageReady++;
                    iStillCapturingOn = ETrue;
                    iStillCaptured = ETrue;
                    if ( iAction == EPrepareAndCaptureAndCancelStillAtSnap ) 
                        {
                        iRunlForStillImage = ETrue;
                        }
    			    iAction = ENone;
    			    iCamAppEngine->CaptureStill();
                    }
                break;
			case EDestroyEngineWhenSnapImageReady:
			case EDestroyEngineWhenSnapImageReadyInStillBurst:
			    iRunlForSnapImage = EFalse;
                delete iCamAppEngine;
                iCamAppEngine = NULL;
                CActiveScheduler::Stop();
			    break;
			default:
			    iRunlForSnapImage = EFalse;
			    break;
			}
	    }
	else if ( iStillCaptureReady )
	    {
	    iStillCaptureReady = EFalse;

		switch ( iAction )
		    {
            case EPrepareAndCaptureStill:
            case ESpecifiedPrepareAndCaptureStill:
            case ESpecifiedPrepareAndCaptureStill2nd:
            case ESetColorTone:
            case ESetColorToneWrongParams:
		    case ESetDisplayIndex:
                iAction = ENone;
                CTimer::After( KNormalTimingDelay );
			    break;
		    case ECaptureCapture:
		    case ESpecPrepCaptureCapture:
			    iAction = ENone;
                iStillCapturingOn = ETrue;
                iStillCaptured = ETrue;
			    iCamAppEngine->CaptureStill();
			    break;
            case ECaptureRecord:
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                iCamAppEngine->InitVideoRecorderL();
                iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName );
                iCamAppEngine->PrepareVideoRecordingL( iVideoQualityLevelIndex );
			    break;
		    case ERecordCaptureComplexSequence1:
		    case ERecordCaptureComplexSequence2:
                iVideoRecordingReady = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
            case EPrepareAndCaptureStillBurst:
            case EPrepareAndCaptureStillBurstExif:
            case EPrepareAndCaptureAndCancelStillBurstExif:
            case ESpecPrepAndCaptureStillBurst:
            case EPrepareAndCaptureAndCancelStillBurst:
            case EPrepareAndCaptureAndCancelStillBurstAtSnap:            
            case EPrepareAndCaptureAndStopStillBurst:
                break;
			case EDestroyEngineWhenStillCaptureReady:
			case EDestroyEngineWhenStillImageReadyInStillBurst:
                delete iCamAppEngine;
                iCamAppEngine = NULL;
                CActiveScheduler::Stop();
			    break;
		    default:
                iAction = ENone;
                iViewFindingUsed = ETrue;
	            #ifdef _DEBUG
                RDebug::Print( _L( "Cae: CCaeTestStatesClient: calling iCamAppEngine->StartViewFinderBitmapsL()" ) );
                #endif
                iCamAppEngine->StartViewFinderBitmapsL( iViewFinderSize );
                iStopViewFinder = ETrue;
                CTimer::After( iTimeout );
			    break;	
		    }
		    
		iRunlForStillImage = EFalse;
	    
	    }
	else if ( iStillBurstCaptureMoment ) 
        {
        iStillBurstCaptureMoment = EFalse;

		switch ( iAction )
		    {
            case EPrepareAndCaptureAndCancelStillBurstAtMoment:
                // Note: Cancelling is done in the callback.
                if ( iCancelCount == 1 )
                    {
                    // To test that preparing does not ruin cancelling.
                    // The burst emerging from this is also cancelled!
                    iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex ); 
                    }
                else 
                    {
    			    iAction = ENone;
                    iStillCapturingOn = ETrue;
                    iStillCaptured = ETrue;
                    // Do not cancel this burst!
                    iCancelStillBurst = EFalse;
    			    iCamAppEngine->CaptureStill();
                    }
                break;
			default:
			    break;
			}
        }
    else if ( iStillBurstReady )
	    {
        iStillBurstReady = EFalse;

		switch ( iAction )
		    {
            case EPrepareAndCaptureCaptureStillBurst:
            case ESpecPrepAndCaptureCaptureStillBurst:
			    iAction = ENone;
                iStillCapturingOn = ETrue;
                iStillCaptured = ETrue;
			    iCamAppEngine->CaptureStill();
			    break;
            case EPrepareAndCaptureStillBurstCaptureStill:
            case ESpecPrepAndCaptureStillBurstCaptureStill:
                // Re-set the image count to one to perform single image capture.
                iImageCountToCapture = iCamAppEngine->SetStillCaptureImageCountL( 1 );
                if ( iImageCountToCapture != 1 )
                    {
                    User::Leave( KErrGeneral );
                    }
			    iAction = ENone;
                iStillCapturingOn = ETrue;
                iStillCaptured = ETrue;
			    iCamAppEngine->CaptureStill();
			    break;
            case EPrepareAndCaptureStillBurst:
            case EPrepareAndCaptureStillBurstExif:
            case EPrepareAndCaptureAndCancelStillBurstExif:
            case ESpecPrepAndCaptureStillBurst:
            case EPrepareAndCaptureAndCancelStillBurst:
            case EPrepareAndCaptureAndStopStillBurst:
                iAction = ENone;
                iImageCountToCapture = iCamAppEngine->SetStillCaptureImageCountL( 1 );
                if ( iImageCountToCapture != 1 )
                    {
                    User::Leave( KErrGeneral );
                    }
                CTimer::After( KNormalTimingDelay );
			    break;	
		    case ERecordCaptureComplexSequence1:
		    case ERecordCaptureComplexSequence2:
                iVideoRecordingReady = ETrue;
                CTimer::After( KNormalTimingDelay );
                break;
			case EDestroyEngineWhenStillBurstReady:
                delete iCamAppEngine;
                iCamAppEngine = NULL;
                CActiveScheduler::Stop();
			    break;
		    default:
                iAction = ENone;
                iImageCountToCapture = iCamAppEngine->SetStillCaptureImageCountL( 1 );
                if ( iImageCountToCapture != 1 )
                    {
                    User::Leave( KErrGeneral );
                    }
                CTimer::After( KNormalTimingDelay );
			    break;	
		    }
        }
	else if ( iVideoRecordingOn )
	    {
	    
        switch ( iAction )
		    {
            case ESwitchPowerOnWhenRecordingVideo:
                iVideoRecordingOn = EFalse;
                iAction = ENone;
                iCamAppEngine->PowerOn();
                CTimer::After( KNormalTimingDelay );
                break;
            case ESwitchPowerOffWhenRecordingVideo:
                iVideoRecordingOn = EFalse;
                iAction = ENone;
                iCamAppEngine->PowerOff();
                CTimer::After( KNormalTimingDelay );
                break;
            case EPrepareRecordPauseStopVideo:
            case EPrepareRecordPauseResumeStopVideo:
                iVideoRecordingOn = EFalse;
                if ( !iVideoRecordingResumed ) 
                    {
                    iCamAppEngine->PauseVideoRecording();
                    }
                else 
                    {
                    iCamAppEngine->StopVideoRecording();
                    }
                break;
            case EPrepareRecordPausePauseStopVideo:
                iVideoRecordingOn = EFalse;
                if ( !iVideoRecordingResumed ) 
                    {
                    iCamAppEngine->PauseVideoRecording();
                    iCamAppEngine->PauseVideoRecording();
                    }
                else 
                    {
                    iCamAppEngine->StopVideoRecording();
                    }
                break;
            case EResumeVideoWhenNotPaused:
                iCamAppEngine->ResumeVideoRecording();
                break;
            case EStartVideoRecordingWhenRecording:
                iCamAppEngine->StartVideoRecording();
                break;
            case EIsVideoRecordingWhenRecording:
                iVideoRecordingOn = EFalse;
                iAction = ENone;
                {
                TBool isRecording = iCamAppEngine->IsVideoRecording();
                if ( !isRecording ) 
                    {
                    User::Leave ( KErrGeneral );
                    }
                }
                break;
            case EPrepareStillCaptureWhenRecording:
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
                break;
			case EDestroyEngineWhenVideoRecordingOn:
                delete iCamAppEngine;
                iCamAppEngine = NULL;
                CActiveScheduler::Stop();
			    break;
            case EDestroyEngineWhenVideoRecordingPaused:
                iVideoRecordingOn = EFalse;
                iCamAppEngine->PauseVideoRecording();
                break;
                
// If interface after S60 2.1.
#ifndef CAE_INTERFACE_21
            case ESetColorToneVideoRecording:
                {
                    MCameraColorToneSettings* CTptr = NULL;

                    if ( iTestedColorTones == 0) // All colors tested?
                        {
                        //Set the default color tone and stop
                        CTptr = static_cast<MCameraColorToneSettings*> ( iCustomInterfacePointer );
                        CTptr->SetColorToneL( iColorToneValue );
                        iCamAppEngine->StopVideoRecording();
                        }
                    else
                        {
                        TInt i, nextColor;

                        // Find out next not tested color
                        for ( i = 0, nextColor = 0; i < 32; i++ )
                            {
                            nextColor = (1 << i);
                            if ( nextColor &  iTestedColorTones )
                                {
                                iTestedColorTones &= ~nextColor; // Remove this color (bit)
                                break;
                                }
                            }
                        // cast the TAny pointer to the real color tone interface pointer and
                        // then set the new color tone and verify that it has been set
                        CTptr = static_cast<MCameraColorToneSettings*> ( iCustomInterfacePointer );
                        CTptr->SetColorToneL( (MCameraColorToneSettings::TColorTone) nextColor );

                        if ( CTptr->ColorTone() == nextColor )
                            {
                            CTimer::After( iTimeout ); // all ok, continue video recording
                            }
                        else
                            {
                          	#ifdef _DEBUG
                            RDebug::Print( _L( "Cae: ESetColorToneVideoRecording: color not set to: %d" ), nextColor );
                            #endif
                            iStop = ETrue;
                            CTimer::After( KNormalTimingDelay );
                            }
                        }
                    break;
                }
#endif
            default:
                iCamAppEngine->StopVideoRecording();
			    break;	
		    }
	    }
	else if ( iVideoRecordingPaused )
	    {
        
        switch ( iAction )
		    {
            case EPrepareRecordPauseStopVideo:
            case EPrepareRecordPausePauseStopVideo:
                iStop = ETrue;
                iCamAppEngine->StopVideoRecording();
                break;
			case EDestroyEngineWhenVideoRecordingPaused:
                delete iCamAppEngine;
                iCamAppEngine = NULL;
                CActiveScheduler::Stop();
			    break;
		    default:
                iCamAppEngine->ResumeVideoRecording();
                iVideoRecordingResumed = ETrue;
			    break;	
		    }
	    }
	else if ( iVideoRecordingReady )
	    {
		switch ( iAction )
		    {
		    case ERecordRecord:
                iAction = EDefault;
			    iVideoRecordingReady = EFalse;
                iVideoRecordingOn = EFalse;
                iVideoRecorded = ETrue;
                // This leads to video prepare and then recording with the new name.
                iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName2 );
			    break;
            case ERecordCapture:
                iAction = EDefault;
                iStillPrepareReady = EFalse;
                iVideoPrepareReady = EFalse;
                iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
                break;
		    case ERecordCaptureComplexSequence1:
            	#ifdef _DEBUG
                RDebug::Print( _L( "Cae: ERecordCaptureComplexSequence1 case: %d" ), iActionCycle );
                #endif
			    iVideoRecordingReady = EFalse;
                iVideoRecordingOn = EFalse;
                iVideoRecorded = ETrue;
		        switch ( iActionCycle )
		            {
		            case 0:
                        // This leads to video prepare and then recording with the new name.
                        iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName2 );
                        break;
		            case 1:
                        // Accept video recording completion error because max clip size.
                        iAcceptVideoCompletionError = ETrue;
                        // This leads to video prepare and then recording with the previously given name.
                        iCamAppEngine->SetVideoClipMaxSizeL( iVideoMaxClipSizeInBytes );
                        break;
                    /*
		            case 2:
                        // This leads to video recording with the previous name.
                        iVideoPrepareReady = ETrue;
                        CTimer::After( KNormalTimingDelay );
                        break;
                    */
		            case 2:
                        // This leads to video prepare and then recording with the new name.
                        iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName3 );
                        break;
		            case 3:
		                // This leads to single still image capture.
                        iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
                        break;
		            case 4:
                        // Note, this is re-initialization of video recorder.
                        // Leads to video prepare and then recording with new name.
                        iAcceptVideoCompletionError = EFalse;
                        iCamAppEngine->InitVideoRecorderL();
                        iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName4 );
                        iCamAppEngine->PrepareVideoRecordingL( iVideoQualityLevelIndex );
                        break;
		            case 5:
		                // This leads to single still image capture.
                        iCamAppEngine->CloseVideoRecording();
                        iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
                        break;
		            case 6:
                        iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName );
                        // Accept video recording completion error because max clip size.
                        iAcceptVideoCompletionError = ETrue;
                        iCamAppEngine->SetVideoClipMaxSizeL( iVideoMaxClipSizeInBytes );
                        iCamAppEngine->PrepareVideoRecordingL( iVideoQualityLevelIndex );
                        break;
		            case 7:
                        // Note, this is re-initialization of the whole engine.
                        // Leads to video prepare and then recording with the original name.
                        iAcceptVideoCompletionError = EFalse;
                        iCamAppEngine->InitL();
                        break;
                    default:
                        iStop = ETrue;
                        CTimer::After( KNormalTimingDelay );
                        break;
                    }
                iActionCycle++;
                break;
		    case ERecordCaptureComplexSequence2:
            	#ifdef _DEBUG
                RDebug::Print( _L( "Cae: ERecordCaptureComplexSequence2 case: %d" ), iActionCycle );
                #endif
			    iVideoRecordingReady = EFalse;
                iVideoRecordingOn = EFalse;
                iVideoRecorded = ETrue;
		        switch ( iActionCycle )
		            {
		            case 0:
                        // This leads to still burst capture.
                        iImageCountToCapture = iCamAppEngine->SetStillCaptureImageCountL( iStillBurstLength );
                        iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
                        break;
		            case 1:
                        // This leads to single image capture.
                        iImageCountToCapture = iCamAppEngine->SetStillCaptureImageCountL( 1 );
                        iStillPrepareReady = ETrue;
                        CTimer::After( KNormalTimingDelay );
                        break;
		            case 2:
                        // This leads to single image capture.
                        iStillPrepareReady = ETrue;
                        CTimer::After( KNormalTimingDelay );
                        break;
		            case 3:
                        // This leads to still burst capture.
                        iImageCountToCapture = iCamAppEngine->SetStillCaptureImageCountL( iStillBurstLength );
                        iStillPrepareReady = ETrue;
                        CTimer::After( KNormalTimingDelay );
                        break;
		            case 4:
                        // This leads to video prepare and then recording with new name.
                        iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName2 );
                        break;
		            case 5:
		                // This leads to single image capture.
                        iCamAppEngine->CloseVideoRecording();
                        iImageCountToCapture = iCamAppEngine->SetStillCaptureImageCountL( 1 );
                        iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
                        break;
		            case 6:
		                // This leads to video prepare and then recording with new name.
                        iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName3 );
                        // Accept video recording completion error because max clip size.
                        iAcceptVideoCompletionError = ETrue;
                        iCamAppEngine->SetVideoClipMaxSizeL( iVideoMaxClipSizeInBytes );
                        iCamAppEngine->PrepareVideoRecordingL( iVideoQualityLevelIndex );
                        break;
		            case 7:
                        // This leads to video prepare and then recording with new name.
                        iCamAppEngine->SetVideoRecordingFileNameL( iVideoFileName4 );
                        break;
                    default:
                        iStop = ETrue;
                        CTimer::After( KNormalTimingDelay );
                        break;
                    }
                iActionCycle++;
                break;
			case EDestroyEngineWhenVideoRecordingReady:
                delete iCamAppEngine;
                iCamAppEngine = NULL;
                CActiveScheduler::Stop();
			    break;
		    default:
			    iAction = ENone;
                iVideoRecordingReady = EFalse;
                iStopViewFinder = ETrue;
                CTimer::After( KNormalTimingDelay );
			    break;	
		    }
	    }
    else if ( !iStillCaptured && !iVideoRecorded && !iVideoRecordingOn && !iStopViewFinder ) 
        {
        switch ( iAction )
		    {
            case EStartVfPrepareCaptureStillStartAndStopVf:
                if ( !iStillPrepareReady ) 
                    {
	                #ifdef _DEBUG
                    RDebug::Print( _L( "Cae: CCaeTestStatesClient: calling iCamAppEngine->PrepareStillCaptureL()" ) );
                    #endif
                    iCamAppEngine->PrepareStillCaptureL( iStillQualityLevelIndex );
                    }
                break;
            case EStartVfPrepareRecordVideoStartAndStopVf:
            case ESetColorToneVideoRecording:
                if ( !iVideoPrepareReady ) 
                    {
	                #ifdef _DEBUG
                    RDebug::Print( _L( "Cae: CCaeTestStatesClient: calling iCamAppEngine->PrepareVideoRecordingL()" ) );
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
        RDebug::Print( _L( "Cae: CCaeTestStatesClient: calling iCamAppEngine->StopViewFinder()" ) );
        #endif
        iCamAppEngine->StopViewFinder();
        // Test IsViewFinding()
        if ( iCamAppEngine->IsViewFinding() )
            {
            User::Leave( KErrGeneral );
            }
        // Test GetViewFinderSize()
        TSize sizeVF;
        iCamAppEngine->GetViewFinderSize( sizeVF );
        if ( sizeVF != iViewFinderSize )           
            {
            User::Leave ( KErrGetNotEqualToValuePreviouslySet );
            }

        iViewFinderStarted = EFalse;
        iAction = ENone;
        CTimer::After( KNormalTimingDelay );
        }

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestStatesClient::RunTrappedL() returning" ) );
    #endif
    }


