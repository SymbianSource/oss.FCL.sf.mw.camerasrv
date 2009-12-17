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
* Description:  Utilities for Camera Application Engine. 
*
*/


#ifndef CAEDBGUTILITIES_H
#define CAEDBGUTILITIES_H

// MACROS

// Logging support
// Serial port logging is enabled in _DEBUG builds by default
// USE_FILE_LOGGER or USE_SERIAL_LOGGER can also be explicitely defined e.g. in MMP file.
//

// #define USE_FILE_LOGGER

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

// Memory info logging.
#ifdef _DEBUG
    #define MEM() PrintMemoryInfo()
#else
    #define MEM()
#endif

// Define assert to include file & line number
#define CAE_ASSERT_FILE__(s) _LIT(KPanicFileName,s)
#define CAE_ASSERT_PANIC__(l) User::Panic(KPanicFileName().Right(12),l) // Note: RVCT compiler puts the full path in __LINE__ 
#define CAE_ASSERT_DEBUG_PANIC__(c,l) if (!(c)) {LOGTEXT3(_L("Error panic! Assert failed in file: %S, line: %d"), &KPanicFileName, l);User::Panic(KPanicFileName().Right(12),l);}

#ifdef _DEBUG
#define CAE_ASSERT_DEBUG(x)  { CAE_ASSERT_FILE__(__FILE__); CAE_ASSERT_DEBUG_PANIC__(x,__LINE__);}
#define CAE_ASSERT_ALWAYS(x) CAE_ASSERT_DEBUG(x)
#else
#define CAE_ASSERT_DEBUG(x)
#define CAE_ASSERT_ALWAYS(x) { CAE_ASSERT_FILE__("CamAppEngine"); __ASSERT_ALWAYS(x, CAE_ASSERT_PANIC__(__LINE__) ); }
#endif


// FUNCTION PROTOTYPES

#ifdef _DEBUG
    void PrintMemoryInfo();
#endif


#endif // CAEDBGUTILITIES_H

// End of File