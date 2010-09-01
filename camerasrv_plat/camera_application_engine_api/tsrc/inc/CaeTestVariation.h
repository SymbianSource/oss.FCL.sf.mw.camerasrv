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
* Description:  Cae Test DLL
*
*/


#ifndef __CAETESTVARIATION_H
#define __CAETESTVARIATION_H

//  INCLUDES

#ifdef USING_CPPUNIT_FRAMEWORK
     /* CppUnit headers */
    #include <CppUnit/Test.h>
    #include <CppUnit/TestCase.h>
    #include <CppUnit/TestCaller.h>
    #include <CppUnit/TestSuite.h>
#else
    /* STIF TFW headers */
    #include "TestFrameWork/test.h"
    #include "TestFrameWork/testCase.h"
    #include "TestFrameWork/testCaller.h"
    #include "TestFrameWork/testSuite.h"
#endif


// CONSTANTS

// Enable/disable test case image and video writing to memory card
#define WRITE_SNAPIMAGE_TO_FILE     0
#define WRITE_STILLIMAGE_TO_FILE    0
#define WRITE_VIDEOCLIP_TO_FILE     1
#define WRITE_VF_TO_FILE            0
#define WRITE_VIDEOTIMES_TO_FILE    0

// Select symbian (1) or optimized (0) jpeg codecs

#if ( defined (__WINS__) || defined (__WINSCW) )
#define SYMBIAN_CODECS  1
#else
#define SYMBIAN_CODECS  0
#endif

// MACROS

// Select only one of these (if any).
#define MIRAGE_X_PROD1 0
#define MIRAGE_X_PROD2 0
#define MIRAGE_X_PROD3 0
#define MIRAGE_X_PROD4 0
#define MIRAGE_X_PROD5 0
#define MIRAGE_X_PROD6 0
#define MIRAGE_X_PROD7 0
#define MIRAGE_X_PROD8 0
#define MIRAGE_X_PROD9 0
#define HW_DEVLON 1 
#define HW_DEVLON52 0

#define VIDEO_RECORDING_TEST_CASES  1
#define SECONDARY_CAMERA_TEST_CASES 0



#if (MIRAGE_X_PROD1) || (MIRAGE_X_PROD2) || (MIRAGE_X_PROD3) || (MIRAGE_X_PROD4) || (MIRAGE_X_PROD5) || (MIRAGE_X_PROD6) || (MIRAGE_X_PROD8) ||\
     (MIRAGE_X_PROD9) || (HW_DEVLON) || (HW_DEVLON52)
    #define MIRAGE_X 1
#else
    #define MIRAGE_X 0
#endif

#if (MIRAGE_X_PROD7)
    #define UNAGI_2M 1
#else
    #define UNAGI_2M 0
#endif

#if ((!MIRAGE_X) && (!UNAGI_2M)) || (MIRAGE_X_PROD5)
    // S60 2.1 interface
    #define CAE_INTERFACE_21
#endif

#if (MIRAGE_X_PROD3) || (MIRAGE_X_PROD4) 
    #define COLOR_TONE_TEST_CASES 1
#endif

// For OOM testing in CppUnit. Set to 1 if you want this on. Set to 0 if not. 
#define CAE_OOM_TESTING_HARNESS 0


// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION


#endif
