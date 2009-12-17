/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Camera Application Engine Extension creation proxy
*
*/



// INCLUDE FILES

#include "CaeEngineExtInterface.h"                   // Engine implementation header.

// ============================ MEMBER FUNCTIONS ===============================

// class MExtension

// -----------------------------------------------------------------------------
// MExtension::NewExtensionL
// Create and construct the extension object via ECom.
// -----------------------------------------------------------------------------
// 
MExtension* MExtension::NewExtensionL( TUid aImplementationUid, MExtensionCallbackInterface* aEngine  )
	{
	TAny* extInterface = REComSession::CreateImplementationL( 
		aImplementationUid, _FOFF (MExtension, iDtor_ID_Key), (TAny*) aEngine);
	return reinterpret_cast <MExtension*> (extInterface);
	}


//  End of File  
