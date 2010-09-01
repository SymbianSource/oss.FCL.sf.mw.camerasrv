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
* Description: CaeTestTop.cpp
*
*/
#include "CaeTestTop.h"
#include "CaeTestRelease.h"
#include "CaeTestStillSettings.h"
#include "CaeTestStillStates.h"
#include "CaeTestVideoStates.h"
#include "CaeTestVideoSettings.h"

#include <e32des8.h>


// Starting point of the DLL (Always needed) 
#ifndef EKA2
GLDEF_C TInt E32Dll( TDllReason )
    {
    return( KErrNone );
    }
#endif

// Exactly one exported function returning the suite of 
// test functions for the test runner in the framework.
// (Always needed)
//


EXPORT_C MTest* CreateTopFct()
    {
	return ( CCaeTestTop::suiteL() );
    }


CCaeTestTop::~CCaeTestTop()
    {
    }


MTest* CCaeTestTop::suiteL()
    {
	CTestSuite *suite = CTestSuite::NewL( _L8( "Camera Application Engine Test Suite Container" ) );
    
    suite->addTestL( CCaeTestRelease::suiteL() );
    
    //suite->addTestL( CCaeTestStillStates::suiteL() );
    //suite->addTestL( CCaeTestVideoStates::suiteL() );
    
  	//suite->addTestL( CCaeTestStillSettings::suiteL() );
    //suite->addTestL( CCaeTestVideoSettings::suiteL() );
    
    return suite;	
    }


