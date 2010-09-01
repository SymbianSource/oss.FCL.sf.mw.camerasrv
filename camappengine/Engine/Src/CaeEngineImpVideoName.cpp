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
*                video file name.
*
*/



// INCLUDE FILES
#include "CaeEngineImp.h"   // For Camera Application Engine implementation.
#include <mmf/server/mmffile.h>        // For MMF definitions (as TMMFFileConfig)


// -----------------------------------------------------------------------------
// CCaeEngineImp::ChangeVideoFileNameL
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ChangeVideoFileNameL()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::ChangeVideoFileNameL() entering" ) );

    // Close if previously prepared.
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

    LOGTEXT( _L( "Cae: CCaeEngineImp::ChangeVideoFileNameL() returning" ) );
    }

//  End of File
