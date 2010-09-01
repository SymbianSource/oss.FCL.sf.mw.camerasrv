/*
* Copyright (c) 2003,2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active object for calling Camera Application Engine methods 
*                indirectly from callbacks
*
*/



#ifndef CAECALLBACKACTIVE_H
#define CAECALLBACKACTIVE_H

// INCLUDES
#include <e32base.h>
#include <e32std.h>

// FORWARD DECLARATIONS
class CCaeEngineImp;


// CLASS DECLARATION

/**
* CCaeCallbackActive
* Active object for calling Camera Application Engine methods 
* indirectly from callbacks 
*/
NONSHARABLE_CLASS( CCaeCallbackActive ) : public CActive
    {
    public: // Constructors and destructor
        
        /**
        * Symbian OS two-phased constructor.
        * @since 2.1
        * @param aCamAppEngine Reference to CCaeEngineImp instance
        */
        static CCaeCallbackActive* NewLC( 
            CCaeEngineImp& aCamAppEngine );

        /**
        * Symbian OS two-phased constructor.
        * @since 2.1
        * @param aCamAppEngine Reference to CCaeEngineImp instance
        */
        static CCaeCallbackActive* NewL( 
            CCaeEngineImp& aCamAppEngine );

        /**
        * Destructor.
        */
        virtual ~CCaeCallbackActive();

    protected: // Constructors and destructor
        
        /**
        * Symbian OS 2nd phase constructor that can leave.
        */
        void ConstructL();

        /**
        * C++ constructor.
        * @since 2.1
        * @param aCamAppEngine Reference to CCaeEngineImp instance
        */
        CCaeCallbackActive( 
            CCaeEngineImp& aCamAppEngine );

    public: // New methods

        /**
        * Switches camera module power on.
        * @since 2.1
        * @return void
        */
        void PowerOn();

    public: // Methods from CActive (was protected)
        
        /**
        * Calls Camera Application Engine operation.
        * @return void
        */
        void RunL();

    protected: // Methods from CActive

        /**
        * Cancels request.
        * @return void
        */
        void DoCancel();

    private:

        // Timer to implement delayed operation calls.
        RTimer iTimer;

        // Reference to Camera Application Engine.
        CCaeEngineImp& iCamAppEngine;

        // Requests available.
        enum TRequest
            {
            /** Default */
            ERequestDefault,
            /** Switch camera power on */
            ERequestPowerOn,
            };

        // Request.
        TRequest iRequest;

    };

#endif // CAECALLBACKACTIVE_H

// End of File
