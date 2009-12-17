/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Utilities for Camera Application Engine.
*
*/


// INCLUDE FILES
#include <e32std.h>
#include <e32svr.h>
#include "CaeDbgUtilities.h"

// ============================= LOCAL FUNCTIONS ===============================

#ifdef _DEBUG

// ---------------------------------------------------------------------------
// PrintMemoryInfo()
// Print memory info. 
// Output format: free heap (biggest block), free RAM
// ---------------------------------------------------------------------------
//
void PrintMemoryInfo()
    {
    // Print memory info
    TInt biggestBlock = 0;
    TInt available = User::Available( biggestBlock );

    TMemoryInfoV1Buf membuf;
    UserHal::MemoryInfo( membuf );
    TMemoryInfoV1 minfo = membuf();

    TBuf<256> text;
    text.Format( _L( "free heap:%d(%d) Kb, free ram: %d Kb" ), 
        available / 1024, biggestBlock / 1024, minfo.iFreeRamInBytes / 1024 );

    LOGTEXT(text);
    }

#endif

// End of File
