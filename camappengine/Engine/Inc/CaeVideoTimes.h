/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Camera Application Engine video times provider class
*
*/



#ifndef CAEVIDEOTIMES_H
#define CAEVIDEOTIMES_H

// INCLUDES
#include <e32base.h>


// CLASS DECLARATION

/**
* Video recording times generator class.
* Generates elapsed and remaining recording time info periodically.
*/
NONSHARABLE_CLASS( CCaeVideoTimes ) : public CPeriodic
    {

    public:  // Constructors and destructor    
        
        /**
        * Two-phased constructor.
        */
        static CCaeVideoTimes* NewL();

        /**
        * Destructor.
        */
        virtual ~CCaeVideoTimes();

    private: // From CPeriodic base class CActive.

        /**
        * From CActive, called if CCaeVideoTimes function RunL() leaves.
        * @param aError Standard Symbian OS error code
        * @return Error code KErrNone
        */
        TInt RunError( 
            TInt aError );

    private: // Private methods.
        
        /**
        * C++ constructor.
        */
        CCaeVideoTimes();

        /**
        * Symbian OS 2nd phase constructor that can leave.
        */
        void ConstructL();

    };

#endif // CAEVIDEOTIMES_H   


// End of File
