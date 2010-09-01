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
* Description:  Still Image Decoder for Camera Application Engine
*
*/



#ifndef CAESTILLDECODER_H
#define CAESTILLDECODER_H

// INCLUDES
#include <e32base.h>
#include <s32file.h>
#include <imageconversion.h>
#include <ecam.h>


// CLASS DECLARATION

/**
*  Still decoder observer class.
*  User of CCaeStillDecoder class must derive from this interface.
*
* @lib CAESTILLCONVERTER.LIB
* @since 2.1
*/
class MCaeStillDecoderObserver
    {
    public:
        /**
        * Called back by CCaeStillDecoder when it completes conversion (decoding) to CFbsBitmap image.
        * @since 2.1
        * @param aImageData Source image (as JPEG)
        * @param aBitmap Converted Symbian OS bitmap image
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code
        * @param aImageSize Decoded image size
        * @return void
        */
        virtual void McaesdoCFbsBitmapImageReady( 
            HBufC8* aImageData, 
            CFbsBitmap* aBitmap, 
            TInt aError,
            TInt aImageSize ) = 0;
    };


// CLASS DECLARATION

/**
* Still Decoder class.
* Implements still image decoding for Camera Application Engine 
* using Symbian Image Conversion Library (ICL)
*
* @lib CAESTILLCONVERTER.LIB
* @since 2.1
*/
NONSHARABLE_CLASS( CCaeStillDecoder ) : public CCaeStillConverter
    {
    public:  // Constructor and destructor    
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CCaeStillDecoder* NewL();

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CCaeStillDecoder();

    public: // From CCaeStillConverter:

        void SetImageCodecL( 
        	TUid aCodecUid );
        	
    public: // New functions
        /**
        * Sets Still Decoder observer.
        * @since 2.1
        * @param aObserver Still Decoder observer
        * @return void
        */
        IMPORT_C void SetObserver( 
            MCaeStillDecoderObserver* aObserver );

        /** 
        * Creates the CImageItem object, places it in the internal queue and 
        * starts the memory image conversion where the memory image (e.g. JPEG) 
        * is converted to bitmap. 
        * @param aImageBuffer Memory image to convert. Ownership is given.
        * @param aTargetBitmapMode Target/destination bitmap color mode
        * @param aTargetBitmapSize Target/destination bitmap size without free scaling decoder
        * @param aFullyScaledTargetBitmapSize Target/destination bitmap size with free scaling decoder
        * @return void
        */
        IMPORT_C void ConvertHBufC8ToCFbsBitmapL( 
            HBufC8* aImageBuffer,
            TDisplayMode aTargetBitmapMode,
            const TSize& aTargetBitmapSize,
            const TSize& aFullyScaledTargetBitmapSize = TSize( 0, 0 ) );

        /**
        * Destroys all allocations for decoding, as the images from the queue.
        * @return void
        */
        IMPORT_C void Cleanup();

    protected: // Constructors
        /**
        * C++ constructor.
        */
        CCaeStillDecoder();

        /**
        * Symbian 2nd phase constructor that can leave.
        */
        void ConstructL();

    private: // From CCaeStillConverter:
        
        /**
        * From CActive, implements cancellation of an outstanding request.
        * @return void
        */
        void DoCancel();

    protected: // From CCaeStillConverter
        
        void ConvertL();
   
        void ConversionComplete( 
            TInt aError );

    private: // Data

        // Still Decoder observer.
        MCaeStillDecoderObserver* iObserver;

        // Pointer to ICL decoder object.
        CImageDecoder* iDecoder;

        // Pointer to decoded image.
        CFbsBitmap* iDecodedImage;

        // Class wide file server handle for file operations
        RFs iFs;
    };


#endif // CAESTILLDECODER_H   


// End of File
