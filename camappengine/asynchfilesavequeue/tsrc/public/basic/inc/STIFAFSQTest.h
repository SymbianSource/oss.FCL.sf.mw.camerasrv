/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  FSQ Test DLL
*
*/



#ifndef STIFAFSQTEST_H
#define STIFAFSQTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

// CONSTANTS

// MACROS

// Logging path
_LIT( KSTIFAFSQTestLogPath, "\\logs\\testframework\\STIFAFSQTest\\" ); 
// Log file
_LIT( KSTIFAFSQTestLogFile, "STIFAFSQTest.txt" ); 

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CSTIFAFSQTest;

// DATA TYPES

// CLASS DECLARATION

/**
*  CSTIFAFSQTest test class for STIF Test Framework TestScripter.
*  This STIF test module provides basic FSQ tests 
*
*  @lib STIFAFSQTest.dll
*  @since Series60_31
*/
NONSHARABLE_CLASS(CSTIFAFSQTest) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static STIFAFSQTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~STIFAFSQTest();

    public: // New functions

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line 
        * @since Series60_31
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * C++ default constructor.
        */
        CWLANTests( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Frees all resources allocated from test methods.
        * @since Series60_31
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */
        virtual TInt SearchNetworkL( CStifItemParser& aItem );
        

    public:     // Data

    protected:  // Data

    private:    // Data
       
        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
       
    protected:  // Friend classes
       
    private:    // Friend classes
       
    };

#endif      // STIFAFSQTEST_H

// End of File
