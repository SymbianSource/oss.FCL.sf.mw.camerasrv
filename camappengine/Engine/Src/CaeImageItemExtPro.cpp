/*
* Copyright (c) 2003-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Still Image Item Extension Processing for Camera Application Engine
*
*/


// INCLUDE FILES
#include "CaeImageItemExtPro.h"             // Still image item class.
#include <fbs.h>                            // For CFbsBitmap


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaeImageItemExtPro::CCaeImageItemExtPro
// -----------------------------------------------------------------------------
//
CCaeImageItemExtPro::CCaeImageItemExtPro() 
    {
    }


// -----------------------------------------------------------------------------
// CCaeImageItemExtPro::CCaeImageItemExtPro
// Destroys image data.
// -----------------------------------------------------------------------------
//
CCaeImageItemExtPro::~CCaeImageItemExtPro()
    {
    delete iBitmap;
    delete iImageData;
    }

//  End of File  
