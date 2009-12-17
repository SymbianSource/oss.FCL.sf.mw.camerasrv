/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Common definitions
*
*/



#ifndef CAECOMMON_H
#define CAECOMMON_H

//  INCLUDES

#include <e32std.h>

// CONSTANTS

enum TCaeExtensionInterfaceIndexes
	{
	KCaeExtProcessImageInterfaceIndex = 0,
	KCaeExtSettingsInterfaceIndex = 1
	};

// Supported extension interfaces
// The order of this array must match to TCaeExtensionInterfaceIndexes.
const TUid KCaeSupportedExtensionInterfaceUids[] = 
	{
		{ KCaeExtProcessImageInterfaceUidValue },
		{ KCaeExtSettingsInterfaceUidValue } ,
		{ NULL } // End mark
	};

// Supported callback interfaces
const TUid KCaeSupportedCallbackInterfaceUids[] = 
	{
		{ KCaeExtEngineInfoCallbackInterfaceUidValue },
		{ NULL } // End mark
	};

// Extension list granularity
const TInt TCaeExtensionInterfaceImplItemGranularity = 10;

// CLASS DECLARATIONS

/**
* Struct for the extension interface implementation array item.
*/

struct TCaeExtensionInterfaceImplItem
	{
    // Priority of the specific implementation
    TInt iInitialPriority;

    // Uid of the specific implementation
	TUid iImplUid;

    // Pointer to the specific implementation
	TAny* iImplPtr;

    // Set to ETrue when image processing is enabled
	TBool iIsActive;
	};

/**
* Struct for the extension interface implementation array item.
*/

struct TCaeExtensionInterfaceImplListItem
	{
	// Uid of the interface 
	TUid iInterfaceUid;

	// List of implementations
	RArray<TCaeExtensionInterfaceImplItem>* iImplementations;
	};

/**
* Struct for the custom interface implementation array item.
*/

struct TCaeCustomInterfaceImplItem
	{
    // Uid of the custom interface that is implemented by the extension 
	TUid iInterfaceUid;

    // Pointer to the implementation
	TAny* iImplPtr;
	};



#endif // CAECOMMON_H