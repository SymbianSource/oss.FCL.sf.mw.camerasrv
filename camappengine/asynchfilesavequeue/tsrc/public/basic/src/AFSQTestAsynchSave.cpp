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
* Description:  FSQ Test DLL
*
*/


#include "../inc/AFSQTestAsynchSave.h"
#include <f32file.h> // for opening / creating files
#include <bautils.h> // for deleting files

CAFSQTestAsynchSave::CAFSQTestAsynchSave()
    {
    RDebug::Print( _L("CAFSQTestAsynchSave::CAFSQTestAsynchSave in") );
    
    // Create a mutex for synchronization purpose
    iMutex.CreateLocal();
    
    RDebug::Print( _L("CAFSQTestAsynchSave::CAFSQTestAsynchSave out") );
    }

CAFSQTestAsynchSave::~CAFSQTestAsynchSave()
    {
    // Nothing to do
    }

void CAFSQTestAsynchSave::setUpL()
    {
    RDebug::Print( _L("CAFSQTestAsynchSave::setUpL in") );
    }

void CAFSQTestAsynchSave::tearDown()
    {
    RDebug::Print( _L("CAFSQTestAsynchSave::tearDown in") );
    }

void CAFSQTestAsynchSave::TestFileManNewLReturnsNotNullL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTFILEMANNEWLRETURNSNOTNULL.001
    // 
    // Action : Test that cfileman::newl returns a value.
    //      
    // Output : A handle to the requested object.   

    RDebug::Print( _L("CAFSQTestAsynchSave::TestFileManNewLReturnsNotNullL in") );

    currentTest = ECONSTRUCT001p1;
    wait = ETrue;

    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestFileManNewLReturnsNotNullL isNotNull=%d notErr=%d"), (iFSQ!=NULL), !err);
    assertL( (iFSQ != NULL) && !err );
    delete iFSQ;
    iFSQ = NULL;
    RDebug::Print( _L("CAFSQTestAsynchSave::TestFileManNewLReturnsNotNullL out") );
    }

void CAFSQTestAsynchSave::TestDestructorL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTDESTRUCTOR.001
    // 
    // Action : Test that delete works
    //      
    // Output : KErrNone on all fronts.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDestructorL in") );

    currentTest = EDESTRUCT001p1;
    wait = ETrue;

    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        delete iFSQ;
        iFSQ = NULL;
        );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDestructorL notErr=%d"), !err );
    
    assertL( !err );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDestructorL out") );
    }
    
void CAFSQTestAsynchSave::TestSetPriorityL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTSETPRIORITY.001
    // 
    // Action : Test that setting the priority doesn't fail
    //      
    // Output : KErrNone on all fronts.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestSetPriorityL in") );

    currentTest = EPRIORITY001p1;
    wait = ETrue;

    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        iFSQ->SetPriority( EPriorityMore );
        delete iFSQ;
        iFSQ = NULL;
        );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestSetPriorityL notErr=%d"), !err );
    
    assertL( !err );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestSetPriorityL out") );
    }   
    
void CAFSQTestAsynchSave::TestCancelL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTCANCEL.001
    // 
    // Action : Test that canceling will....
    //      
    // Output : An error code.   

    RDebug::Print( _L("CAFSQTestAsynchSave::TestCancelL in") );

    TInt error = KErrNone;
    currentTest = ECANCEL001p1;
    numSaved = 0;
    wait = ETrue;

    // Read the file to be used in testing.
    TFileName aFilename = KNibbles;
    RFs aFs;
    User::LeaveIfError(aFs.Connect());
    RFile file;
    TInt connectErr = file.Open(aFs, aFilename, EFileRead);
    TInt iSize;   
    file.Size(iSize);
    HBufC8* buf = HBufC8::NewL(iSize);
    TPtr8  myData = buf->Des();
    if ( !connectErr )
        {
        file.Read( myData );
        }
    file.Close();
    
    // Delete the file if it exists
    TFileName cancel001 = KNibblesCANCEL001;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, cancel001, EFileRead);
    deleter.Close();

    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
        RDebug::Print( _L("CAFSQTestAsynchSave::TestCancelL exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, cancel001);
        }
    aFs.Close();
    
    // Save    
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();        
        //add this as a callback
        iFSQ->SetCallback( this );
        error = iFSQ->SaveAndDestroy( *buf, cancel001 );
        buf = NULL; // transfer ownership 
        iFSQ->Cancel();
        );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestCancelL isKErrNone=%d notErr=%d"), (error == KErrNone), !err );
   
    // do not wait for callback to assert, since we have canceled
    delayedBool = ( (error == KErrNone) && !err );
    assertL( delayedBool );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestCancelL out") );
    }
    
void CAFSQTestAsynchSave::WaitForAsynchCompleteL()
    {
    RDebug::Print( _L("CAFSQTestAsynchSave::WaitForAsynchCompleteL in") );

    RDebug::Print( _L("CAFSQTestAsynchSave::WaitForAsynchCompleteL starting AS now") );
    CActiveScheduler::Start();
        
    RDebug::Print( _L("CAFSQTestAsynchSave::WaitForAsynchCompleteL closing") );
    //close up and assert
    User::After( TTimeIntervalMicroSeconds32(1000000) );
    if( iFSQ )
        {
        delete iFSQ;
        iFSQ = NULL; 
        }
        
    RDebug::Print( _L("CAFSQTestAsynchSave::WaitForAsynchCompleteL we are asserting the following: %d"), assertion );
        
    assertL( assertion );
       
    RDebug::Print( _L("CAFSQTestAsynchSave::WaitForAsynchCompleteL out") );
    }

void CAFSQTestAsynchSave::MAsynchFSQEvent( TInt aError )
    {
    RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent in, waiting"));
    User::After( TTimeIntervalMicroSeconds32(2000000) );
    RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent in, done waiting"));

    numSaved++;
    RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent in, numToSave=%d"),numToSave);

    switch( currentTest )
        {
        case EADD002p1:
            RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(2) delayedBool=%d !aError=%d"), delayedBool, !aError );
            CActiveScheduler::Stop();
            RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(2)"));
    
            assertion = ( delayedBool && !aError );
            break;
            
        case EADD003p1:
            {
            RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(3)"));
            CActiveScheduler::Stop();
            RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(3)"));
            
            //check that the file exists
            RFs fsSession;
            TInt connectErr = fsSession.Connect();
            RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(3) connectErr=%d"), connectErr );
            RFile file;
            TFileName add003 = KNibblesADD003;
            TInt fileErr = file.Open(fsSession, add003, EFileRead);
            file.Close();
            fsSession.Close();

            RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(3) aErrorIsKErrNone=%d !fileErr=%d"), (aError == KErrNone), !fileErr );
            assertion = ( (aError == KErrNone) && !fileErr);
            break;        
            }           
        case EADD004p1:
            {
            if( numToSave == numSaved )
                {
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(4)"));
                CActiveScheduler::Stop();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(4)"));
                
                //check that the file exists
                RFs fsSession;
                TInt connectErr = fsSession.Connect();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(4) connectErr=%d"), connectErr );
                RFile file1;
                RFile file2;
                TFileName add004A = KNibblesADD004A;
                TFileName add004B = KNibblesADD004B;
                TInt fileErr1 = file1.Open(fsSession, add004A, EFileRead);
                TInt fileErr2 = file2.Open(fsSession, add004B, EFileRead);
                file1.Close();
                file2.Close();
                fsSession.Close();

                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(4) delayedBool=%d !aError=%d !fileErr1=%d !fileErr2=%d"), delayedBool, !aError, !fileErr1, !fileErr2 );
                assertion = ( delayedBool && !aError && !fileErr1 && !fileErr2);
                }
            break;
            }
        case EADD005p1:
            {        
            if( numToSave == numSaved )
                {
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(5)"));
                CActiveScheduler::Stop();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(5)"));
                
                //check that the file exists
                RFs fsSession;
                TInt connectErr = fsSession.Connect();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(5) connectErr=%d"), connectErr );
                RFile file1;
                RFile file2;
                TFileName add005A = KNibblesADD005A;
                TFileName add005B = KNibblesADD005B;
                TInt fileErr1 = file1.Open(fsSession, add005A, EFileRead);
                TInt fileErr2 = file2.Open(fsSession, add005B, EFileRead);
                file1.Close();
                file2.Close();
                fsSession.Close();
                    
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(5) isaErrorKErrNone=%d"), (aError==KErrNone) );
                assertion = ( aError == KErrNone );
                }
            break;
            }        
        case EADD007p1:
            {        
            RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(EADD007p1) delayedBool=%d !aError=%d"), delayedBool, !aError );
            CActiveScheduler::Stop();
            RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(EADD007p1)"));
    
            assertion = ( delayedBool && !aError );
            break;
            }                
        case EDEL003p1:
            {
            if( numToSave == numSaved )
                {
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(8)"));
                CActiveScheduler::Stop();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(8)"));
                
                //check that file was deleted
                RFs fsSession2;
                TInt connectErr = fsSession2.Connect();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(8) connectErr=%d"), connectErr );
                RFile file3;
                TFileName del003A = KNibblesDEL003A;
                TInt fileError = file3.Open(fsSession2, del003A, EFileRead);
                file3.Close();
                fsSession2.Close();
    
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(8) delayedBool=%d !aError=%d fileError=%d"), delayedBool, !aError, !(!fileError) );
                assertion = ( delayedBool && !aError && !(!fileError) );
                }
            break;
            }    
        case EDEL004p1:
            {
            if( numToSave == numSaved )
                {
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(9)"));
                CActiveScheduler::Stop();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(9)"));
                
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(9) delayedBool=%d !aError=%d"), delayedBool, !aError );
                assertion = ( delayedBool && !aError );
                }
            break;
            }    
        case EDEL005p1:
            {
            if( numToSave == numSaved )
                {
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(10)"));
                CActiveScheduler::Stop();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(10)"));
                
                //check that file was deleted
                RFs fsSession2;
                TInt connectErr = fsSession2.Connect();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(10) connectErr=%d"), connectErr );
                RFile file3;
                TFileName del005A = KNibblesDEL005A;
                TInt fileError = file3.Open(fsSession2, del005A, EFileRead);
                file3.Close();
            
                //check that file was saved
                RFile file4;
                TFileName del005B = KNibblesDEL005B;
                TInt fileError2 = file4.Open(fsSession2, del005B, EFileRead);
                file4.Close();
                
                //check that file was saved
                RFile file5;
                TFileName del005C = KNibblesDEL005C;
                TInt fileError3 = file5.Open(fsSession2, del005C, EFileRead);
                file5.Close();
                fsSession2.Close();
            
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(10) delayedBool=%d !aError=%d fileError=%d !fileError2=%d !fileError3=%d"), delayedBool, !aError, !(!fileError), !fileError2, !fileError3 );
                assertion = ( delayedBool && !aError && !(!fileError) && !fileError2 && !fileError3 );
                }
            break;
            }    
        case EDEL006p1:
            {
            //check that file was saved
            RFs fsSession2;
            TInt connectErr = fsSession2.Connect();
            RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(11) connectErr=%d"), connectErr );
            RFile file5;
            TFileName del006A = KNibblesDEL006A;
            TInt fileError3 = file5.Open(fsSession2, del006A, EFileRead);
            file5.Close();
            
            if( iFSQ->ActionsLeft() == 0 )
                {
                //we should be done with the rest...
                
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(11)"));
                CActiveScheduler::Stop();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(11)"));
                
                //check that file was deleted
                RFile file3;
                TFileName del006C = KNibblesDEL006C;
                TInt fileError = file3.Open(fsSession2, del006C, EFileRead);
                file3.Close();
                
                //check that file was saved
                RFile file4;
                TFileName del006B = KNibblesDEL006B;
                TInt fileError2 = file4.Open(fsSession2, del006B, EFileRead);
                file4.Close();
                
            
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(11) delayedBool=%d !aError=%d fileError=%d fileError2=%d !fileError3=%d"), delayedBool, !aError, !(!fileError), !(!fileError2), !fileError3 );
                assertion = ( delayedBool && !aError && !(!fileError) && !(!fileError2) && !fileError3 );
                }
                
            fsSession2.Close();
            break;
            }    
        case EREN003p1:
            {
            if( numToSave == numSaved )
                {
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(15)"));
                CActiveScheduler::Stop();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(15)"));
                
                //check that file was renamed
                RFs fsSession2;
                TInt connectErr = fsSession2.Connect();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(15) connectErr=%d"), connectErr );
                RFile file3;
                TFileName ren003A2 = KNibblesREN003A2;
                TInt fileError = file3.Open(fsSession2, ren003A2, EFileRead);
                file3.Close();
                
                //check that file was saved
                RFile file4;
                TFileName ren003B = KNibblesREN003B;
                TInt fileError2 = file4.Open(fsSession2, ren003B, EFileRead);
                file4.Close();
                
                //check that file was renamed (so this one wont exist anymore)
                RFile file5;
                TFileName ren003A = KNibblesREN003A;
                TInt fileError3 = file5.Open(fsSession2, ren003A, EFileRead);
                file5.Close();
                fsSession2.Close();
            
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(15) delayedBool=%d !aError=%d !fileError=%d !fileError2=%d !!fileError3=%d"), delayedBool, !aError, !fileError, !fileError2, !(!fileError3) );
                assertion = ( delayedBool && !aError && !fileError && !fileError2 && !(!fileError3) );
                }
            break;
            }    
        case EREN004p1:
            {
            if( numToSave == numSaved )
                {
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(16)"));
                CActiveScheduler::Stop();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(16)"));
                
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(16) delayedBool=%d !aError=%d"), delayedBool, !aError );
                assertion = ( delayedBool && !aError );
                }
            break;
            }    
        case EREN005p1:
            {
            if( numToSave == numSaved )
                {
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(17)"));
                CActiveScheduler::Stop();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(17)"));
                
                //check that file was renamed
                RFs fsSession2;
                TInt connectErr = fsSession2.Connect();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(17) connectErr=%d"), connectErr );
                RFile file3;
                TFileName ren005A2 = KNibblesREN005A2;
                TInt fileError = file3.Open(fsSession2, ren005A2, EFileRead);
                file3.Close();
                
                //check that file was saved
                RFile file4;
                TFileName ren005B = KNibblesREN005B;
                TInt fileError2 = file4.Open(fsSession2, ren005B, EFileRead);
                file4.Close();
                
                //check that file was renamed (so this one wont exist anymore)
                RFile file5;
                TFileName ren005A = KNibblesREN005A;
                TInt fileError3 = file5.Open(fsSession2, ren005A, EFileRead);
                file5.Close();
                
                //check that file was saved
                RFile file6;
                TFileName ren005C = KNibblesREN005C;
                TInt fileError4 = file6.Open(fsSession2, ren005C, EFileRead);
                file6.Close();
                fsSession2.Close();
            
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(17) delayedBool=%d !aError=%d !fileError=%d !fileError2=%d !!fileError3=%d !fileError4=%d"), delayedBool, !aError, !fileError, !fileError2, !(!fileError3), !fileError4 );
                assertion = ( delayedBool && !aError && !fileError && !fileError2 && !(!fileError3) && !fileError4 );
                }
            break;
            }    
        case EREN006p1:
            {
            if( numToSave == numSaved )
                {
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(18)"));
                CActiveScheduler::Stop();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(18)"));
                
                //check that file was renamed
                RFs fsSession2;
                TInt connectErr = fsSession2.Connect();
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(18) connectErr=%d"), connectErr );
                RFile file3;
                TFileName ren006C2 = KNibblesREN006C2;
                TInt fileError = file3.Open(fsSession2, ren006C2, EFileRead);
                file3.Close();
                
                //check that file was saved
                RFile file4;
                TFileName ren006B2 = KNibblesREN006B2;
                TInt fileError2 = file4.Open(fsSession2, ren006B2, EFileRead);
                file4.Close();
                
                //check that file was renamed (so this one wont exist anymore)
                RFile file5;
                TFileName ren006B = KNibblesREN006B;
                TInt fileError3 = file5.Open(fsSession2, ren006B, EFileRead);
                file5.Close();
                
                //check that file was saved
                RFile file6;
                TFileName ren006A = KNibblesREN006A;
                TInt fileError4 = file6.Open(fsSession2, ren006A, EFileRead);
                file6.Close();
                
                //check that file was renamed (so this one wont exist anymore)
                RFile file7;
                TFileName ren006C = KNibblesREN006C;
                TInt fileError5 = file7.Open(fsSession2, ren006C, EFileRead);
                file7.Close();
                
                fsSession2.Close();
            
                RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(17) delayedBool=%d !aError=%d !fileError=%d !fileError2=%d !!fileError3=%d !fileError4=%d !!fileError5=%d"), delayedBool, !aError, !fileError, !fileError2, !(!fileError3), !fileError4, !(!fileError5) );
                assertion = ( delayedBool && !aError && !fileError && !fileError2 && !(!fileError3) && !fileError4 && !(!fileError5) );
                }
            break;
            }
        case EADD006p1:
            {
            RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(19)"));
            CActiveScheduler::Stop();
            RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(19)"));
            assertion = ( delayedBool && ( aError == KErrAlreadyExists ));
            break;
            }
        case ECANCEL001p1:
            {
            RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(ECANCEL001p1) delayedBool=%d !aError=%d"), delayedBool, !aError );
            // do nothing, we are waiting for cancel
            RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent(ECANCEL001p1)"));
            break;
            }
        default:
            break;
        }

    RDebug::Print( _L("CAFSQTestAsynchSave::MAFSQEvent out") );
    }

MTest* CAFSQTestAsynchSave::suiteL ()
    {
    // An own static function to collect the test functions into one 
    // suite of tests. The framework will run the tests and free the
    // memory allocated for the test suite.

    RDebug::Print( _L("CAFSQTestAsynchSave::suiteL in") );
    
    // Always use NewL (Do not use NewLC here)
    CTestSuite *suite = CTestSuite::NewL( _L8("CAFSQTestAsynchSave") );

    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTFILEMANNEWLRETURNSNOTNULL.001"), &TestFileManNewLReturnsNotNullL ) );
    
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTDESTRUCTOR.001"), &TestDestructorL ) );
    
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTSETPRIORITY.001"), &TestSetPriorityL ) );
    
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTCANCEL.001"), &TestCancelL ) );
    
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTADDOBJECT.001"), &TestAddEmptyObjectToQueueL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTADDOBJECT.002"), &TestAddObjectToEmptyQueueL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTADDOBJECT.003"), &TestAddObjectToEmptyQueueWillSaveL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTADDOBJECT.004"), &TestAddObjectToNonEmptyQueueL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTADDOBJECT.005"), &TestAddObjectToNonEmptyQueueWillSaveL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTADDOBJECT.006"), &TestAddObjectThatExistsAlreadyL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTADDOBJECT.007"), &TestAddObjectToEmptyQueueLManualStart ) );
    
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTDELETEOBJECT.001"), &TestDeleteFromEmptyNotExistL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTDELETEOBJECT.002"), &TestDeleteFromEmptyExistsL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTDELETEOBJECT.003"), &TestDeleteNonEmptyExistsButNotInQueueL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTDELETEOBJECT.004"), &TestDeleteNonEmptyNotExistAndNotInQueueL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTDELETEOBJECT.005"), &TestDeleteInQueueNotActiveL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTDELETEOBJECT.006"), &TestDeleteInQueueActiveL ) );
    
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTRENAMEOBJECT.001"), &TestRenameFromEmptyNotExistL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTRENAMEOBJECT.002"), &TestRenameFromEmptyExistsL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTRENAMEOBJECT.003"), &TestRenameNonEmptyExistsButNotInQueueL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTRENAMEOBJECT.004"), &TestRenameNonEmptyNotExistAndNotInQueueL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTRENAMEOBJECT.005"), &TestRenameInQueueNotActiveL ) );
    suite->addTestL( CTestCaller<CAFSQTestAsynchSave>::NewL( _L8("CAE.FM.TESTRENAMEOBJECT.006"), &TestRenameInQueueActiveL ) );

    RDebug::Print( _L("CAFSQTestAsynchSave::suiteL returning") );
    return suite;
    }

