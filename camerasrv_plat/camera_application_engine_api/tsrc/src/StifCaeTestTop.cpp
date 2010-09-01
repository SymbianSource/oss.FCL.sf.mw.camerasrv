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
* Description:  Cae Test DLL
*
*/


// INCLUDE FILES
#include <e32math.h>
#include "StifCaeTestTop.h"
#include "TestFrameWork/testSuite.h"
#include "CaeTestTop.h"
#include "CaeTestRelease.h"
#include "CaeTestStillSettings.h"
#include "CaeTestStillStates.h"
#include "CaeTestVideoStates.h"
#include "CaeTestVideoSettings.h"

// EXTERNAL DATA STRUCTURES
// None

// EXTERNAL FUNCTION PROTOTYPES  
// None

// CONSTANTS
// None

// MACROS
// None

// LOCAL CONSTANTS AND MACROS
// None

// MODULE DATA STRUCTURES
// None

// LOCAL FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
// None

// ==================== LOCAL FUNCTIONS =======================================

           
/*
-------------------------------------------------------------------------------

    DESCRIPTION
    
    This file (DemoModule.cpp) contains all STIF test framework related parts of
    this test module.

-------------------------------------------------------------------------------
*/
EXPORT_C TInt SetRequirements( CTestModuleParam*& aTestModuleParam, TUint32& aParameterValid )
    {
    aParameterValid = KStifTestModuleParameterChanged;

    CTestModuleParamVer01* param = CTestModuleParamVer01::NewL();

    // Stack size
    param->iTestThreadStackSize= 2*16384; // 2*16K stack

    // Heap sizes
    param->iTestThreadMinHeap = 4096;   // 4K heap min
    param->iTestThreadMaxHeap = 8*1048576;// 8M heap max

    aTestModuleParam = param;

    return KErrNone;
    }

// ================= MEMBER FUNCTIONS =========================================


CTestModule::CTestModule()
    {
    }


void CTestModule::ConstructL()
    {  
    iTestSuite = CTestSuite::NewL(_L8("Test Suite Container"));

    iTestSuite->addTestL( CCaeTestRelease::suiteL() );    
    
    //iTestSuite->addTestL( CCaeTestStillStates::suiteL() );
    //iTestSuite->addTestL( CCaeTestVideoStates::suiteL() );
    //iTestSuite->addTestL( CCaeTestStillSettings::suiteL() );
    //iTestSuite->addTestL( CCaeTestVideoSettings::suiteL() );

    
    //SetAllocFailureSimulation( RHeap::EDeterministic, 0 ); // Enable OOM test loop
    SetAllocFailureSimulation( RHeap::ENone, 0 ); // Disable OOM test loop

    /* Install an active scheduler */
    iScheduler = new(ELeave)CActiveScheduler;
    CActiveScheduler::Install(iScheduler);

    }


CTestModule* CTestModule::NewL()
    {

    // Construct new CTestModule instance.
    CTestModule* self = new ( ELeave ) CTestModule();    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
  
    }


CTestModule::~CTestModule()
    {
    delete iTestSuite;
    delete iScheduler;
    }


/*
-------------------------------------------------------------------------------

    Class: CTestModule

    Method: GetTestCases

    Description: GetTestCases is used to inquire test cases 
    from the test module. Because this test module have hard coded test cases
    (i.e cases are not read from file), paramter aConfigFile is not used.

    This function loops through all cases defined in Cases() function and 
    adds corresponding items to aTestCases array.

    Parameters: const TFileName&  : in: Configuration file name. Not used                                                       
                RPointerArray<TTestCaseInfo>& aTestCases: out: 
                      Array of TestCases.
    
    Return Values: KErrNone: No error

    Errors/Exceptions: Function leaves if any memory allocation operation fails

    Status: Proposal
    
-------------------------------------------------------------------------------
*/      
TInt CTestModule::GetTestCasesL( const TFileName& /*aConfigFile*/, 
                                     RPointerArray<TTestCaseInfo>& aTestCases )
    {

    // Loop through all test cases and create new
    // TTestCaseInfo items and append items to aTestCase array    
    for( TInt i = 0; i< iTestSuite->CountTestCases(); i++ )
        {
 
	    // Allocate new TTestCaseInfo from heap for a testcase definition.
        TTestCaseInfo* newCase = new( ELeave ) TTestCaseInfo();
    
	    // PushL TTestCaseInfo to CleanupStack.    
        CleanupStack::PushL( newCase );

	    // Set number for the testcase.
	    // When the testcase is run, this comes as a parameter to RunTestCaseL.
        newCase->iCaseNumber = i;

	    // Set title for the test case. This is shown in UI to user.
        newCase->iTitle.Copy( iTestSuite->TestCaseName(i) );

	    // Append TTestCaseInfo to the testcase array. After appended 
	    // successfully the TTestCaseInfo object is owned (and freed) 
	    // by the TestServer. 
        User::LeaveIfError(aTestCases.Append ( newCase ) );
        
	    // Pop TTestCaseInfo from the CleanupStack.
        CleanupStack::Pop( newCase );

        }
    
    return KErrNone;
  
    }

/*
-------------------------------------------------------------------------------

    Class: CTestModule

    Method: RunTestCase

    Description: Run a specified testcase.

    Function runs a test case specified by test case number. Test case file
    parameter is not used.

    If case number is valid, this function runs a test case returned by
    function Cases(). 
  
    Parameters: const TInt aCaseNumber: in: Testcase number 
                const TFileName& : in: Configuration file name. Not used
                TTestResult& aResult: out: Testcase result
    
    Return Values: KErrNone: Testcase ran.
                   KErrNotFound: Unknown testcase

    Errors/Exceptions: None
    
    Status: Proposal
    
-------------------------------------------------------------------------------
*/
TInt CTestModule::RunTestCaseL( const TInt aCaseNumber, 
                                    const TFileName& /*aConfig*/,
                                    TTestResult& aResult )
    {
    // Next is a temporary solution for Stif memory leak error
     TestModuleIf().SetBehavior(CTestModuleIf::ETestLeaksMem);
     TestModuleIf().SetBehavior(CTestModuleIf::ETestLeaksRequests);
     TestModuleIf().SetBehavior(CTestModuleIf::ETestLeaksHandles);

    if ( iAllocFailureType != RHeap::ENone )
        {
        RunTestCaseInOOMLoopL( aCaseNumber, aResult );
        }
    else
        {
        iTestSuite->ExecuteTestL( aResult, aCaseNumber );
        }

    // Return case execution status (not the result of the case execution)
    return KErrNone; 
    }


/*
*/    

void CTestModule::RunTestCaseInOOMLoopL( const TInt aCaseNumber,
                                        TTestResult& aResult )
    {

    // 
    TInt error = KErrNone; 
    aResult.SetResult( KErrNoMemory, _L("") ); // Initial value

    for (; ( aResult.iResult == KErrNoMemory ) && !error;)
        {      
        // Switches on alloc failure simulation if the parameters are set for it
        AllocFailureSimulation( ETrue ); 
        RDebug::Print(_L("*** OOM loop : %d ***\n"), iCurrentAllocFailureRate );
        TRAP( error, iTestSuite->ExecuteTestL( aResult, aCaseNumber ) );
        if ( error == KErrNoMemory )
            {
            error = KErrNone;
            }
        }
    
    //
    AllocFailureSimulation( EFalse );
    User::LeaveIfError( error ); 
    }


/*
*/    

void CTestModule::AllocFailureSimulation (TBool aSwitchedOn)
	{
	if (aSwitchedOn)
		{
		__UHEAP_SETFAIL ( iAllocFailureType, iCurrentAllocFailureRate );
        iCurrentAllocFailureRate++;
		}
	else
		{
		__UHEAP_RESET;
        iCurrentAllocFailureRate = iAllocFailureRate;
		}
	}


/*
*/    

void CTestModule::SetAllocFailureSimulation( RHeap::TAllocFail aAllocFailureType, TInt aAllocFailureRate )
    {
    iAllocFailureType = aAllocFailureType;
    iAllocFailureRate = aAllocFailureRate;
    iCurrentAllocFailureRate = aAllocFailureRate;
    }

// ================= OTHER EXPORTED FUNCTIONS =================================

/*
-------------------------------------------------------------------------------
   
    Function: LibEntryL

    Description: Polymorphic Dll Entry Point
    
    Test framework calls this function to obtain new instance of test module
    class. 

    Parameters:    None
    
    Return Values: CTestModule*    Pointer to CTestModule instance
    
    Errors/Exceptions: Leaves if CTestModule::NewL leaves
    
    Status: Approved

-------------------------------------------------------------------------------
*/
EXPORT_C CTestModule* LibEntryL()
    {
    return CTestModule::NewL();

    }

/*
-------------------------------------------------------------------------------
   
    Function: E32Dll

    Description: DLL entry point function
    
    Parameters:    TDllReason: Not used
    
    Return Values: KErrNone: always
    
    Errors/Exceptions: None

    Status: Approved

-------------------------------------------------------------------------------
*/
#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
    {
    return( KErrNone );

    }
#endif

// End of File
