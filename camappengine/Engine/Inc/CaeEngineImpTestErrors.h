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
* Description:  Camera Application Engine error simulation global functions  
*                for testing (CAE_TEST_VERSION only).
*
*/


#ifdef CAE_TEST_VERSION

#ifndef CAEENGINEIMPTESTERRORS_H
#define CAEENGINEIMPTESTERRORS_H

IMPORT_C void CaeSetMcaeseoHBufC8ImageReadyError( TInt aError );

IMPORT_C void CaeMcaeseoHBufC8ImageReadyError( TInt& aError );

IMPORT_C void CaeSetMcaesdoCFbsBitmapImageReadyError( TInt aError );

IMPORT_C void CaeMcaesdoCFbsBitmapImageReadyError( TInt& aError );

IMPORT_C void CaeSetPowerOnCompleteError( TInt aError );

IMPORT_C void CaePowerOnCompleteError( TInt& aError );

IMPORT_C void CaeSetReserveCompleteError( TInt aError );

IMPORT_C void CaeReserveCompleteError( TInt& aError );

IMPORT_C void CaeSetImageReadyError( TInt aError );

IMPORT_C void CaeImageReadyError( TInt& aError );

IMPORT_C void CaeSetMvruoOpenCompleteError( TInt aError );

IMPORT_C void CaeMvruoOpenCompleteError( TInt& aError );

IMPORT_C void CaeSetMvruoPrepareCompleteError( TInt aError );

IMPORT_C void CaeMvruoPrepareCompleteError( TInt& aError );

IMPORT_C void CaeSetMvruoRecordCompleteError( TInt aError );

IMPORT_C void CaeMvruoRecordCompleteError( TInt& aError );

IMPORT_C void CaeSetPrepareVideoSettingsError( TInt aError );

IMPORT_C void CaePrepareVideoSettingsErrorL();

IMPORT_C void CaeSetCreateAndDeliverSnapImageError( TInt aError );

IMPORT_C void CaeCreateAndDeliverSnapImageError( TInt& aError );

IMPORT_C void CaeSetCreateAndDeliverStillImageError( TInt aError );

IMPORT_C void CaeCreateAndDeliverStillImageError( TInt& aError );

#endif // CAEENGINEIMPTESTERRORS_H

#endif // #ifdef CAE_TEST_VERSION
