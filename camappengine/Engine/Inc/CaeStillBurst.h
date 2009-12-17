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
* Description:  Camera Application Engine still image burst class
*
*/



#ifndef CAESTILLBURST_H
#define CAESTILLBURST_H


// FORWARD DECLARATIONS
class HBufC8;
class CFbsBitmap;
class CCaeImageItem;
template <class T> class RPointerArray;

// CLASS DECLARATION

/**
* Camera application engine still image burst capturing class.
*
*/

NONSHARABLE_CLASS( CCaeStillBurst ) : public CBase
    {

    public: // Construction and destruction

        /**
        * Two-phased constructor.
        */
        static CCaeStillBurst* NewL();

        /**
        * Destructor.
        */
        virtual ~CCaeStillBurst();

    public: // Settings

        /**
        * Sets the desired length of the burst. That many captures should be done in the burst.
        * Note that the final burst length really captured may be different if 
        * there has been memory allocation problems or if the burst has been stopped.
        * @since 2.1
        * @param aLength The desired length of the burst
        * @return void
        */
        void SetLengthL( 
            TInt aLength );

        /**
        * Gets the length of the burst. That many captures will be done in the burst.
        * Note that the final burst length really captured may be different if 
        * there has been memory allocation problems or if the burst has been stopped.
        * @since 2.1
        * @return The desired length of the burst
        */
        TInt Length() const;

    public: // Burst handling

        /**
        * Appends an image item into the image queue.
        * @since 2.1
        * @param aBitmap Symbian OS bitmap image.
        * @param aImageData Other than Symbian OS bitmap image.
        * @param aError Error code related to the image.
        * @return Error code KErrNone, if the insertion is successful, 
        * otherwise one of the system wide error codes.
        */
        TInt AppendImage( 
            CFbsBitmap* aBitmap, 
            HBufC8*     aImageData, 
            TInt        aError );

        /**
        * Gets the count of image captures.
        * @since 2.1
        * @return The count of image captures
        */
        TInt CaptureCount() const;

        /**
        * Gets the count of image items in the image queue.
        * @since 2.1
        * @return The count of image items in the image queue
        */
        TInt ImageCount() const;
            
        /**
        * Gets the next image from image queue.
        * @since 2.1
        * @param aBitmap Symbian OS bitmap image.
        * @param aImageData Other than Symbian OS bitmap image.
        * @param aError Error code related to the image.
        * @return Error code KErrNone, if the operation is successful, 
        * otherwise KErrUnderflow (= no images in the queue).
        */
        TInt GetNextImage( 
            CFbsBitmap*& aBitmap, 
            HBufC8*&     aImageData, 
            TInt&        aError );

        /**
        * Returns Boolean indicating if the burst is fully captured or not.
        * @since 2.1
        * @return Boolean indicating if the burst is fully captured or not
        */
        TBool IsBurstCaptured() const;

        /**
        * Resets and destroys image items from the image queue.
        * @since 2.1
        * @return void
        */
        void ResetAndDestroyImages();

    private: // Private member methods

        /**
        * C++ constructor.
        */
        CCaeStillBurst();

        /**
        * Symbian OS 2nd phase constructor that can leave.
        */
        void ConstructL();

    private: // Data

        // Image queue for the images captured in burst.
        RPointerArray<CCaeImageItem>* iImageQueue;

		// Heap for image data in iImageQueue
		RHeap* iImageHeap; 

        // Desired burst length (number of images).
        TInt iDesiredLength;

        // That many (successfull) captures have been made.
        TInt iCountOfBurstAppends;

        // Additional delay between separate captures.
        TTimeIntervalMicroSeconds iCaptureInterval;
    };


#endif // CAESTILLBURST_H
