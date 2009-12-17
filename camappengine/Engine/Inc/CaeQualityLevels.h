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
* Description:  Camera Application Engine imaging quality level and container
*
*/



#ifndef CAEQUALITYLEVELS_H
#define CAEQUALITYLEVELS_H

// INCLUDES
#include <ecam.h>
#include <e32base.h>
#include <e32std.h>

// CONSTANTS
const TInt KCaeDefaultQltyLevelCount = 3;
const TInt KCaeMaxSdKeyLength = 10;
const TInt KCaeMaxQltyLevelStringLength = 256;

// FORWARD DECLARATIONS
class CQualityLevel;
class RSharedDataClient;

// CLASS DECLARATION

/**
* Camera Application Engine image/video quality levels container.
*
*/
NONSHARABLE_CLASS( CCaeQualityLevelsCont ) : public CBase
    {

    public: // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CCaeQualityLevelsCont();

        /**
        * Two-phased constructor.
        */
        static CCaeQualityLevelsCont* NewL();

        /**
        * Initialize quality levels from Shared Data ini-file.
        * @since 2.1
        * @param aSdUidQltyLevels Shared Data ini-file UID
        * @return Count of quality levels initialized
        */
        TInt InitFromSdL( 
            const TUid&  aSdUidQltyLevels );

        /**
        * Initializes quality levels to hardcoded default values.
        * @since 2.1
        * @return Count of quality levels initialized
        */
        virtual TInt InitDefaultsL() = 0;
        
        /**
        * Gets the quality level count.
        * @since 2.1
        * @return Quality level count
        */
        virtual TInt Count() const = 0;

        /**
        * Get the specified quality level Shared Data key.
        * @since 2.1
        * @param aIndex Quality level index
        * @return Quality level Shared Data key
        */
        virtual const TDes& Key( 
            TInt aIndex ) const = 0;

    protected:

        /**
        * C++ constructor.
        */
        CCaeQualityLevelsCont();

    private: // Private methods.

        /**
        * Fetches namebase setting from SharedData Camera ini file.
        * @since 2.1
        * @param aSdUidQltyLevels Shared Data ini-file UID
        * @param aIndex Quality level index
        * @return void
        */
        void FetchQualityLevelL(
            RSharedDataClient& aSharedData, 
            const TUid&        aSdUidQltyLevels, 
            TInt               aIndex );

    private: // Private pure virtual methods.

        /**
        * Resizes quality level array.
        * @since 2.1
        * @param aQualityLevelCount Quality level count for the new array
        * @return void
        */
        virtual void ResizeL( 
            TInt aQualityLevelCount ) = 0;

        /**
        * Gets the quality level count Shared Data key.
        * @since 2.1
        * @return Quality level count Shared Data key
        */
        virtual const TDesC& QltyLevelCountSdKey() const = 0;

        /**
        * Creates quality level string for SharedData Camera ini file.
        * @since 2.1
        * @param aIndex Quality level index
        * @param aDefaultQualityLevel Output quality level string
        * @return void
        */
        virtual void CreateQualityLevelString( 
            TInt  aIndex, 
            TDes& aDefaultQualityLevel ) = 0;

        /**
        * Parses quality level string to quality level object data.
        * @since 2.1
        * @param aQualityLevelString Quality level string to parse
        * @param aIndex Quality level index
        * @return void
        */
        virtual void ParseQualityLevelFromStringL( 
            const TDesC& qualityLevelString, 
            TInt         aIndex ) = 0;
    };



// CLASS DECLARATION

/**
* Camera Application Engine image/video quality level.
*
*/
NONSHARABLE_CLASS( CQualityLevel ) : public CBase
    {
    public:
        
        /**
        * Constructor.
        */
        CQualityLevel();

        /**
        * Destructor.
        */
        virtual ~CQualityLevel();

    public:

        // Shared Data ini-file key.
        TBuf<KCaeMaxSdKeyLength> iKey;

    };

#endif // CAEQUALITYLEVELS_H
