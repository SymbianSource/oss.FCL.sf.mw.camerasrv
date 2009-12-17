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
* Description:  Camera Application Engine still image quality level and container
*
*/



#ifndef CAESTILLQUALITYLEVELS_H
#define CAESTILLQUALITYLEVELS_H

//  INCLUDES
#include <ecam.h>
#include <e32base.h>
#include <e32std.h>
#include "CaeSdKeys.h"
#include "CaeQualityLevels.h"

// CONSTANTS

const TInt KCaeMaxStillQltyLevelCount = 6;

const TInt KCaeImgWidthQQVGA    = 160; 
const TInt KCaeImgHeightQQVGA   = 120; 

const TInt KCaeImgWidthVGA      = 640; 
const TInt KCaeImgHeightVGA     = 480; 


// FORWARD DECLARATIONS
class CStillQualityLevel;


// CLASS DECLARATION

/**
* Camera Application Engine still image quality levels container.
*
*/
NONSHARABLE_CLASS( CCaeStillQualityLevelsCont ) : public CCaeQualityLevelsCont
    {

    public: // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CCaeStillQualityLevelsCont();

        /**
        * Two-phased constructor.
        */
        static CCaeStillQualityLevelsCont* NewL();

        /**
        * Initializes quality levels to hardcoded default values.
        * @since 2.1
        * @return The count of quality levels initialized
        */
        TInt InitDefaultsL();
        
        /**
        * Gets the quality level count.
        * @since 2.1
        * @return Quality level count
        */
        TInt Count() const;

        /**
        * Gets the specified quality level.
        * @since 2.1
        * @param aIndex Quality level index
        * @return Still quality level
        */
        CStillQualityLevel& At( 
            TInt aIndex ) const;

        /**
        * Gets the Shared Data key of specified quality level.
        * @since 2.1
        * @param aIndex Quality level index
        * @return Shared Data key
        */
        const TDes& Key( 
            TInt aIndex ) const;

    private:

        /**
        * C++ constructor.
        */
        CCaeStillQualityLevelsCont();

        /**
        * Symbian OS 2nd phase constructor that can leave.
        */
        void ConstructL();

    private:

        /**
        * Resizes quality level array.
        * @since 2.1
        * @param aQualityLevelCount Quality level count for the new array
        * @return void
        */
        void ResizeL( 
            TInt aQualityLevelCount );

        /**
        * Gets the quality level count Shared Data key.
        * @since 2.1
        * @return Quality level count Shared Data key
        */
        const TDesC& QltyLevelCountSdKey() const;

    private: // For Shared Data support.

        /**
        * Creates quality level string for SharedData Camera ini file.
        * @since 2.1
        * @param aIndex Quality level index
        * @param aDefaultQualityLevel Output quality level string
        * @return void
        */
        void CreateQualityLevelString( 
            TInt  aIndex, 
            TDes& aDefaultQualityLevel );

        /**
        * Parses quality level string to quality level object data.
        * @since 2.1
        * @param aQualityLevelString Quality level string to parse
        * @param aIndex Quality level index
        * @return void
        */
        void ParseQualityLevelFromStringL( 
            const TDesC& qualityLevelString, 
            TInt         aIndex );

    private:
        
        // Supported still capture quality levels.
        CArrayFixFlat<CStillQualityLevel>* iQualityLevels;
    
        // Quality level count Shared Data key.
        const TDesC& iQltyLevelCountSdKey;    
    };



// CLASS DECLARATION

/**
* Camera Application Engine still image quality level.
*
*/
NONSHARABLE_CLASS( CStillQualityLevel ) : public CQualityLevel
    {
    public:
        
        /**
        * Constructor.
        */
        CStillQualityLevel();

        /**
        * Destructor.
        */
        virtual ~CStillQualityLevel();

    public:

        // Still image frame size.
        TSize iFrameSize;

        // Still image output format.
        CCamera::TFormat iOutputFormat;

        // Still image compression quality.
        TInt iCompressionQuality;

        // Estimated size of an image in bytes.
        TInt iByteSize;
    };

#endif // CAESTILLQUALITYLEVELS_H
