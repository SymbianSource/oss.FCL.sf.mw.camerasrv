/*
* Copyright (c) 2003-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Camera Application Engine still image queue for extension processing
*
*/



#ifndef CAEIMAGEQUEUEEXTPRO_H
#define CAEIMAGEQUEUEEXTPRO_H


// FORWARD DECLARATIONS
class HBufC8;
class CFbsBitmap;
class CCaeImageItemExtPro;
template <class T> class RPointerArray;

// CLASS DECLARATION

/**
* Camera Application Engine still image queue for extension processing class.
*
*/

NONSHARABLE_CLASS( CCaeImageQueueExtPro ) : public CBase
    {

    public: // Construction and destruction

        /**
        * Two-phased constructor.
        */
        static CCaeImageQueueExtPro* NewL();

        /**
        * Destructor.
        */
        virtual ~CCaeImageQueueExtPro();

    public:

        /**
        * Appends an image item into the image queue.
        * @since 3.2
        * @param aBitmap Symbian OS bitmap snap image or captured bitmap image.
        * @param aImageData Other than Symbian OS bitmap image.
        * @param aLastImage Set when last image of the burst is completed.
        * @param aSnapImage Set when aBitmap is snap image.
        * @return Error code KErrNone, if the insertion is successful, 
        * otherwise one of the system wide error codes.
        */
        TInt AppendImage( 
            CFbsBitmap* aBitmap, 
            HBufC8*     aImageData,
            TBool       aLastImage,
            TBool       aSnapImage );

        /**
        * Gets the count of image items in the image queue.
        * @since 3.2
        * @return The count of image items in the image queue
        */
        TInt ImageCount() const;
            
        /**
        * Gets the next image from image queue.
        * @since 3.2
        * @param aBitmap Symbian OS bitmap image.
        * @param aImageData Other than Symbian OS bitmap image.
        * @param aLastImage Set when last image of the burst is completed.
        * @param aSnapImage Set when aBitmap is snap image.
        * @return Error code KErrNone, if the operation is successful, 
        * otherwise KErrUnderflow (= no images in the queue).
        */
        TInt GetNextImage( 
            CFbsBitmap*& aBitmap, 
            HBufC8*&     aImageData,
            TBool&       aLastImage,
            TBool&       aSnapImage );

        /**
        * Resets and destroys image items from the image queue.
        * @since 3.2
        * @return void
        */
        void ResetAndDestroyImages();

    private: // Private member methods

        /**
        * C++ constructor.
        */
        CCaeImageQueueExtPro();

        /**
        * Symbian OS 2nd phase constructor that can leave.
        */
        void ConstructL();

    private: // Data

        // Image queue for the images captured in burst.
        RPointerArray<CCaeImageItemExtPro>* iImageQueue;

    };


#endif // CAEIMAGEQUEUEEXTPRO_H
