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
* Description:  Camera Application Engine video times provider class 
*
*/



// INCLUDE FILES

#include "CaeVideoTimes.h"



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaeVideoTimes::CCaeVideoTimes
// Constructor.
// -----------------------------------------------------------------------------
//
CCaeVideoTimes::CCaeVideoTimes() : 
    CPeriodic( CActive::EPriorityStandard - 1 )
    {
    }


// -----------------------------------------------------------------------------
// CCaeVideoTimes::~CCaeVideoTimes
// Destructor.
// -----------------------------------------------------------------------------
//
CCaeVideoTimes::~CCaeVideoTimes()
    {
    Cancel();
    }


// -----------------------------------------------------------------------------
// CCaeVideoTimes::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCaeVideoTimes::ConstructL()
    {
    CPeriodic::ConstructL();
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CCaeVideoTimes::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaeVideoTimes* CCaeVideoTimes::NewL()
    {
    CCaeVideoTimes* self = new( ELeave ) CCaeVideoTimes;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CCaeVideoTimes::RunError
// Called if CCaeVideoTimes function RunL() leaves.
// -----------------------------------------------------------------------------
//
TInt CCaeVideoTimes::RunError( 
    TInt /*aError*/ )
    {
    return KErrNone;
    }

//  End of File  
