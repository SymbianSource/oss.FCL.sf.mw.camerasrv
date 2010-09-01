/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Camera Application Engine implementation for changing 
*                video file name using MMF Video Recorder custom command interface
*
*/



// INCLUDE FILES
#include "CaeEngineImp.h" // For Camera Application Engine implementation.
#include "CamCControllerCustomCommands.h" // For Camcorder plug-in custom commands and UId.
#include <mmf/server/mmffile.h> // For MMF definitions (as TMMFFileConfig)
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "CaeEngineImpVideoNameCustomTraces.h"
#endif

// -----------------------------------------------------------------------------
// CCaeEngineImp::ChangeVideoFileNameL
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ChangeVideoFileNameL()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCAEENGINEIMP_CHANGEVIDEOFILENAMEL, "e_CAM_CAE_CHANGE_VIDEO_FILE_NAME 1" );
    LOGTEXT( _L( "Cae: CCaeEngineImp::ChangeVideoFileNameL() entering" ) );

    if ( iVideoControllerUid == KCamCControllerImplementationUid ) 
        {
        // Use custom command interface of Video Recorder to change the file name.
        // We know that the custom command is available with this specific implementation.
        TMMFFileConfig pckg;
        pckg().iPath = iVideoClipFileName->Des();
        OstTrace0( CAMERASRV_PERFORMANCE, DUP2_CCAEENGINEIMP_CHANGEVIDEOFILENAMEL, "e_CAM_CAE_CUSTOM_COMMAND 1" );
        TInt err = iVideoRecorder->CustomCommandSync( TMMFMessageDestination( iVideoControllerUid, 
                                                                   KMMFObjectHandleController ), 
                                           ECamCControllerCCNewFilename, 
                                           pckg, 
                                           KNullDesC8 );
        OstTrace0( CAMERASRV_PERFORMANCE, DUP3_CCAEENGINEIMP_CHANGEVIDEOFILENAMEL, "e_CAM_CAE_CUSTOM_COMMAND 0" );
        if (err)
            {
            LOGTEXT2( _L( "Cae: CCaeEngineImp::ChangeVideoFileNameL() CustomCommandSync failed err=%d, leaving" ), err );
            User::Leave(err);
            }
        iPrepPars = EFalse;
        OstTrace0( CAMERASRV_PERFORMANCE, DUP4_CCAEENGINEIMP_CHANGEVIDEOFILENAMEL, "e_CAM_CAE_VIDEORECORDER_PREPARE 1" );
        iVideoRecorder->Prepare();
        OstTrace0( CAMERASRV_PERFORMANCE, DUP5_CCAEENGINEIMP_CHANGEVIDEOFILENAMEL, "e_CAM_CAE_VIDEORECORDER_PREPARE 0" );
        }
    else 
        {
        // With 3rd party video controllers we need to re-open video recording because
        // we can not rely on using custom command for changing the file name.
        CloseVideoRecording();
        // Open video recorder.
        iVideoOpened = ETrue; // This is always set to ETrue when 
                              // OpenFileL has been called to allow 
                              // freeing resources by CloseVideoRecording().
        OstTrace0( CAMERASRV_PERFORMANCE, DUP6_CCAEENGINEIMP_CHANGEVIDEOFILENAMEL, "e_CAM_CAE_VIDEORECORDER_OPENFILE 1" );
        iVideoRecorder->OpenFileL( iVideoClipFileName->Des(),
                                   iCameraHandle,
                                   iVideoControllerUid,
                                   iVideoFormatUid, 
                                   iVideoType->Des(),  
                                   iVideoAudioType );
        OstTrace0( CAMERASRV_PERFORMANCE, DUP7_CCAEENGINEIMP_CHANGEVIDEOFILENAMEL, "e_CAM_CAE_VIDEORECORDER_OPENFILE 0" );
        }
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAEENGINEIMP_CHANGEVIDEOFILENAMEL, "e_CAM_CAE_CHANGE_VIDEO_FILE_NAME 0" );
    LOGTEXT( _L( "Cae: CCaeEngineImp::ChangeVideoFileNameL() returning" ) );
    }

//  End of File
