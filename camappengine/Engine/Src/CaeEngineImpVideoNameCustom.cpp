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

// -----------------------------------------------------------------------------
// CCaeEngineImp::ChangeVideoFileNameL
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ChangeVideoFileNameL()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::ChangeVideoFileNameL() entering" ) );

    if ( iVideoControllerUid == KCamCControllerImplementationUid ) 
        {
        // Use custom command interface of Video Recorder to change the file name.
        // We know that the custom command is available with this specific implementation.
        TMMFFileConfig pckg;
        pckg().iPath = iVideoClipFileName->Des();
        TInt err = iVideoRecorder->CustomCommandSync( TMMFMessageDestination( iVideoControllerUid, 
                                                                   KMMFObjectHandleController ), 
                                           ECamCControllerCCNewFilename, 
                                           pckg, 
                                           KNullDesC8 );
        if (err)
            {
            LOGTEXT2( _L( "Cae: CCaeEngineImp::ChangeVideoFileNameL() CustomCommandSync failed err=%d, leaving" ), err );
            User::Leave(err);
            }
        iPrepPars = EFalse;
        iVideoRecorder->Prepare();
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
        iVideoRecorder->OpenFileL( iVideoClipFileName->Des(),
                                   iCameraHandle,
                                   iVideoControllerUid,
                                   iVideoFormatUid, 
                                   iVideoType->Des(),  
                                   iVideoAudioType );
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::ChangeVideoFileNameL() returning" ) );
    }

//  End of File
