/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Still Image Converter base class for Camera Application Engine
*
*/



// INCLUDE FILES
#include <eikenv.h>

#include "CaeStillConverter.h"

#ifdef CAE_TEST_VERSION
#include "CaeStillConverterTestErrors.h"
#endif

// CONSTANTS
const TInt KImageQueueGranularity = 6;



// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CCaeStillConverter::~CCaeStillConverter
// Destructor.
// ---------------------------------------------------------
//
CCaeStillConverter::~CCaeStillConverter()
    {
    if ( iImageQueue )
        {
        iImageQueue->ResetAndDestroy();
        delete iImageQueue;
        }

    // For RTRT code coverage analysis.
    // #pragma attol insert _ATCPQ_DUMP(0);
    }


// ---------------------------------------------------------
// CCaeStillConverter::RunL
// Handles an active object’s request completion event.
// ---------------------------------------------------------
//
void CCaeStillConverter::RunL()
    {
    switch ( iState )
        {
        case EIdle:
            break;

        case EConvert:
            {
            ConversionComplete( iStatus.Int() );

            // Start the next image conversion
            if ( iImageQueue->Count() > 0 )
                {
                TRAPD( error, ConvertL() );
                if ( error != KErrNone )
                    {
                    ConversionComplete( error );
                    }
                }
            }
            break;

        default:
            break;
        }
    }


// ---------------------------------------------------------
// CCaeStillConverter::RunError
// From CActive, handles the leaving RunL().
// ---------------------------------------------------------
//
TInt CCaeStillConverter::RunError( TInt aError )
    {
    ConversionComplete( aError );
    return KErrNone;
    }


// ---------------------------------------------------------
// CCaeStillConverter::CCaeStillConverter
// Default constructor. 
// This can NOT leave.
// ---------------------------------------------------------
//
CCaeStillConverter::CCaeStillConverter() :
CActive( EPriorityStandard ),
iState( EIdle )
    {
    }


// ---------------------------------------------------------
// CCaeStillConverter::ConstructL
// Symbian 2nd phase constructor that can leave.
// ---------------------------------------------------------
//
void CCaeStillConverter::ConstructL()
    {
    iImageQueue = new( ELeave ) RPointerArray<CImageItem>( KImageQueueGranularity );
    }


// ---------------------------------------------------------
// CCaeStillConverter::IsBusy
// Returns ETrue if Still Converter is busy.
// ---------------------------------------------------------
//
TBool CCaeStillConverter::IsBusy() const
    {
    return ( iState != EIdle );
    }


//  End of File  
