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
* Description:  Asynchronous File Saving Queue
*
*/



// INCLUDE FILES
#include <asynchfsq.h>
#include <bautils.h> // for deleting files
#include "asynchatom.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAsynchFSQ::CAsynchFSQ
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAsynchFSQ::CAsynchFSQ()
    {
    LOGTEXT( _L( "CAsynchFSQ::CAsynchFSQ() entering" ) );
    }

    
// -----------------------------------------------------------------------------
// CAsynchFSQ::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAsynchFSQ::ConstructL()
    {
    LOGTEXT( _L( "CAsynchFSQ::ConstructL() entering" ) );
    iPriority = EPriorityNormal;
    iManualStart = EFalse;
    LOGTEXT( _L( "CAsynchFSQ::ConstructL() exiting" ) );
    }

// -----------------------------------------------------------------------------
// CAsynchFSQ::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CAsynchFSQ* CAsynchFSQ::NewL()
    {
    LOGTEXT( _L( "CAsynchFSQ::NewL() entering" ) );
    CAsynchFSQ* self = new( ELeave ) CAsynchFSQ();   
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CAsynchFSQ::~CAsynchFSQ   
// Destructor
// -----------------------------------------------------------------------------
//
CAsynchFSQ::~CAsynchFSQ()
    {
    LOGTEXT( _L( "CAsynchFSQ::~CAsynchFSQ() entering" ) );

    // clean up queues
    iQueue.ResetAndDestroy();
    
    LOGTEXT( _L( "CAsynchFSQ::~CAsynchFSQ() exiting" ) );
    }

// -----------------------------------------------------------------------------
// CAsynchFSQ::SaveAndDestroy
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAsynchFSQ::SaveAndDestroy( TDesC8& aData, const TDesC& aPath, TBool aOverwrite )
    {
    LOGTEXT( _L( "CAsynchFSQ::SaveAndDestroy() entering" ) );
    
    TInt err = KErrNone;
    
    // return if NULL descriptor.
    if( !&aData )
        {
        return KErrArgument;
        }
    
    iOverwrite = aOverwrite;
    
    LOGTEXT2( _L( "CAsynchFSQ::SaveAndDestroy() iQueue.Count()=%d" ), iQueue.Count() );
    LOGTEXT2( _L( "CAsynchFSQ::SaveAndDestroy() ActionsLeft()=%d" ), ActionsLeft() );
    
    // add data to our queue
    err = Enqueue( EFileSave, aData, aPath, KNullDesC, ENullSchemaType, KNullDesC, KNullDesC );
    
    LOGTEXT2( _L( "CAsynchFSQ::SaveAndDestroy() iManualStart=%d" ), iManualStart );

    if ( !err && !iManualStart )
        {
        err = Go();	
        }
        
    LOGTEXT2( _L( "CAsynchFSQ::SaveAndDestroy() returning err=%d" ), err );
    return err;
    }
        
// -----------------------------------------------------------------------------
// CAsynchFSQ::Go
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAsynchFSQ::Go()
    {
    LOGTEXT( _L( "CAsynchFSQ::Go() entering" ) );
    TInt err = KErrNone;
    
    // make sure the queue has at least one object
    if( iQueue.Count() == 0 )
        {
        err = KErrNotReady;	
        }
    
    // activate AO's
    
    for( TInt i = 0; i < iQueue.Count(); i++ )
        {
        if(( !err )&&( iQueue[i]->GetState() == EPending ))
            {
            err = iQueue[i]->Go();
            }
        }
    
    LOGTEXT2( _L( "CAsynchFSQ::Go() returning err=%d" ), err );
    return err;
    }
        
// -----------------------------------------------------------------------------
// CAsynchFSQ::SetManualStart
// -----------------------------------------------------------------------------
//
EXPORT_C void CAsynchFSQ::SetManualStart( TBool aManualStart )
    {
    LOGTEXT( _L( "CAsynchFSQ::ManualStart() entering" ) );
    iManualStart = aManualStart;
    LOGTEXT( _L( "CAsynchFSQ::ManualStart() returning" ) );
    }

// -----------------------------------------------------------------------------
// CAsynchFSQ::Delete
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAsynchFSQ::Delete( const TDesC& aPath )
    {
    LOGTEXT( _L( "CAsynchFSQ::Delete() entering" ) );
	TInt err = KErrNotFound;

    // search for path name and delete if found
    for( TInt i = 0; i < iQueue.Count(); i++ )
        {
        if ( iQueue[i]->DoesLocalSave() )
            {
            if ( (iQueue[i]->GetPath()).Compare( aPath ) == 0 )
                {
                LOGTEXT( _L( "CAsynchFSQ::Delete() file found, deleting" ) );
                err = iQueue[i]->DeleteLocal();
                }
            }
        }
        
    // if still not found,
    // attempt to delete from file system
    if ( err == KErrNotFound )
        {
        LOGTEXT( _L( "CAsynchFSQ::Delete() not in queues, trying file system" ) );
        RFs fs;
        TInt connectError = fs.Connect();
        BaflUtils ba;
        if( !connectError && ba.FileExists( fs, aPath ) )
            {
            LOGTEXT( _L( "CAsynchFSQ::Delete() exists, deleting from file system" ) );
            err = KErrNone;
            ba.DeleteFile( fs, aPath );
            }
        fs.Close();
        }
       
    LOGTEXT2( _L( "CAsynchFSQ::Delete() returning err=%d" ), err );
    return err;
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQ::Rename
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAsynchFSQ::Rename( const TDesC& aOld, const TDesC& aNew )
    {
    LOGTEXT( _L( "CAsynchFSQ::Rename() entering" ) );
  
	TInt err = KErrNotFound;

    // search for path name and delete if found
    for( TInt i = 0; i < iQueue.Count(); i++ )
        {
        if ( iQueue[i]->DoesLocalSave() )
            {
            if ( (iQueue[i]->GetPath()).Compare( aOld ) == 0 )
                {
                LOGTEXT( _L( "CAsynchFSQ::Delete() file found, renaming" ) );
                err = iQueue[i]->RenameLocal( aNew );
                }
            }
        }       
    
    // if still not found,
    // attempt to rename from file system
    if ( err == KErrNotFound )
        {
        LOGTEXT( _L( "CAsynchFSQ::Rename() not in queues, trying file system" ) );
        RFs fs;
        TInt connectError = fs.Connect();
        BaflUtils ba;
        if( !connectError && ba.FileExists( fs, aOld ) )
            {
            LOGTEXT( _L( "CAsynchFSQ::Rename() exists, renaming in file system" ) );
            err = KErrNone;
            ba.RenameFile( fs, aOld, aNew );
            }
        fs.Close();
        }

    LOGTEXT2( _L( "CAsynchFSQ::Rename() returning err=%d" ), err );
    return err;
    }


    
// -----------------------------------------------------------------------------
// CAsynchFSQ::SetThreadPriority
// -----------------------------------------------------------------------------
//
EXPORT_C void CAsynchFSQ::SetPriority( TInt aPriority ) 
    {
    LOGTEXT2( _L( "CAsynchFSQ::SetThreadPriority() entering aPriority=%d" ), aPriority );
    iPriority = aPriority;
    LOGTEXT( _L( "CAsynchFSQ::SetThreadPriority() exiting" ) );
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQ::SetCallback
// -----------------------------------------------------------------------------
//
EXPORT_C void CAsynchFSQ::SetCallback( MAsynchFSQObserver* aCallback )
    {
    LOGTEXT( _L( "CAsynchFSQ::SetCallback() entering" ) );
    iCallback = aCallback;
    LOGTEXT( _L( "CAsynchFSQ::SetCallback() exiting" ) );
    }

// -----------------------------------------------------------------------------
// CAsynchFSQ::ActionsLeft
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAsynchFSQ::ActionsLeft()
    {
    TInt actionsLeft = 0;
    
    for( TInt i = 0; i < iQueue.Count(); i++ )
        {
        actionsLeft += iQueue[i]->ActionsLeft();
        }
        
    LOGTEXT2( _L( "CAsynchFSQ::ActionsLeft() actionsLeft=%d" ), actionsLeft );
    return actionsLeft;
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQ::Cancel
// -----------------------------------------------------------------------------
//
EXPORT_C void CAsynchFSQ::Cancel()
    {
    LOGTEXT( _L( "CAsynchFSQ::Cancel() entering" ) );
    
    while( iQueue.Count() )
        {
        delete iQueue[0];
        iQueue[0] = NULL;
        iQueue.Remove( 0 ); // Data MUST be deleted first
        }
        
    LOGTEXT( _L( "CAsynchFSQ::Cancel() exiting" ) );
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQ::Notify
// -----------------------------------------------------------------------------
//
void CAsynchFSQ::Notify( TInt aError )
    {
    LOGTEXT( _L( "CAsynchFSQ::Notify() entering" ) );
    iCallback->MAsynchFSQEvent( aError );
    
    // Scan and delete completed atoms
    for( TInt i = 0; i < iQueue.Count(); i++ )
        {
        if( iQueue[i]->ActionsLeft() == 0 )
            {
            delete iQueue[i];
            iQueue[i] = NULL;
            iQueue.Remove( i ); // Data MUST be deleted first
            i--;
            }
        }
        
    LOGTEXT( _L( "CAsynchFSQ::Notify() exiting" ) );
    }
        
// -----------------------------------------------------------------------------
// CAsynchFSQ::Enqueue
// -----------------------------------------------------------------------------
//
TInt CAsynchFSQ::Enqueue( TFSQActionType aActionType, TDesC8& aData, 
                          const TDesC& aPath, const TDesC& aURL, 
                          TFSQSchemaType aSchema, const TDesC& aUserName, 
                          const TDesC& aPassword )
    {
    LOGTEXT( _L( "CAsynchFSQ::Enqueue() entering" ) );
    
    TInt err = KErrNone;
    
    TInt queueError = KErrNone; 
    TRAP( err,
        // create atom
        CAsynchFSQAtom* atom = 
            CAsynchFSQAtom::NewL( this, 
                                  iPriority,
                                  aActionType,
                                  aData,
                                  aPath,
                                  aURL,
                                  aSchema,
                                  aUserName,
                                  aPassword );
        
        //append to queue
        iQueue.Append( atom );
        );

    if ( !err && queueError )
        {
        err = queueError;
        }

    LOGTEXT2( _L( "CAsynchFSQ::Enqueue() returning err=%d" ), err );
    return err;
    }
    
//  End of File  
