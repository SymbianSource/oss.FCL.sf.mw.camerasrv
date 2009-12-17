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



#ifndef ASYNCHFSQ_H
#define ASYNCHFSQ_H

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

// Type of action the FSQ will perform
enum TFSQActionType
    {
    EFileSave,
    EWebUpload,
    EFileSaveAndWebUpload
    };
    
// Web service schema definition
enum TFSQSchemaType
    {
    ENullSchemaType,
    ENokiaPictureUploadSchema1v0
    };    


// FORWARD DECLARATIONS
class MAsynchFSQObserver;
class CAsynchFSQAtom;


// CLASS DECLARATIONS

/**
* CAsynchFSQ observer class.
* Mixin base class for the clients of the asynchronous file manager.
*
* @since 3.2
*/
class MAsynchFSQObserver
    {
    public:

        /**
        * Callback function which gets called after each thread event.
        * @return void
        */
        virtual void MAsynchFSQEvent( TInt aError ) = 0;
        
    };
    
/**
* Asynchronous file manager queue and related operations.
*
* @since 3.2
*/
class CAsynchFSQ : CBase 
    {

    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CAsynchFSQ* NewL();
                 
        /**
        * Destructor.
        */
        virtual ~CAsynchFSQ();

    public: // New functions
        
        /**
        * Starts async copy operation
        */
        IMPORT_C TInt SaveAndDestroy( TDesC8& aData, const TDesC& aPath,
                                      TBool aOverwrite = ETrue );

        /**
        * Notifies the queue that the client wishes to start the thread manually
        */ 
        IMPORT_C void SetManualStart( TBool aManualStart );

        /**
        * Starts the saving thread
        *   If iManualStart is not activated, this is essentially a NO-OP
        */ 
        IMPORT_C TInt Go();
                            
        /**
        * Deletes a file from the file system
        */ 
        IMPORT_C TInt Delete( const TDesC& aPath );
        
        /**
        * Renames a file in the file system
        */
        IMPORT_C TInt Rename( const TDesC& aOld, const TDesC& aNew );

        /**
        * Sets the thread priority
        */
        IMPORT_C void SetPriority( TInt aPriority );
        
        /**
        * Sets the callback
        */
        IMPORT_C void SetCallback( MAsynchFSQObserver* aCallback );
        
        /**
        * Returns the number of actions left for the queue to perform
        */
        IMPORT_C TInt ActionsLeft();
        
        /**
        * Cancels any ongoing activity and clears the threads.
        */
        IMPORT_C void Cancel();
        
        /**
        * Used to indicate a state change from a data atom
        */
        void Notify( TInt aError );
        
   private: 
   
        /**
        * C++ default constructor.
        */
        CAsynchFSQ();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Adds one index to the end of each queue.
        */
        TInt Enqueue( TFSQActionType aActionType, TDesC8& aData, 
                      const TDesC& aPath, const TDesC& aURL, 
                      TFSQSchemaType aSchema, const TDesC& aUserName, 
                      const TDesC& aPassword );
        
    private:    // Data
    
        // Data queues
        RPointerArray<CAsynchFSQAtom> iQueue;
        
        // Is overwriting allowed?
        TBool iOverwrite;
        
        // Does the client start the activity?
        TBool iManualStart;
        
        // Most recent callback
        MAsynchFSQObserver* iCallback;
        
        // Performance modifiers
        TInt iPriority;
        
    };

#endif // CAsynchFSQ_H
