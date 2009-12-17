/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Still Image Item for Camera Application Engine
*
*/


// INCLUDE FILES
#include "CaeImageItem.h"                   // Still image item class.
#include <fbs.h>                            // For CFbsBitmap


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaeImageItem::CCaeImageItem
// -----------------------------------------------------------------------------
//
CCaeImageItem::CCaeImageItem( RHeap& aImageHeap ) 
	: iImageHeap( aImageHeap ) 
    {
    }


// -----------------------------------------------------------------------------
// CCaeImageItem::~CCaeImageItem
// Destroys image queue.
// -----------------------------------------------------------------------------
//
CCaeImageItem::~CCaeImageItem()
    {
    delete iBitmap;
    iImageHeap.Free(iImageData);
    }

//  End of File  
