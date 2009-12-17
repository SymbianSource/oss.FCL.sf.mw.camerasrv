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
* Description:  Still Image Item for Camera Application Engine for extension processing
*
*/



#ifndef CAEIMAGEITEMEXTPRO_H
#define CAEIMAGEITEMEXTPRO_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class HBufC8;
class CFbsBitmap;

// CLASS DECLARATION

// An image item class. Can be used in image queues.
NONSHARABLE_CLASS( CCaeImageItemExtPro ) : public CBase
    {
    
    public:
        /**
        * Default constructor.
        */
        CCaeImageItemExtPro();
         
        /**
        * Destructor.
        */
        virtual ~CCaeImageItemExtPro();
        
    public:
        // Bitmap pointer for snap or captured image.
        CFbsBitmap* iBitmap;
        
        // Image pointer for main captured image.
        HBufC8* iImageData;

        // Flag for last burst image
        TBool iLastImage;

        // Flag for indicating if the iBitmap is snap or captured image
        TBool iSnapImage;
    };



#endif // CAEIMAGEITEMEXTPRO_H   

// End of File
