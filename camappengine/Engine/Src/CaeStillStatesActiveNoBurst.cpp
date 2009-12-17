/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Camera Application Engine still burst 
*                interface implementation when NOT SUPPORTED
*
*/



// INCLUDE FILES

#include "CaeEngineImp.h"                   // Engine implementation header.
#include "CaeStillStatesActive.h"                


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetCaeStillBurstObserver
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetCaeStillBurstObserver( 
    MCaeStillBurstObserver& /*aObserver*/ )
    {
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetStillCaptureImageCountL
// -----------------------------------------------------------------------------
//
TInt CCaeStillStatesActive::SetStillCaptureImageCountL( 
    TInt /*aImageCount*/ )
    {
    return 1; // Always one without burst support
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::StillCaptureImageCount
// -----------------------------------------------------------------------------
//
TInt CCaeStillStatesActive::StillCaptureImageCount() const
    {
    return 1; // Always one without burst support
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::SetStillBurstCaptureIntervalL
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::SetStillBurstCaptureIntervalL( 
    TTimeIntervalMicroSeconds /*aInterval*/ )
    {
    }
        

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::StillBurstCaptureInterval
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CCaeStillStatesActive::StillBurstCaptureInterval() const 
    {
    return 0;
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::StopStillBurstCapture
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::StopStillBurstCapture()
    {
    }


// ==================== PRIVATE MEMBER FUNCTIONS ===============================


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::HandleAppendCapturedBurstImageReady()
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleAppendCapturedBurstImageReady()
    {
    }


// ---------------------------------------------------------------------------
// CCaeStillStatesActive::HandleBurstImageFetchReady()
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleBurstImageFetchReady()
    {
    }

// ---------------------------------------------------------------------------
// CCaeStillStatesActive::HandleDeliverStillBurstImageReady()
// ---------------------------------------------------------------------------
//
void CCaeStillStatesActive::HandleDeliverStillBurstImageReady()
    {
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoCaptureStillBurst
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoCaptureStillBurst()
    {
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoCaptureStillBurstImage
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoCaptureStillBurstImage()
    {
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoAppendCapturedBurstImageToArray
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoAppendCapturedBurstImageToArray()
    {
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoFetchNextBurstImage
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoFetchNextBurstImage()
    {
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::CheckForStillBurstCompletion
// -----------------------------------------------------------------------------
//
TBool CCaeStillStatesActive::CheckForStillBurstCompletion()
    {
    return( ETrue );
    }
    

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::BurstErrorRecovery
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::BurstErrorRecovery( TInt /*aError*/)
    {
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoCompleteStillBurst
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoCompleteStillBurst()
    {
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DoDeliverStillBurstImage
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DoDeliverStillBurstImage()
    {
    }


// -----------------------------------------------------------------------------
// CCaeStillStatesActive::CompleteStillBurst()
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::CompleteStillBurst()
    {
    }

// -----------------------------------------------------------------------------
// CCaeStillStatesActive::DeleteStillBurst
// -----------------------------------------------------------------------------
//
void CCaeStillStatesActive::DeleteStillBurst()
    {
    }


//  End of File  
