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
* Description:  Camera Application Engine Still Converter error simulations 
*                (CAE_TEST_VERSION only).
*
*/


#ifdef CAE_TEST_VERSION

//  INCLUDES
#include <e32base.h>


// Error simulation variables.
TInt CaeConvertCFbsBitmapToHBufC8ErrorValue( 0 );
TInt CaeConvertHBufC8ToCFbsBitmapErrorValue( 0 );
TInt CaeStillConvertErrorValue( 0 );


// ============================ GLOBAL FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CaeSetConvertCFbsBitmapToHBufC8Error
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetConvertCFbsBitmapToHBufC8Error( TInt aError )
    {
    CaeConvertCFbsBitmapToHBufC8ErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaeConvertCFbsBitmapToHBufC8ErrorL
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeConvertCFbsBitmapToHBufC8ErrorL()
    {
    if ( CaeConvertCFbsBitmapToHBufC8ErrorValue != KErrNone )
        {
        User::Leave( CaeConvertCFbsBitmapToHBufC8ErrorValue );
        }
    }

// -----------------------------------------------------------------------------
// CaeSetConvertHBufC8ToCFbsBitmapError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetConvertHBufC8ToCFbsBitmapError( TInt aError )
    {
    CaeConvertHBufC8ToCFbsBitmapErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaeConvertHBufC8ToCFbsBitmapErrorL
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeConvertHBufC8ToCFbsBitmapErrorL()
    {
    if ( CaeConvertHBufC8ToCFbsBitmapErrorValue != KErrNone )
        {
        User::Leave( CaeConvertHBufC8ToCFbsBitmapErrorValue );
        }
    }

// -----------------------------------------------------------------------------
// CaeSetStillConvertError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetStillConvertError( TInt aError )
    {
    CaeStillConvertErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaeStillConvertErrorL
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeStillConvertErrorL()
    {
    if ( CaeStillConvertErrorValue != KErrNone )
        {
        User::Leave( CaeStillConvertErrorValue );
        }
    }


#endif // CAE_TEST_VERSION

//  End of File  
