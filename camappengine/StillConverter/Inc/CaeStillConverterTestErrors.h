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
* Description:  Camera Application Engine Still Converter error simulation 
*                global functions for testing (CAE_TEST_VERSION only)
*
*/


#ifdef CAE_TEST_VERSION

#ifndef CAESTILLCONVERTERTESTERRORS_H
#define CAESTILLCONVERTERTESTERRORS_H

IMPORT_C void CaeSetConvertCFbsBitmapToHBufC8Error( TInt aError );

IMPORT_C void CaeConvertCFbsBitmapToHBufC8ErrorL();

IMPORT_C void CaeSetConvertHBufC8ToCFbsBitmapError( TInt aError );

IMPORT_C void CaeConvertHBufC8ToCFbsBitmapErrorL();

IMPORT_C void CaeSetStillConvertError( TInt aError );

IMPORT_C void CaeStillConvertErrorL();

#endif // CAESTILLCONVERTERTESTERRORS_H

#endif // CAE_TEST_VERSION
