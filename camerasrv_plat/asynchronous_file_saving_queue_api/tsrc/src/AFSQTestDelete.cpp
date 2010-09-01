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

void CAFSQTestAsynchSave::TestDeleteFromEmptyNotExistL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTDELETEOBJECT.001
    // 
    // Action : Test that cfileman::delete returns error if no such file.
    //      
    // Output : A KErrNotFound.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteFromEmptyNotExistL in") );
	
    currentTest = EDEL001p1;
    numSaved = 0;
    wait = ETrue;
    TInt error = KErrNone;
    
    // Delete the file if it exists
    RFs aFs;
    User::LeaveIfError(aFs.Connect());
    TFileName del001 = KNibblesDEL001;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, del001, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteFromEmptyNotExistL exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del001);
        }
    aFs.Close();
    
    // Try delete
    CAsynchFSQ* aFSQ = NULL;
    TRAPD( err, 
        aFSQ = CAsynchFSQ::NewL();
        error = aFSQ->Delete( del001 );
        );
    TRAP_IGNORE(
        delete aFSQ;
        aFSQ = NULL;
        );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteFromEmptyNotExistL errorIsKErrNotFound=%d notErr=%d"), (error == KErrNotFound), !err );
    
    assertL( (error == KErrNotFound) && !err );
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteFromEmptyNotExistL out") );
    }

void CAFSQTestAsynchSave::TestDeleteFromEmptyExistsL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTDELETEOBJECT.002
    // 
    // Action : Test that delete works if file exists and queue empty
    //      
    // Output : A KErrNone.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteFromEmptyExistsL in") );

    currentTest = EDEL002p1;
    numSaved = 0;
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
    TFileName del002 = KNibblesDEL002;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, del002, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteFromEmptyExistsL exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del002);
        }
    aFs.Close();

    // Write the file to be used in testing
    TInt err2 = KErrNone;
    RFs fsSession;
    err2 = fsSession.Connect();
    RFile myFile;
    if ( !err2 )
        {
        err2 = myFile.Create( fsSession, del002, 
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

    //delete
    CAsynchFSQ* aFSQ = NULL;
    TRAPD( err, 
        aFSQ = CAsynchFSQ::NewL();
        error = aFSQ->Delete( del002 );
        );
    TRAP_IGNORE(
        delete aFSQ;
        aFSQ = NULL;
        );
    
    //check that file was deleted
    RFs fsSession2;
    TInt fsConnectErr = fsSession2.Connect();
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteFromEmptyExistsL fsConnectErr=%d"), fsConnectErr );
    RFile file2;
    TInt fileError = file2.Open(fsSession2, del002, EFileRead);
    file2.Close();
    fsSession2.Close();
    
    //assert
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteFromEmptyExistsL errorIsKErrNone=%d notErr=%d isFileError=%d"), (error == KErrNone), !err, !(!(fileError )) );
    
    assertL( (error == KErrNone) && !err && (fileError!=KErrNone) );

    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteFromEmptyExistsL out") );
    }

void CAFSQTestAsynchSave::TestDeleteNonEmptyExistsButNotInQueueL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTDELETEOBJECT.003
    // 
    // Action : Test that delete will work if the queue is not empty but
    //          the requested file is not in the queue (but it also exists)
    //      
    // Output : A KErrNone.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyExistsButNotInQueueL in") );
	
    currentTest = EDEL003p1;
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
    TInt connectErr2 = file2.Open(aFs, aFilename2, EFileRead);
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
    TInt connectErr3 = file3.Open(aFs, aFilename3, EFileRead);
    file3.Size(iSize);
    HBufC8* buf3 = HBufC8::NewL(iSize);
    TPtr8  myData3 = buf3->Des();
    if ( !connectErr3 )
        {
        file3.Read( myData3 );
        }
    file3.Close();

    // Delete the file if it exists
    TFileName del003A = KNibblesDEL003A;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, del003A, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyExistsButNotInQueueL 1st exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del003A);
        }

    // Delete the file if it exists
    TFileName del003B = KNibblesDEL003B;
    RFile deleter2;
    TInt isDeleted2 = deleter2.Open(aFs, del003B, EFileRead);
    deleter2.Close();
    if ( isDeleted2 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyExistsButNotInQueueL 2nd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del003B);
        }
        
    // Delete the file if it exists
    TFileName del003C = KNibblesDEL003C;
    RFile deleter3;
    TInt isDeleted3 = deleter3.Open(aFs, del003C, EFileRead);
    deleter3.Close();
    if ( isDeleted3 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyExistsButNotInQueueL 3rd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del003C);
        }

    // Write the file to be used in testing
    TInt err2 = KErrNone;
    RFs fsSession;
    err2 = fsSession.Connect();
    RFile myFile;
    if ( !err2 )
        {
        err2 = myFile.Create( fsSession, del003A, 
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
        error = iFSQ->SaveAndDestroy( *buf2, del003B );
        buf2 = NULL;
        error = iFSQ->SaveAndDestroy( *buf3, del003C );
        buf3 = NULL;
        error = iFSQ->Delete( del003A );
        );

    //delayed assert on callback
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyExistsButNotInQueueL errorIsKErrNone=%d notErr=%d"), (error == KErrNone), !err );
    
    delayedBool = ( (error == KErrNone) && !err );
    WaitForAsynchCompleteL();

    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyExistsButNotInQueueL out") );
    }

void CAFSQTestAsynchSave::TestDeleteNonEmptyNotExistAndNotInQueueL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTDELETEOBJECT.004
    // 
    // Action : Test that delete will return error if the queue is not empty but
    //          the requested file is not in the queue (but it doesnt exist)
    //      
    // Output : A KErrNotFound.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyNotExistAndNotInQueueL in") );

    currentTest = EDEL004p1;
    numSaved = 0;
    numToSave = 2;
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
    TFileName del004A = KNibblesDEL004A;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, del004A, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyNotExistAndNotInQueueL 1st exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del004A);
        }

    // Delete the file if it exists
    TFileName del004B = KNibblesDEL004B;
    RFile deleter2;
    TInt isDeleted2 = deleter2.Open(aFs, del004B, EFileRead);
    deleter2.Close();
    if ( isDeleted2 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyNotExistAndNotInQueueL 2nd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del004B);
        }
        
    // Delete the file if it exists
    TFileName del004C = KNibblesDEL004C;
    RFile deleter3;
    TInt isDeleted3 = deleter3.Open(aFs, del004C, EFileRead);
    deleter3.Close();
    if ( isDeleted3 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyNotExistAndNotInQueueL 3rd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del004C);
        }

    aFs.Close();
    delete buf;
    buf = NULL;

    //delete
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        iFSQ->SetCallback( this );
        error = iFSQ->SaveAndDestroy( *buf2, del004B );
        buf2 = NULL;
        error = iFSQ->SaveAndDestroy( *buf3, del004C );
        buf3 = NULL;
        error = iFSQ->Delete( del004A );
        );

    //delayed assert on callback
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyNotExistAndNotInQueueL errorIsKErrNotFound=%d notErr=%d"), (error == KErrNotFound), !err );
    
    delayedBool = ( (error == KErrNotFound) && !err );
    WaitForAsynchCompleteL();

    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyNotExistAndNotInQueueL out") );
    }

void CAFSQTestAsynchSave::TestDeleteInQueueNotActiveL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTDELETEOBJECT.005
    // 
    // Action : Test that delete will work ok if the queue is not empty and
    //          the file is in the queue.  Should also check filesystem for existence.
    //          This case checks that the file NOT ACTIVELY being saved gets deleted.
    //      
    // Output : A KErrNone.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteInQueueNotActiveL in") );
	
    currentTest = EDEL005p1;
    numSaved = 0;
    numToSave = 2;
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
    TFileName del005A = KNibblesDEL005A;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, del005A, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteInQueueNotActiveL 1st exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del005A);
        }

    // Delete the file if it exists
    TFileName del005B = KNibblesDEL005B;
    RFile deleter2;
    TInt isDeleted2 = deleter2.Open(aFs, del005B, EFileRead);
    deleter2.Close();
    if ( isDeleted2 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyNotExistAndNotInQueueL 2nd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del005B);
        }
        
    // Delete the file if it exists
    TFileName del005C = KNibblesDEL005C;
    RFile deleter3;
    TInt isDeleted3 = deleter3.Open(aFs, del005C, EFileRead);
    deleter3.Close();
    if ( isDeleted3 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteInQueueNotActiveL 3rd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del005C);
        }

    aFs.Close();

    //delete
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        iFSQ->SetCallback( this );
        error = iFSQ->SaveAndDestroy( *buf2, del005B );
        buf2 = NULL;
        error = iFSQ->SaveAndDestroy( *buf3, del005C );
        buf3 = NULL;
        error = iFSQ->SaveAndDestroy( *buf, del005A );
        buf = NULL;
        error = iFSQ->Delete( del005A );
        );

    //delayed assert on callback
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteInQueueNotActiveL errorIsKErrNone=%d notErr=%d"), (error == KErrNone), !err );
    
    delayedBool = ( (error == KErrNone) && !err );
    WaitForAsynchCompleteL();

    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteInQueueNotActiveL out") );
    }

void CAFSQTestAsynchSave::TestDeleteInQueueActiveL()
    {
    // Group:   Asynch Save
    // 
    // ID:      CAE.FM.TESTDELETEOBJECT.006
    // 
    // Action : Test that delete will work ok if the queue is not empty and
    //          the file is in the queue.  Should also check filesystem for existence.
    //          This case checks that the file ACTIVELY being saved gets deleted.
    //      
    // Output : A KErrNone.   
	
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteInQueueActiveL in") );
	
    currentTest = EDEL006p1;
    numSaved = 0;
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
    TFileName del006A = KNibblesDEL006A;
    RFile deleter;
    TInt isDeleted = deleter.Open(aFs, del006A, EFileRead);
    deleter.Close();
    if ( isDeleted == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteInQueueNotActiveL 1st exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del006A);
        }

    // Delete the file if it exists
    TFileName del006B = KNibblesDEL006B;
    RFile deleter2;
    TInt isDeleted2 = deleter2.Open(aFs, del006B, EFileRead);
    deleter2.Close();
    if ( isDeleted2 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteNonEmptyNotExistAndNotInQueueL 2nd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del006B);
        }
        
    // Delete the file if it exists
    TFileName del006C = KNibblesDEL006C;
    RFile deleter3;
    TInt isDeleted3 = deleter3.Open(aFs, del006C, EFileRead);
    deleter3.Close();
    if ( isDeleted3 == KErrNone )
        {
        // if no error, assume the file exists, so delete it
    	RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteInQueueNotActiveL 3rd exists, deleting") );
        BaflUtils ba;
        ba.DeleteFile(aFs, del006C);
        }

    aFs.Close();
	    
    //delete
    TInt errorA = 0;
    TInt errorB = 0;
    TRAPD( err, 
        iFSQ = CAsynchFSQ::NewL();
        iFSQ->SetCallback( this );
        error = iFSQ->SaveAndDestroy( *buf2, del006B );
        buf2 = NULL;
        error = iFSQ->SaveAndDestroy( *buf3, del006C );
        buf3 = NULL;
        error = iFSQ->SaveAndDestroy( *buf, del006A );
        buf = NULL;
        errorA = iFSQ->Delete( del006B );
        errorB = iFSQ->Delete( del006C );
        );

    //delayed assert on callback
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteInQueueActiveL errorIsKErrNone=%d notErr=%d !errorA=%d !errorB=%d"), (error == KErrNone), !err, !errorA, !errorB );
    
    delayedBool = ( (error == KErrNone) && !err && !errorA && !errorB );
    WaitForAsynchCompleteL();
    
    RDebug::Print( _L("CAFSQTestAsynchSave::TestDeleteInQueueActiveL out") );
    }


