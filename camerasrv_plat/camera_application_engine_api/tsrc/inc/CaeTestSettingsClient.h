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
* Description: CaeTestSettingsClient.h
*
*/
#ifndef CAE_TEST_SETTINGS_CLIENT_H
#define CAE_TEST_SETTINGS_CLIENT_H

#include <e32base.h>
#include "CaeTestClient.h"

class CCaeTestSettingsClient : public CCaeTestClient
    {
    public:
        static CCaeTestSettingsClient* NewL();
        ~CCaeTestSettingsClient();

    public:
        void RunTestActionL( TestClientActions aAction );

    protected:
	    CCaeTestSettingsClient();
        void ConstructL();

    private:
        void RunTrappedL();
    };

#endif // CAE_TEST_SETTINGS_CLIENT_H
