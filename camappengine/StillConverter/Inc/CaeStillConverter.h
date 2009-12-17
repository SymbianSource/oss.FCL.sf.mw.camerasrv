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
* Description:  Still Image Converter base class for Camera Application Engine
*
*/



#ifndef CAESTILLCONVERTER_H
#define CAESTILLCONVERTER_H

// INCLUDES
#include <imageconversion.h>

// CONSTANTS

const TUid KUidSpecialFreeScalingDecoder = { 0x101FF555 }; // This decoder supports free scaling

// CLASS DECLARATION

/**
* Still Converter class.
* Base class for still image conversions by the Camera Application Engine.
*/
NONSHARABLE_CLASS( CCaeStillConverter ) : public CActive
    {
    
    public: 
        
        /**
        * Destructor.
        */
        virtual ~CCaeStillConverter();

    protected:

        /**
        * Internal state.
        */
        enum 
            {
            EIdle,
            EConvert
            };

    protected:  // Functions from base classes
        /**
        * From CActive, handles an active object’s request completion event.
        * @return void
        */
        void RunL();

        /**
        * From CActive, called if CCaeStillConverter function RunL() leaves.
        * @param aError Standard Symbian OS error code
        * @return Error code KErrNone
        */
        TInt RunError( 
            TInt aError );

    protected:
        /**
        * C++ constructor.
        */
        CCaeStillConverter();

        /**
        * Symbian 2nd phase constructor that can leave.
        * This is intended to be called from derived class ConstructL().
        */
        void ConstructL();

    protected:
        /**
        * Returns the engine state.
        * @return Boolean indicating if Still Converter is busy
        */
        TBool IsBusy() const;

    protected: // To be implemented in a derived class.
        /**
        * Converts (encodes or decodes) the image.
        * @return void
        */
        virtual void ConvertL() = 0;
   
        /**
        * Perfoms necessary cleanup and delivers the result to the client.
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code
        * @return void
        */
        virtual void ConversionComplete( 
            TInt aError ) = 0;

        /** 
        * Sets the specific image codec implementation to be used in decoding and encoding.
        * @since 3.1
        * @param aCodecUid The UID of the specific image codec to be used.
        * @return void
        */
        virtual void SetImageCodecL( 
        	TUid aCodecUid ) = 0;
        	
    protected:  // Data

        // Class for image data.
        class CImageItem: public CBase
            {
            public:
                CImageItem() {};
                virtual ~CImageItem() 
                    {
                    delete iBitmap;
                    delete iImageBuffer;
                    delete iFrameImageData;
                    };
            public:
                // Bitmap.
                CFbsBitmap* iBitmap;
                // Bitmap target size.
                TSize iBitmapSize;
                // Bitmap target size for free scaling.
                TSize iFullyScaledBitmapSize;
               // Bitmap display mode.
                TDisplayMode iBitmapDisplayMode;
                // Image in some common image file format (like Jpeg)
                HBufC8* iImageBuffer;
                 // Info for encoder
                CFrameImageData* iFrameImageData;
                // UID of conversion target image type.
                TUid iImageTypeUid;
            };

        // Specific image codec UID.
        TUid iImageCodecUid;

        // Pointer to array of CImageItem.
        RPointerArray<CImageItem>* iImageQueue;

        // Still Converter state.
        TInt iState;
        
    };


#endif // CAESTILLCONVERTER_H

// End of File
