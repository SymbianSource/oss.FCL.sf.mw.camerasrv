/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Still Image Converter Common
*
*/



#ifndef CAESTILLCOMMON_H
#define CAESTILLCOMMON_H

// INCLUDES

// CONSTANTS

// Logging support
// Serial port logging is enabled in _DEBUG builds by default
// USE_FILE_LOGGER or USE_SERIAL_LOGGER can also be explicitely defined e.g. in MMP file.
//
#if defined (_DEBUG) && !defined(USE_FILE_LOGGER)                   
    #define USE_SERIAL_LOGGER 
#endif
                                    
#if defined (USE_FILE_LOGGER)
    #include <flogger.h>
    _LIT(KLogFile,"cae.txt");
    _LIT(KLogFolder,"cae");
    #define LOGTEXT(AAA)                RFileLogger::Write(KLogFolder(),KLogFile(),EFileLoggingModeAppend,AAA)
    #define LOGTEXT2(AAA,BBB)           RFileLogger::WriteFormat(KLogFolder(),KLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(AAA),BBB)
    #define LOGTEXT3(AAA,BBB,CC)       RFileLogger::WriteFormat(KLogFolder(),KLogFile(),EFileLoggingModeAppend,TRefByValue<const TDesC>(AAA),BBB,CC)
    #define LOGHEXDUMP(AAA,BBB,CC,DDD) RFileLogger::HexDump(KLogFolder(),KLogFile(),EFileLoggingModeAppend,AAA,BBB,CC,DDD)    
#elif defined (USE_SERIAL_LOGGER) 
    #include <e32svr.h>
    #define LOGTEXT(AAA)                RDebug::Print(AAA)
    #define LOGTEXT2(AAA,BBB)           RDebug::Print(AAA,BBB)
    #define LOGTEXT3(AAA,BBB,CC)       RDebug::Print(AAA,BBB,CC)
    #define LOGHEXDUMP(AAA,BBB,CC,DDD)
#else
    #define LOGTEXT(AAA)                
    #define LOGTEXT2(AAA,BBB)           
    #define LOGTEXT3(AAA,BBB,CC)       
    #define LOGHEXDUMP(AAA,BBB,CC,DDD) 
#endif                              


// CLASS DECLARATION



#endif // CAESTILLCOMMON_H

// End of File
