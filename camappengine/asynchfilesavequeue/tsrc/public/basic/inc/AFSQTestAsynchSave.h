/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  AsynchFSQTest test module.
*
*/


#ifndef AFSQ_TEST_ASYNCH_SAVE_H
#define AFSQ_TEST_ASYNCH_SAVE_H


// INCLUDES

#include <e32base.h>
#include "ASynchFSQ.h"
#include "TestFramework/test.h"
#include "TestFramework/TestCase.h"
#include "TestFramework/TestCaller.h"
#include "TestFramework/TestSuite.h"

// CONSTANTS

enum TTestCaseNumber
    {
    ECONSTRUCT001p1,
    EDESTRUCT001p1,
    EPRIORITY001p1,
    EADD001p1,
    EADD002p1,
    EADD003p1,
    EADD004p1,
    EADD005p1,
    EADD006p1,
    EADD007p1,
    EDEL001p1,
    EDEL002p1,
    EDEL003p1,
    EDEL004p1,
    EDEL005p1,
    EDEL006p1,
    EREN001p1,
    EREN002p1,
    EREN003p1,
    EREN004p1,
    EREN005p1,
    EREN006p1,
    ECANCEL001p1
    };


// MACROS

#define KNibbles TFileName(_L("E:\\testing\\data\\StifAFSQTest\\Nibbles.jpg"));
#define KNibbles2 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\Nibbles2.jpg"));
#define KNibbles3 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\Nibbles3.jpg"));

#define KNibblesCANCEL001 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesADD001.jpg"));

#define KNibblesADD001 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesADD001.jpg"));
#define KNibblesADD002 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesADD002.jpg"));
#define KNibblesADD003 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesADD003.jpg"));
#define KNibblesADD004A TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesADD004A.jpg"));
#define KNibblesADD004B TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesADD004B.jpg"));
#define KNibblesADD005A TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesADD005A.jpg"));
#define KNibblesADD005B TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesADD005B.jpg"));
#define KNibblesADD006 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesADD006.jpg"));
#define KNibblesADD007 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesADD007.jpg"));

#define KNibblesDEL001 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL001.jpg"));
#define KNibblesDEL002 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL002.jpg"));
#define KNibblesDEL003A TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL003A.jpg"));
#define KNibblesDEL003B TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL003B.jpg"));
#define KNibblesDEL003C TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL003C.jpg"));
#define KNibblesDEL004A TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL004A.jpg"));
#define KNibblesDEL004B TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL004B.jpg"));
#define KNibblesDEL004C TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL004C.jpg"));
#define KNibblesDEL005A TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL005A.jpg"));
#define KNibblesDEL005B TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL005B.jpg"));
#define KNibblesDEL005C TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL005C.jpg"));
#define KNibblesDEL006A TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL006A.jpg"));
#define KNibblesDEL006B TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL006B.jpg"));
#define KNibblesDEL006C TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesDEL006C.jpg"));

#define KNibblesREN001A TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN001A.jpg"));
#define KNibblesREN001B TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN001B.jpg"));
#define KNibblesREN002A TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN002A.jpg"));
#define KNibblesREN002B TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN002B.jpg"));
#define KNibblesREN003A TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN003A.jpg"));
#define KNibblesREN003B TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN003B.jpg"));
#define KNibblesREN003C TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN003C.jpg"));
#define KNibblesREN003A2 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN003A2.jpg"));
#define KNibblesREN004A TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN004A.jpg"));
#define KNibblesREN004B TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN004B.jpg"));
#define KNibblesREN004C TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN004C.jpg"));
#define KNibblesREN004A2 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN004A2.jpg"));
#define KNibblesREN005A TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN005A.jpg"));
#define KNibblesREN005B TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN005B.jpg"));
#define KNibblesREN005C TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN005C.jpg"));
#define KNibblesREN005A2 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN005A2.jpg"));
#define KNibblesREN006A TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN006A.jpg"));
#define KNibblesREN006B TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN006B.jpg"));
#define KNibblesREN006C TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN006C.jpg"));
#define KNibblesREN006B2 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN006B2.jpg"));
#define KNibblesREN006C2 TFileName(_L("E:\\testing\\data\\StifAFSQTest\\NibblesREN006C2.jpg"));


// CLASS DECLARATION

class CAFSQTestAsynchSave : public CTestCase,
                            public MAsynchFSQObserver
    {
    public:

        CAFSQTestAsynchSave ();
        ~CAFSQTestAsynchSave ();

        // Allocate the resources for one test function
        void setUpL ();

        // Free the resources reserved in setUpL()
        void tearDown ();

        // A function to collect and return a suite of tests
        static MTest* suiteL ();

    protected:
    
        // stalling function
        void WaitForAsynchCompleteL();

        // Own test functions that use assertions and may leave:
        
        //constructor
        void TestFileManNewLReturnsNotNullL();   
    
        //add
        void TestAddEmptyObjectToQueueL();
        void TestAddObjectToEmptyQueueL();
        void TestAddObjectToEmptyQueueWillSaveL();
        void TestAddObjectToNonEmptyQueueL();
        void TestAddObjectToNonEmptyQueueWillSaveL();
        void TestAddObjectThatExistsAlreadyL();
        void TestAddObjectToEmptyQueueLManualStart();
    
        //delete
        void TestDeleteFromEmptyNotExistL();
        void TestDeleteFromEmptyExistsL();
        void TestDeleteNonEmptyExistsButNotInQueueL();
        void TestDeleteNonEmptyNotExistAndNotInQueueL();
        void TestDeleteInQueueNotActiveL();
        void TestDeleteInQueueActiveL();
    
        //destructor
        void TestDestructorL();
        
        //priority
        void TestSetPriorityL();
        
        //cancel
        void TestCancelL();
        
        //rename
        void TestRenameFromEmptyNotExistL();
        void TestRenameFromEmptyExistsL();
        void TestRenameNonEmptyExistsButNotInQueueL();
        void TestRenameNonEmptyNotExistAndNotInQueueL();
        void TestRenameInQueueNotActiveL();
        void TestRenameInQueueActiveL();
    
    protected:  // from MAsynchFSQObserver
    
        void MAsynchFSQEvent( TInt aError );
    
    private:
    
        CAsynchFSQ* iFSQ;
        CActiveScheduler* iScheduler;
        TTestCaseNumber currentTest;
        TBool delayedBool;
        TBool wait;
        // Needed for synchronization
        RMutex iMutex;
        TInt assertion;
        TInt numSaved;
        TInt numToSave;
        
    };

#endif // AFSQ_TEST_ASYNCH_SAVE_H

