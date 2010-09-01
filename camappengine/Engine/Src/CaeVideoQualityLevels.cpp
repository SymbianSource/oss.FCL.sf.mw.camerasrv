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



// INCLUDE FILES

#include "CaeVideoQualityLevels.h"

#include "CaeEngineImp.h"       // For LOGTEXT

// CONSTANTS
const TInt KQualityLevelArrayGranularity = 3;



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaeVideoQualityLevelsCont::CCaeVideoQualityLevelsCont
// Default constructor.
// -----------------------------------------------------------------------------
//
CCaeVideoQualityLevelsCont::CCaeVideoQualityLevelsCont()
: iQltyLevelCountSdKey( KCaeVideoQltyLevelCount )
    {
    }


// -----------------------------------------------------------------------------
// CCaeVideoQualityLevelsCont::~CCaeVideoQualityLevelsCont
// Destructor.
// -----------------------------------------------------------------------------
//
CCaeVideoQualityLevelsCont::~CCaeVideoQualityLevelsCont()
    {
    delete iQualityLevels;
    }


// -----------------------------------------------------------------------------
// CCaeVideoQualityLevelsCont::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCaeVideoQualityLevelsCont::ConstructL()
    {
    LOGTEXT( _L( "Cae: CCaeVideoQualityLevelsCont::ConstructL() entering" ) );

    iQualityLevels = new( ELeave ) CArrayFixFlat<CVideoQualityLevel>( KQualityLevelArrayGranularity );

    LOGTEXT( _L( "Cae: CCaeVideoQualityLevelsCont::ConstructL() returning" ) );
    }



// -----------------------------------------------------------------------------
// CCaeVideoQualityLevelsCont::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaeVideoQualityLevelsCont* CCaeVideoQualityLevelsCont::NewL()
    {
    LOGTEXT( _L( "Cae: CCaeVideoQualityLevelsCont::NewL() entering" ) );

    CCaeVideoQualityLevelsCont* self = new( ELeave ) CCaeVideoQualityLevelsCont;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    LOGTEXT( _L( "Cae: CCaeVideoQualityLevelsCont::NewL() returning" ) );

    return self;
    }
    

// -----------------------------------------------------------------------------
// CCaeVideoQualityLevelsCont::InitDefaultsL
// Sets hardcoded default parameters for default video quality levels that 
// are created.
// -----------------------------------------------------------------------------
//
TInt CCaeVideoQualityLevelsCont::InitDefaultsL()
    {
    LOGTEXT( _L( "Cae: CCaeVideoQualityLevelsCont::InitL() entering" ) );

    // Delete all elements from the array and 
    // free the memory allocated to the array buffer.
    iQualityLevels->Reset();

    // Initialize hardcoded default quality levels.

    TSize size;

    CVideoQualityLevel* videoQualityLevel = new( ELeave ) CVideoQualityLevel;
    CleanupStack::PushL( videoQualityLevel );
    
    // Level 0:
    size.SetSize( KImgWidthQCIF, KImgHeightQCIF );
    videoQualityLevel->iFrameSize    = size;
    videoQualityLevel->iFrameRate    = 15.0;  // Frames per second. Basically HW dependent.
    videoQualityLevel->iBitRate      = 64000; // Bits per second. Basically HW dependent.
    videoQualityLevel->iAudioEnabled = EFalse;
    videoQualityLevel->iAudioBitRate = 12200; // Bits per second. Basically HW dependent.
    videoQualityLevel->iMimeType.Copy( KVideoMimeType );
    videoQualityLevel->iPreferredSupplier.Copy( KPreferredSupplier );
    videoQualityLevel->iVideoType.Copy( KVideoType );
    videoQualityLevel->iAudioType.Copy( KAudioType );
    videoQualityLevel->iStorageRate = 78000;  // That many bits per second to store. Estimate only
    videoQualityLevel->iKey = KCaeVideoQltyLevel0;

    iQualityLevels->AppendL( *videoQualityLevel );

    // Level 1:
    size.SetSize( KImgWidthSubQCIF, KImgHeightSubQCIF );
    videoQualityLevel->iFrameSize    = size;
    videoQualityLevel->iFrameRate    = 15.0;  // Frames per second. Basically HW dependent.
    videoQualityLevel->iBitRate      = 60000; // Bits per second. Basically HW dependent.
    videoQualityLevel->iAudioEnabled = EFalse;
    videoQualityLevel->iAudioBitRate = 6700;  // Bits per second. Basically HW dependent.
    videoQualityLevel->iMimeType.Copy( KVideoMimeType );
    videoQualityLevel->iPreferredSupplier.Copy( KPreferredSupplier );
    videoQualityLevel->iVideoType.Copy( KVideoType );
    videoQualityLevel->iAudioType.Copy( KAudioType );
    videoQualityLevel->iStorageRate = 68000;  // That many bits per second to store. Estimate only
    videoQualityLevel->iKey = KCaeVideoQltyLevel1;

    iQualityLevels->AppendL( *videoQualityLevel );

    // Level 2:
    size.SetSize( KImgWidthSubQCIF, KImgHeightSubQCIF );
    videoQualityLevel->iFrameSize    = size;
    videoQualityLevel->iFrameRate    = 5.0;   // Frames per second. Basically HW dependent.
    videoQualityLevel->iBitRate      = 40000; // Bits per second. Basically HW dependent.
    videoQualityLevel->iAudioEnabled = EFalse;
    videoQualityLevel->iAudioBitRate = 5150;  // Bits per second. Basically HW dependent.
    videoQualityLevel->iMimeType.Copy( KVideoMimeType );
    videoQualityLevel->iPreferredSupplier.Copy( KPreferredSupplier );
    videoQualityLevel->iVideoType.Copy( KVideoType );
    videoQualityLevel->iAudioType.Copy( KAudioType );
    videoQualityLevel->iStorageRate = 47000;  // That many bits per second to store. Estimate only
    videoQualityLevel->iKey = KCaeVideoQltyLevel2;

    iQualityLevels->AppendL( *videoQualityLevel );

    CleanupStack::PopAndDestroy( videoQualityLevel );

    LOGTEXT( _L( "Cae: CCaeVideoQualityLevelsCont::InitL() returning" ) );

    return iQualityLevels->Count();
    }


// -----------------------------------------------------------------------------
// CCaeVideoQualityLevelsCont::Count
// -----------------------------------------------------------------------------
//
TInt CCaeVideoQualityLevelsCont::Count() const
    {
    return iQualityLevels->Count();
    }

        
// -----------------------------------------------------------------------------
// CCaeVideoQualityLevelsCont::At
// -----------------------------------------------------------------------------
//
CVideoQualityLevel& CCaeVideoQualityLevelsCont::At( 
    TInt aIndex ) const
    {
    return iQualityLevels->At( aIndex );
    }


// -----------------------------------------------------------------------------
// CCaeVideoQualityLevelsCont::Key
// -----------------------------------------------------------------------------
//
const TDes& CCaeVideoQualityLevelsCont::Key( 
    TInt aIndex ) const
    {
    return iQualityLevels->At( aIndex ).iKey;
    }


// -----------------------------------------------------------------------------
// CCaeVideoQualityLevelsCont::ResizeL
// -----------------------------------------------------------------------------
//
void CCaeVideoQualityLevelsCont::ResizeL( 
    TInt aQualityLevelCount )
    {
    iQualityLevels->ResizeL( aQualityLevelCount );
    }


// -----------------------------------------------------------------------------
// CCaeVideoQualityLevelsCont::QltyLevelCountSdKey
// -----------------------------------------------------------------------------
//
const TDesC& CCaeVideoQualityLevelsCont::QltyLevelCountSdKey() const
    {
    return iQltyLevelCountSdKey;
    }


/////////////////////////////
// CVideoQualityLevel methods 
/////////////////////////////


// -----------------------------------------------------------------------------
// CVideoQualityLevel::CVideoQualityLevel
// Default constructor.
// -----------------------------------------------------------------------------
//
CVideoQualityLevel::CVideoQualityLevel()
:CQualityLevel()
    {
    }


// -----------------------------------------------------------------------------
// CVideoQualityLevel::~CVideoQualityLevel
// Destructor.
// -----------------------------------------------------------------------------
//
CVideoQualityLevel::~CVideoQualityLevel()
    {
    }


//  End of File  
