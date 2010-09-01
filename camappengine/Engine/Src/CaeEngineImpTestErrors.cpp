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
* Description:  Camera Application Engine error simulations (CAE_TEST_VERSION only)
*
*/


#ifdef CAE_TEST_VERSION

//  INCLUDES
#include <e32base.h>
#include "CaeEngineImpTestErrors.h"


// Error simulation variables.
TInt CaeMcaeseoHBufC8ImageReadyErrorValue( 0 );
TInt CaeCaeMcaesdoCFbsBitmapImageReadyErrorValue( 0 );
TInt CaePowerOnCompleteErrorValue( 0 );
TInt CaeReserveCompleteErrorValue( 0 );
TInt CaeImageReadyErrorValue( 0 );
TInt CaeMvruoOpenCompleteErrorValue( 0 );
TInt CaeMvruoPrepareCompleteErrorValue( 0 );
TInt CaeMvruoRecordCompleteErrorValue( 0 );
TInt CaePrepareVideoSettingsErrorValue( 0 );
TInt CaeCreateAndDeliverSnapImageErrorValue( 0 );
TInt CaeCreateAndDeliverStillImageErrorValue( 0 );


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CaeSetMcaeseoHBufC8ImageReadyError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetMcaeseoHBufC8ImageReadyError( TInt aError )
    {
    CaeMcaeseoHBufC8ImageReadyErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaeMcaeseoHBufC8ImageReadyError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeMcaeseoHBufC8ImageReadyError( TInt& aError )
    {
    if ( aError == KErrNone ) 
        {
        aError = CaeMcaeseoHBufC8ImageReadyErrorValue;
        }
    }

// -----------------------------------------------------------------------------
// CaeSetMcaesdoCFbsBitmapImageReadyError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetMcaesdoCFbsBitmapImageReadyError( TInt aError )
    {
    CaeCaeMcaesdoCFbsBitmapImageReadyErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaeMcaesdoCFbsBitmapImageReadyError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeMcaesdoCFbsBitmapImageReadyError( TInt& aError )
    {
    if ( aError == KErrNone ) 
        {
        aError = CaeCaeMcaesdoCFbsBitmapImageReadyErrorValue;
        }
    }

// -----------------------------------------------------------------------------
// CaeSetPowerOnCompleteError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetPowerOnCompleteError( TInt aError )
    {
    CaePowerOnCompleteErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaePowerOnCompleteError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaePowerOnCompleteError( TInt& aError )
    {
    if ( aError == KErrNone ) 
        {
        aError = CaePowerOnCompleteErrorValue;
        }
    }

// -----------------------------------------------------------------------------
// CaeSetReserveCompleteError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetReserveCompleteError( TInt aError )
    {
    CaeReserveCompleteErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaeReserveCompleteError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeReserveCompleteError( TInt& aError )
    {
    if ( aError == KErrNone ) 
        {
        aError = CaeReserveCompleteErrorValue;
        }
    }

// -----------------------------------------------------------------------------
// CaeSetImageReadyError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetImageReadyError( TInt aError )
    {
    CaeImageReadyErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaeImageReadyError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeImageReadyError( TInt& aError )
    {
    if ( aError == KErrNone ) 
        {
        aError = CaeImageReadyErrorValue;
        }
    }

// -----------------------------------------------------------------------------
// CaeSetMvruoOpenCompleteError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetMvruoOpenCompleteError( TInt aError )
    {
    CaeMvruoOpenCompleteErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaeMvruoOpenCompleteError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeMvruoOpenCompleteError( TInt& aError )
    {
    if ( aError == KErrNone ) 
        {
        aError = CaeMvruoOpenCompleteErrorValue;
        }
    }

// -----------------------------------------------------------------------------
// CaeSetMvruoPrepareCompleteError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetMvruoPrepareCompleteError( TInt aError )
    {
    CaeMvruoPrepareCompleteErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaeMvruoPrepareCompleteError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeMvruoPrepareCompleteError( TInt& aError )
    {
    if ( aError == KErrNone ) 
        {
        aError = CaeMvruoPrepareCompleteErrorValue;
        }
    }

// -----------------------------------------------------------------------------
// CaeSetMvruoRecordCompleteError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetMvruoRecordCompleteError( TInt aError )
    {
    CaeMvruoRecordCompleteErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaeMvruoRecordCompleteError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeMvruoRecordCompleteError( TInt& aError )
    {
    if ( aError == KErrNone ) 
        {
        aError = CaeMvruoRecordCompleteErrorValue;
        }
    }

// -----------------------------------------------------------------------------
// CaeSetPrepareVideoSettingsError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetPrepareVideoSettingsError( TInt aError )
    {
    CaePrepareVideoSettingsErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaePrepareVideoSettingsErrorL
// -----------------------------------------------------------------------------
//
EXPORT_C void CaePrepareVideoSettingsErrorL()
    {
    if ( CaePrepareVideoSettingsErrorValue != KErrNone )
        {
        User::Leave( CaePrepareVideoSettingsErrorValue );
        }
    }

// -----------------------------------------------------------------------------
// CaeSetCreateAndDeliverSnapImageError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetCreateAndDeliverSnapImageError( TInt aError )
    {
    CaeCreateAndDeliverSnapImageErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaeCreateAndDeliverSnapImageError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeCreateAndDeliverSnapImageError( TInt& aError )
    {
    if ( aError == KErrNone ) 
        {
        aError = CaeCreateAndDeliverSnapImageErrorValue;
        }
    }

// -----------------------------------------------------------------------------
// CaeSetCreateAndDeliverStillImageError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeSetCreateAndDeliverStillImageError( TInt aError )
    {
    CaeCreateAndDeliverStillImageErrorValue = aError;
    }

// -----------------------------------------------------------------------------
// CaeCreateAndDeliverStillImageError
// -----------------------------------------------------------------------------
//
EXPORT_C void CaeCreateAndDeliverStillImageError( TInt& aError )
    {
    if ( aError == KErrNone ) 
        {
        aError = CaeCreateAndDeliverStillImageErrorValue;
        }
    }

#endif // CAE_TEST_VERSION

//  End of File  
