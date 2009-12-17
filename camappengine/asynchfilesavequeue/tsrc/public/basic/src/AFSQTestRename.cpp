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

void CAFSQTestAsynchSave::TestRenameFromEmptyNotExistL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTRENAMEOBJECT.001
    // 
    // Action : Test that cfileman::rename returns error if no such file.
    //      
    // Output : A KErrNotFound.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameFromEmptyNotExistL in") );
	
    currentTest = EREN001p1;
    numSaved = 0;
    wait = ETrue;
    TInt error = KErrNone;
    
    // Delete the file if it exists
    RFs aFs;
    User::LeaveIfError(aFs.Connect());
    TFileName ren001A = KNibblesREN001A;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, ren001A, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameFromEmptyNotExistL exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren001A);
        }
    aFs.Close();
    
    // Try delete
    CAsynchFSQ* aFSQ = NULL;
    TFileName ren001B = KNibblesREN001B;
    TRAPD( err, 
        aFSQ = CAsynchFSQ::NewL();
        error = aFSQ->Rename( ren001A, ren001B );
        );
    TRAP_IGNORE(
        delete aFSQ;
        aFSQ = NULL;
        );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameFromEmptyNotExistL errorIsKErrNotFound=%d notErr=%d"), (error == KErrNotFound), !err );
    
    assertL( (error == KErrNotFound) && !err );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameFromEmptyNotExistL out") );
    }

void CAFSQTestAsynchSave::TestRenameFromEmptyExistsL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTRENAMEOBJECT.002
    // 
    // Action : Test that rename works if file exists and queue empty
    //      
    // Output : A KErrNone.  
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameFromEmptyExistsL in") );
	 
    currentTest = EREN002p1;
    wait = ETrue;
    TInt error = KErrNone;

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
    TFileName ren002A = KNibblesREN002A;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, ren002A, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameFromEmptyExistsL exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren002A);
        }
    aFs.Close();

    // Write the file to be used in testing
    TInt err2 = KErrNone;
    RFs fsSession;
    err2 = fsSession.Connect();
    RFile myFile;
    if ( !err2 )
        {
        err2 = myFile.Create( fsSession, ren002A, 
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
    delete buf;
    buf = NULL;

    //delete file
    CAsynchFSQ* aFSQ = NULL;
    TFileName ren002B = KNibblesREN002B;
    TRAPD( err, 
        aFSQ = CAsynchFSQ::NewL();
        error = aFSQ->Rename( ren002A, ren002B );
        );
    TRAP_IGNORE(
        delete aFSQ;
        aFSQ = NULL;
        );

    //check that file was deleted
    RFs fsSession2;
    TInt fsConnectErr = fsSession2.Connect();
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameFromEmptyExistsL connectErr=%d"), fsConnectErr );
    RFile file2;
    TInt fileError = file2.Open(fsSession2, ren002B, EFileRead);
    file2.Close();
    fsSession2.Close();

    //assert
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameFromEmptyExistsL errorIsKErrNone=%d notErr=%d !fileError=%d"), (error == KErrNone), !err, !(fileError ) );
    
    assertL( (error == KErrNone) && !err && (fileError==KErrNone) );

    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameFromEmptyExistsL out") );   
    }

void CAFSQTestAsynchSave::TestRenameNonEmptyExistsButNotInQueueL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTRENAMEOBJECT.003
    // 
    // Action : Test that rename will work if the queue is not empty but
    //          the requested file is not in the queue (but it also exists)
    //      
    // Output : A KErrNone.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyExistsButNotInQueueL in") );
	
    currentTest = EREN003p1;
    numSaved = 0;
    numToSave = 2;
    wait = ETrue;
    TInt error = KErrNone;

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

    // Read the file to be used in testing.
    TFileName aFilename3 = KNibbles3;
    RFile file3;
    TInt connectErr3 = file3.Open(aFs, aFilename3,EFileRead);
    file3.Size(iSize);
    HBufC8* buf3 = HBufC8::NewL(iSize);
    TPtr8  myData3 = buf3->Des();
    if ( !connectErr3 )
        {
        file3.Read( myData3 );
        }
    file3.Close();

    // Delete the file if it exists
    TFileName ren003A = KNibblesREN003A;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, ren003A, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyExistsButNotInQueueL 1st exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren003A);
        }

    // Delete the file if it exists
    TFileName ren003B = KNibblesREN003B;
    RFile deleter2;
    TInt isDeleted2 = deleter2.Open(aFs, ren003B, EFileRead);
    deleter2.Close();
    if ( isDeleted2 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyExistsButNotInQueueL 2nd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren003B);
        }

    // Delete the file if it exists
    TFileName ren003C = KNibblesREN003C;
    RFile deleter3;
    TInt isDeleted3 = deleter3.Open(aFs, ren003C, EFileRead);
    deleter3.Close();
    if ( isDeleted3 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyExistsButNotInQueueL 3rd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren003C);
        }
        
    // Delete the file if it exists
    TFileName ren003A2 = KNibblesREN003A2;
    RFile deleter4;
    TInt isDeleted4 = deleter4.Open(aFs, ren003A2, EFileRead);
    deleter4.Close();
    if ( isDeleted4 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyExistsButNotInQueueL 4th exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren003A2);
        }    

    // Write the file to be used in testing
    TInt err2 = KErrNone;
    RFs fsSession;
    err2 = fsSession.Connect();
    RFile myFile;
    if ( !err2 )
        {
        err2 = myFile.Create( fsSession, ren003A, 
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
    aFs.Close();
    delete buf;
    buf = NULL;
	    
    //delete
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        iFSQ->SetCallback( this );
        error = iFSQ->SaveAndDestroy( *buf2, ren003B );
        buf2 = NULL;
        error = iFSQ->SaveAndDestroy( *buf3, ren003C );
        buf3 = NULL;
        error = iFSQ->Rename( ren003A, ren003A2);
        );

    //delayed assert on callback
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyExistsButNotInQueueL errorIsKErrNone=%d notErr=%d"), (error == KErrNone), !err );
    
    delayedBool = ( (error == KErrNone) && !err );
    WaitForAsynchCompleteL();
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyExistsButNotInQueueL out") );
    }

void CAFSQTestAsynchSave::TestRenameNonEmptyNotExistAndNotInQueueL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTRENAMEOBJECT.004
    // 
    // Action : Test that rename will return error if the queue is not empty but
    //          the requested file is not in the queue (but it doesnt exist)
    //      
    // Output : A KErrNotFound.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyNotExistAndNotInQueueL in") );
	
    currentTest = EREN004p1;
    numSaved = 0;
    numToSave = 2;
    wait = ETrue;
    TInt error = KErrNone;

    // Read the file to be used in testing.
    TFileName aFilename = KNibbles;
    RFs aFs;
    TInt connectError = User::LeaveIfError(aFs.Connect());
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

    // Read the file to be used in testing.
    TFileName aFilename3 = KNibbles3;
    RFile file3;
    TInt connectErr3 = file3.Open(aFs, aFilename3,EFileRead);
    file3.Size(iSize);
    HBufC8* buf3 = HBufC8::NewL(iSize);
    TPtr8  myData3 = buf3->Des();
    if ( !connectErr3 )
        {
        file3.Read( myData3 );
        }
    file3.Close();

    // Delete the file if it exists
    TFileName ren004A = KNibblesREN004A;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, ren004A, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyNotExistAndNotInQueueL 1st exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren004A);
        }

    // Delete the file if it exists
    TFileName ren004B = KNibblesREN004B;
    RFile deleter2;
    TInt isDeleted2 = deleter2.Open(aFs, ren004B, EFileRead);
    deleter2.Close();
    if ( isDeleted2 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyNotExistAndNotInQueueL 2nd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren004B);
        }

    // Delete the file if it exists
    TFileName ren004C = KNibblesREN004C;
    RFile deleter3;
    TInt isDeleted3 = deleter3.Open(aFs, ren004C,EFileRead);
    deleter3.Close();
    if ( isDeleted3 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyNotExistAndNotInQueueL 3rd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren004C);
        }
    aFs.Close();
    delete buf;
    buf = NULL;

    //delete
    TFileName ren004A2 = KNibblesREN004A2;
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        iFSQ->SetCallback( this );
        error = iFSQ->SaveAndDestroy( *buf2, ren004B );
        buf2 = NULL;
        error = iFSQ->SaveAndDestroy( *buf3, ren004C );
        buf3 = NULL;
        error = iFSQ->Rename( ren004A, ren004A2 );
        );

    //delayed assert on callback
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyNotExistAndNotInQueueL errorIsKErrNotFound=%d notErr=%d"), (error == KErrNotFound), !err );
    
    delayedBool = ( (error == KErrNotFound) && !err );
    WaitForAsynchCompleteL();
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyNotExistAndNotInQueueL out") );
    }

void CAFSQTestAsynchSave::TestRenameInQueueNotActiveL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTRENAMEOBJECT.005
    // 
    // Action : Test that rename will work ok if the queue is not empty and
    //          the file is in the queue.  Should also check filesystem for existence.
    //          This case checks that the file NOT ACTIVELY being saved gets renamed.
    //      
    // Output : A KErrNone.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueNotActiveL in") );
	
    currentTest = EREN005p1;
    numSaved = 0;
    numToSave = 3;
    wait = ETrue;
    TInt error = KErrNone;

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

    // Read the file to be used in testing.
    TFileName aFilename3 = KNibbles3;
    RFile file3;
    TInt connectErr3 = file3.Open(aFs, aFilename3,EFileRead);
    file3.Size(iSize);
    HBufC8* buf3 = HBufC8::NewL(iSize);
    TPtr8  myData3 = buf3->Des();
    if ( !connectErr3 )
        {
        file3.Read( myData3 );
        }
    file3.Close();

    // Delete the file if it exists
    TFileName ren005A = KNibblesREN005A;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, ren005A, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueNotActiveL 1st exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren005A);
        }

    // Delete the file if it exists
    TFileName ren005B = KNibblesREN005B;
    RFile deleter2;
    TInt isDeleted2 = deleter2.Open(aFs, ren005B, EFileRead);
    deleter2.Close();
    if ( isDeleted2 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueNotActiveL 2nd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren005B);
        }

    // Delete the file if it exists
    TFileName ren005C = KNibblesREN005C;
    RFile deleter3;
    TInt isDeleted3 = deleter3.Open(aFs, ren005C, EFileRead);
    deleter3.Close();
    if ( isDeleted3 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueNotActiveL 3rd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren005C);
        }

    // Delete the file if it exists
    TFileName ren005A2 = KNibblesREN005A2;
    RFile deleter4;
    TInt isDeleted4 = deleter4.Open(aFs, ren005A2, EFileRead);
    deleter4.Close();
    if ( isDeleted4 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameNonEmptyExistsButNotInQueueL 4th exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren005A2);
        }    

    aFs.Close();
	    
    //delete
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        iFSQ->SetCallback( this );
        error = iFSQ->SaveAndDestroy( *buf2, ren005B );
        buf2 = NULL;
        error = iFSQ->SaveAndDestroy( *buf3, ren005C );
        buf3 = NULL;
        error = iFSQ->SaveAndDestroy( *buf, ren005A );
        buf = NULL;
        error = iFSQ->Rename( ren005A, ren005A2 );
        );

    //delayed assert on callback
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueNotActiveL errorIsKErrNone=%d notErr=%d"), (error == KErrNone), !err );
    
    delayedBool = ( (error == KErrNone) && !err );
    WaitForAsynchCompleteL();
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueNotActiveL out") );
    }

void CAFSQTestAsynchSave::TestRenameInQueueActiveL()
{
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTRENAMEOBJECT.006
    // 
    // Action : Test that rename will work ok if the queue is not empty and
    //          the file is in the queue.  Should also check filesystem for existence?
    //          This case checks that the file ACTIVELY being saved gets renamed.
    //      
    // Output : A KErrNone.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueActiveL in") );
	
    currentTest = EREN006p1;
    numSaved = 0;
    numToSave = 3;
    wait = ETrue;
    TInt error = KErrNone;

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
    
    // Read the file to be used in testing.
    TFileName aFilename3 = KNibbles3;
    RFile file3;
    TInt connectErr3 = file3.Open(aFs, aFilename3,EFileRead);
    file3.Size(iSize);
    HBufC8* buf3 = HBufC8::NewL(iSize);
    TPtr8  myData3 = buf3->Des();
    if ( !connectErr3 )
        {
        file3.Read( myData3 );
        }
    file3.Close();

    // Delete the file if it exists
    TFileName ren006A = KNibblesREN006A;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, ren006A, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueActiveL 1st exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren006A);
        }

    // Delete the file if it exists
    TFileName ren006B = KNibblesREN006B;
    RFile deleter2;
    TInt isDeleted2 = deleter2.Open(aFs, ren006B, EFileRead);
    deleter2.Close();
    if ( isDeleted2 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueActiveL 2nd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren006B);
        }
        
    // Delete the file if it exists
    TFileName ren006C = KNibblesREN006C;
    RFile deleter3;
    TInt isDeleted3 = deleter3.Open(aFs, ren006C, EFileRead);
    deleter3.Close();
    if ( isDeleted3 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueActiveL 3rd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren006C);
        }
        
    // Delete the file if it exists
    TFileName ren006B2 = KNibblesREN006B2;
    RFile deleter4;
    TInt isDeleted4 = deleter4.Open(aFs, ren006B2, EFileRead);
    deleter4.Close();
    if ( isDeleted4 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueActiveL 4th exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren006B2);
        }
        
    // Delete the file if it exists
    TFileName ren006C2 = KNibblesREN006C2;
    RFile deleter5;
    TInt isDeleted5 = deleter5.Open(aFs, ren006C2, EFileRead);
    deleter5.Close();
    if ( isDeleted5 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueActiveL 5th exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, ren006C2);
        }                

    aFs.Close();

    //delete
    TInt errorA = 0;
    TInt errorB = 0;
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        iFSQ->SetCallback( this );
        error = iFSQ->SaveAndDestroy( *buf2, ren006B );
        buf2 = NULL;
        error = iFSQ->SaveAndDestroy( *buf3, ren006C );
        buf3 = NULL;
        error = iFSQ->SaveAndDestroy( *buf, ren006A );
        buf = NULL;
        errorA = iFSQ->Rename( ren006B, ren006B2 );
        errorB = iFSQ->Rename( ren006C, ren006C2 );
        );

    //delayed assert on callback
    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueActiveL errorIsKErrNone=%d notErr=%d !errorA=%d !errorB=%d"), (error == KErrNone), !err, !errorA, !errorB );
    
    delayedBool = ( (error == KErrNone) && !err && !errorA && !errorB );
    WaitForAsynchCompleteL();

    RDebug::Print( _L("CAFSQTestAsynchSave::TestRenameInQueueActiveL out") );
    }
    
    
