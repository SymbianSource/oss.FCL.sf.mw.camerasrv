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
* Description: CaeTestTop.h
*
*/
#ifndef CAE_TEST_TOP_H
#define CAE_TEST_TOP_H

#include <e32base.h>
#include "CaeTestVariation.h"

EXPORT_C  MTest* CreateTopFct();

class CCaeTestTop : public CTestSuite
    {
    public:
	    CCaeTestTop();
        ~CCaeTestTop();

	    // A function to collect and return a suite of tests
	    static MTest* suiteL();

    protected:

    private:

    };

#endif // CAE_TEST_TOP_H
