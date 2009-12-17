/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: CaeTestStatesClient.h
*
*/
#ifndef CAE_TEST_STATES_CLIENT_H
#define CAE_TEST_STATES_CLIENT_H

#include <e32base.h>
#include "CaeTestClient.h"

class CCaeTestStatesClient : public CCaeTestClient
    {
    
    public:
        static CCaeTestStatesClient* NewL();
        ~CCaeTestStatesClient();

    public:
        void RunTestActionL( TestClientActions aAction );

    protected:
	    CCaeTestStatesClient();
        void ConstructL();

    private:
        void RunTrappedL();
    };

#endif // CAE_TEST_STATES_CLIENT_H
