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
* Description:  Camera Application Engine video quality levels container
*                with Shared Data API support.
*
*/



// INCLUDE FILES

#include "CaeVideoQualityLevels.h"


// ============================ MEMBER FUNCTIONS ===============================


// ---------------------------------------------------------------------------
// CCaeVideoQualityLevelsCont::CreateQualityLevelString()
// Create quality level string for SharedData Camera ini file.
// ---------------------------------------------------------------------------
//
void CCaeVideoQualityLevelsCont::CreateQualityLevelString( 
    TInt  /*aIndex*/,
    TDes& /*aDefaultQualityLevel*/ )
    {
    }


// ---------------------------------------------------------------------------
// CCaeVideoQualityLevelsCont::ParseQualityLevelFromStringL
// Parse quality level string to quality level object data.
// ---------------------------------------------------------------------------
//
void CCaeVideoQualityLevelsCont::ParseQualityLevelFromStringL( 
    const TDesC& /*aQualityLevelString*/, 
    TInt         /*aIndex*/ )
    {
    }


//  End of File  
