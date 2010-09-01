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



// INCLUDE FILES

#include "CaeStillQualityLevels.h"

#include "CaeEngineImp.h"       // For LOGTEXT


// CONSTANTS
const TInt KQualityLevelArrayGranularity = 3;



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaeStillQualityLevelsCont::CCaeStillQualityLevelsCont
// Constructor.
// -----------------------------------------------------------------------------
//
CCaeStillQualityLevelsCont::CCaeStillQualityLevelsCont()
: iQltyLevelCountSdKey( KCaeStillQltyLevelCount )
    {
    }


// -----------------------------------------------------------------------------
// CCaeStillQualityLevelsCont::~CCaeStillQualityLevelsCont
// Destructor.
// -----------------------------------------------------------------------------
//
CCaeStillQualityLevelsCont::~CCaeStillQualityLevelsCont()
    {
    delete iQualityLevels;
    }


// -----------------------------------------------------------------------------
// CCaeStillQualityLevelsCont::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCaeStillQualityLevelsCont::ConstructL()
    {
    LOGTEXT( _L( "Cae: CCaeStillQualityLevelsCont::ConstructL() entering" ) );

    iQualityLevels = new( ELeave ) CArrayFixFlat<CStillQualityLevel>( KQualityLevelArrayGranularity );

    LOGTEXT( _L( "Cae: CCaeStillQualityLevelsCont::ConstructL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeStillQualityLevelsCont::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaeStillQualityLevelsCont* CCaeStillQualityLevelsCont::NewL()
    {
    LOGTEXT( _L( "Cae: CCaeStillQualityLevelsCont::NewL() entering" ) );

    CCaeStillQualityLevelsCont* self = new( ELeave ) CCaeStillQualityLevelsCont;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOGTEXT( _L( "Cae: CCaeStillQualityLevelsCont::NewL() returning" ) );

    return self;
    }

    
// -----------------------------------------------------------------------------
// CCaeStillQualityLevelsCont::InitDefaultsL
// Sets hardcoded default parameters for default still quality levels that 
// are created.
// -----------------------------------------------------------------------------
//
TInt CCaeStillQualityLevelsCont::InitDefaultsL()
    {
    LOGTEXT( _L( "Cae: CCaeStillQualityLevelsCont::InitDefaultsL() entering" ) );

    // Delete all elements from the array and 
    // free the memory allocated to the array buffer.
    iQualityLevels->Reset();

    // Initialize hardcoded default quality levels.

    TSize size;

    CStillQualityLevel* stillQualityLevel = new( ELeave ) CStillQualityLevel;
    CleanupStack::PushL( stillQualityLevel );

    // Level 0:
    size.SetSize( KCaeImgWidthVGA, KCaeImgHeightVGA );
    stillQualityLevel->iFrameSize = size;
    stillQualityLevel->iOutputFormat = CCamera::EFormatJpeg;
    stillQualityLevel->iCompressionQuality = 90; // From range [0,100] [worst,best]
    stillQualityLevel->iByteSize = 90000; // Estimate only for a typical size in bytes.
    stillQualityLevel->iKey = KCaeStillQltyLevel0;
    iQualityLevels->AppendL( *stillQualityLevel );

    // Level 1:
    size.SetSize( KCaeImgWidthVGA, KCaeImgHeightVGA );
    stillQualityLevel->iFrameSize = size;
    stillQualityLevel->iOutputFormat = CCamera::EFormatJpeg;
    stillQualityLevel->iCompressionQuality = 50; // From range [0,100] [worst,best]
    stillQualityLevel->iByteSize = 35000; // Estimate only for a typical size in bytes.
    stillQualityLevel->iKey = KCaeStillQltyLevel1;
    iQualityLevels->AppendL( *stillQualityLevel );

    // Level 2:
    size.SetSize( KCaeImgWidthQQVGA, KCaeImgHeightQQVGA );
    stillQualityLevel->iFrameSize = size;
    stillQualityLevel->iOutputFormat = CCamera::EFormatJpeg;
    stillQualityLevel->iCompressionQuality = 90; // From range [0,100] [worst,best]
    stillQualityLevel->iByteSize = 12000; // Estimate only for a typical size in bytes.
    stillQualityLevel->iKey = KCaeStillQltyLevel2;
    iQualityLevels->AppendL( *stillQualityLevel );

    CleanupStack::PopAndDestroy( stillQualityLevel );
    
    LOGTEXT( _L( "Cae: CCaeStillQualityLevelsCont::InitDefaultsL() returning" ) );

    return iQualityLevels->Count();
    }


// -----------------------------------------------------------------------------
// CCaeStillQualityLevelsCont::Count
// -----------------------------------------------------------------------------
//
TInt CCaeStillQualityLevelsCont::Count() const
    {
    return iQualityLevels->Count();
    }

        
// -----------------------------------------------------------------------------
// CCaeStillQualityLevelsCont::At
// -----------------------------------------------------------------------------
//
CStillQualityLevel& CCaeStillQualityLevelsCont::At( 
    TInt aIndex ) const 
    {
    return iQualityLevels->At( aIndex );
    }


// -----------------------------------------------------------------------------
// CCaeStillQualityLevelsCont::Key
// -----------------------------------------------------------------------------
//
const TDes& CCaeStillQualityLevelsCont::Key( 
    TInt aIndex ) const
    {
    return iQualityLevels->At( aIndex ).iKey;
    }


// -----------------------------------------------------------------------------
// CCaeStillQualityLevelsCont::ResizeL
// -----------------------------------------------------------------------------
//
void CCaeStillQualityLevelsCont::ResizeL( 
    TInt aQualityLevelCount )
    {
    iQualityLevels->ResizeL( aQualityLevelCount );
    }


// -----------------------------------------------------------------------------
// CCaeStillQualityLevelsCont::QltyLevelCountSdKey
// -----------------------------------------------------------------------------
//
const TDesC& CCaeStillQualityLevelsCont::QltyLevelCountSdKey() const
    {
    return iQltyLevelCountSdKey;
    }


/////////////////////////////
// CStillQualityLevel methods 
/////////////////////////////


// -----------------------------------------------------------------------------
// CStillQualityLevel::CStillQualityLevel
// Default constructor.
// -----------------------------------------------------------------------------
//
CStillQualityLevel::CStillQualityLevel()
:CQualityLevel()
    {
    }


// -----------------------------------------------------------------------------
// CStillQualityLevel::~CStillQualityLevel
// Destructor.
// -----------------------------------------------------------------------------
//
CStillQualityLevel::~CStillQualityLevel()
    {
    }


//  End of File  
