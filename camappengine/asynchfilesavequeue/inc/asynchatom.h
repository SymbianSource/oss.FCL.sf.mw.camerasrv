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
* Description:  Asynchronous FSQ Atom Class
*
*/



#ifndef ASYNCHATOM_H
#define ASYNCHATOM_H

//  INCLUDES
#include <f32file.h>
#include <e32base.h>


//   FOR DEBUGGING
#if defined (_DEBUG)
    #include <e32svr.h>
    #define LOGTEXT(AAA)                RDebug::Print(AAA)
    #define LOGTEXT2(AAA,BBB)           RDebug::Print(AAA,BBB)
    #define LOGTEXT3(AAA,BBB,CC)        RDebug::Print(AAA,BBB,CC)
#else
    #define LOGTEXT(AAA)                
    #define LOGTEXT2(AAA,BBB)           
    #define LOGTEXT3(AAA,BBB,CC)       
#endif           


// CONSTANTS

enum TFSQAtomState
    {
    EPending,
    ESaving,
    ESavePending,
    EUploading,
    EUploadPending,
    EComplete
    };

// FORWARD DECLARATIONS
class CAsynchFSQ;

// CLASS DECLARATIONS

/**
* Asynchronous file manager queue and related operations.
*
* @since 3.2
*/
class CAsynchFSQAtom : public CActive
    {
    
    public: // Constructor and Descructor
    
        /**
        * Two-phased constructor.
        */
        static CAsynchFSQAtom* NewL(  CAsynchFSQ* aOwner, 
                                      TInt aPriority,
                                      TFSQActionType aActionType,  
                                      TDesC8& aData, 
                                      const TDesC& aPath, 
                                      const TDesC& aURL, 
                                      TFSQSchemaType aSchema, 
                                      const TDesC& aUserName, 
                                      const TDesC& aPassword  );
    
        /**
        * Two-phased constructor.
        */
        virtual ~CAsynchFSQAtom();            
                
    public: // Data
                

        
    public: // Public member functions
    
        /**
        * C++ default constructor.
        */
        CAsynchFSQAtom();
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(  CAsynchFSQ* aOwner, 
                          TInt aPriority,
                          TFSQActionType aActionType,  
                          TDesC8& aData, 
                          const TDesC& aPath, 
                          const TDesC& aURL, 
                          TFSQSchemaType aSchema, 
                          const TDesC& aUserName, 
                          const TDesC& aPassword  );
        
        /**
        * 
        */
        TInt Go();
        
        /**
        * 
        */
        TInt ActionsLeft();
        
        /**
        *
        */
        TFSQAtomState GetState();
        
        /**
        *
        */
        TBool DoesLocalSave();
        
        /**
        *
        */
        const TDesC& GetPath() const;
        
        /**
        *
        */
        TInt DeleteLocal();
        
        /**
        *
        */
        TInt RenameLocal( const TDesC& aNew );
        
    private: // Private member functions
    
        /**
        * From CActive
        */
        void RunL();

        /** 
        * From CActive
        */
        void DoCancel();        

        /** 
        * From CActive
        */
        TInt RunError( TInt aError );

        /**
        * 
        */
        void SaveL();
        
        /**
        * 
        */
        void SaveCleanupL();
        
        /**
        * 
        */
        void UploadL();
        
        /**
        * 
        */
        void UploadCleanupL();
        
    private: // Private data members
    
        // Activity specific items
        TFSQActionType iActionType;
        TDesC8* iData;
        HBufC* iPath;           // EFileSave only
        HBufC* iURL;            // EWebUpload only
        TFSQSchemaType iSchema; // EWebUpload only
        HBufC* iUserName;       // EWebUpload only
        HBufC* iPassword;       // EWebUpload only
        
        // Variables specifics to owning class
        CAsynchFSQ* iOwner;
        TInt iError;
        TBool iOverwrite;
        
        // State machine variable
        TFSQAtomState iState;
        
        // Helper variables
        RFile iFile;
        RFs iFs;
        TBool iDelayedLocalDelete;
        TBool iDelayedLocalRename;
        HBufC* iDelayedFileName;
        
    };

#endif // CAsynchATOM_H
