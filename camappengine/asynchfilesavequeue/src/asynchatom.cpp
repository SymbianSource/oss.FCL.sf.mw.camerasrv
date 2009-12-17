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
* Description:  Asynchronous FSQ Atom
*
*/



// INCLUDE FILES
#include <asynchfsq.h>
#include <sysutil.h> // for disk space query
#include <bautils.h> // for deleting files
#include "asynchatom.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAsynchFSQAtom::CAsynchFSQAtom
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAsynchFSQAtom::CAsynchFSQAtom() : CActive( EPriorityNormal )
    {
    LOGTEXT( _L( "CAsynchFSQAtom::CAsynchFSQAtom() entering" ) );
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAsynchFSQAtom::ConstructL( CAsynchFSQ* aOwner, 
                                 TInt aPriority,
                                 TFSQActionType aActionType,  
                                 TDesC8& aData, 
                                 const TDesC& aPath, 
                                 const TDesC& aURL, 
                                 TFSQSchemaType aSchema, 
                                 const TDesC& aUserName, 
                                 const TDesC& aPassword )
    {
    LOGTEXT( _L( "CAsynchFSQAtom::ConstructL() entering" ) );
    iOwner = aOwner;
    iState = EPending;
    iDelayedFileName = NULL;
    SetPriority(aPriority);
    iSchema = aSchema;
    
    // prep to copy
    iData = NULL;
    iPath = NULL;
    iURL = NULL;
    iUserName = NULL;
    iPassword = NULL;
    
    // copy
    iActionType = aActionType;
    
    iData = &aData;  
    if ( aPath != KNullDesC ) 
        {
        iPath = HBufC::NewL( aPath.Length() ); 
        iPath->Des().Append( aPath ); 
        }
    if ( aURL != KNullDesC )
        {
        iURL = HBufC::NewL( aURL.Length() ); 
        iURL->Des().Append( aURL ); 
        }
    if ( aUserName != KNullDesC )
        {
        iUserName = HBufC::NewL( aUserName.Length() ); 
        iUserName->Des().Append( aUserName ); 
        }
    if ( aPassword != KNullDesC )
        {
        iPassword = HBufC::NewL( aPassword.Length() ); 
        iPassword->Des().Append( aPassword ); 
        }
    
    LOGTEXT( _L( "CAsynchFSQAtom::ConstructL() adding AO" ) );
    CActiveScheduler::Add( this );
    LOGTEXT( _L( "CAsynchFSQAtom::ConstructL() exiting" ) );
    }

// -----------------------------------------------------------------------------
// CAsynchFSQAtom::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAsynchFSQAtom* CAsynchFSQAtom::NewL( CAsynchFSQ* aOwner, 
                                      TInt aPriority,
                                      TFSQActionType aActionType,  
                                      TDesC8& aData, 
                                      const TDesC& aPath, 
                                      const TDesC& aURL, 
                                      TFSQSchemaType aSchema, 
                                      const TDesC& aUserName, 
                                      const TDesC& aPassword )
    {
    LOGTEXT( _L( "CAsynchFSQAtom::NewL() entering" ) );
    CAsynchFSQAtom* self = new( ELeave ) CAsynchFSQAtom();   
    CleanupStack::PushL( self );
    self->ConstructL(  aOwner, 
                       aPriority,
                       aActionType,  
                       aData, 
                       aPath, 
                       aURL, 
                       aSchema, 
                       aUserName, 
                       aPassword  );
                       
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CAsynchFSQAtom::~CAsynchFSQAtom   
// Destructor for the atom class
// -----------------------------------------------------------------------------
//
CAsynchFSQAtom::~CAsynchFSQAtom()
    {
    LOGTEXT( _L( "CAsynchFSQAtom::~CAsynchFSQAtom() entering" ) );
    
    LOGTEXT( _L( "CAsynchFSQAtom::~CAsynchFSQAtom() canceling" ) );
    Cancel();
    LOGTEXT( _L( "CAsynchFSQAtom::~CAsynchFSQAtom() canceled" ) );
    
    // Close file
    iFile.Close(); 
    
    // Close file system
    iFs.Close();
    
    // Free leftover data
    if( iData )
        {
        LOGTEXT( _L( "CAsynchFSQAtom::~CAsynchFSQAtom() deleting iData" ) );
        delete iData;
        iData = NULL;
        }

    if( iPath )
        {
        LOGTEXT( _L( "CAsynchFSQAtom::~CAsynchFSQAtom() deleting iPath" ) );
        delete iPath;
        iPath = NULL;
        }
        
    if( iURL )
        {
        LOGTEXT( _L( "CAsynchFSQAtom::~CAsynchFSQAtom() deleting iURL" ) );
        delete iURL;
        iURL = NULL;
        }
        
    if( iUserName )
        {
        LOGTEXT( _L( "CAsynchFSQAtom::~CAsynchFSQAtom() deleting iUserName" ) );
        delete iUserName;
        iUserName = NULL;
        }
        
    if( iPassword )
        {
        LOGTEXT( _L( "CAsynchFSQAtom::~CAsynchFSQAtom() deleting iPassword" ) );
        delete iPassword;
        iPassword = NULL;
        }                
        
    if( iDelayedFileName )
        {
        LOGTEXT( _L( "CAsynchFSQAtom::~CAsynchFSQAtom() deleting iDelayedFileName" ) );
        delete iDelayedFileName;
        iPassword = NULL;
        }      
        
    LOGTEXT( _L( "CAsynchFSQAtom::~CAsynchFSQAtom() exiting" ) );
    }

// -----------------------------------------------------------------------------
// CAsynchFSQAtom::Go
// -----------------------------------------------------------------------------
//
TInt CAsynchFSQAtom::Go()
    {
    LOGTEXT( _L( "CAsynchFSQAtom::Go() entering" ) );
    TInt err = KErrNone;
    
    if (( iState != EPending )||( IsActive() ))
        {
        err = KErrInUse;
        }
    else
        {
        // set new state
        switch ( iActionType )
	        {
    	    case EFileSave:
            case EFileSaveAndWebUpload:
    	        {
    	        LOGTEXT( _L( "CAsynchFSQAtom::Go() iState => ESaving" ) );
    	        iState = ESaving;
    	        break;	        
    	        }
    	    case EWebUpload:
    	        {
    	        LOGTEXT( _L( "CAsynchFSQAtom::Go() iState => EUploading" ) );
    	        iState = EUploading;
    	        break;	        
    	        }
    	    default:
    	        {
       	        LOGTEXT( _L( "CAsynchFSQAtom::Go() undefined activity requested" ) );
                err = KErrNotSupported;
    	        break;
    	        }
	        }	        
    
        // initiate AO activity
        LOGTEXT( _L( "CAsynchFSQAtom::Go() setting active" ) );
        SetActive();
        TRequestStatus* statusPtr = &iStatus;
        User::RequestComplete( statusPtr, KErrNone );
        LOGTEXT( _L( "CAsynchFSQAtom::Go() request completed" ) );
        }
        
    LOGTEXT( _L( "CAsynchFSQAtom::Go() returning" ) );
    return err;
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::GetState
// -----------------------------------------------------------------------------
//
TFSQAtomState CAsynchFSQAtom::GetState()
    {
    LOGTEXT( _L( "CAsynchFSQAtom::GetState() entering & returning" ) );
    return iState;
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::DoesLocalSave
// -----------------------------------------------------------------------------
//
TBool CAsynchFSQAtom::DoesLocalSave()
    {
    LOGTEXT( _L( "CAsynchFSQAtom::DoesLocalSave() entering" ) );
    TBool ret = EFalse;
    if (( iActionType == EFileSave )||( iActionType == EFileSaveAndWebUpload ))
        {
        ret = ETrue;
        }
    LOGTEXT2( _L( "CAsynchFSQAtom::DoesLocalSave() returning, ret=%d" ),ret );
    return ret;
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::GetPath
// -----------------------------------------------------------------------------
//
const TDesC& CAsynchFSQAtom::GetPath() const
    {
    LOGTEXT( _L( "CAsynchFSQAtom::GetPath() entering & returning" ) );
    return *iPath;
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::DeleteLocal
// -----------------------------------------------------------------------------
//
TInt CAsynchFSQAtom::DeleteLocal()
    {
    LOGTEXT( _L( "CAsynchFSQAtom::DeleteLocal() entering" ) );
    TInt err = KErrNone;
    
    // delete or delayed delete
    if (( iState == EPending ) || ( iState == ESaving )||( iState == ESavePending ))
        {
        LOGTEXT( _L( "CAsynchFSQAtom::DeleteLocal() delayed delete" ) );
        // we are currently saving, so we have to delete later
        iDelayedLocalDelete = ETrue;
        }
    else
        {
        LOGTEXT( _L( "CAsynchFSQAtom::DeleteLocal() deleting now" ) );
        // delete now
        err = KErrNotFound;
        RFs fs;
        TInt connectError = fs.Connect();
        BaflUtils ba;
        if( !connectError && ba.FileExists( fs, *iPath ) )
            {
            err = KErrNone;
            ba.DeleteFile( fs, *iPath );
            }
        fs.Close();
        }
    
    LOGTEXT2( _L( "CAsynchFSQAtom::DeleteLocal() returning, err=%d" ),err );
    return err;
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::RenameLocal
// -----------------------------------------------------------------------------
//
TInt CAsynchFSQAtom::RenameLocal( const TDesC& aNew )
    {
    LOGTEXT( _L( "CAsynchFSQAtom::RenameLocal() entering" ) );
    TInt err = KErrNone;
    
    // delete or delayed delete
    if (( iState == EPending ) || ( iState == ESaving )||( iState == ESavePending ))
        {
        LOGTEXT( _L( "CAsynchFSQAtom::RenameLocal() delayed rename" ) );
        // we are currently saving, so we have to delete later
        iDelayedLocalRename = ETrue;
        TRAP( err,
            iDelayedFileName = HBufC::NewL( aNew.Length() );
            iDelayedFileName->Des().Append( aNew );
            );
        }
    else
        {
        LOGTEXT( _L( "CAsynchFSQAtom::RenameLocal() renaming now" ) );
        err = KErrNotFound;
        RFs fs;
        TInt connectError = fs.Connect();
        BaflUtils ba;
        if( !connectError && ba.FileExists( fs, *iPath ) )
            {
            err = KErrNone;
            ba.RenameFile( fs, *iPath, aNew );
            }
        fs.Close();
        }
    
    LOGTEXT2( _L( "CAsynchFSQAtom::RenameLocal() returning, err=%d" ),err );
    return err;
    }    
    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::SaveL
// -----------------------------------------------------------------------------
//
void CAsynchFSQAtom::SaveL()
    {
    LOGTEXT( _L( "CAsynchFSQAtom::SaveL() entering" ) );
    iError = KErrNone;
    
    if ( iState != ESaving )
        {
        // the state machine is lost
        LOGTEXT( _L( "CAsynchFSQAtom::SaveL() bad state, leaving..." ) );
        User::Leave( KErrGeneral );
        }

    // Init
    iError = iFs.Connect();
    LOGTEXT2( _L( "CAsynchFSQAtom::SaveL() fsSession.Connect iError=%d" ),iError );
    
    // Get drive number
    TInt drive = 0;
    if( !iError )
        {
        iError = RFs::CharToDrive( iPath->Des()[0], drive );
        LOGTEXT2( _L( "CAsynchFSQAtom::SaveL() CharToDrive iError=%d" ),iError );
        }

    // Check disk space
    if ( !iError )
        {
        TBool fullDisk = EFalse;
        TRAPD( utilErr,
               fullDisk = SysUtil::DiskSpaceBelowCriticalLevelL( 
                          &iFs, iData->Length(), drive ) );
        if( utilErr )
            {
            LOGTEXT2( _L( "CAsynchFSQAtom::SaveL() SysUtil iError=%d" ),iError );
            iError = utilErr;
            }
        else if( fullDisk )
            {
            iError = KErrDiskFull;
            }
        }
   
    // Attempt to create the file
    if( !iError )
        {    
        if ( iOverwrite )
            {
            iError = iFile.Replace( iFs, iPath->Des(), EFileWrite );
            LOGTEXT2( _L( "CAsynchFSQAtom::SaveL() file.Replace iError=%d" ),iError );
            }
        else
            {
            iError = iFile.Create( iFs, iPath->Des(), EFileWrite );
            LOGTEXT2( _L( "CAsynchFSQAtom::SaveL() file.Open iError=%d" ),iError );
            }
        }
        
    // Write the file
    if( !iError )
        {
        LOGTEXT( _L( "CAsynchFSQAtom::SaveL() about to write" ) );
        SetActive();
        iFile.Write( *iData, iStatus );
        LOGTEXT( _L( "CAsynchFSQAtom::SaveL() write requested" ) );
        }
    
    // Update state
    LOGTEXT( _L( "CAsynchFSQAtom::SaveL() iState => ESavePending" ) );
    iState = ESavePending;
    
    // leave if error
    if ( iError )
        {
        LOGTEXT2( _L( "CAsynchFSQAtom::SaveL() Leaving with iError=%d" ),iError );
        User::Leave( iError );
        }
    
    LOGTEXT2( _L( "CAsynchFSQAtom::SaveL() exiting, iError=%d" ), iError );
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::SaveCleanupL
// -----------------------------------------------------------------------------
//
void CAsynchFSQAtom::SaveCleanupL()
    {
    LOGTEXT( _L( "CAsynchFSQAtom::SaveCleanupL() entering" ) );
        
    if ( iState != ESavePending )
        {
        // the state machine is lost
        LOGTEXT( _L( "CAsynchFSQAtom::SaveCleanupL() bad state, leaving..." ) );
        User::Leave( KErrGeneral );
        }
        
    // Flush file.
    if( !iError )
        {
        LOGTEXT( _L( "CAsynchFSQAtom::SaveCleanupL() flushing" ) );
        iError = iFile.Flush();
        LOGTEXT( _L( "CAsynchFSQAtom::SaveCleanupL() flushed" ) );
        }
        
    // Close file
    iFile.Close(); 
        
    // Delayed rename, if needed
    if (( !iError ) && ( iDelayedLocalRename ))
        {
        LOGTEXT( _L( "CAsynchFSQAtom::SaveCleanupL() doing delayed rename" ) );
        BaflUtils ba;
        if( ba.FileExists( iFs, *iPath ) )
            {
            iError = KErrNone;
            TPtrC newPath = *iDelayedFileName;
            ba.RenameFile( iFs, *iPath, newPath );
            }
        iDelayedLocalRename = EFalse;
        }
        
    // Delayed delete, if needed
    if (( !iError ) && ( iDelayedLocalDelete ))
        {
        LOGTEXT( _L( "CAsynchFSQAtom::SaveCleanupL() doing delayed delete" ) );
        iError = KErrNotFound;
        BaflUtils ba;
        if( ba.FileExists( iFs, *iPath ) )
            {
            iError = KErrNone;
            ba.DeleteFile( iFs, *iPath );
            }
        iDelayedLocalDelete = EFalse;
        LOGTEXT2( _L( "CAsynchFSQAtom::SaveCleanupL() delete done, iError=%d" ), iError );
        }
        
    // Close file system
    iFs.Close();
        
    // Update state
    switch ( iActionType )
	    {
	    case EFileSave:
	        {
            LOGTEXT( _L( "CAsynchFSQAtom::SaveCleanupL() iState => EComplete" ) );
            iState = EComplete;            
	        break;	        
	        }
	    case EFileSaveAndWebUpload:
	        {
            LOGTEXT( _L( "CAsynchFSQAtom::SaveCleanupL() iState => EUploading" ) );
            iState = EUploading;            
            
            // re-initiate AO activity
            LOGTEXT( _L( "CAsynchFSQAtom::SaveCleanupL() setting active" ) );
            SetActive();
            TRequestStatus* statusPtr = &iStatus;
            User::RequestComplete( statusPtr, KErrNone );
            LOGTEXT( _L( "CAsynchFSQAtom::SaveCleanupL() request completed" ) );
	        break;	        
	        }     	        
	    default:
	        {
	        // the state machine is lost
	        LOGTEXT( _L( "CAsynchFSQAtom::SaveCleanupL() bad action, leaving" ) );
	        User::Leave( KErrGeneral );
	        break;        
	        }	        
	    }	
    
    // report completion
    LOGTEXT2( _L( "CAsynchFSQAtom::SaveCleanupL() notifying, iError=%d" ), iError );
    iOwner->Notify( iError );
        
    LOGTEXT( _L( "CAsynchFSQAtom::SaveCleanupL() exiting" ));
    }    
    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::UploadL
// -----------------------------------------------------------------------------
//
void CAsynchFSQAtom::UploadL()
    {
    LOGTEXT( _L( "CAsynchFSQAtom::UploadL() entering" ) );
    iError = KErrNone;
        
    if ( iState != EUploading )
        {
        // the state machine is lost
        LOGTEXT( _L( "CAsynchFSQAtom::UploadL() bad state, leaving..." ) );
        User::Leave( KErrGeneral );
        }
    
    // This activity is not yet supported, so just feign completion
    LOGTEXT( _L( "CAsynchFSQAtom::UploadL() iState => EComplete" ) );
    iState = EUploadPending;
    
    // re-initiate AO activity
    LOGTEXT( _L( "CAsynchFSQAtom::UploadL() setting active" ) );
    SetActive();
    TRequestStatus* statusPtr = &iStatus;
    User::RequestComplete( statusPtr, KErrNone );
    LOGTEXT( _L( "CAsynchFSQAtom::UploadL() request completed" ) );
    
    LOGTEXT( _L( "CAsynchFSQAtom::UploadL() exiting" ) );
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::UploadCleanup
// -----------------------------------------------------------------------------
//
void CAsynchFSQAtom::UploadCleanupL()
    {
    LOGTEXT( _L( "CAsynchFSQAtom::UploadCleanupL() entering" ) );
    
    if ( iState != ESaving )
        {
        // the state machine is lost
        LOGTEXT( _L( "CAsynchFSQAtom::UploadCleanupL() bad state, leaving..." ) );
        User::Leave( KErrGeneral );
        }
    
    // This activity is not yet supported, so just feign completion
    LOGTEXT( _L( "CAsynchFSQAtom::UploadCleanupL() iState => EComplete" ) );
    iState = EComplete;
    
    // report completion
    LOGTEXT2( _L( "CAsynchFSQAtom::UploadCleanupL() notifying, iError=%d" ), iError );
    iOwner->Notify( iError );
    
    LOGTEXT( _L( "CAsynchFSQAtom::UploadCleanupL() exiting" ) );
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::ActionsLeft
// -----------------------------------------------------------------------------
//
TInt CAsynchFSQAtom::ActionsLeft()
    {
    TInt actionsLeft = 0;
    
    // logically, this section is liable to change if more
    // action types are added to the class
    switch ( iState )
	    {
	    case EPending:
	        {
	        if ( iActionType == EFileSaveAndWebUpload )
	            {
	            actionsLeft = 2;
	            }
	        else
	            {
	            actionsLeft = 1;
	            }
	        break;
	        }
	    case ESaving:
	    case ESavePending:
	        {
	        if ( iActionType == EFileSave )
	            {
	            actionsLeft = 1;
	            }
	        else
	            {
	            actionsLeft = 2;
	            }
	        break;	        
	        }
	    case EUploading:
	    case EUploadPending:
	        {
	        actionsLeft = 1;
	        break;
	        }	        	        
	    case EComplete:
	        {
	        actionsLeft = 0;
	        break;
	        }	        	        
	    }	
    
    LOGTEXT2( _L( "CAsynchFSQAtom::ActionsLeft() returning, actionsLeft=%d" ), actionsLeft );
    return actionsLeft;
    }

    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::RunL
// -----------------------------------------------------------------------------
//
void CAsynchFSQAtom::RunL()
    {
    LOGTEXT( _L( "CAsynchFSQAtom::RunL() entering" ) );

    switch ( iState )
	    {
	    case ESaving:
	        {
	        SaveL();
	        break;	        
	        }
	    case ESavePending:
	        {
	        SaveCleanupL();
	        break;	        
	        }
	    case EUploading:
	        {
	        UploadL();
	        break;	        
	        }	        	        
	    case EUploadPending:
	        {
	        UploadCleanupL();
	        break;
	        }	        	        
	    default:
	        {
	        // the state machine is lost
	        LOGTEXT( _L( "CAsynchFSQAtom::RunL() bad state, leaving..." ) );
	        User::Leave( KErrGeneral );
	        break;        
	        }	        
	    }	

    LOGTEXT( _L( "CAsynchFSQAtom::RunL() exiting" ) );
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::RunError
// -----------------------------------------------------------------------------
//
TInt CAsynchFSQAtom::RunError( TInt aError )
    {
    LOGTEXT2( _L( "CAsynchFSQAtom::RunError() entering, aError=%d" ), aError );
    // notify and wait for teardown
    iState = EComplete;
    iOwner->Notify( aError );
    LOGTEXT( _L( "CAsynchFSQAtom::RunError() returning" ) );
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CAsynchFSQAtom::DoCancel
// -----------------------------------------------------------------------------
//
void CAsynchFSQAtom::DoCancel()
    {
    LOGTEXT( _L( "CAsynchFSQAtom::DoCancel() entering" ) );

    LOGTEXT( _L( "CAsynchFSQAtom::DoCancel() exit" ) );
    }
    

//  End of File  
