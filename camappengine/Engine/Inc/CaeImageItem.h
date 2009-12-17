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



#ifndef CAEIMAGEITEM_H
#define CAEIMAGEITEM_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class HBufC8;
class CFbsBitmap;

// CLASS DECLARATION

// An image item class. Can be used in image queues.
NONSHARABLE_CLASS( CCaeImageItem ) : public CBase
    {
    
    public:
        /**
        * Default constructor.
        */
        CCaeImageItem( RHeap& aImageHeap );
         
        /**
        * Destructor.
        */
        virtual ~CCaeImageItem();
        
    public:
        // Bitmap pointer.
        CFbsBitmap* iBitmap;
        // Image pointer (not in bitmap format).
        TAny* iImageData;
        // Size of iImageData
        TInt iImageDataSize;
		// Heap for iImageData
		RHeap& iImageHeap; 
        // Possible error related to the image (from capturing).
        TInt iError;
        
    };



#endif // CAEIMAGEITEM_H   

// End of File
