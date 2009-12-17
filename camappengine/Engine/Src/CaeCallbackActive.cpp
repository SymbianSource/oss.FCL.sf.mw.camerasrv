/*
* Copyright (c) 2003,2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active object for calling Camera Application Engine methods 
*                indirectly from callbacks
*
*/


// INCLUDE FILES
#include "CaeCallbackActive.h"
#include "CaeEngineImp.h"


// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------------------------
// CCaeCallbackActive::CCaeCallbackActive()
// C++ constructor.
// Adds the object to the Active Scheduler.
// ---------------------------------------------------------------------------
//
CCaeCallbackActive::CCaeCallbackActive( 
    CCaeEngineImp& aCamAppEngine )
: CActive( EPriorityStandard ), iCamAppEngine( aCamAppEngine )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// CCaeCallbackActive::CCaeCallbackActive()
// Destructor. 
// Cancels operation and closes the timer.
// ---------------------------------------------------------------------------
//
CCaeCallbackActive::~CCaeCallbackActive()
    {
    Cancel();
    iTimer.Close();
    }


// ---------------------------------------------------------------------------
// CCaeCallbackActive::NewLC()
// Symbian OS two-phased constructor.
// ---------------------------------------------------------------------------
//
CCaeCallbackActive* CCaeCallbackActive::NewLC( 
    CCaeEngineImp& aCamAppEngine )
    {
    CCaeCallbackActive* self = new( ELeave ) CCaeCallbackActive( aCamAppEngine );

    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CCaeCallbackActive::NewL()
// Symbian OS two-phased constructor.
// ---------------------------------------------------------------------------
//
CCaeCallbackActive* CCaeCallbackActive::NewL( 
    CCaeEngineImp& aCamAppEngine )
    {
    CCaeCallbackActive* self = NewLC( aCamAppEngine );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CCaeCallbackActive::ConstructL()
// Symbian OS 2nd phase constructor that can leave.
// ---------------------------------------------------------------------------
//
void CCaeCallbackActive::ConstructL()
    {
    User::LeaveIfError( iTimer.CreateLocal() );
    }


// ---------------------------------------------------------------------------
// CCaeCallbackActive::PowerOn()
// Switches camera power on.
// ---------------------------------------------------------------------------
//
void CCaeCallbackActive::PowerOn()
    {
    LOGTEXT( _L( "Cae: CCaeCallbackActive::PowerOn() entering" ) );

    _LIT( KPowerOnPanic, "CCaeCallbackActive::PowerOn");
    __ASSERT_ALWAYS( !IsActive(), User::Panic( KPowerOnPanic, 1 ) );

    iRequest = CCaeCallbackActive::ERequestPowerOn;

    SetActive();
    TRequestStatus* statusPtr = &iStatus;
    User::RequestComplete( statusPtr, KErrNone );

    LOGTEXT( _L( "Cae: CCaeCallbackActive::PowerOn() returning" ) );
    }


// ---------------------------------------------------------------------------
// CCaeCallbackActive::RunL()
// Calls Camera Application Engine operation.
// ---------------------------------------------------------------------------
//
void CCaeCallbackActive::RunL()
    {
    LOGTEXT( _L( "Cae: CCaeCallbackActive::RunL() entering" ) );

    switch ( iRequest ) 
        {
        case CCaeCallbackActive::ERequestPowerOn:
            // Call Camera Application Engine to switch camera power on.
            iCamAppEngine.PowerOn();
            break;
        default:
            break;
        }

    LOGTEXT( _L( "Cae: CCaeCallbackActive::RunL() returning" ) );
    }


// ---------------------------------------------------------------------------
// CCaeCallbackActive::DoCancel()
// Cancels request.
// ---------------------------------------------------------------------------
//
void CCaeCallbackActive::DoCancel()
    {
    iTimer.Cancel();
    iRequest = CCaeCallbackActive::ERequestDefault;
    }


// End of File  
