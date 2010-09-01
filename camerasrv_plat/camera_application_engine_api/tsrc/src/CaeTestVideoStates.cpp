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
* Description: CaeTestVideoStates.cpp
*
*/

#include "CaeTestVideoStates.h"
#include <f32file.h>


//const TReal32 KViewFinderFrameFrequencyMinimum = 13.0;


CCaeTestVideoStates::CCaeTestVideoStates ()
    {
    }


CCaeTestVideoStates::~CCaeTestVideoStates ()
    {
    }


void CCaeTestVideoStates::setUpL ()
    {
	iStatesClient = CCaeTestStatesClient::NewL( );

    iStatesClient->SetFileNameBase( KFileNameBase );
    }


void CCaeTestVideoStates::tearDown ()
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
/*  VIDEO RECORDING PREPARE AND RECORD QUALITY LEVEL    */
/********************************************************/

void CCaeTestVideoStates::TestPrepareAndRecordQualityLevel0L()
    {
	// ID:      CAE.VR.VIDEORECORDING.QCIF64000FR15.004
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 0 
    //          video clip when initialization is done.
	//      
	// Output : No leave. File cae_novf_ql_0.3gp.  

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetTimeout( 5000000 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoFileName( _L("cae_novf_ql_0.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAndRecordQualityLevel1L()
    {
	// ID:      CAE.VR.VIDEORECORDING.SUBQCIF60000FR15.005
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 1 
    //          video clip when initialization is done.
	//      
	// Output : No leave. File cae_novf_ql_1.3gp.  

    iStatesClient->SetVideoQualityLevelIndex( 1 );

    iStatesClient->SetTimeout( 5000000 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoFileName( _L("cae_novf_ql_1.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAndRecordQualityLevel2L()
    {
	// ID:      CAE.VR.VIDEORECORDING.SUBQCIF40000FR5.006
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 2 
    //          video clip when initialization is done.
	//      
	// Output : No leave. File cae_novf_ql_2.3gp.  

    iStatesClient->SetVideoQualityLevelIndex( 2 );

    iStatesClient->SetTimeout( 5000000 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoFileName( _L("cae_novf_ql_2.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


/********************************************************/
/*  VIDEO RECORDING SPECIAL PREPARE AND RECORD          */
/********************************************************/

void CCaeTestVideoStates::TestVideoRecordingQcif60000Fr10NoAudioNoVfL()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.QCIF.003
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording QCIF sized video works. 
    //          Video recording prepared for QCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 20000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetRunningWithViewFinder( EFalse );

    iStatesClient->SetVideoFileName( _L("cae_qcif10fps.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestVideoRecordingQcif60000Fr10WithAudioNoVfL()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.QCIF.004
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording QCIF sized video works. 
    //          Video recording prepared for QCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 20000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
#if ( defined (__WINS__) || defined (__WINSCW) ) // this different on hw
    iStatesClient->SetVideoAudio( EFalse ); // Audio does not work in emulator, error -9
#else
    iStatesClient->SetVideoAudio( ETrue );
#endif
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetRunningWithViewFinder( EFalse );

    iStatesClient->SetVideoFileName( _L("cae_qcif10fps_audio.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestVideoRecordingQcif60000Fr10NoAudioWithVfL()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.QCIF.005
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording QCIF sized video works. 
    //          Video recording prepared for QCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 20000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetViewFinderSize( sizeQcif );
    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoFileName( _L("cae_qcif10fps_vf.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestVideoRecordingQcif60000Fr10WithAudioWithVfL()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.QCIF.006
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording QCIF sized video works. 
    //          Video recording prepared for QCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 20000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
#if ( defined (__WINS__) || defined (__WINSCW) ) // this different on hw
    iStatesClient->SetVideoAudio( EFalse );// Audio does not work in emulator, error -9
#else
    iStatesClient->SetVideoAudio( ETrue );
#endif
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetViewFinderSize( sizeQcif );
    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoFileName( _L("cae_qcif10fps_audio_vf.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*              PAUSE VIDEO RECORDING                   */
/********************************************************/

void CCaeTestVideoStates::TestRecordPauseStopL()
    {
	// ID:      CAE.VR.PAUSEVIDEORECORDING.001
    //
    // Group:   Record.
    // 
    // Action : Pause video clip recording.Video recordin prepared.
	//      
	// Output : No leave. File KVideoFileName, "videoclip.3gp".   

    iStatesClient->SetTimeout( 3000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareRecordPauseStopVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPauseWhenPreparedL()
    {
	// ID:      CAE.VR.PAUSEVIDEORECORDING.002
    //
    // Group:   Record.
    // 
    // Action : Call Pause when video is prepared but video recordind is not started.
	//      
	// Output : Leave..   

    iStatesClient->SetTimeout( 5000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPauseVideoWhenPrepared ) );
    assertL( err == KErrNotReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

void CCaeTestVideoStates::TestRecordPausePauseStopL()
    {
	// ID:      CAE.VR.PAUSEVIDEORECORDING.003
    //
    // Group:   Record.
    // 
    // Action : Pause video clip recording twice.Video recordin prepared.
	//      
	// Output : No leave. File KVideoFileName, "videoclip.3gp".   

    iStatesClient->SetTimeout( 3000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareRecordPausePauseStopVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 2 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*              RESUME VIDEO RECORDING                  */
/********************************************************/

void CCaeTestVideoStates::TestResumeWhenPreparedL()
    {
	// ID:      CAE.VR.RESUMEVIDEORECORDING.002
    //
    // Group:   Record.
    // 
    // Action : Resume video clip recording before pause. 
    //          Video recording.
	//      
	// Output : Leave.   

    iStatesClient->SetTimeout( 3000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoFileName( _L("cae_pause_resume.3gp") );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::EResumeVideoWhenPrepared ) );
    assertL( err == KErrNotReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    }

void CCaeTestVideoStates::TestResumeWhenNotPausedL()
    {
	// ID:      CAE.VR.RESUMEVIDEORECORDING.003
    //
    // Group:   Record.
    // 
    // Action : Resume video clip recording before pause. 
    //          Video recording.
	//      
	// Output : Leave.   

    iStatesClient->SetTimeout( 5000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoFileName( _L("cae_pause_resume.3gp") );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::EResumeVideoWhenNotPaused ) );
    assertL( err == KErrNotReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 2 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*              PREPARE VIDEO RECORDING                 */
/********************************************************/

void CCaeTestVideoStates::TestPrepareVideoRecordingNeg1L()
    {
	// ID: CAE.VR.PREPAREVIDEORECORDINGL.005
    //
    // Group:   Record
    // 
    // Action : Try to prepare the engine for video recording using invalid quality level index. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetVideoQualityLevelIndex( -1 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    //iStatesClient->SetVideoFileName( _L("cae_prepare_vr_-1.3gp") );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareVideoRecording ) );
    assertL( error != KErrNone );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 0 );
    }

void CCaeTestVideoStates::TestPrepareVideoRecording3L()
    {
	// ID: CAE.VR.PREPAREVIDEORECORDINGL.006
    //
    // Group:   Record
    // 
    // Action : Try to prepare the engine for video recording using invalid quality level index. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetVideoQualityLevelIndex( 3 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    //iStatesClient->SetVideoFileName( _L("cae_prepare_vr_3.3gp") );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareVideoRecording ) );
    assertL( error != KErrNone );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 0 );
    }

void CCaeTestVideoStates::TestSpecPrepareVRUnsupportedFormatWhenVRInitializedL()
{
    // ID: CAE.VR.PREPAREVIDEORECORDINGL.008
    //
    // Group:   Record.
    // 
    // Action : Set video recording format to a format unsupported by MMF Camcorder plug-in 
    //          and Camera API implementation. 
    //          Video recorder initialization is done.
	//      
	// Output : Leave.    

    iStatesClient->SetTimeout( 3000000 );

    TSize sizeTooBig( KImgWidthTooBig, KImgHeightTooBig );
    iStatesClient->SetVideoFrameSize( sizeTooBig );
    iStatesClient->SetVideoFrameRate( 10.0 );
    iStatesClient->SetVideoBitRate( 50000 );
    iStatesClient->SetVideoAudio( ETrue );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    //iStatesClient->SetVideoFileName( _L("cae_video_unsupported_format.3gp") );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareVideo ) );//verified if is correct action
    assertL( error != KErrNone );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    }

void CCaeTestVideoStates::TestSpecPrepareVRSupportedFormatWhenPreparedL()
{
    // ID: CAE.VR.PREPAREVIDEORECORDINGL.009
    //
    // Group:   Record.
    // 
    // Action : Set video recording format to a format supported by MMF Camcorder plug-in 
    //          and Camera API implementation when video recording is prepared already. .
	//      
	// Output : OK.    

    iStatesClient->SetTimeout( 5000000 );

    TSize sizeSubqcif( KImgWidthSubQCIF, KImgHeightSubQCIF );
    iStatesClient->SetVideoFrameSize( sizeSubqcif );
    iStatesClient->SetVideoFrameRate( 10.0 );
    iStatesClient->SetVideoBitRate( 50000 );
    iStatesClient->SetVideoAudio( ETrue );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    //iStatesClient->SetVideoFileName( _L("cae_video_format_prepared.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareVideoWhenPrepared );//verified if is correct action

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 2 );
    }

/********************************************************/
/*              START VIDEO RECORDING                   */
/********************************************************/

void CCaeTestVideoStates::TestStartVideoRecordingWhenVRInitializedL()
    {
	// ID: CAE.VR.STARTVIDEORECORDING.001
    //
    // Group:   Record
    // 
    // Action : Start video clip recording. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetTimeout( 3000000 );
    
    iStatesClient->SetVideoQualityLevelIndex( 0 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVideoRecordingWhenInitialized ) );//we have to put correct action
    assertL( error == KErrNotReady );
    
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    }

void CCaeTestVideoStates::TestStartVideoRecordingWhenRecordingL()
    {
	// ID: CAE.VR.STARTVIDEORECORDING.002
    //
    // Group:   Record
    // 
    // Action : Start video clip recording. 
    //          Video recording on.
	//      
	// Output : Leave.   

    iStatesClient->SetTimeout( 3000000 );
    
    iStatesClient->SetVideoQualityLevelIndex( 0 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVideoRecordingWhenRecording ) );//we have to put correct action
    assertL( error == KErrNotReady );
    
    assertL( iStatesClient->CountVideoRecordingOn() == 2 );
    }

/********************************************************/
/*              STOP VIDEO RECORDING                    */
/********************************************************/

void CCaeTestVideoStates::TestStopVideoRecordingWhenVRInitializedL()
    {
	// ID: CAE.VR.STOPVIDEORECORDING.001
    //
    // Group:   Record
    // 
    // Action : Stop video clip recording. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStopVideoRecordingWhenInitialized ) );//we have to put correct action
    assertL( error == KErrNotReady );
    
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestStopVideoRecordingWhenPreparedL()
    {
	// ID: CAE.VR.STOPVIDEORECORDING.002
    //
    // Group:   Record
    // 
    // Action : Stop video clip recording. 
    //          Video recording prepared.
	//      
	// Output : Leave.   

    iStatesClient->SetVideoQualityLevelIndex( 0 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    //iStatesClient->SetVideoFileName( _L("cae_stop_vr_prepared.3gp") );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStopVideoRecordingWhenPrepared ) );//we have to put correct action
    assertL( error == KErrNotReady );
    
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*                    STATE CHANGE                      */
/********************************************************/

void CCaeTestVideoStates::TestCaptureRecordL()
    {
	// ID:      CAE.STATECHANGE.CAPTURERECORD.002
    //
    // Group:   Capture & Record.
    // 
    // Action : Capture photo first and then record video successively
	//      
	// Output : No leave.    

    iStatesClient->SetTimeout( 1000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetStillQualityLevelIndex( 0 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ECaptureRecord );

    assertL( iStatesClient->CountInitComplete() == 1 );

    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );

    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestRecordCaptureL()
    {
	// ID:      CAE.STATECHANGE.RECORDCAPTURE.003
    //
    // Group:   Record & Capture.
    // 
    // Action : Record video first and then capture photo successively
	//      
	// Output : No leave.    

    iStatesClient->SetTimeout( 1000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetStillQualityLevelIndex( 0 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERecordCapture );

    assertL( iStatesClient->CountInitComplete() == 1 );

    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );

    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestVideoStates::TestRecordRecordL()
    {
	// ID:      CAE.STATECHANGE.RECORDRECORD.004
    //
    // Group:   Record.
    // 
    // Action : Record video two times successively
	//      
	// Output : No leave.    

    iStatesClient->SetTimeout( 2000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERecordRecord );

    assertL( iStatesClient->CountInitComplete() == 1 );

    // This should equal 2 because before 2nd recording we change the file name 
    // and it needs preparation.
    assertL( iStatesClient->CountVideoPrepareComplete() == 2 );

    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 2 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 2 );
    }


void CCaeTestVideoStates::TestRecordCaptureComplexSequence1L()
    {
	// ID:      CAE.STATECHANGE.RECORDCAPTURECOMPLEX1.005
    //
    // Group:   Record.
    // 
    // Action : Record and capture multiple times successively
	//      
	// Output : No leave.    

    iStatesClient->SetTimeout( 15000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetVideoClipMaxSize( 10000 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERecordCaptureComplexSequence1 );

    assertL( iStatesClient->CountInitComplete() == 2 );
    assertL( iStatesClient->CountStillPrepareComplete() == 2 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 7 );
    assertL( iStatesClient->CountVideoRecordingOn() == 7 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 7 );
    assertL( iStatesClient->CountSnapImageReady() == 2 );
    assertL( iStatesClient->CountStillImageReady() == 2 );
    }


void CCaeTestVideoStates::TestRecordCaptureComplexSequence2L()
    {
	// ID:      CAE.STATECHANGE.RECORDCAPTURECOMPLEX2.006
    //
    // Group:   Record.
    // 
    // Action : Record and capture multiple times successively. Contains also still bursts!
	//      
	// Output : No leave.    

    const TInt KStillBurstLength = 3;

    iStatesClient->SetSnapFileName( _L("cae_complex2.mbm") );

    iStatesClient->SetStillFileName( _L("cae_complex2.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->SetTimeout( 15000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetStillQualityLevelIndex( 0 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoClipMaxSize( 10000 );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERecordCaptureComplexSequence2 );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 2 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 4 /*3*/ );
    assertL( iStatesClient->CountVideoRecordingOn() == 4 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 4 );
    assertL( iStatesClient->CountSnapImageReady() == ( ( 2 * KStillBurstLength ) + 3 ) );
    assertL( iStatesClient->CountStillImageReady() == ( ( 2 * KStillBurstLength ) + 3 ) );

    assertL( iStatesClient->CountStillBurstCaptureMoment() == ( 2 * KStillBurstLength ) );
    assertL( iStatesClient->CountStillBurstComplete() == 2 );
    }


void CCaeTestVideoStates::TestReleaseReserveRecordL()
    {
    // ID: CAE.GEN.RELESEANDRESERVE.006
	//
    // General
    // 
    // Action : Try to re-reserve the camera module after initial reservation
    //          and releasing. Then capture still image.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 3000000 );

    iStatesClient->SetVideoQualityLevelIndex( 2 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoFileName( _L("cae_relres_ql_2.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EReleaseReserveRecord );

    assertL( iStatesClient->CountInitComplete() == 2 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


/********************************************************/
/*                "DESTROY ENGINE"-TESTS                */
/********************************************************/


void CCaeTestVideoStates::TestDestroy1L()
    {
	// ID: CAE.VR.DESTROY.001
    //
    // Group:   Record
    // 
    // Action : Test to destroy the engine when video is prepared.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 10000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenVideoPrepareReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestDestroy2L()
    {
	// ID: CAE.VR.DESTROY.002
    //
    // Group:   Record
    // 
    // Action : Test to destroy the engine when video recording is on.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 10000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenVideoRecordingOn );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestDestroy3L()
    {
	// ID: CAE.VR.DESTROY.003
    //
    // Group:   Record
    // 
    // Action : Test to destroy the engine when video recording is paused.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 10000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenVideoRecordingPaused );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestDestroy4L()
    {
	// ID: CAE.VR.DESTROY.004
    //
    // Group:   Record
    // 
    // Action : Test to destroy the engine when video recording is completed.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 10000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenVideoRecordingReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


/********************************************************/
/*                ERROR SIMULATIONS                     */
/********************************************************/

void CCaeTestVideoStates::TestVideoRecorderOpenErrorL()
    {
	// ID:      CAE.VR.ERRORS.001
    //
    // Group:   Errors.
    // 
    // Action : Test error handling with video recorder opening.
	//      
	// Output : Leave with code KErrAbort.

    iStatesClient->SetVideoQualityLevelIndex( 0 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EVideoRecorderOpenError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestVideoRecorderPrepareErrorL()
    {
	// ID:      CAE.VR.ERRORS.002
    //
    // Group:   Errors.
    // 
    // Action : Test error handling with video recorder preparation.
	//      
	// Output : Leave with code KErrAbort.

    iStatesClient->SetVideoQualityLevelIndex( 0 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EVideoRecorderPrepareError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestVideoRecordingCompleteErrorL()
    {
	// ID:      CAE.VR.ERRORS.003
    //
    // Group:   Errors.
    // 
    // Action : Test error handling with video recording completion.
	//      
	// Output : Leave with code KErrAbort.

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EVideoRecordingCompleteError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


void CCaeTestVideoStates::TestPrepareVideoSettingsErrorL()
    {
	// ID:      CAE.VR.ERRORS.004
    //
    // Group:   Errors.
    // 
    // Action : Test error handling with video recorder settings preparation.
	//      
	// Output : Leave with code KErrAbort.

    iStatesClient->SetVideoQualityLevelIndex( 0 );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareVideoSettingsError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

/********************************************************/
/*      PREPARE AUDIO BIT RATE AND RECORD VIDEO         */
/********************************************************/

void CCaeTestVideoStates::TestPrepareAudioBitRateVR0L()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.002
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 0 ) );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_0.3gp") );

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo ) );
    assertL( err == KErrArgument );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR5150L()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.003
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 5150 ) );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_5150.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR12200L()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.004
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 12200 ) );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_12200.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR20000L()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.005
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 20000 ) );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_20000.3gp") );

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo ) );
    assertL( err == KErrArgument );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR12200AudioType0L()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.006
    //
    // Group:   Record
    // 
    // Action : Test with special audio type 0
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 12200 ) );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_12200AT0.3gp") );

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideoAT0 );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR12200AudioTypeInvL()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.007
    //
    // Group:   Record
    // 
    // Action : Test with special audio type
	//      
	// Output : Leave

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 12200 ) );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_12200ATInv.3gp") );

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideoATInv ) );
    assertL( err == KErrArgument );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

/********************************************************/
/*      MISC tests                                      */
/********************************************************/
void CCaeTestVideoStates::TestInitVideoRecorderWithUidL()
    {
    // ID: CAE.GEN.INITVIDEORECORDERL.002
	//
    // General
    // 
    // Action : Test Call InitVideoRecorderL() when initialization is done with Uid.
	//      
	// Output : Leave, Uid init is not supported.

    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video
    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::EInitVideoRecorderWithUid ));
    assertL( err == KErrNotSupported );
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

/********************************************************/
/*        AMR audio tests (set 1 )                      */
/********************************************************/
/********************************************************/
/*  VIDEO RECORDING PREPARE AND RECORD QUALITY LEVEL    */
/********************************************************/

void CCaeTestVideoStates::TestPrepareAndRecordQualityLevel0QL()
    {
	// ID:      CAE.VR.VIDEORECORDING.QCIF64000FR15.Q.004
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 0 
    //          video clip when initialization is done.
	//      
	// Output : No leave. File cae_novf_ql_0.3gp.  

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetTimeout( 5000000 );

    iStatesClient->SetVideoFileName( _L("cae_novf_ql_0Q.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAndRecordQualityLevel1QL()
    {
	// ID:      CAE.VR.VIDEORECORDING.SUBQCIF60000FR15.Q.005
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 1 
    //          video clip when initialization is done.
	//      
	// Output : No leave. File cae_novf_ql_1.3gp.  

    iStatesClient->SetVideoQualityLevelIndex( 1 );

    iStatesClient->SetTimeout( 5000000 );

    iStatesClient->SetVideoFileName( _L("cae_novf_ql_1Q.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAndRecordQualityLevel2QL()
    {
	// ID:      CAE.VR.VIDEORECORDING.SUBQCIF40000FR5.Q.006
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 2 
    //          video clip when initialization is done.
	//      
	// Output : No leave. File cae_novf_ql_2.3gp.  

    iStatesClient->SetVideoQualityLevelIndex( 2 );

    iStatesClient->SetTimeout( 5000000 );

    iStatesClient->SetVideoFileName( _L("cae_novf_ql_2Q.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


/********************************************************/
/*  VIDEO RECORDING SPECIAL PREPARE AND RECORD          */
/********************************************************/

void CCaeTestVideoStates::TestVideoRecordingQcif60000Fr10NoAudioNoVfQL()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q.003
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording QCIF sized video works. 
    //          Video recording prepared for QCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 20000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );

    iStatesClient->SetRunningWithViewFinder( EFalse );

    iStatesClient->SetVideoFileName( _L("cae_qcif10fpsQ.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestVideoRecordingQcif60000Fr10WithAudioNoVfQL()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q.004
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording QCIF sized video works. 
    //          Video recording prepared for QCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 20000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( ETrue );

    iStatesClient->SetRunningWithViewFinder( EFalse );

    iStatesClient->SetVideoFileName( _L("cae_qcif10fps_audioQ.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestVideoRecordingQcif60000Fr10NoAudioWithVfQL()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q.005
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording QCIF sized video works. 
    //          Video recording prepared for QCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 20000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );

    iStatesClient->SetViewFinderSize( sizeQcif );
    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoFileName( _L("cae_qcif10fps_vfQ.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestVideoRecordingQcif60000Fr10WithAudioWithVfQL()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q.006
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording QCIF sized video works. 
    //          Video recording prepared for QCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 20000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( ETrue );

    iStatesClient->SetViewFinderSize( sizeQcif );
    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoFileName( _L("cae_qcif10fps_audio_vfQ.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*              PAUSE VIDEO RECORDING                   */
/********************************************************/

void CCaeTestVideoStates::TestRecordPauseStopQL()
    {
	// ID:      CAE.VR.PAUSEVIDEORECORDING.Q.001
    //
    // Group:   Record.
    // 
    // Action : Pause video clip recording.Video recordin prepared.
	//      
	// Output : No leave. File KVideoFileName, "videoclip.3gp".   

    iStatesClient->SetTimeout( 3000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareRecordPauseStopVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPauseWhenPreparedQL()
    {
	// ID:      CAE.VR.PAUSEVIDEORECORDING.Q.002
    //
    // Group:   Record.
    // 
    // Action : Call Pause when video is prepared but video recordind is not started.
	//      
	// Output : Leave..   

    iStatesClient->SetTimeout( 5000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPauseVideoWhenPrepared ) );
    assertL( err == KErrNotReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

void CCaeTestVideoStates::TestRecordPausePauseStopQL()
    {
	// ID:      CAE.VR.PAUSEVIDEORECORDING.Q.003
    //
    // Group:   Record.
    // 
    // Action : Pause video clip recording twice.Video recordin prepared.
	//      
	// Output : No leave. File KVideoFileName, "videoclip.3gp".   

    iStatesClient->SetTimeout( 3000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareRecordPausePauseStopVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 2 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*              RESUME VIDEO RECORDING                  */
/********************************************************/

void CCaeTestVideoStates::TestResumeWhenPreparedQL()
    {
	// ID:      CAE.VR.RESUMEVIDEORECORDING.Q.002
    //
    // Group:   Record.
    // 
    // Action : Resume video clip recording before pause. 
    //          Video recording.
	//      
	// Output : Leave.   

    iStatesClient->SetTimeout( 3000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoFileName( _L("cae_pause_resumeQ.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::EResumeVideoWhenPrepared ) );
    assertL( err == KErrNotReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    }

void CCaeTestVideoStates::TestResumeWhenNotPausedQL()
    {
	// ID:      CAE.VR.RESUMEVIDEORECORDING.Q.003
    //
    // Group:   Record.
    // 
    // Action : Resume video clip recording before pause. 
    //          Video recording.
	//      
	// Output : Leave.   

    iStatesClient->SetTimeout( 5000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoFileName( _L("cae_pause_resumeQ.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::EResumeVideoWhenNotPaused ) );
    assertL( err == KErrNotReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 2 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*              PREPARE VIDEO RECORDING                 */
/********************************************************/

void CCaeTestVideoStates::TestPrepareVideoRecordingNeg1QL()
    {
	// ID: CAE.VR.PREPAREVIDEORECORDINGL.Q.005
    //
    // Group:   Record
    // 
    // Action : Try to prepare the engine for video recording using invalid quality level index. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetVideoQualityLevelIndex( -1 );

    //iStatesClient->SetVideoFileName( _L("cae_prepare_vr_-1Q.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareVideoRecording ) );
    assertL( error != KErrNone );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 0 );
    }

void CCaeTestVideoStates::TestPrepareVideoRecording3QL()
    {
	// ID: CAE.VR.PREPAREVIDEORECORDINGL.Q.006
    //
    // Group:   Record
    // 
    // Action : Try to prepare the engine for video recording using invalid quality level index. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetVideoQualityLevelIndex( 3 );

    //iStatesClient->SetVideoFileName( _L("cae_prepare_vr_3Q.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareVideoRecording ) );
    assertL( error != KErrNone );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 0 );
    }

void CCaeTestVideoStates::TestSpecPrepareVRUnsupportedFormatWhenVRInitializedQL()
{
    // ID: CAE.VR.PREPAREVIDEORECORDINGL.Q.008
    //
    // Group:   Record.
    // 
    // Action : Set video recording format to a format unsupported by MMF Camcorder plug-in 
    //          and Camera API implementation. 
    //          Video recorder initialization is done.
	//      
	// Output : Leave.    

    iStatesClient->SetTimeout( 3000000 );

    TSize sizeTooBig( KImgWidthTooBig, KImgHeightTooBig );
    iStatesClient->SetVideoFrameSize( sizeTooBig );
    iStatesClient->SetVideoFrameRate( 10.0 );
    iStatesClient->SetVideoBitRate( 50000 );
    iStatesClient->SetVideoAudio( ETrue );

    //iStatesClient->SetVideoFileName( _L("cae_video_unsupported_formatQ.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareVideo ) );//verified if is correct action
    assertL( error != KErrNone );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    }

void CCaeTestVideoStates::TestSpecPrepareVRSupportedFormatWhenPreparedQL()
{
    // ID: CAE.VR.PREPAREVIDEORECORDINGL.Q.009
    //
    // Group:   Record.
    // 
    // Action : Set video recording format to a format supported by MMF Camcorder plug-in 
    //          and Camera API implementation when video recording is prepared already. .
	//      
	// Output : OK.    

    iStatesClient->SetTimeout( 5000000 );

    TSize sizeSubqcif( KImgWidthSubQCIF, KImgHeightSubQCIF );
    iStatesClient->SetVideoFrameSize( sizeSubqcif );
    iStatesClient->SetVideoFrameRate( 10.0 );
    iStatesClient->SetVideoBitRate( 50000 );
    iStatesClient->SetVideoAudio( ETrue );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    //iStatesClient->SetVideoFileName( _L("cae_video_format_preparedQ.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareVideoWhenPrepared );//verified if is correct action

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 2 );
    }

/********************************************************/
/*              START VIDEO RECORDING                   */
/********************************************************/

void CCaeTestVideoStates::TestStartVideoRecordingWhenVRInitializedQL()
    {
	// ID: CAE.VR.STARTVIDEORECORDING.Q.001
    //
    // Group:   Record
    // 
    // Action : Start video clip recording. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetTimeout( 3000000 );
    
    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVideoRecordingWhenInitialized ) );//we have to put correct action
    assertL( error == KErrNotReady );
    
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    }

void CCaeTestVideoStates::TestStartVideoRecordingWhenRecordingQL()
    {
	// ID: CAE.VR.STARTVIDEORECORDING.Q.002
    //
    // Group:   Record
    // 
    // Action : Start video clip recording. 
    //          Video recording on.
	//      
	// Output : Leave.   

    iStatesClient->SetTimeout( 3000000 );
    
    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVideoRecordingWhenRecording ) );//we have to put correct action
    assertL( error == KErrNotReady );
    
    assertL( iStatesClient->CountVideoRecordingOn() == 2 );
    }

/********************************************************/
/*              STOP VIDEO RECORDING                    */
/********************************************************/

void CCaeTestVideoStates::TestStopVideoRecordingWhenVRInitializedQL()
    {
	// ID: CAE.VR.STOPVIDEORECORDING.Q.001
    //
    // Group:   Record
    // 
    // Action : Stop video clip recording. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStopVideoRecordingWhenInitialized ) );//we have to put correct action
    assertL( error == KErrNotReady );
    
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestStopVideoRecordingWhenPreparedQL()
    {
	// ID: CAE.VR.STOPVIDEORECORDING.Q.002
    //
    // Group:   Record
    // 
    // Action : Stop video clip recording. 
    //          Video recording prepared.
	//      
	// Output : Leave.   

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    //iStatesClient->SetVideoFileName( _L("cae_stop_vr_preparedQ.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStopVideoRecordingWhenPrepared ) );//we have to put correct action
    assertL( error == KErrNotReady );
    
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*                    STATE CHANGE                      */
/********************************************************/

void CCaeTestVideoStates::TestCaptureRecordQL()
    {
	// ID:      CAE.STATECHANGE.CAPTURERECORD.Q.002
    //
    // Group:   Capture & Record.
    // 
    // Action : Capture photo first and then record video successively
	//      
	// Output : No leave.    

    iStatesClient->SetTimeout( 1000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ECaptureRecord );

    assertL( iStatesClient->CountInitComplete() == 1 );

    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );

    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestRecordCaptureQL()
    {
	// ID:      CAE.STATECHANGE.RECORDCAPTURE.Q.003
    //
    // Group:   Record & Capture.
    // 
    // Action : Record video first and then capture photo successively
	//      
	// Output : No leave.    

    iStatesClient->SetTimeout( 1000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERecordCapture );

    assertL( iStatesClient->CountInitComplete() == 1 );

    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );

    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestVideoStates::TestRecordRecordQL()
    {
	// ID:      CAE.STATECHANGE.RECORDRECORD.Q.004
    //
    // Group:   Record.
    // 
    // Action : Record video two times successively
	//      
	// Output : No leave.    

    iStatesClient->SetTimeout( 2000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERecordRecord );

    assertL( iStatesClient->CountInitComplete() == 1 );

    // This should equal 2 because before 2nd recording we change the file name 
    // and it needs preparation.
    assertL( iStatesClient->CountVideoPrepareComplete() == 2 );

    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 2 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 2 );
    }


void CCaeTestVideoStates::TestRecordCaptureComplexSequence1QL()
    {
	// ID:      CAE.STATECHANGE.RECORDCAPTURECOMPLEX1.Q.005
    //
    // Group:   Record.
    // 
    // Action : Record and capture multiple times successively
	//      
	// Output : No leave.    

    iStatesClient->SetTimeout( 15000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetVideoClipMaxSize( 10000 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERecordCaptureComplexSequence1 );

    assertL( iStatesClient->CountInitComplete() == 2 );
    assertL( iStatesClient->CountStillPrepareComplete() == 2 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 7 );
    assertL( iStatesClient->CountVideoRecordingOn() == 7 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 7 );
    assertL( iStatesClient->CountSnapImageReady() == 2 );
    assertL( iStatesClient->CountStillImageReady() == 2 );
    }


void CCaeTestVideoStates::TestRecordCaptureComplexSequence2QL()
    {
	// ID:      CAE.STATECHANGE.RECORDCAPTURECOMPLEX2.Q.006
    //
    // Group:   Record.
    // 
    // Action : Record and capture multiple times successively. Contains also still bursts!
	//      
	// Output : No leave.    

    const TInt KStillBurstLength = 3;

    iStatesClient->SetSnapFileName( _L("cae_complex2Q.mbm") );

    iStatesClient->SetStillFileName( _L("cae_complex2Q.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->SetTimeout( 15000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetStillQualityLevelIndex( 2 );

    iStatesClient->SetVideoClipMaxSize( 10000 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERecordCaptureComplexSequence2 );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 2 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 4 /*3*/ );
    assertL( iStatesClient->CountVideoRecordingOn() == 4 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 4 );
    assertL( iStatesClient->CountSnapImageReady() == ( ( 2 * KStillBurstLength ) + 3 ) );
    assertL( iStatesClient->CountStillImageReady() == ( ( 2 * KStillBurstLength ) + 3 ) );

    assertL( iStatesClient->CountStillBurstCaptureMoment() == ( 2 * KStillBurstLength ) );
    assertL( iStatesClient->CountStillBurstComplete() == 2 );
    }


void CCaeTestVideoStates::TestReleaseReserveRecordQL()
    {
    // ID: CAE.GEN.RELESEANDRESERVE.Q.006
	//
    // General
    // 
    // Action : Try to re-reserve the camera module after initial reservation
    //          and releasing. Then capture still image.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 3000000 );

    iStatesClient->SetVideoQualityLevelIndex( 2 );

    iStatesClient->SetVideoFileName( _L("cae_relres_ql_2Q.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EReleaseReserveRecord );

    assertL( iStatesClient->CountInitComplete() == 2 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


/********************************************************/
/*                "DESTROY ENGINE"-TESTS                */
/********************************************************/


void CCaeTestVideoStates::TestDestroy1QL()
    {
	// ID: CAE.VR.DESTROY.Q.001
    //
    // Group:   Record
    // 
    // Action : Test to destroy the engine when video is prepared.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 10000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );

    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenVideoPrepareReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestDestroy2QL()
    {
	// ID: CAE.VR.DESTROY.Q.002
    //
    // Group:   Record
    // 
    // Action : Test to destroy the engine when video recording is on.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 10000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );

    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenVideoRecordingOn );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestDestroy3QL()
    {
	// ID: CAE.VR.DESTROY.Q.003
    //
    // Group:   Record
    // 
    // Action : Test to destroy the engine when video recording is paused.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 10000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );

    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenVideoRecordingPaused );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestDestroy4QL()
    {
	// ID: CAE.VR.DESTROY.Q.004
    //
    // Group:   Record
    // 
    // Action : Test to destroy the engine when video recording is completed.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 10000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );

    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenVideoRecordingReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


/********************************************************/
/*                ERROR SIMULATIONS                     */
/********************************************************/

void CCaeTestVideoStates::TestVideoRecorderOpenErrorQL()
    {
	// ID:      CAE.VR.ERRORS.Q.001
    //
    // Group:   Errors.
    // 
    // Action : Test error handling with video recorder opening.
	//      
	// Output : Leave with code KErrAbort.

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EVideoRecorderOpenError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestVideoRecorderPrepareErrorQL()
    {
	// ID:      CAE.VR.ERRORS.Q.002
    //
    // Group:   Errors.
    // 
    // Action : Test error handling with video recorder preparation.
	//      
	// Output : Leave with code KErrAbort.

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EVideoRecorderPrepareError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestVideoRecordingCompleteErrorQL()
    {
	// ID:      CAE.VR.ERRORS.Q.003
    //
    // Group:   Errors.
    // 
    // Action : Test error handling with video recording completion.
	//      
	// Output : Leave with code KErrAbort.

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EVideoRecordingCompleteError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


void CCaeTestVideoStates::TestPrepareVideoSettingsErrorQL()
    {
	// ID:      CAE.VR.ERRORS.Q.004
    //
    // Group:   Errors.
    // 
    // Action : Test error handling with video recorder settings preparation.
	//      
	// Output : Leave with code KErrAbort.

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareVideoSettingsError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

/********************************************************/
/*      PREPARE AUDIO BIT RATE AND RECORD VIDEO         */
/********************************************************/

void CCaeTestVideoStates::TestPrepareAudioBitRateVR0QL()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q.002
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 0 ) );

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_0Q.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo ) );
    assertL( err == KErrArgument );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR5150QL()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q.003
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 5150 ) );

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_5150Q.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR12200QL()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q.004
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 12200 ) );

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_12200Q.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR20000QL()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q.005
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 20000 ) );

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_20000Q.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo ) );
    assertL( err == KErrArgument );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR12200AudioType0QL()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q.006
    //
    // Group:   Record
    // 
    // Action : Test with special audio type 0
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 12200 ) );

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_12200AT0Q.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideoAT0 );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR12200AudioTypeInvQL()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q.007
    //
    // Group:   Record
    // 
    // Action : Test with special audio type
	//      
	// Output : Leave

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 12200 ) );

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_12200ATInvQ.3gp") );

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideoATInv ) );
    assertL( err == KErrArgument );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

/********************************************************/
/*      MISC tests                                      */
/********************************************************/
void CCaeTestVideoStates::TestInitVideoRecorderWithUidQL()
    {
    // ID: CAE.GEN.INITVIDEORECORDERL.Q.002
	//
    // General
    // 
    // Action : Test Call InitVideoRecorderL() when initialization is done with Uid.
	//      
	// Output : Leave, Uid init is not supported.

    iStatesClient->SetVideoCodecSet( 1 ); // 1: AMR audio, H.263 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::EInitVideoRecorderWithUid ));
    assertL( err == KErrNotSupported );
    assertL( iStatesClient->CountInitComplete() == 1 );
    }





/********************************************************/
/*      AAC audio tests (set 2 )                      */
/********************************************************/
/********************************************************/
/*  VIDEO RECORDING PREPARE AND RECORD QUALITY LEVEL    */
/********************************************************/

void CCaeTestVideoStates::TestPrepareAndRecordQualityLevel0Q4L()
    {
	// ID:      CAE.VR.VIDEORECORDING.QCIF64000FR15.Q4.004
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 0 
    //          video clip when initialization is done.
	//      
	// Output : No leave. File cae_novf_ql_0.3gp.  

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetTimeout( 5000000 );

    iStatesClient->SetVideoFileName( _L("cae_novf_ql_0Q4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAndRecordQualityLevel1Q4L()
    {
	// ID:      CAE.VR.VIDEORECORDING.SUBQCIF60000FR15.Q4.005
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 1 
    //          video clip when initialization is done.
	//      
	// Output : No leave. File cae_novf_ql_1.3gp.  

    iStatesClient->SetVideoQualityLevelIndex( 1 );

    iStatesClient->SetTimeout( 5000000 );

    iStatesClient->SetVideoFileName( _L("cae_novf_ql_1Q4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAndRecordQualityLevel2Q4L()
    {
	// ID:      CAE.VR.VIDEORECORDING.SUBQCIF40000FR5.Q4.006
    //
    // Group:   Record.
    // 
    // Action : Test to prepare video recording and record quality level 2 
    //          video clip when initialization is done.
	//      
	// Output : No leave. File cae_novf_ql_2.3gp.  

    iStatesClient->SetVideoQualityLevelIndex( 2 );

    iStatesClient->SetTimeout( 5000000 );

    iStatesClient->SetVideoFileName( _L("cae_novf_ql_2Q4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


/********************************************************/
/*  VIDEO RECORDING SPECIAL PREPARE AND RECORD          */
/********************************************************/

void CCaeTestVideoStates::TestVideoRecordingQcif60000Fr10NoAudioNoVfQ4L()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q4.003
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording QCIF sized video works. 
    //          Video recording prepared for QCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 20000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );

    iStatesClient->SetRunningWithViewFinder( EFalse );

    iStatesClient->SetVideoFileName( _L("cae_qcif10fpsQ4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestVideoRecordingQcif60000Fr10WithAudioNoVfQ4L()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q4.004
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording QCIF sized video works. 
    //          Video recording prepared for QCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 20000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( ETrue );

    iStatesClient->SetRunningWithViewFinder( EFalse );

    iStatesClient->SetVideoFileName( _L("cae_qcif10fps_audioQ4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestVideoRecordingQcif60000Fr10NoAudioWithVfQ4L()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q4.005
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording QCIF sized video works. 
    //          Video recording prepared for QCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 20000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );

    iStatesClient->SetViewFinderSize( sizeQcif );
    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoFileName( _L("cae_qcif10fps_vfQ4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestVideoRecordingQcif60000Fr10WithAudioWithVfQ4L()
    {
	// ID: CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q4.006
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
    //          To ensure that starting and stopping video recording works. 
    //          To ensure that recording QCIF sized video works. 
    //          Video recording prepared for QCIF format.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 20000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( ETrue );

    iStatesClient->SetViewFinderSize( sizeQcif );
    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoFileName( _L("cae_qcif10fps_audio_vfQ4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*              PAUSE VIDEO RECORDING                   */
/********************************************************/

void CCaeTestVideoStates::TestRecordPauseStopQ4L()
    {
	// ID:      CAE.VR.PAUSEVIDEORECORDING.Q4.001
    //
    // Group:   Record.
    // 
    // Action : Pause video clip recording.Video recordin prepared.
	//      
	// Output : No leave. File KVideoFileName, "videoclip.3gp".   

    iStatesClient->SetTimeout( 3000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareRecordPauseStopVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPauseWhenPreparedQ4L()
    {
	// ID:      CAE.VR.PAUSEVIDEORECORDING.Q4.002
    //
    // Group:   Record.
    // 
    // Action : Call Pause when video is prepared but video recordind is not started.
	//      
	// Output : Leave..   

    iStatesClient->SetTimeout( 5000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPauseVideoWhenPrepared ) );
    assertL( err == KErrNotReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

void CCaeTestVideoStates::TestRecordPausePauseStopQ4L()
    {
	// ID:      CAE.VR.PAUSEVIDEORECORDING.Q4.003
    //
    // Group:   Record.
    // 
    // Action : Pause video clip recording twice.Video recordin prepared.
	//      
	// Output : No leave. File KVideoFileName, "videoclip.3gp".   

    iStatesClient->SetTimeout( 3000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareRecordPausePauseStopVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 2 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*              RESUME VIDEO RECORDING                  */
/********************************************************/

void CCaeTestVideoStates::TestResumeWhenPreparedQ4L()
    {
	// ID:      CAE.VR.RESUMEVIDEORECORDING.Q4.002
    //
    // Group:   Record.
    // 
    // Action : Resume video clip recording before pause. 
    //          Video recording.
	//      
	// Output : Leave.   

    iStatesClient->SetTimeout( 3000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoFileName( _L("cae_pause_resumeQ4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::EResumeVideoWhenPrepared ) );
    assertL( err == KErrNotReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    }

void CCaeTestVideoStates::TestResumeWhenNotPausedQ4L()
    {
	// ID:      CAE.VR.RESUMEVIDEORECORDING.Q4.003
    //
    // Group:   Record.
    // 
    // Action : Resume video clip recording before pause. 
    //          Video recording.
	//      
	// Output : Leave.   

    iStatesClient->SetTimeout( 5000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoFileName( _L("cae_pause_resumeQ4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::EResumeVideoWhenNotPaused ) );
    assertL( err == KErrNotReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 2 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*              PREPARE VIDEO RECORDING                 */
/********************************************************/

void CCaeTestVideoStates::TestPrepareVideoRecordingNeg1Q4L()
    {
	// ID: CAE.VR.PREPAREVIDEORECORDINGL.Q4.005
    //
    // Group:   Record
    // 
    // Action : Try to prepare the engine for video recording using invalid quality level index. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetVideoQualityLevelIndex( -1 );

    //iStatesClient->SetVideoFileName( _L("cae_prepare_vr_-1Q4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareVideoRecording ) );
    assertL( error != KErrNone );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 0 );
    }

void CCaeTestVideoStates::TestPrepareVideoRecording3Q4L()
    {
	// ID: CAE.VR.PREPAREVIDEORECORDINGL.Q4.006
    //
    // Group:   Record
    // 
    // Action : Try to prepare the engine for video recording using invalid quality level index. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetVideoQualityLevelIndex( 3 );

    //iStatesClient->SetVideoFileName( _L("cae_prepare_vr_3Q4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareVideoRecording ) );
    assertL( error != KErrNone );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 0 );
    }

void CCaeTestVideoStates::TestSpecPrepareVRUnsupportedFormatWhenVRInitializedQ4L()
{
    // ID: CAE.VR.PREPAREVIDEORECORDINGL.Q4.008
    //
    // Group:   Record.
    // 
    // Action : Set video recording format to a format unsupported by MMF Camcorder plug-in 
    //          and Camera API implementation. 
    //          Video recorder initialization is done.
	//      
	// Output : Leave.    

    iStatesClient->SetTimeout( 3000000 );

    TSize sizeTooBig( KImgWidthTooBig, KImgHeightTooBig );
    iStatesClient->SetVideoFrameSize( sizeTooBig );
    iStatesClient->SetVideoFrameRate( 10.0 );
    iStatesClient->SetVideoBitRate( 50000 );
    iStatesClient->SetVideoAudio( ETrue );

    //iStatesClient->SetVideoFileName( _L("cae_video_unsupported_formatQ4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareVideo ) );//verified if is correct action
    assertL( error != KErrNone );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    }

void CCaeTestVideoStates::TestSpecPrepareVRSupportedFormatWhenPreparedQ4L()
{
    // ID: CAE.VR.PREPAREVIDEORECORDINGL.Q4.009
    //
    // Group:   Record.
    // 
    // Action : Set video recording format to a format supported by MMF Camcorder plug-in 
    //          and Camera API implementation when video recording is prepared already. .
	//      
	// Output : OK.    

    iStatesClient->SetTimeout( 5000000 );

    TSize sizeSubqcif( KImgWidthSubQCIF, KImgHeightSubQCIF );
    iStatesClient->SetVideoFrameSize( sizeSubqcif );
    iStatesClient->SetVideoFrameRate( 10.0 );
    iStatesClient->SetVideoBitRate( 50000 );
    iStatesClient->SetVideoAudio( ETrue );
    iStatesClient->SetVideoCodecSet( 0 ); // 0: AMR audio, H.263 video

    //iStatesClient->SetVideoFileName( _L("cae_video_format_preparedQ4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareVideoWhenPrepared );//verified if is correct action

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 2 );
    }

/********************************************************/
/*              START VIDEO RECORDING                   */
/********************************************************/

void CCaeTestVideoStates::TestStartVideoRecordingWhenVRInitializedQ4L()
    {
	// ID: CAE.VR.STARTVIDEORECORDING.Q4.001
    //
    // Group:   Record
    // 
    // Action : Start video clip recording. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetTimeout( 3000000 );
    
    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVideoRecordingWhenInitialized ) );//we have to put correct action
    assertL( error == KErrNotReady );
    
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    }

void CCaeTestVideoStates::TestStartVideoRecordingWhenRecordingQ4L()
    {
	// ID: CAE.VR.STARTVIDEORECORDING.Q4.002
    //
    // Group:   Record
    // 
    // Action : Start video clip recording. 
    //          Video recording on.
	//      
	// Output : Leave.   

    iStatesClient->SetTimeout( 3000000 );
    
    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStartVideoRecordingWhenRecording ) );//we have to put correct action
    assertL( error == KErrNotReady );
    
    assertL( iStatesClient->CountVideoRecordingOn() == 2 );
    }

/********************************************************/
/*              STOP VIDEO RECORDING                    */
/********************************************************/

void CCaeTestVideoStates::TestStopVideoRecordingWhenVRInitializedQ4L()
    {
	// ID: CAE.VR.STOPVIDEORECORDING.Q4.001
    //
    // Group:   Record
    // 
    // Action : Stop video clip recording. 
    //          Initialization is done.
	//      
	// Output : Leave.   

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStopVideoRecordingWhenInitialized ) );//we have to put correct action
    assertL( error == KErrNotReady );
    
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestStopVideoRecordingWhenPreparedQ4L()
    {
	// ID: CAE.VR.STOPVIDEORECORDING.Q4.002
    //
    // Group:   Record
    // 
    // Action : Stop video clip recording. 
    //          Video recording prepared.
	//      
	// Output : Leave.   

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    //iStatesClient->SetVideoFileName( _L("cae_stop_vr_preparedQ4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EStopVideoRecordingWhenPrepared ) );//we have to put correct action
    assertL( error == KErrNotReady );
    
    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

/********************************************************/
/*                    STATE CHANGE                      */
/********************************************************/

void CCaeTestVideoStates::TestCaptureRecordQ4L()
    {
	// ID:      CAE.STATECHANGE.CAPTURERECORD.Q4.002
    //
    // Group:   Capture & Record.
    // 
    // Action : Capture photo first and then record video successively
	//      
	// Output : No leave.    

    iStatesClient->SetTimeout( 1000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ECaptureRecord );

    assertL( iStatesClient->CountInitComplete() == 1 );

    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );

    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestRecordCaptureQ4L()
    {
	// ID:      CAE.STATECHANGE.RECORDCAPTURE.Q4.003
    //
    // Group:   Record & Capture.
    // 
    // Action : Record video first and then capture photo successively
	//      
	// Output : No leave.    

    iStatesClient->SetTimeout( 1000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERecordCapture );

    assertL( iStatesClient->CountInitComplete() == 1 );

    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );

    assertL( iStatesClient->CountStillPrepareComplete() == 1 );
    assertL( iStatesClient->CountSnapImageReady() == 1 );
    assertL( iStatesClient->CountStillImageReady() == 1 );
    }

void CCaeTestVideoStates::TestRecordRecordQ4L()
    {
	// ID:      CAE.STATECHANGE.RECORDRECORD.Q4.004
    //
    // Group:   Record.
    // 
    // Action : Record video two times successively
	//      
	// Output : No leave.    

    iStatesClient->SetTimeout( 2000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERecordRecord );

    assertL( iStatesClient->CountInitComplete() == 1 );

    // This should equal 2 because before 2nd recording we change the file name 
    // and it needs preparation.
    assertL( iStatesClient->CountVideoPrepareComplete() == 2 );

    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 2 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 2 );
    }


void CCaeTestVideoStates::TestRecordCaptureComplexSequence1Q4L()
    {
	// ID:      CAE.STATECHANGE.RECORDCAPTURECOMPLEX1.Q4.005
    //
    // Group:   Record.
    // 
    // Action : Record and capture multiple times successively
	//      
	// Output : No leave.    

    iStatesClient->SetTimeout( 15000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetStillQualityLevelIndex( 0 );

    iStatesClient->SetVideoClipMaxSize( 10000 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERecordCaptureComplexSequence1 );

    assertL( iStatesClient->CountInitComplete() == 2 );
    assertL( iStatesClient->CountStillPrepareComplete() == 2 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 7 );
    assertL( iStatesClient->CountVideoRecordingOn() == 7 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 7 );
    assertL( iStatesClient->CountSnapImageReady() == 2 );
    assertL( iStatesClient->CountStillImageReady() == 2 );
    }


void CCaeTestVideoStates::TestRecordCaptureComplexSequence2Q4L()
    {
	// ID:      CAE.STATECHANGE.RECORDCAPTURECOMPLEX2.Q4.006
    //
    // Group:   Record.
    // 
    // Action : Record and capture multiple times successively. Contains also still bursts!
	//      
	// Output : No leave.    

    const TInt KStillBurstLength = 3;

    iStatesClient->SetSnapFileName( _L("cae_complex2Q4.mbm") );

    iStatesClient->SetStillFileName( _L("cae_complex2Q4.jpg") );

    iStatesClient->SetStillBurstLength( KStillBurstLength );

    iStatesClient->SetTimeout( 15000000 );

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetStillQualityLevelIndex( 2 );

    iStatesClient->SetVideoClipMaxSize( 10000 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERecordCaptureComplexSequence2 );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountStillPrepareComplete() == 2 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 4 /*3*/ );
    assertL( iStatesClient->CountVideoRecordingOn() == 4 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 4 );
    assertL( iStatesClient->CountSnapImageReady() == ( ( 2 * KStillBurstLength ) + 3 ) );
    assertL( iStatesClient->CountStillImageReady() == ( ( 2 * KStillBurstLength ) + 3 ) );

    assertL( iStatesClient->CountStillBurstCaptureMoment() == ( 2 * KStillBurstLength ) );
    assertL( iStatesClient->CountStillBurstComplete() == 2 );
    }


void CCaeTestVideoStates::TestReleaseReserveRecordQ4L()
    {
    // ID: CAE.GEN.RELESEANDRESERVE.Q4.006
	//
    // General
    // 
    // Action : Try to re-reserve the camera module after initial reservation
    //          and releasing. Then capture still image.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 3000000 );

    iStatesClient->SetVideoQualityLevelIndex( 2 );

    iStatesClient->SetVideoFileName( _L("cae_relres_ql_2Q4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EReleaseReserveRecord );

    assertL( iStatesClient->CountInitComplete() == 2 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


/********************************************************/
/*                "DESTROY ENGINE"-TESTS                */
/********************************************************/


void CCaeTestVideoStates::TestDestroy1Q4L()
    {
	// ID: CAE.VR.DESTROY.Q4.001
    //
    // Group:   Record
    // 
    // Action : Test to destroy the engine when video is prepared.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 10000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );

    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenVideoPrepareReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestDestroy2Q4L()
    {
	// ID: CAE.VR.DESTROY.Q4.002
    //
    // Group:   Record
    // 
    // Action : Test to destroy the engine when video recording is on.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 10000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );

    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenVideoRecordingOn );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestDestroy3Q4L()
    {
	// ID: CAE.VR.DESTROY.Q4.003
    //
    // Group:   Record
    // 
    // Action : Test to destroy the engine when video recording is paused.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 10000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );

    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenVideoRecordingPaused );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestDestroy4Q4L()
    {
	// ID: CAE.VR.DESTROY.Q4.004
    //
    // Group:   Record
    // 
    // Action : Test to destroy the engine when video recording is completed.
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 10000000 );
    
    TSize sizeQcif( KImgWidthQCIF, KImgHeightQCIF );

    iStatesClient->SetVideoFrameSize( sizeQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 60000 ) );
    iStatesClient->SetVideoAudio( EFalse );

    iStatesClient->SetRunningWithViewFinder( ETrue );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::EDestroyEngineWhenVideoRecordingReady );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingPaused() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


/********************************************************/
/*                ERROR SIMULATIONS                     */
/********************************************************/

void CCaeTestVideoStates::TestVideoRecorderOpenErrorQ4L()
    {
	// ID:      CAE.VR.ERRORS.Q4.001
    //
    // Group:   Errors.
    // 
    // Action : Test error handling with video recorder opening.
	//      
	// Output : Leave with code KErrAbort.

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EVideoRecorderOpenError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestVideoRecorderPrepareErrorQ4L()
    {
	// ID:      CAE.VR.ERRORS.Q4.002
    //
    // Group:   Errors.
    // 
    // Action : Test error handling with video recorder preparation.
	//      
	// Output : Leave with code KErrAbort.

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EVideoRecorderPrepareError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }


void CCaeTestVideoStates::TestVideoRecordingCompleteErrorQ4L()
    {
	// ID:      CAE.VR.ERRORS.Q4.003
    //
    // Group:   Errors.
    // 
    // Action : Test error handling with video recording completion.
	//      
	// Output : Leave with code KErrAbort.

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EVideoRecordingCompleteError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }


void CCaeTestVideoStates::TestPrepareVideoSettingsErrorQ4L()
    {
	// ID:      CAE.VR.ERRORS.Q4.004
    //
    // Group:   Errors.
    // 
    // Action : Test error handling with video recorder settings preparation.
	//      
	// Output : Leave with code KErrAbort.

    iStatesClient->SetVideoQualityLevelIndex( 0 );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( error, iStatesClient->RunTestActionL( CCaeTestStatesClient::EPrepareVideoSettingsError ) );
    assertL( error == KErrAbort );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

/********************************************************/
/*      PREPARE AUDIO BIT RATE AND RECORD VIDEO         */
/********************************************************/

void CCaeTestVideoStates::TestPrepareAudioBitRateVR0Q4L()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q4.002
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 0 ) );

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_0Q4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo ) );
    assertL( err == KErrArgument );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR5150Q4L()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q4.003
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 5150 ) );

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_5150Q4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR12200Q4L()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q4.004
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 12200 ) );

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_12200Q4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR20000Q4L()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q4.005
    //
    // Group:   Record
    // 
    // Action : To ensure that the implementation is compatible with camcorder plug-in. 
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 20000 ) );

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_20000Q4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideo ) );
    assertL( err == KErrArgument );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR12200AudioType0Q4L()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q4.006
    //
    // Group:   Record
    // 
    // Action : Test with special audio type 0
	//      
	// Output : OK.   

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 12200 ) );

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_12200AT0Q4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideoAT0 );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 1 );
    assertL( iStatesClient->CountVideoRecordingOn() == 1 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 1 );
    }

void CCaeTestVideoStates::TestPrepareAudioBitRateVR12200AudioTypeInvQ4L()
    {
	// ID: CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q4.007
    //
    // Group:   Record
    // 
    // Action : Test with special audio type
	//      
	// Output : Leave

    iStatesClient->SetTimeout( 4000000 );

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );

    iStatesClient->SetVideoFrameSize( sizeSubQcif );
    iStatesClient->SetVideoFrameRate( TReal32( 10.0 ) );
    iStatesClient->SetVideoBitRate( TInt( 25000 ) );
    
    iStatesClient->SetAudioBitRate( TInt( 12200 ) );

    iStatesClient->SetVideoFileName( _L("cae_subqcif_AudioBitRate_12200ATInvQ4.3gp") );

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::ESpecifiedPrepareAudioBrAndRecordVideoATInv ) );
    assertL( err == KErrArgument );

    assertL( iStatesClient->CountInitComplete() == 1 );
    assertL( iStatesClient->CountVideoPrepareComplete() == 0 );
    assertL( iStatesClient->CountVideoRecordingOn() == 0 );
    assertL( iStatesClient->CountVideoRecordingComplete() == 0 );
    }

/********************************************************/
/*      MISC tests                                      */
/********************************************************/
void CCaeTestVideoStates::TestInitVideoRecorderWithUidQ4L()
    {
    // ID: CAE.GEN.INITVIDEORECORDERL.Q4.002
	//
    // General
    // 
    // Action : Test Call InitVideoRecorderL() when initialization is done with Uid.
	//      
	// Output : Leave, Uid init is not supported.

    iStatesClient->SetVideoCodecSet( 2 ); // 2: AAC audio, MPEG-4 video

    TRAPD( err, iStatesClient->RunTestActionL( CCaeTestStatesClient::EInitVideoRecorderWithUid ));
    assertL( err == KErrNotSupported );
    assertL( iStatesClient->CountInitComplete() == 1 );
    }

/********************************************************/
/*            REMAINING VIDEO RECORDING TIME            */
/********************************************************/

void CCaeTestVideoStates::TestRemainingTimeWhenVideoNotInitializedL()
{
	// Group:   Video.
    // 
    // ID:      CAE.VR.REMAININGVIDEORECORDINGTIME.002
    //
    // Action : Get RemainingVideoRecordingTime, when video recording is not initialized.
	//      
	// Output : No leave.   

    iStatesClient->RunTestActionL( CCaeTestStatesClient::ERemainingTimeWhenVideoNotInitialized );
}
//
// An own static function to collect the test functions into one 
// suite of tests. The framework will run the tests and free the
// memory allocated for the test suite.
// 


MTest* CCaeTestVideoStates::suiteL ()
    {
	// Always use NewL (Do not use NewLC) !!!
    CTestSuite *suite = CTestSuite::NewL( _L8("CCaeTestVideoStates") ); 

    // AMR audio, H.263 video
#if (!MIRAGE_X_PROD3)
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.VIDEORECORDING.QCIF64000FR15.004"), &TestPrepareAndRecordQualityLevel0L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.VIDEORECORDING.SUBQCIF60000FR15.005"), &TestPrepareAndRecordQualityLevel1L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.VIDEORECORDING.SUBQCIF40000FR5.006"), &TestPrepareAndRecordQualityLevel2L ) );

    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.QCIF.003"), &TestVideoRecordingQcif60000Fr10NoAudioNoVfL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.QCIF.004"), &TestVideoRecordingQcif60000Fr10WithAudioNoVfL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.QCIF.005"), &TestVideoRecordingQcif60000Fr10NoAudioWithVfL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.QCIF.006"), &TestVideoRecordingQcif60000Fr10WithAudioWithVfL ) );

    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PAUSEVIDEORECORDING.001"), &TestRecordPauseStopL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PAUSEVIDEORECORDING.002"), &TestPauseWhenPreparedL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PAUSEVIDEORECORDING.003"), &TestRecordPausePauseStopL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.RESUMEVIDEORECORDING.002"), &TestResumeWhenPreparedL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.RESUMEVIDEORECORDING.003"), &TestResumeWhenNotPausedL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.005"), &TestPrepareVideoRecordingNeg1L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.006"), &TestPrepareVideoRecording3L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.008"), &TestSpecPrepareVRUnsupportedFormatWhenVRInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.009"), &TestSpecPrepareVRSupportedFormatWhenPreparedL ) );

    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTVIDEORECORDING.001"), &TestStartVideoRecordingWhenVRInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTVIDEORECORDING.002"), &TestStartVideoRecordingWhenRecordingL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STOPVIDEORECORDING.001"), &TestStopVideoRecordingWhenVRInitializedL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STOPVIDEORECORDING.002"), &TestStopVideoRecordingWhenPreparedL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.CAPTURERECORD.002"), &TestCaptureRecordL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.RECORDCAPTURE.003"), &TestRecordCaptureL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.RECORDRECORD.004"), &TestRecordRecordL ) );
        
    // These "complex sequences" take quite long, probably a minute or more.
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.RECORDCAPTURECOMPLEX1.005"), &TestRecordCaptureComplexSequence1L ) );
    
#ifdef __CAMERA_BURST_MODE
    // Only if Still BURST is supported (built in).
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.RECORDCAPTURECOMPLEX2.006"), &TestRecordCaptureComplexSequence2L ) );
#endif
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.GEN.RELESEANDRESERVE.006"), &TestReleaseReserveRecordL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.002"), &TestPrepareAudioBitRateVR0L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.003"), &TestPrepareAudioBitRateVR5150L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.004"), &TestPrepareAudioBitRateVR12200L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.005"), &TestPrepareAudioBitRateVR20000L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.006"), &TestPrepareAudioBitRateVR12200AudioType0L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.007"), &TestPrepareAudioBitRateVR12200AudioTypeInvL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.DESTROY.001"), &TestDestroy1L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.DESTROY.002"), &TestDestroy2L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.DESTROY.003"), &TestDestroy3L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.DESTROY.004"), &TestDestroy4L ) );
 
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.GEN.INITVIDEORECORDERL.002"), &TestInitVideoRecorderWithUidL ) );

    #ifdef CAE_TEST_VERSION
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.ERRORS.001"), &TestVideoRecorderOpenErrorL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.ERRORS.002"), &TestVideoRecorderPrepareErrorL ) );
    // suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.ERRORS.003"), &TestVideoRecordingCompleteErrorL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.ERRORS.004"), &TestPrepareVideoSettingsErrorL ) );
    #endif
#endif //!MIRAGE_X_PROD3

    // AMR audio, H.263 video
#if (MIRAGE_X_PROD3)
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.VIDEORECORDING.QCIF64000FR15.Q.004"), &TestPrepareAndRecordQualityLevel0QL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.VIDEORECORDING.SUBQCIF60000FR15.Q.005"), &TestPrepareAndRecordQualityLevel1QL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.VIDEORECORDING.SUBQCIF40000FR5.Q.006"), &TestPrepareAndRecordQualityLevel2QL ) );

    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q.003"), &TestVideoRecordingQcif60000Fr10NoAudioNoVfQL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q.004"), &TestVideoRecordingQcif60000Fr10WithAudioNoVfQL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q.005"), &TestVideoRecordingQcif60000Fr10NoAudioWithVfQL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q.006"), &TestVideoRecordingQcif60000Fr10WithAudioWithVfQL ) );

    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PAUSEVIDEORECORDING.Q.001"), &TestRecordPauseStopQL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PAUSEVIDEORECORDING.Q.002"), &TestPauseWhenPreparedQL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PAUSEVIDEORECORDING.Q.003"), &TestRecordPausePauseStopQL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.RESUMEVIDEORECORDING.Q.002"), &TestResumeWhenPreparedQL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.RESUMEVIDEORECORDING.Q.003"), &TestResumeWhenNotPausedQL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.Q.005"), &TestPrepareVideoRecordingNeg1QL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.Q.006"), &TestPrepareVideoRecording3QL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.Q.008"), &TestSpecPrepareVRUnsupportedFormatWhenVRInitializedQL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.Q.009"), &TestSpecPrepareVRSupportedFormatWhenPreparedQL ) );

    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTVIDEORECORDING.Q.001"), &TestStartVideoRecordingWhenVRInitializedQL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTVIDEORECORDING.Q.002"), &TestStartVideoRecordingWhenRecordingQL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STOPVIDEORECORDING.Q.001"), &TestStopVideoRecordingWhenVRInitializedQL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STOPVIDEORECORDING.Q.002"), &TestStopVideoRecordingWhenPreparedQL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.CAPTURERECORD.Q.002"), &TestCaptureRecordQL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.RECORDCAPTURE.Q.003"), &TestRecordCaptureQL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.RECORDRECORD.Q.004"), &TestRecordRecordQL ) );
        
    // These "complex sequences" take quite long, probably a minute or more.
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.RECORDCAPTURECOMPLEX1.Q.005"), &TestRecordCaptureComplexSequence1QL ) );
    
#ifdef __CAMERA_BURST_MODE
    // Only if Still BURST is supported (built in).
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.RECORDCAPTURECOMPLEX2.Q.006"), &TestRecordCaptureComplexSequence2QL ) );
#endif
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.GEN.RELESEANDRESERVE.Q.006"), &TestReleaseReserveRecordQL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q.002"), &TestPrepareAudioBitRateVR0QL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q.003"), &TestPrepareAudioBitRateVR5150QL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q.004"), &TestPrepareAudioBitRateVR12200QL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q.005"), &TestPrepareAudioBitRateVR20000QL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q.006"), &TestPrepareAudioBitRateVR12200AudioType0QL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q.007"), &TestPrepareAudioBitRateVR12200AudioTypeInvQL ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.DESTROY.Q.001"), &TestDestroy1QL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.DESTROY.Q.002"), &TestDestroy2QL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.DESTROY.Q.003"), &TestDestroy3QL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.DESTROY.Q.004"), &TestDestroy4QL ) );
 
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.GEN.INITVIDEORECORDERL.Q.002"), &TestInitVideoRecorderWithUidQL ) );

    #ifdef CAE_TEST_VERSION
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.ERRORS.Q.001"), &TestVideoRecorderOpenErrorQL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.ERRORS.Q.002"), &TestVideoRecorderPrepareErrorQL ) );
    // suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.ERRORS.Q.003"), &TestVideoRecordingCompleteErrorQL ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.ERRORS.Q.004"), &TestPrepareVideoSettingsErrorQL ) );
    #endif
#endif //MIRAGE_X_PROD3

    // AAC audio, MPEG-4 video
#if (MIRAGE_X_PROD3)
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.VIDEORECORDING.QCIF64000FR15.Q4.004"), &TestPrepareAndRecordQualityLevel0Q4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.VIDEORECORDING.SUBQCIF60000FR15.Q4.005"), &TestPrepareAndRecordQualityLevel1Q4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.VIDEORECORDING.SUBQCIF40000FR5.Q4.006"), &TestPrepareAndRecordQualityLevel2Q4L ) );

    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q4.003"), &TestVideoRecordingQcif60000Fr10NoAudioNoVfQ4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q4.004"), &TestVideoRecordingQcif60000Fr10WithAudioNoVfQ4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q4.005"), &TestVideoRecordingQcif60000Fr10NoAudioWithVfQ4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTSTOPVIDEORECORDING.QCIF.Q4.006"), &TestVideoRecordingQcif60000Fr10WithAudioWithVfQ4L ) );

    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PAUSEVIDEORECORDING.Q4.001"), &TestRecordPauseStopQ4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PAUSEVIDEORECORDING.Q4.002"), &TestPauseWhenPreparedQ4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PAUSEVIDEORECORDING.Q4.003"), &TestRecordPausePauseStopQ4L ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.RESUMEVIDEORECORDING.Q4.002"), &TestResumeWhenPreparedQ4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.RESUMEVIDEORECORDING.Q4.003"), &TestResumeWhenNotPausedQ4L ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.Q4.005"), &TestPrepareVideoRecordingNeg1Q4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.Q4.006"), &TestPrepareVideoRecording3Q4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.Q4.008"), &TestSpecPrepareVRUnsupportedFormatWhenVRInitializedQ4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREVIDEORECORDINGL.Q4.009"), &TestSpecPrepareVRSupportedFormatWhenPreparedQ4L ) );

    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTVIDEORECORDING.Q4.001"), &TestStartVideoRecordingWhenVRInitializedQ4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STARTVIDEORECORDING.Q4.002"), &TestStartVideoRecordingWhenRecordingQ4L ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STOPVIDEORECORDING.Q4.001"), &TestStopVideoRecordingWhenVRInitializedQ4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.STOPVIDEORECORDING.Q4.002"), &TestStopVideoRecordingWhenPreparedQ4L ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.CAPTURERECORD.Q4.002"), &TestCaptureRecordQ4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.RECORDCAPTURE.Q4.003"), &TestRecordCaptureQ4L ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.RECORDRECORD.Q4.004"), &TestRecordRecordQ4L ) );
        
    // These "complex sequences" take quite long, probably a minute or more.
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.RECORDCAPTURECOMPLEX1.Q4.005"), &TestRecordCaptureComplexSequence1Q4L ) );
    
#ifdef __CAMERA_BURST_MODE
    // Only if Still BURST is supported (built in).
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.STATECHANGE.RECORDCAPTURECOMPLEX2.Q4.006"), &TestRecordCaptureComplexSequence2QL ) );
#endif
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.GEN.RELESEANDRESERVE.Q4.006"), &TestReleaseReserveRecordQ4L ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q4.002"), &TestPrepareAudioBitRateVR0Q4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q4.003"), &TestPrepareAudioBitRateVR5150Q4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q4.004"), &TestPrepareAudioBitRateVR12200Q4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q4.005"), &TestPrepareAudioBitRateVR20000Q4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q4.006"), &TestPrepareAudioBitRateVR12200AudioType0Q4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.PREPAREAUDIOBITRATEANDRECORDVIDEO.Q4.007"), &TestPrepareAudioBitRateVR12200AudioTypeInvQ4L ) );
    
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.DESTROY.Q4.001"), &TestDestroy1Q4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.DESTROY.Q4.002"), &TestDestroy2Q4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.DESTROY.Q4.003"), &TestDestroy3Q4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.DESTROY.Q4.004"), &TestDestroy4Q4L ) );
 
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.GEN.INITVIDEORECORDERL.Q4.002"), &TestInitVideoRecorderWithUidQ4L ) );

    #ifdef CAE_TEST_VERSION
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.ERRORS.Q4.001"), &TestVideoRecorderOpenErrorQ4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.ERRORS.Q4.002"), &TestVideoRecorderPrepareErrorQ4L ) );
    // suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.ERRORS.Q4.003"), &TestVideoRecordingCompleteErrorQ4L ) );
    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.ERRORS.Q4.004"), &TestPrepareVideoSettingsErrorQ4L ) );
    #endif
#endif //MIRAGE_X_PROD3

    suite->addTestL( CTestCaller<CCaeTestVideoStates>::NewL( _L8("CAE.VR.REMAININGVIDEORECORDINGTIME.002"), &TestRemainingTimeWhenVideoNotInitializedL ) );

    return suite;
    }


