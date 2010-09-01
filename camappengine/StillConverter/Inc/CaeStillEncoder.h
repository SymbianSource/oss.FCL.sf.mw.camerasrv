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
* Description:  Still Image Encoder for Camera Application Engine
*
*/



#ifndef CAESTILLENCODER_H
#define CAESTILLENCODER_H

// INCLUDES
#include <e32base.h>
#include <s32file.h>
#include <imageconversion.h>
#include "CaeStillConverter.h"

// CONSTANTS
const TInt KJpegQualityMin = 0;
const TInt KJpegQualityMax = 100;

const TJpegImageData::TColorSampling KCaeJpegColorSampling = TJpegImageData::EColor420;


// CLASS DECLARATION

/**
*  Still encoder observer class.
*  User of CCaeStillEncoder class must derive from this interface.
*
* @lib CAESTILLCONVERTER.LIB
* @since 2.1
*/
class MCaeStillEncoderObserver
    {
    public:
        /**
        * Called back by CCaeStillEncoder when it completes conversion (encoding) to HBufC8 image data (as JPEG).
        * single media item during media items enumeration.
        * @since 2.1
        * @param aBitmap Source Symbian OS bitmap image
        * @param aImageData Converted image (as JPEG)
        * @param aError Error code KErrNone to indicate success or a standard Symbian OS error code
        * @param aImageSize Encoded image size
        * @return void
        */
        virtual void McaeseoHBufC8ImageReady(
            CFbsBitmap* aBitmap,
            HBufC8* aImageData, 
            TInt aError,
            TInt aImageSize ) = 0;
    };


// CLASS DECLARATION

/**
* Still Encoder class. Derived from CaeStillConverter. 
* Implements still image encoding for Camera Application Engine 
* using Symbian Image Conversion Library (ICL).
*
* @lib CAESTILLCONVERTER.LIB
* @since 2.1
*/
NONSHARABLE_CLASS( CCaeStillEncoder ) : public CCaeStillConverter
    {
    public: // Conversion target image type.

        enum TTargetFormat
            {
            ETargetFormatJPEG
            };

    public: // Constructor and destructor    
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CCaeStillEncoder* NewL();

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CCaeStillEncoder();

    public: // From CCaeStillConverter:

        void SetImageCodecL( 
        	TUid aCodecUid );
        	
    public: // New functions
        /**
        * Sets Still Encoder observer.
        * @since 2.1
        * @param aObserver Still Encoder observer
        * @return void
        */
        IMPORT_C void SetObserver( 
            MCaeStillEncoderObserver* aObserver );

        /**
        * Sets compression quality.
        * @since 2.1
        * @param aQuality Compression quality
        * @return void
        */
        IMPORT_C void SetCompressionQuality( 
            TInt aQuality );

        /**
        * Gets compression quality.
        * @since 2.1
        * @return Compression quality
        */
        IMPORT_C TInt CompressionQuality() const;

        /** 
        * Creates the CImageItem object, places it in the internal queue and 
        * starts the memory image conversion where the memory bitmap is 
        * converted to another format (e.g. JPEG). 
        * @since 2.1
        * @param aBitmap Bitmap to convert
        * @param aTargetFormat Target/destination image format (JPEG by default)
        * @return void
        */
        IMPORT_C void ConvertCFbsBitmapToHBufC8L( 
            CFbsBitmap* aBitmap,
            TTargetFormat aTargetFormat = ETargetFormatJPEG );

        /**
        * Destroys all allocations for encoding, as the images from the queue.
        * @return void
        */
        IMPORT_C void Cleanup();

    private: // Constructors
        /**
        * C++ constructor.
        */
        CCaeStillEncoder();

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

    private: // From CCaeStillConverter:
        
        void ConvertL();
   
        void ConversionComplete( 
            TInt aError );

    private: // Data

        // Still Encoder observer.
        MCaeStillEncoderObserver* iObserver;

        // Pointer to ICL encoder object.
        CImageEncoder* iEncoder;

        // Pointer to encoded image.
        HBufC8* iEncodedImage;

        // Quality factor for JPEG images.
        TInt iJpegQuality;
    };


#endif // CAESTILLENCODER_H


// End of File
