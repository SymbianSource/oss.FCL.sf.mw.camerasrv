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
* Description:  Camcorder controller plugin custom commands
*
*/



#ifndef CAMCCONTROLLERCUSTOMCOMMANDS_H
#define CAMCCONTROLLERCUSTOMCOMMANDS_H

// Controller UId, can be used by the client to identify the controller, e.g. if the custom command can be used
const TUid KCamCControllerImplementationUid = {0x101F8503};

// TMMFEvent UIDs for Async stop
const TUid KCamCControllerCCVideoRecordStopped = {0x2000E546};
const TUid KCamCControllerCCVideoFileComposed = {0x2000E547};


// Custom command for setting a new filename without closing & reopening the controller
enum TCamCControllerCustomCommands
    {   
    ECamCControllerCCNewFilename = 0,
    ECamCControllerCCVideoStopAsync
    };

#endif      // CAMCCONTROLLERCUSTOMCOMMANDS_H  
// End of File
