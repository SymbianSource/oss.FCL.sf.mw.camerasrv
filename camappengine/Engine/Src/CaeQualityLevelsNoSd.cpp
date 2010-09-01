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
* Description:  Camera Application Engine imaging quality levels container 
*                methods without Shared Data ini-file support.
*
*/



// INCLUDE FILES

#include "CaeQualityLevels.h"

#include "CaeEngineImp.h"       // For LOGTEXT


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaeQualityLevelsCont::InitFromSdL
// -----------------------------------------------------------------------------
//
TInt CCaeQualityLevelsCont::InitFromSdL( 
    const TUid&  /*aSdUidQltyLevels*/ )
    {
    LOGTEXT( _L( "Cae: CCaeQualityLevelsCont::InitFromSdL(): NOT SUPPORTED, leaving" ) );

    User::Leave( KErrNotSupported );

    return 0;
    }


// ---------------------------------------------------------------------------
// CCaeQualityLevelsCont::FetchQualityLevelL
// ---------------------------------------------------------------------------
//
void CCaeQualityLevelsCont::FetchQualityLevelL( 
    RSharedDataClient& /*aSharedData*/, 
    const TUid&        /*aSdUidQltyLevels*/, 
    TInt               /*aIndex*/ )
    {
    LOGTEXT( _L( "Cae: CCaeQualityLevelsCont::FetchQualityLevelL(): NOT SUPPORTED, leaving" ) );

    User::Leave( KErrNotSupported );
    }


//  End of File  
