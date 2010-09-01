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
* Description:  Camera Application Engine video quality level and container
*
*/



#ifndef CAEVIDEOQUALITYLEVELS_H
#define CAEVIDEOQUALITYLEVELS_H

//  INCLUDES
#include <ecam.h>
#include <e32base.h>
#include <e32std.h>
#include "CaeSdKeys.h"
#include "CaeQualityLevels.h"

// CONSTANTS

const TInt KCaeMaxVideoStringParLength = 32;
const TInt KCaeMaxAudioTypeStringParLength = 4;

const TInt KCaeMaxVideoQltyLevelCount = 6;

const TInt KImgWidthSubQCIF  = 128; 
const TInt KImgHeightSubQCIF = 96; 

const TInt KImgWidthQCIF     = 176; 
const TInt KImgHeightQCIF    = 144; 

const TInt KImgWidthCIF      = 352; 
const TInt KImgHeightCIF     = 288; 

_LIT8(KVideoMimeType, "video/3gpp");

_LIT(KPreferredSupplier, "Nokia");

_LIT8(KVideoType, "video/H263-2000");

_LIT8(KAudioType, " AMR");


// FORWARD DECLARATIONS
class CVideoQualityLevel;


// CLASS DECLARATION

/**
* Camera Application Engine video recording quality levels container.
*
*/
NONSHARABLE_CLASS( CCaeVideoQualityLevelsCont ) : public CCaeQualityLevelsCont
    {

    public: // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CCaeVideoQualityLevelsCont();

        /**
        * Two-phased constructor.
        */
        static CCaeVideoQualityLevelsCont* NewL();

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
        * @return Video quality level
        */
        CVideoQualityLevel& At( 
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
        CCaeVideoQualityLevelsCont();

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
            const TDesC& aQualityLevelString, 
            TInt         aIndex );

    private:
    
        // Supported video recording quality levels.
        CArrayFixFlat<CVideoQualityLevel>* iQualityLevels;

        // Quality level count Shared Data key.
        const TDesC& iQltyLevelCountSdKey;    
    };



// CLASS DECLARATION

/**
* Camera Application Engine video recording quality level.
*
*/
NONSHARABLE_CLASS( CVideoQualityLevel ) : public CQualityLevel
    {
    public:
        
        /**
        * Constructor.
        */
        CVideoQualityLevel();

        /**
        * Destructor.
        */
        virtual ~CVideoQualityLevel();

    public:

        // The video recording frame size.
        TSize iFrameSize;

        // The video recording frame rate.
        TReal32 iFrameRate;

        // The video recording bit rate.
        TInt iBitRate;

        // A Boolean indicating if video recording audio is enabled or not.
        TBool iAudioEnabled;

        // The video recording audio bit rate.
        TInt iAudioBitRate;

        // The video MIME type (e.g. "video/3gpp").
        TBuf8<KCaeMaxVideoStringParLength> iMimeType;

        // The preferred supplier of video controller (e.g. "Company X").
        TBuf<KCaeMaxVideoStringParLength> iPreferredSupplier;

        // The video type (e.g. "video/H263-2000").
        TBuf8<KCaeMaxVideoStringParLength> iVideoType;

        // The audio type expressed with four characters (e.g. " AMR").
        TBuf8<KCaeMaxAudioTypeStringParLength> iAudioType;

        // The estimated video recording bit rate to storage (bits per second).
        TInt iStorageRate;
    };


#endif // CAEVIDEOQUALITYLEVELS_H
