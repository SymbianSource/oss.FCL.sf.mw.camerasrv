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

void CAFSQTestAsynchSave::TestAddEmptyObjectToQueueL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTADDOBJECT.001
    // 
    // Action : Test that adding an null object to the queue will return an error.
    //      
    // Output : An error code.   

    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddEmptyObjectToQueueL in") );

    TInt error = KErrNone;
    currentTest = EADD001p1;
    numSaved = 0;
    wait = ETrue;
    
    HBufC8 * myData = HBufC8::NewL(1000);
    delete myData;
    myData = NULL;
    
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        iFSQ->SetCallback( this );
        TFileName add001 = KNibblesADD001;
        error = iFSQ->SaveAndDestroy( *myData, add001);
        );
    TRAPD( err2,
        delete iFSQ;
        iFSQ = NULL;
        );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddEmptyObjectToQueueL isKErrArgument=%d notErr=%d notErr2=%d"), (error == KErrArgument), !err, !err2 );
    
    //do not delay assert because queue should not be active
    assertL( (error == KErrArgument) && !err && !err2);
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddEmptyObjectToQueueL out") );
    }

void CAFSQTestAsynchSave::TestAddObjectToEmptyQueueL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTADDOBJECT.002
    // 
    // Action : Test that adding an object to an empty queue will at least start OK.
    //      
    // Output : An error code.  

    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToEmptyQueueL in") );

    TInt error = KErrNone;
    currentTest = EADD002p1;
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
    TFileName add002 = KNibblesADD002;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, add002, EFileRead);
    deleter.Close();

    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToEmptyQueueL exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, add002);
        }
    aFs.Close();
    
    // Save    
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();        
        //add this as a callback
        iFSQ->SetCallback( this );
        error = iFSQ->SaveAndDestroy( *buf, add002 );
        buf = NULL; // transfer ownership 
        );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToEmptyQueueL isKErrNone=%d notErr=%d"), (error == KErrNone), !err );
   
    delayedBool = ( (error == KErrNone) && !err );
    //wait for callback to assert
    WaitForAsynchCompleteL();
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToEmptyQueueL out") );
    }

void CAFSQTestAsynchSave::TestAddObjectToEmptyQueueWillSaveL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTADDOBJECT.003
    // 
    // Action : Test that adding an object to an empty queue will finish OK.
    //      
    // Output : Asynchronous report.    

    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToEmptyQueueWillSaveL in") );

    currentTest = EADD003p1;
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
    TFileName add003 = KNibblesADD003;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, add003, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToEmptyQueueL exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, add003);
        }
    aFs.Close();
    
    TRAP_IGNORE(  
        iFSQ = CAsynchFSQ::NewL();
        //add this as a callback
        iFSQ->SetCallback( this );
        iFSQ->SaveAndDestroy( *buf, add003 );
        buf = NULL; // transfer ownership
        );
    
    //wait for callback to assert
    WaitForAsynchCompleteL();
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToEmptyQueueWillSaveL out") );
    }

void CAFSQTestAsynchSave::TestAddObjectToNonEmptyQueueL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTADDOBJECT.004
    // 
    // Action : Test that adding an object to a non empty queue will at least start OK.
    //      
    // Output : An error code.

    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToNonEmptyQueueL in") );

    TInt error = KErrNone;
    TInt error2 = KErrNone;
    currentTest = EADD004p1;
    numSaved = 0;
    numToSave = 2;
    wait = ETrue;
   
    // Read the file to be used in testing.
    TFileName aFilename = KNibbles;
    RFs aFs;
    User::LeaveIfError(aFs.Connect());
    RFile file;
    TInt connectErr = file.Open(aFs, aFilename,EFileRead);
    TInt iSize;   
    file.Size(iSize);
    HBufC8* buf = HBufC8::NewL(iSize);
    TPtr8  myData = buf->Des();
    if ( !connectErr )
        {
        file.Read( myData );
        }
    file.Close();

    // Read the file to be used in testing.
    TFileName aFilename2 = KNibbles2;
    RFile file2;
    TInt connectErr2 = file2.Open(aFs, aFilename2,EFileRead);
    file2.Size(iSize);
    HBufC8* buf2 = HBufC8::NewL(iSize);
    TPtr8  myData2 = buf2->Des();
    if ( !connectErr2 )
        {
        file2.Read( myData2 );
        }
    file2.Close();

    // Delete the file if it exists
    TFileName add004A = KNibblesADD004A;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, add004A, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToEmptyQueueL 1st exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, add004A);
        }

    // Delete the file if it exists
    TFileName add004B = KNibblesADD004B;
    RFile deleter2;
    TInt isDeleted2 = deleter2.Open(aFs, add004B, EFileRead);
    deleter2.Close();
    if ( isDeleted2 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToEmptyQueueL 2nd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, add004B);
        }
    aFs.Close();

    // Save    
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        iFSQ->SetCallback( this );
        error = iFSQ->SaveAndDestroy( *buf, add004A );
        buf = NULL; // transfer ownership
        error2 = iFSQ->SaveAndDestroy( *buf2, add004B );
        buf2 = NULL; // transfer ownership
        );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToNonEmptyQueueL isKErrNone=%d is2KErrNone=%d notErr=%d"), (error == KErrNone), (error2 == KErrNone), !err );
    
    delayedBool = ( (error == KErrNone) && (error2 == KErrNone) && (!err) );
    //wait for callback to assert
    WaitForAsynchCompleteL();
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToNonEmptyQueueL out") );
    }

void CAFSQTestAsynchSave::TestAddObjectToNonEmptyQueueWillSaveL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTADDOBJECT.005
    // 
    // Action : Test that adding an object to a non empty queue will finish OK.
    //      
    // Output : Asynchronous report.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToNonEmptyQueueWillSaveL in") );

    //TInt error = KErrNone;
    currentTest = EADD005p1;
    numSaved = 0;
    numToSave = 2;
    wait = ETrue;
    
    // Read the file to be used in testing.
    TFileName aFilename = KNibbles;
    RFs aFs;
    User::LeaveIfError(aFs.Connect());
    RFile file;
    TInt connectErr = file.Open(aFs, aFilename,EFileRead);
    TInt iSize;   
    file.Size(iSize);
    HBufC8* buf = HBufC8::NewL(iSize);
    TPtr8  myData = buf->Des();
    if ( !connectErr )
        {
        file.Read( myData );
        }
    file.Close();

    // Read the file to be used in testing.
    TFileName aFilename2 = KNibbles2;
    RFile file2;
    TInt connectErr2 = file2.Open(aFs, aFilename2,EFileRead);
    file2.Size(iSize);
    HBufC8* buf2 = HBufC8::NewL(iSize);
    TPtr8  myData2 = buf2->Des();
    if ( !connectErr2 )
        {
        file2.Read( myData2 );
        }
    file2.Close();

    // Delete the file if it exists
    TFileName add005A = KNibblesADD005A;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, add005A, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToNonEmptyQueueWillSaveL 1st exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, add005A);
        }

    // Delete the file if it exists
    TFileName add005B = KNibblesADD005B;
    RFile deleter2;
    TInt isDeleted2 = deleter2.Open(aFs, add005B, EFileRead);
    deleter2.Close();
    if ( isDeleted2 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToNonEmptyQueueWillSaveL 2nd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, add005B);
        }
    aFs.Close();

    // Save    
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        iFSQ->SetCallback( this );
        iFSQ->SaveAndDestroy( *buf, add005A );
        buf = NULL; // transfer ownership
        iFSQ->SaveAndDestroy( *buf2, add005B );
        buf2 = NULL; // transfer ownership
        );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToNonEmptyQueueWillSaveL notErr=%d"), !err );
    
    //wait for callback to assert
    WaitForAsynchCompleteL();
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToNonEmptyQueueWillSaveL out") );
    }

void CAFSQTestAsynchSave::TestAddObjectThatExistsAlreadyL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTADDOBJECT.006
    // 
    // Action : Test that adding an object to an empty queue which exists int the file
    //          system will report KErrAlreadyExists.
    //      
    // Output : An error code.  

    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectThatExistsAlready in") );

    TInt error = KErrNone;
    currentTest = EADD006p1;
    numSaved = 0;
    wait = ETrue;

    // Read the file to be used in testing.
    TFileName aFilename = KNibbles;
    RFs aFs;
    User::LeaveIfError(aFs.Connect());
    RFile file;
    TInt connectErr = file.Open(aFs, aFilename,EFileRead);
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
    TFileName add006 = KNibblesADD006;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, add006, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectThatExistsAlreadyL exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, add006);
        }
    aFs.Close();

    // Write the file to be used in testing
    TInt err2 = KErrNone;
    RFs fsSession;
    err2 = fsSession.Connect();
    RFile myFile;
    if ( !err2 )
        {
        err2 = myFile.Create( fsSession, add006, 
                             EFileStream | EFileShareExclusive | EFileWrite );
        }
    if( !err2 )
        {
        err2 = myFile.Write( myData );
        }
    if( !err2 )
        {
        err2 = myFile.Flush();
        }
	myFile.Close();
	fsSession.Close();
	
    // Reuse the file name, to write where it exists.
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        //add this as a callback
        iFSQ->SetCallback( this );
        error = iFSQ->SaveAndDestroy( *buf, add006, EFalse );
        buf = NULL; // transfer ownership 
        );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectThatExistsAlreadyL isKErrNone=%d notErr=%d"), (error == KErrNone), !err );
   
    delayedBool = ( (error == KErrNone) && !err );
    //wait for callback to assert
    WaitForAsynchCompleteL();
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectThatExistsAlreadyL out") );
    }


void CAFSQTestAsynchSave::TestAddObjectToEmptyQueueLManualStart()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTADDOBJECT.007
    // 
    // Action : Test that adding an object to an empty manual queue will be OK.
    //      
    // Output : An error code.  

    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToEmptyQueueLManualStart in") );

    TInt error = KErrNone;
    TInt error2 = KErrNone;
    currentTest = EADD007p1;
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
    TFileName add007 = KNibblesADD007;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, add007, EFileRead);
    deleter.Close();

    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
        RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToEmptyQueueLManualStart exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, add007);
        }
    aFs.Close();
    
    // Save    
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();        
        //add this as a callback
        iFSQ->SetCallback( this );
        iFSQ->SetManualStart( ETrue );
        error = iFSQ->SaveAndDestroy( *buf, add007 );
        buf = NULL; // transfer ownership 
        error2 = iFSQ->Go();
        );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToEmptyQueueLManualStart isKErrNone=%d notErr=%d"), ((error == KErrNone) && (error2 == KErrNone)), !err );
   
    delayedBool = ( (error == KErrNone) && (error2 == KErrNone) && !err );
    //wait for callback to assert
    WaitForAsynchCompleteL();
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestAddObjectToEmptyQueueLManualStart out") );
    }
