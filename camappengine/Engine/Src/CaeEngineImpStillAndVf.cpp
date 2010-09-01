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
* Description:  Camera Application Engine implementation still capturing, 
*                settings, and view finder methods
*
*/



// INCLUDE FILES

#include <fbs.h>                            // For CFbsBitmap

#include "CaeEngineImp.h"                   // Engine implementation header.
#include "CaeStillQualityLevels.h"          // For still capturing quality levels.
#include "CaeVideoQualityLevels.h"          // Needed just in destructor.
#include "CaeVideoTimes.h"                  // Needed just in destructor.
#include "CaeStillStatesActive.h"                

#ifdef CAE_TEST_VERSION
#include "CaeEngineImpTestErrors.h"         // For TEST_VERSION compilation only
#endif

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "CaeEngineImpStillAndVfTraces.h"
#endif


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaeEngine::NewL
// Two-phased constructor. Calls CCaeEngineImp's NewL().
// -----------------------------------------------------------------------------
//
EXPORT_C CCaeEngine* CCaeEngine::NewL()
    {
    return CCaeEngineImp::NewL();
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::CCaeEngineImp
// Default constructor.
// -----------------------------------------------------------------------------
//
CCaeEngineImp::CCaeEngineImp(): 
    iZoomMode( EZoomModeDigital ), 
    iExposureMode( CCamera::EExposureAuto ),
    iWhiteBalanceMode( CCamera::EWBAuto ), 
    iFlashMode( CCamera::EFlashNone ),
    iVideoTimesInterval( KVideoTimesIntervalDefault ), 
    iMaxClipSizeInBytes( KMMFNoMaxClipSize ), 
    iMaxClipSizeInBytesPrep( KMMFNoMaxClipSize )
    {
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::~CCaeEngineImp
// Destructor.
// -----------------------------------------------------------------------------
//
CCaeEngineImp::~CCaeEngineImp()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::~CCaeEngineImp() entering" ) );

    // Cancels/closes/stops still capturing, video recording, and view finding.
    CancelAllActivities();

    // Delete extension implementations
	iExtension.ResetAndDestroy();

    // Delete the state machine
    // Note: This must be done after destroying extensions because there is some
    // validity checking in the iStillStatesActive destructor.
    delete( iStillStatesActive );

    // Delete interface implementation lists. 
	// Close for each iImplementations before closing 
	// iExtInterfaceImplementationLists.
	for ( TInt i = 0; i < iExtInterfaceImplementationLists.Count(); i++ )
		{
		iExtInterfaceImplementationLists[i].iImplementations->Close();
		delete( iExtInterfaceImplementationLists[i].iImplementations );
		}
	iExtInterfaceImplementationLists.Close();

    // Delete custom interface list items
	iExtCustomInterfaceImplementations.Close();

    delete iVideoType;
    delete iVideoClipFileName;

    // Turn camera power off first, then release camera.
    if ( iCamera ) 
        {
        if ( iReserved ) 
            {
            if ( iPowerOn ) 
                {
                iCamera->PowerOff();
                }
            iCamera->Release();
            }
        }

    delete iVideoFrameSizePrep;
    delete iVideoFrameSize;

    delete iVideoTimes;
    delete iVideoTimesCallback;
    delete iVideoRecorder;

    delete iVideoQualityLevelsCont;
    delete iStillQualityLevelsCont;
    
    delete iCallbackActive;

    delete iInfo;

    delete iCamera;

	REComSession::FinalClose();

    LOGTEXT( _L( "Cae: CCaeEngineImp::~CCaeEngineImp() returning" ) );

    // For RTRT code coverage analysis.
    // #pragma attol insert _ATCPQ_DUMP(0);
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::ConstructL
// Symbian 2nd phase constructor that can leave.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ConstructL( TInt aCameraIndex, TInt aDisplayIndex )
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::ConstructL() entering with camera index %d" ), 
    	aCameraIndex );
    
    OstTrace0( CAMERASRV_PERFORMANCE, CCAEENGINEIMP_CONSTRUCTL, "e_CAM_ENG_INIT 1" );   //CAE_ENGINE_INIT_START
        
    if ( ( aCameraIndex < 0 ) || 
         ( aCameraIndex >= CCamera::CamerasAvailable() ) )
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::ConstructL leaving KErrHardwareNotAvailable, aCameraIndex=%d" ), aCameraIndex );
        User::Leave( KErrHardwareNotAvailable );
        }

    // Create a new Camera API implementation object, if supported
    TRAPD( err, iCamera = CCamera::New2L( static_cast<MCameraObserver2&>(*this), aCameraIndex, KCameraPriority ) );
    if ( err )
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::ConstructL() CCamera::New2L return code=%d" ), err ); 

        // Create old Camera API implementation object.
        iCamera = CCamera::NewL( static_cast<MCameraObserver&>(*this), aCameraIndex );
        LOGTEXT( _L( "Cae: CCaeEngineImp::ConstructL() using MCameraObserver" )); 
        }
    else
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::ConstructL() using MCameraObserver2" )); 
        }


    // Get camera handle.
    iCameraHandle = iCamera->Handle();

    // Set display index
    iDisplayIndex = aDisplayIndex;

    // Create and initialize info object.
    iInfo = new( ELeave ) TCamAppEngineInfo;
    InitializeInfo( *iCamera );

    // Create active object for calling methods from callbacks.
    iCallbackActive = CCaeCallbackActive::NewL( *this );

    // Create still quality levels container object.
    iStillQualityLevelsCont = CCaeStillQualityLevelsCont::NewL();

    // Set default value for video recording stopping 
    iAsyncVideoStopEnabled = EFalse;
    
	// Create interface lists. 
	// The order of interface lists in iExtInterfaceImplementationLists
	// should not be changed later as the order must always match to 
	// TCaeExtensionInterfaceIndexes enum.
	for ( TInt i = 0; KCaeSupportedExtensionInterfaceUids[i].iUid != NULL; i++ )
		{
		TCaeExtensionInterfaceImplListItem listItem;

		RArray<TCaeExtensionInterfaceImplItem>* emptyImplementationArray = 
			new(ELeave) RArray<TCaeExtensionInterfaceImplItem>(
			TCaeExtensionInterfaceImplItemGranularity );

		listItem.iInterfaceUid = KCaeSupportedExtensionInterfaceUids[i];
		listItem.iImplementations = emptyImplementationArray; 
		
		// Copy the list item to the end of the list.Give ownership of 
        // emptyImplementationArray.
        CleanupStack::PushL( emptyImplementationArray );
		iExtInterfaceImplementationLists.AppendL( listItem ); 
        CleanupStack::Pop( emptyImplementationArray );
		}

    // Create the state machine for still capturing
    iStillStatesActive = CCaeStillStatesActive::NewL( *iCamera, *iInfo,
        *( iExtInterfaceImplementationLists[KCaeExtProcessImageInterfaceIndex].iImplementations  ) );

	// Create extensions and populate the extension array 
	PopulateExtensionArrayL();
    
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAEENGINEIMP_CONSTRUCTL, "e_CAM_ENG_INIT 0" );  //CAE_ENGINE_INIT_END
    
    LOGTEXT( _L( "Cae: CCaeEngineImp::ConstructL() returning" ) );
    }



// -----------------------------------------------------------------------------
// CCaeEngineImp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCaeEngineImp* CCaeEngineImp::NewL()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::NewL() entering" ) );

    CCaeEngineImp* self = new( ELeave ) CCaeEngineImp();

    CleanupStack::PushL( self );
    self->ConstructL( 0, 0 ); // Construct with default camera index 0 and display index 0
    CleanupStack::Pop( self );

    LOGTEXT( _L( "Cae: CCaeEngineImp::NewL() returning" ) );

    return self;
    }

    
// -----------------------------------------------------------------------------
// CCaeEngineImp::PopulateExtensionArrayL
// Get a list of extension implementations via ECom, construct the extensions and 
// add those the the array.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PopulateExtensionArrayL()
	{
	LOGTEXT( _L( "Cae: CCaeEngineImp::PopulateExtensionArrayL() entering" ) );

	// Get a list of all extensions from ECom
	TEComResolverParams resolverParams;
	resolverParams.SetDataType ( KExtensionTypeStringDesc );
	resolverParams.SetWildcardMatch ( EFalse );
	RImplInfoPtrArray implInfoArray;
	REComSession::ListImplementationsL( KExtensionInterfaceUid, resolverParams, 
		implInfoArray );
	// This should be called only after ListImplementationsL
	CleanupResetAndDestroyPushL( implInfoArray ); 
    
	LOGTEXT2( _L( "Cae: CCaeEngineImp::PopulateExtensionArrayL: extension count: %d" ),
		implInfoArray.Count());
	
	// Create each extension via ECom and add it to the list 
	for ( TInt i = 0; i < implInfoArray.Count(); i++ )
		{		
		MExtension* extension = NULL;
		
		// Trap the construction as if one extension fails to construct it 
		// should not stop others from constructing
    	LOGTEXT2( _L( "Cae: CCaeEngineImp::PopulateExtensionArrayL: load extension uid=%x" ),
	    	implInfoArray[i]->ImplementationUid().iUid);
		
		TRAPD( error, extension = MExtension::NewExtensionL( 
			implInfoArray[i]->ImplementationUid(), 
			(MExtensionCallbackInterface*) this ));

		if ( error || !extension)
			{
			// If one extension is failing we skip that.
			#ifdef _DEBUG
			TBuf<256> text;
			text.Format(_L( "Cae: CCaeEngineImp::PopulateExtensionArrayL: NewExtensionL leaved, uid=%x, error=%d, extension=%x" ),
				implInfoArray[i]->ImplementationUid().iUid, error, extension );
			LOGTEXT( text );
			#endif

			delete( extension );
			}
		else
			{
			iExtension.AppendL( extension );
			}
		}

	CleanupStack::PopAndDestroy( &implInfoArray );

	LOGTEXT( _L( "Cae: CCaeEngineImp::PopulateExtensionArrayL() returning" ) );
	}


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetCamAppEngineObserver
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetCamAppEngineObserver( MCamAppEngineObserver& aObserver )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SetCamAppEngineObserver()" ) );

    iCaeObserver = &aObserver;
    iStillStatesActive->SetCamAppEngineObserver( aObserver );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::GetInfo
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::GetInfo( TCamAppEngineInfo& aInfo ) const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::GetInfo()" ) );
 
    aInfo = *iInfo;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::InitL
// Initialise the engine for still image capturing using default parameters.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::InitL( TBool aCreateSnapImage )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::InitL() entering" ) );

    iStillStatesActive->SetSnapImageCreation( aCreateSnapImage );

    // To allow re-initialization, release the camera (first cancels possible 
    // activities and turns camera power off).
    Release();

    // Init to default quality levels.
    iInfo->iNumStillQualityLevelsSupported = 
        iStillQualityLevelsCont->InitDefaultsL();

    // Reserve camera (and after succesfull reserve, camera power will be 
    // turned on).

    iResetToPreviousSettings = ETrue; // This requests here to set the camera
                                      // to default settings after power is on.
    iCamera->Reserve();

    LOGTEXT( _L( "Cae: CCaeEngineImp::InitL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::InitL
// Initialise the engine for still image capturing either using default 
// parameters or parameters from ini-file.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::InitL( 
    TUid aSdUidStillQltyLevels, 
    TBool aCreateSnapImage )
    {
    if( aSdUidStillQltyLevels == KNullUid )
        {
        InitL( aCreateSnapImage );
        }
    else
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::InitL(aSdUidStillQltyLevels): NOT SUPPORTED, leaving" ) );
        User::Leave( KErrNotSupported );
        }
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::Reserve
// Calls Camera API Reserve().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::Reserve()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::Reserve() entering" ) );

    if ( !iReserved ) 
        {
        iResetToPreviousSettings = ETrue;
        iCamera->Reserve();
        }
    else if ( !iPowerOn ) // in case previous reserve ok, but poweron failed
        {
        PowerOn();
        }


    LOGTEXT( _L( "Cae: CCaeEngineImp::Reserve() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::Release
// Calls Camera API Release().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::Release()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::Release() entering" ) );

    if ( iReserved ) 
        {
        PowerOff();         // Cancel all activities (if any) and turn power off.
        iCamera->Release(); // Release Camera HW.
        iReserved = EFalse;
        iStillPrepared = EFalse;
        iVideoPrepared = EFalse;
        iVideoOpened = EFalse;
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::Release() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::PowerOn
// Calls Camera API PowerOn().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PowerOn()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::PowerOn() entering" ) );

    if ( iReserved && !iPowerOn ) 
        {
        iCamera->PowerOn();
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::PowerOn() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::PowerOff
// Calls Camera API PowerOff().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PowerOff()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::PowerOff() entering" ) );

    if ( iPowerOn ) 
        {
        CancelAllActivities();
        iCamera->PowerOff();
        iPowerOn = EFalse;
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::PowerOff() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetZoomModeL
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetZoomModeL( 
    TZoomMode aZoomMode )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SetZoomModeL() entering" ) );

    // Currently supporting digital and optical zooms, not EZoomModeOpticalDigital.
    if ( ( aZoomMode != EZoomModeDigital ) && ( aZoomMode != EZoomModeOptical ) )
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::SetZoomModeL leaving KErrNotSupported, aZoomMode=%d" ), aZoomMode );
        User::Leave( KErrNotSupported );
        }

    iZoomMode = aZoomMode;

    LOGTEXT( _L( "Cae: CCaeEngineImp::SetZoomModeL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::ZoomMode
// -----------------------------------------------------------------------------
//
CCaeEngine::TZoomMode CCaeEngineImp::ZoomMode() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::ZoomMode()" ) );

    return iZoomMode;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetZoomValueL
// Calls Camera API SetDigitalZoomFactorL() or SetZoomFactorL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetZoomValueL( 
    TInt aZoomValue )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SetZoomValueL() entering" ) );

    CheckPowerL();

    switch ( iZoomMode )
        {
        case EZoomModeDigital:
            // Leave if zoom factor is out of range.
            if ( ( aZoomValue < 0 ) || 
                 ( aZoomValue > iInfo->iMaxDigitalZoom ) )
                {
                LOGTEXT2( _L( "Cae: CCaeEngineImp::SetZoomValueL leaving KErrArgument, aZoomValue=%d" ), aZoomValue );
                User::Leave( KErrArgument );
                }
            // Set DIGITAL zoom value.
            iCamera->SetDigitalZoomFactorL( aZoomValue );
            iZoomValue = aZoomValue;
            break;
        case EZoomModeOptical:
            // Leave if zoom factor is out of range.
            if ( ( aZoomValue < iInfo->iMinZoom ) || 
                 ( aZoomValue > iInfo->iMaxZoom ) )
                {
                LOGTEXT2( _L( "Cae: CCaeEngineImp::SetZoomValueL leaving KErrArgument, aZoomValue=%d" ), aZoomValue );
                User::Leave( KErrArgument );
                }
            // Set OPTICAL zoom value.
            iCamera->SetZoomFactorL( aZoomValue );
            iZoomValue = aZoomValue;
            break;
        default:
            // EZoomModeOpticalDigital not supported
            LOGTEXT2( _L( "Cae: CCaeEngineImp::SetZoomValueL leaving KErrNotSupported, iZoomMode=%d" ), iZoomMode );
            User::Leave( KErrNotSupported );
            break;
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::SetZoomValueL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::ZoomValue
// Calls Camera API DigitalZoomFactor() or ZoomFactor().
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::ZoomValue() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::ZoomValue()" ) );

    TInt zoomValue( 0 );
    if ( iPowerOn )
        {
        switch ( iZoomMode )
            {
            case EZoomModeDigital:
                zoomValue = iCamera->DigitalZoomFactor();
                break;
            case EZoomModeOptical:
                zoomValue = iCamera->ZoomFactor();
                break;
            default:
                // EZoomModeOpticalDigital not supported
                break;
            }
        }
    return zoomValue;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetBrightnessL
// Calls Camera API SetBrightnessL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetBrightnessL( 
    TInt aBrightness )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SetBrightnessL() entering" ) );

    // Leave if not supported.
    if ( !( iInfo->iOptionsSupported & TCameraInfo::EBrightnessSupported ) )
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::SetBrightnessL leaving KErrNotSupported, aBrightness=%d" ), aBrightness );
        User::Leave( KErrNotSupported );
        }

    CheckPowerL();

    iCamera->SetBrightnessL( aBrightness );
    iBrightness = aBrightness;

    LOGTEXT( _L( "Cae: CCaeEngineImp::SetBrightnessL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::Brightness
// Calls Camera API Brightness().
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::Brightness() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::Brightness()" ) );

    TInt brightness( 0 );
    if ( iPowerOn && ( iInfo->iOptionsSupported & TCameraInfo::EBrightnessSupported ) )
        {
        brightness = iCamera->Brightness();
        }
    return brightness;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetContrastL
// Calls Camera API SetContrastL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetContrastL( 
    TInt aContrast )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SetContrastL() entering" ) );

    // Leave if not supported.
    if ( !( iInfo->iOptionsSupported & TCameraInfo::EContrastSupported ) )
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::SetContrastL leaving KErrNotSupported, aContrast=%d" ), aContrast );
        User::Leave( KErrNotSupported );
        }

    CheckPowerL();

    iCamera->SetContrastL( aContrast );
    iContrast = aContrast;

    LOGTEXT( _L( "Cae: CCaeEngineImp::SetContrastL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::Contrast
// Calls Camera API Contrast().
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::Contrast() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::Contrast()" ) );

    TInt contrast( 0 );
    if ( iPowerOn && ( iInfo->iOptionsSupported & TCameraInfo::EContrastSupported ) )
        {
        contrast = iCamera->Contrast();
        }
    return contrast;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetExposureModeL
// Calls Camera API SetExposureModeL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetExposureModeL( 
    CCamera::TExposure aExposureMode )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SetExposureModeL() entering" ) );

    // Leave is requested exposure mode is not supported, 
    // EExposureAuto should be always supported.
    if ( ( ( aExposureMode != CCamera::EExposureAuto ) && 
          !( aExposureMode & iInfo->iExposureModesSupported ) ) ||
           ( aExposureMode < 0 ) )
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::SetExposureModeL leaving KErrNotSupported, aExposureMode=%d" ), aExposureMode );
        User::Leave( KErrNotSupported );
        }

    CheckPowerL();

    iCamera->SetExposureL( aExposureMode );
    iExposureMode = aExposureMode;

    LOGTEXT( _L( "Cae: CCaeEngineImp::SetExposureModeL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::ExposureMode
// Calls Camera API Exposure().
// -----------------------------------------------------------------------------
//
CCamera::TExposure CCaeEngineImp::ExposureMode() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::ExposureMode()" ) );

    CCamera::TExposure exposureMode( CCamera::EExposureAuto );
    if ( iPowerOn )
        {
        exposureMode = iCamera->Exposure();
        }
    return exposureMode;
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::SetEvCompensationL
// If the interface exists, calls Camera API SetEvCompensationL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetEvCompensationL( 
    TInt /*aEvIndex*/ )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SetEvCompensationL() (not supported) leaving" ) );

    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::EvCompensation
// If the interface exists, calls Camera API EvCompensation().
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::EvCompensation() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::EvCompensation() (not supported!)" ) );

    return( 0 );
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::SetWhiteBalanceModeL
// Calls Camera API SetWhiteBalanceL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetWhiteBalanceModeL( 
    CCamera::TWhiteBalance aWhiteBalanceMode )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SetWhiteBalanceModeL() entering" ) );

    // Leave is requested WB mode is not supported. 
    // EWBAuto is always supported.
    if ( ( ( aWhiteBalanceMode != CCamera::EWBAuto ) && 
          !( aWhiteBalanceMode & iInfo->iWhiteBalanceModesSupported ) ) ||
           ( aWhiteBalanceMode < 0 ) )
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::SetWhiteBalanceModeL leaving KErrNotSupported, aWhiteBalanceMode=%d" ), aWhiteBalanceMode );
        User::Leave( KErrNotSupported );
        }
    
    CheckPowerL();

    iCamera->SetWhiteBalanceL( aWhiteBalanceMode );
    iWhiteBalanceMode = aWhiteBalanceMode;

    LOGTEXT( _L( "Cae: CCaeEngineImp::SetWhiteBalanceModeL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::WhiteBalanceMode
// Calls Camera API WhiteBalance().
// -----------------------------------------------------------------------------
//
CCamera::TWhiteBalance CCaeEngineImp::WhiteBalanceMode() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::WhiteBalanceMode()" ) );

    CCamera::TWhiteBalance whiteBalanceMode( CCamera::EWBAuto );
    if ( iPowerOn )
        {
        whiteBalanceMode = iCamera->WhiteBalance();
        }
    return whiteBalanceMode;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetFlashModeL
// Calls Camera API SetFlashL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetFlashModeL( 
    CCamera::TFlash aFlashMode )
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::SetFlashModeL() entering, mode=%d" ), aFlashMode );

    // Leave is requested flash mode is not supported. 
    // EFlashNone is always supported.
    if ( ( ( aFlashMode != CCamera::EFlashNone ) && 
          !( aFlashMode & iInfo->iFlashModesSupported ) ) || 
           ( aFlashMode < 0 ) )
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::SetFlashModeL leaving KErrNotSupported, aFlashMode=%d" ), aFlashMode );
        User::Leave( KErrNotSupported );
        }

    CheckPowerL();

    iCamera->SetFlashL( aFlashMode );
    iFlashMode = aFlashMode;

    LOGTEXT( _L( "Cae: CCaeEngineImp::SetFlashModeL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::FlashMode
// Calls Camera API Flash().
// -----------------------------------------------------------------------------
//
CCamera::TFlash CCaeEngineImp::FlashMode() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::FlashMode()" ) );

    CCamera::TFlash flashMode( CCamera::EFlashNone );
    if ( iPowerOn )
        {
        flashMode = iCamera->Flash();
        }
    return flashMode;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::ResetToDefaultsL
// Resets the following settings: exposure mode, white balance mode, 
// zoom mode, zoom value, flash mode, brightness, and contrast.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ResetToDefaultsL()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::ResetToDefaultsL() entering" ) );

    SetExposureModeL();
    SetWhiteBalanceModeL();
    SetZoomModeL();
    SetZoomValueL();
    SetFlashModeL();

    // Reset this setting only if it is supported by Camera API.
    if ( iInfo->iOptionsSupported & TCameraInfo::EBrightnessSupported )
        {
        SetBrightnessL();
        }

    // Reset this setting only if it is supported by Camera API.
    if ( iInfo->iOptionsSupported & TCameraInfo::EContrastSupported )
        {
        SetContrastL();
        }

 	// Process extensions
	RArray<TCaeExtensionInterfaceImplItem>* settingsImplementationList = 
		iExtInterfaceImplementationLists[KCaeExtSettingsInterfaceIndex].iImplementations;

	for ( TInt i = 0; i < settingsImplementationList->Count(); i++ )
		{
		TRAPD(err, STATIC_CAST( MCaeExtSettingsInterface*, 
		(*settingsImplementationList)[i].iImplPtr )->ResetToDefaultsL() );

	    #ifdef _DEBUG
		if ( err )
		    {
			LOGTEXT3(_L("Cae: CCaeEngineImp::ResetToDefaultsL(). Error %d in ResetToDefaultsL() for Extension %x"),
			    err, (*settingsImplementationList)[i].iImplUid.iUid );
			}
		#endif
		
		User::LeaveIfError( err ); 
		}

   LOGTEXT( _L( "Cae: CCaeEngineImp::ResetToDefaultsL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetViewFinderMirrorL
// Calls Camera API SetViewFinderMirrorL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetViewFinderMirrorL( 
    TBool aMirror )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SetViewFinderMirrorL() entering" ) );

    // Leave if not supported.
    if ( !( iInfo->iOptionsSupported & TCameraInfo::EViewFinderMirrorSupported ) )
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::SetViewFinderMirrorL leaving KErrNotSupported, aMirror=%d" ), aMirror );
        User::Leave( KErrNotSupported );
        }
        
    CheckPowerL();

    iCamera->SetViewFinderMirrorL( aMirror );    

    LOGTEXT( _L( "Cae: CCaeEngineImp::SetViewFinderMirrorL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::ViewFinderMirror
// Calls Camera API ViewFinderMirror().
// -----------------------------------------------------------------------------
//
TBool CCaeEngineImp::ViewFinderMirror() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::ViewFinderMirror()" ) );

    // If supported, query the setting from Camera API.
    TBool viewFinderMirror( EFalse );
    if ( iPowerOn && ( iInfo->iOptionsSupported & TCameraInfo::EViewFinderMirrorSupported ) )
        {
        viewFinderMirror = iCamera->ViewFinderMirror();
        }
    return viewFinderMirror;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::StartViewFinderBitmapsL
// Calls Camera API StartViewFinderBitmapsL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::StartViewFinderBitmapsL( 
    TSize& aVfFrameSize )
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::StartViewFinderBitmapsL() entering aVfFrameSize wxh=%dx%d" ),
            aVfFrameSize.iWidth, aVfFrameSize.iHeight );

    // Leave if not supported.
    if ( !( iInfo->iOptionsSupported & TCameraInfo::EViewFinderBitmapsSupported ) )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::StartViewFinderBitmapsL leaving KErrNotSupported" ));
        User::Leave( KErrNotSupported );
        }

    if ( iPowerOn ) 
        {
        if ( !iCamera->ViewFinderActive() ) 
            {
            iTrueViewFinderSize = aVfFrameSize;
            iCamera->StartViewFinderBitmapsL( iTrueViewFinderSize );
            iViewFinderRunning = ETrue;
            iStillStatesActive->SetViewFinderMode( iViewFinderRunning );
            }
        }

    if ( !iViewFinderRunning )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::StartViewFinderBitmapsL leaving KErrNotReady (iViewFinderRunning)" ));
        User::Leave( KErrNotReady );
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::StartViewFinderBitmapsL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::StartViewFinderBitmapsL
// Cropped version.
// Calls Camera API StartViewFinderBitmapsL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::StartViewFinderBitmapsL( 
    TSize& aVfFrameSize, 
    TRect& aCropRect )
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::StartViewFinderBitmapsL() entering aVfFrameSize wxh=%dx%d" ),
            aVfFrameSize.iWidth, aVfFrameSize.iHeight );

    // Leave if not supported.
    if ( !( iInfo->iOptionsSupported & TCameraInfo::EViewFinderBitmapsSupported ) || 
         !( iInfo->iOptionsSupported & TCameraInfo::EViewFinderClippingSupported )  )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::StartViewFinderBitmapsL (cropped) leaving KErrNotSupported" ));
        User::Leave( KErrNotSupported );
        }

    if ( iPowerOn ) 
        {
        if ( !iCamera->ViewFinderActive() ) 
            {
            iTrueViewFinderSize = aVfFrameSize;
            iCamera->StartViewFinderBitmapsL( iTrueViewFinderSize, aCropRect );
            iViewFinderRunning = ETrue;
            iStillStatesActive->SetViewFinderMode( iViewFinderRunning );
            }
        }

    if ( !iViewFinderRunning )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::StartViewFinderBitmapsL (cropped) leaving KErrNotReady" ));
        User::Leave( KErrNotReady );
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::StartViewFinderBitmapsL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::StartViewFinderDirectL
// Calls Camera API StartViewFinderDirectL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::StartViewFinderDirectL( 
    RWsSession& aWs, 
    CWsScreenDevice& aScreenDevice, 
    RWindowBase& aWindow, 
    TRect& aScreenRect )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::StartViewFinderDirectL() entering" ) );

    // Leave if not supported.
    if ( !( iInfo->iOptionsSupported & TCameraInfo::EViewFinderDirectSupported ) )
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::StartViewFinderDirectL leaving KErrNotSupported, iOptionsSupported=0x%x" ), iInfo->iOptionsSupported );
        User::Leave( KErrNotSupported );
        }

    if ( iPowerOn )
        {
        if ( !iCamera->ViewFinderActive() )
            {
            iCamera->StartViewFinderDirectL( aWs, aScreenDevice, aWindow, aScreenRect );
            iViewFinderRunning = ETrue;
            iStillStatesActive->SetViewFinderMode( iViewFinderRunning );
            }
        }
    
    if ( !iViewFinderRunning )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::StartViewFinderDirectL leaving KErrNotReady" ));
        User::Leave( KErrNotReady );
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::StartViewFinderDirectL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::StartViewFinderDirectL
// Cropped version.
// Calls Camera API StartViewFinderDirectL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::StartViewFinderDirectL( 
    RWsSession& aWs,
    CWsScreenDevice& aScreenDevice,
    RWindowBase& aWindow,
    TRect& aScreenRect,
    TRect& aCropRect )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::StartViewFinderDirectL() entering" ) );

    // Leave if not supported.
    if ( !( iInfo->iOptionsSupported & TCameraInfo::EViewFinderDirectSupported ) || 
         !( iInfo->iOptionsSupported & TCameraInfo::EViewFinderClippingSupported ) )
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::StartViewFinderDirectL (cropped) leaving KErrNotSupported, iOptionsSupported=0x%x" ), iInfo->iOptionsSupported );
        User::Leave( KErrNotSupported );
        }

    if ( iPowerOn )
        {
        if ( !iCamera->ViewFinderActive() )
            {
            iCamera->StartViewFinderDirectL( aWs, aScreenDevice, aWindow, aScreenRect, aCropRect );
            iViewFinderRunning = ETrue;
            iStillStatesActive->SetViewFinderMode( iViewFinderRunning );
           }
        }

    if ( !iViewFinderRunning )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::StartViewFinderDirectL (cropped) leaving KErrNotReady" ));
        User::Leave( KErrNotReady );
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::StartViewFinderDirectL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::StopViewFinder
// Calls Camera API ViewFinderActive() and StopViewFinder().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::StopViewFinder()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::StopViewFinder() entering" ) );

    if ( iPowerOn )
        {
        if ( iCamera->ViewFinderActive() )
            {
            iCamera->StopViewFinder();
            iViewFinderRunning = EFalse;
            iStillStatesActive->SetViewFinderMode( iViewFinderRunning );
            }
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::StopViewFinder() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::IsViewFinding
// Calls Camera API ViewFinderActive().
// -----------------------------------------------------------------------------
//
TBool CCaeEngineImp::IsViewFinding() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::IsViewFinding()" ) );

    TBool isViewFinding( EFalse );
    if ( iPowerOn ) 
        {
        isViewFinding = iCamera->ViewFinderActive();
        }
    return isViewFinding;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::GetViewFinderSize
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::GetViewFinderSize( 
    TSize& aVfFrameSize )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::GetViewFinderSize()" ) );

    aVfFrameSize = iTrueViewFinderSize;
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::SetCaeStillBurstObserver
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetCaeStillBurstObserver( 
    MCaeStillBurstObserver& aObserver )
    {
    iStillStatesActive->SetCaeStillBurstObserver( aObserver );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetStillCaptureImageCountL
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::SetStillCaptureImageCountL( 
    TInt aImageCount )
    {
    return ( iStillStatesActive->SetStillCaptureImageCountL( aImageCount ) );
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::StillCaptureImageCount
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::StillCaptureImageCount() const
    {
    return ( iStillStatesActive->StillCaptureImageCount() );
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::SetStillBurstCaptureIntervalL
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetStillBurstCaptureIntervalL( 
    TTimeIntervalMicroSeconds aInterval )
    {
    iStillStatesActive->SetStillBurstCaptureIntervalL( aInterval );
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::StillBurstCaptureInterval
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CCaeEngineImp::StillBurstCaptureInterval() const 
    {
    return ( iStillStatesActive->StillBurstCaptureInterval() );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::StopStillBurstCapture
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::StopStillBurstCapture()
    {
    iStillStatesActive->StopStillBurstCapture();
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::StillBurstSupport()
// Return option flag for still burst support.
// -----------------------------------------------------------------------------
//
TCamAppEngineInfo::TOptions CCaeEngineImp::StillBurstSupport()
    {
    return TCamAppEngineInfo::EStillBurstSupported;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::PrepareStillCaptureL
// Retrieves parameters for the specified quality level and calls 
// overloading PrepareStillCaptureL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PrepareStillCaptureL( 
    TInt aStillQualityIndex )
    {
    TSize size = iStillStatesActive->SnapImageSize();
    PrepareStillCaptureL( aStillQualityIndex, size);
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::PrepareStillCaptureL
// Retrieves parameters for the specified quality level and calls 
// overloading PrepareStillCaptureL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PrepareStillCaptureL( 
    TInt         aStillQualityIndex, 
    TSize&       aSnapSize )
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::PrepareStillCaptureL() entering aSnapSize wxh=%dx%d" ),
            aSnapSize.iWidth, aSnapSize.iHeight );

    if ( aStillQualityIndex < 0 || 
         aStillQualityIndex >= iStillQualityLevelsCont->Count() ) 
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::PrepareStillCaptureL leaving KErrArgument, aStillQualityIndex=%d" ), aStillQualityIndex );
        User::Leave( KErrArgument );
        }

    TSize frameSize         = iStillQualityLevelsCont->At( aStillQualityIndex ).iFrameSize;
    CCamera::TFormat format = iStillQualityLevelsCont->At( aStillQualityIndex ).iOutputFormat;
    TInt compressionQuality = iStillQualityLevelsCont->At( aStillQualityIndex ).iCompressionQuality;
    TRect fullRect( frameSize );
    
    PrepareStillCaptureL( frameSize, format, compressionQuality, fullRect, aSnapSize );
    
    iStillQualityIndex = aStillQualityIndex;

    LOGTEXT( _L( "Cae: CCaeEngineImp::PrepareStillCaptureL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::PrepareStillCaptureL
// Cropped version.
// Not supported.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PrepareStillCaptureL( 
    TInt /*aStillQualityIndex*/, 
    const TRect& /*aCropRect*/ )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::PrepareStillCaptureL() cropped leaving KErrNotSupported" ) );

    User::Leave( KErrNotSupported );
    }



// -----------------------------------------------------------------------------
// CCaeEngineImp::PrepareStillCaptureL
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PrepareStillCaptureL( 
    const TSize&      aFrameSize,
    CCamera::TFormat  aFormat,  
    TInt              aCompressionQuality )
    {    
    TRect fullRect( aFrameSize );
    PrepareStillCaptureL( aFrameSize, aFormat, aCompressionQuality, fullRect );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::PrepareStillCaptureL
// Cropped version. Using cropping only if supported by Camera API impl.
// Calls RetrieveStillSizeIndex() that calls Camera API EnumerateCaptureSizes().
// Calls Camera API PrepareImageCaptureL().
// Optionally calls Camera API SetJpegQuality().
// Finally calls McaeoStillPrepareComplete call-back.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PrepareStillCaptureL( 
    const TSize&      aFrameSize, 
    CCamera::TFormat  aFormat, 
    TInt              aCompressionQuality, 
    const TRect&      aCropRect )
    {
    TSize snapSize = iStillStatesActive->SnapImageSize();
    PrepareStillCaptureL(aFrameSize, aFormat, aCompressionQuality, aCropRect, snapSize );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::PrepareStillCaptureL
// Cropped version. Using cropping only if supported by Camera API impl.
// Calls RetrieveStillSizeIndex() that calls Camera API EnumerateCaptureSizes().
// Calls Camera API PrepareImageCaptureL().
// Optionally calls Camera API SetJpegQuality().
// Finally calls McaeoStillPrepareComplete call-back.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PrepareStillCaptureL( 
    const TSize&      aFrameSize, 
    CCamera::TFormat  aFormat, 
    TInt              aCompressionQuality, 
    const TRect&      aCropRect,
    TSize&            aSnapSize )
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::PrepareStillCaptureL() entering aFrameSize wxh=%dx%d" ),
            aFrameSize.iWidth, aFrameSize.iHeight );

    OstTrace0( CAMERASRV_PERFORMANCE, CCAEENGINEIMP_PREPARESTILLCAPTUREL, "e_CAM_ENG_STILL_INIT 1" );   //CAE_ENGINE_STILL_INIT_START

    // Leave if not supported.
    if ( !( iInfo->iOptionsSupported & TCameraInfo::EImageCaptureSupported ) )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::PrepareStillCaptureL() leaving KErrNotSupported (EImageCaptureSupported)" ) );
        User::Leave( KErrNotSupported );
        }

    CheckPowerL();

    // Leave if still capturing or video recording is running.
    if ( iStillStatesActive->IsRunning() || iVideoRecordingRunning ) 
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::PrepareStillCaptureL() leaving KErrNotReady (IsRunning)" ) );
        User::Leave( KErrNotReady );
        }

    if ( ( aFormat == CCamera::EFormatJpeg ) || ( aFormat == CCamera::EFormatExif ) ) 
        {
    	SetJpegQuality( aCompressionQuality ); 
        }
    
    iStillStatesActive->PrepareStillCaptureL( aFrameSize, aFormat, aCropRect, aSnapSize );

    // Prepare ready now.
    iStillPrepared = ETrue;

    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAEENGINEIMP_PREPARESTILLCAPTUREL, "e_CAM_ENG_STILL_INIT 0" );  //CAE_ENGINE_STILL_INIT_END

    // Inform client about the success.
    iCaeObserver->McaeoStillPrepareComplete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::StillQualityIndex
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::StillQualityIndex() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::StillQualityIndex()" ) );    

    TInt qualityIndex( -1 );
    if ( iStillPrepared )
        {
        qualityIndex = iStillQualityIndex;
        }
    return qualityIndex;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::GetStillFrameSize
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::GetStillFrameSize( 
    TInt aStillQualityIndex, 
    TSize& aFrameSize ) const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::GetStillFrameSize()" ) );
    
    if ( aStillQualityIndex >= 0 &&  
         aStillQualityIndex < iStillQualityLevelsCont->Count() )
        {
        aFrameSize = iStillQualityLevelsCont->At( aStillQualityIndex ).iFrameSize;
        }
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::EstimatedStillSizeInBytes
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::EstimatedStillSizeInBytes( 
    TInt aStillQualityIndex ) const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::EstimatedStillSizeInBytes()" ) );

    TInt byteSize( 0 );
    if ( aStillQualityIndex >= 0 &&  
         aStillQualityIndex < iStillQualityLevelsCont->Count() )
        {
        byteSize = iStillQualityLevelsCont->At( aStillQualityIndex ).iByteSize;
        }
    return byteSize;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::CaptureStill
// Starts still capturing.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::CaptureStill()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::CaptureStill() entering" ) );

    OstTrace0( CAMERASRV_PERFORMANCE, CCAEENGINEIMP_CAPTURESTILL, "e_CAM_ENG_SHOT_TO_STILL 1" );    //CAE_ENGINE_SHOT_TO_STILL_START

    if ( !iStillStatesActive->IsRunning() ) 
        {
        if ( iStillPrepared && iPowerOn && !iVideoRecordingRunning ) 
            {
                // Call CAPI without active object delay (for perf reasons)
                iStillStatesActive->StartQuickCapture();

            }
        else
            {
            // Handle error
            iStillStatesActive->ErrorRecovery( KErrNotReady );
            }    
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::CaptureStill() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::CancelCaptureStill
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::CancelCaptureStill()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::CancelCaptureStill() entering" ) );

    if ( iStillStatesActive ) 
        {
        iStillStatesActive->CancelCaptureStill();
        }

    if ( iCallbackActive ) 
        {
        iCallbackActive->Cancel();
        }

    if ( iCamera ) 
        {
        iCamera->CancelCaptureImage();
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::CancelCaptureStill() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetSnapImageCreation
// Sets snap image creation on/off.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetSnapImageCreation( 
    TBool aCreateSnapImage )
    {
    iStillStatesActive->SetSnapImageCreation( aCreateSnapImage );
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::SetSnapImageSource
// Sets snap image source.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetSnapImageSourceL( 
    CCaeEngine::TSnapImageSource aSnapImageSource )
    {
    iStillStatesActive->SetSnapImageSourceL( aSnapImageSource );
    }


// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CCaeEngineImp::SetSnapImageSizeL
// Sets the size of the snap image.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetSnapImageSizeL( 
    TSize& aSize )
    {
    iStillStatesActive->SetSnapImageSizeL( aSize );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetSnapImageColorMode
// Sets the color mode of the snap image.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetSnapImageColorMode( 
    TDisplayMode aMode )
    {
    iStillStatesActive->SetSnapImageColorMode( aMode );
    }


// -----------------------------------------------------------------------------
// CCaeEngine::NewL
// Two-phased constructor. Calls CCaeEngineImp's NewL() with camera index and
// default display index 0.
// -----------------------------------------------------------------------------
//
EXPORT_C CCaeEngine* CCaeEngine::NewL( 
    TInt aCameraIndex )
    {
    return CCaeEngineImp::NewL( aCameraIndex, 0 );
    }


// -----------------------------------------------------------------------------
// CCaeEngine::NewL
// Two-phased constructor. Calls CCaeEngineImp's NewL() with camera index.
// -----------------------------------------------------------------------------
//
EXPORT_C CCaeEngine* CCaeEngine::NewL( 
    TInt aCameraIndex, TInt aDisplayIndex )
    {
    return CCaeEngineImp::NewL( aCameraIndex, aDisplayIndex );
    }


// -----------------------------------------------------------------------------
// CCaeEngine::CamerasAvailable
// Determines the number of cameras on the device.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CCaeEngine::CamerasAvailable()
    {
    LOGTEXT2( _L( "Cae: CCaeEngine::CamerasAvailable(): %d" ), CCamera::CamerasAvailable() );
    return CCamera::CamerasAvailable();
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::NewL
// Two-phased constructor. Inputs camera index.
// -----------------------------------------------------------------------------
//
CCaeEngineImp* CCaeEngineImp::NewL( 
    TInt aCameraIndex, TInt aDisplayIndex )
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::NewL() entering with camera index %d" ), aCameraIndex );

    CCaeEngineImp* self = new( ELeave ) CCaeEngineImp();

    CleanupStack::PushL( self );
    self->ConstructL( aCameraIndex, aDisplayIndex );
    CleanupStack::Pop( self );

    LOGTEXT( _L( "Cae: CCaeEngineImp::NewL() returning" ) );

    return self;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::EnumerateStillCaptureSizes
// Enumerates through the available image capture sizes.
// Calls Camera API EnumerateCaptureSizes().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::EnumerateStillCaptureSizes( 
    TSize& aSize, 
    TInt aSizeIndex, 
    CCamera::TFormat aFormat ) const
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::EnumerateStillCaptureSizes() entering aSizeIndex=%d" ), aSizeIndex );
    iCamera->EnumerateCaptureSizes( aSize, aSizeIndex, aFormat );
    LOGTEXT3( _L( "Cae: CCaeEngineImp::EnumerateStillCaptureSizes() returning aSize wxh=%dx%d" ),
           aSize.iWidth, aSize.iHeight );
   }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetJpegQuality
// Sets the compression quality value to use with JPEG and Exif image formats.
// Calls Camera API SetJpegQuality() and the still state machine SetJpegQuality().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetJpegQuality( 
    TInt aCompressionQuality )
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::SetJpegQuality() entering with %d" ), aCompressionQuality );
    // Clamp and memorize the image compression quality.
    if ( aCompressionQuality < 1 )
        {
        aCompressionQuality = 1;
        }
    if ( aCompressionQuality > 100 ) 
        {
        aCompressionQuality = 100;
        }

    // Set JPEG quality on Camera API.
    iCamera->SetJpegQuality( aCompressionQuality );

    // Set JPEG quality on the state machine
    iStillStatesActive->SetJpegQuality( aCompressionQuality );

    LOGTEXT2( _L( "Cae: CCaeEngineImp::SetJpegQuality() returning, memorized %d" ), aCompressionQuality );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::JpegQuality
// Gets the current compression quality value setting for 
// JPEG and Exif image formats.
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::JpegQuality() const
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::JpegQuality(): %d" ), iStillStatesActive->JpegQuality() );
    // Memorized image compression quality, set in  
    // SetJpegQuality() or PrepareStillCaptureL().
    return( iStillStatesActive->JpegQuality() );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetImageCodecsL
// Sets the specific image codec implementation to be used in decoding and 
// encoding.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetImageCodecsL( 
	TUid aDecoderUid, TUid aEncoderUid )
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::SetImageCodecsL(): %x, %x" ), aDecoderUid, aEncoderUid );
    iStillStatesActive->SetImageCodecsL( aDecoderUid, aEncoderUid );
    }

        
// -----------------------------------------------------------------------------
// CCaeEngineImp::ReserveComplete
// Camera reservation is complete. 
// Called asynchronously when CCamera::Reserve() completes.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ReserveComplete(                                 
    TInt aError )
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::ReserveComplete() entering, aError=%d" ), aError );

    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaeReserveCompleteError( aError );
    #endif    

    if ( aError == KErrNone )
        {
        iReserved = ETrue;
        // Call the active object to switch the power on indirectly.
        iCallbackActive->PowerOn();
        }
    else
        {
        iCaeObserver->McaeoInitComplete( aError );
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::ReserveComplete() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::PowerOnComplete
// Indicates camera power on is complete.
// Called on completion of CCamera:PowerOn().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PowerOnComplete( 
    TInt aError )
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::PowerOnComplete() entering, aError=%d" ), aError );

    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaePowerOnCompleteError( aError );
    #endif    

    if ( aError == KErrNone ) 
        {
        iPowerOn = ETrue;
        if ( iResetToPreviousSettings )
            {
            iResetToPreviousSettings = EFalse;
            TRAP( aError, ResetToPreviousSettingsL() );
            }
        }
    
    iCaeObserver->McaeoInitComplete( aError );

    LOGTEXT( _L( "Cae: CCaeEngineImp::PowerOnComplete() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::ViewFinderFrameReady
// Transfers the current view finder image from the camera. The frame is 
// first delivered to the extensions and then to the client.
// Called periodically in response to the use of 
// CCamera::StartViewFinderBitmapsL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ViewFinderFrameReady( 
    CFbsBitmap& aFrame )
    {
    //LOGTEXT( _L( "Cae: CCaeEngineImp::ViewFinderFrameReady() entering" ) );

	// Process extensions
	RArray<TCaeExtensionInterfaceImplItem>* processImageImplList = 
		iExtInterfaceImplementationLists[KCaeExtProcessImageInterfaceIndex].iImplementations;

	TInt extError = KErrNone;
	for ( TInt i = 0; i < processImageImplList->Count() && !extError; i++ )
		{
		if ( (*processImageImplList)[i].iIsActive )
		    {
    		STATIC_CAST( MCaeExtProcessImageInterface*, 
	    	(*processImageImplList)[i].iImplPtr )->ProcessViewFinderFrame( aFrame, extError );
		    }
		else
		    {
			// LOGTEXT2(_L("Cae: CCaeEngineImp::ViewFinderFrameReady(). Skipped extension %x"), (*processImageImplList)[i].iImplUid.iUid );
		    }
		
	    #ifdef _DEBUG
		if ( extError )
		    {
			LOGTEXT3(_L("Cae: CCaeEngineImp::ViewFinderFrameReady(). Error %d in extension processing for Extension %x"),
			    extError, (*processImageImplList)[i].iImplUid.iUid );
			}
		#endif
		}

    iCaeObserver->McaeoViewFinderFrameReady( aFrame, extError );

    iStillStatesActive->ViewFinderFrameReady();

    //LOGTEXT( _L( "Cae: CCaeEngineImp::ViewFinderFrameReady() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::ImageReady
// Transfers the current image from the camera. 
// Called asynchronously when CCamera::CaptureImage() completes.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ImageReady( 
    CFbsBitmap* aBitmap, 
    HBufC8*     aImageData, 
    TInt        aError )
    {
    iStillStatesActive->ImageReady( aBitmap, aImageData, aError );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::CheckPowerL
// Leaves with KErrNotReady if power is off.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::CheckPowerL()
    {
    if ( !iPowerOn ) 
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::CheckPowerL() leaving KErrNotReady (iPowerOn)" ) );
        User::Leave( KErrNotReady );
        }
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::CancelAllActivities()
// Cancels/closes/stops still capturing, video recording, and view finding.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::CancelAllActivities()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::CancelAllActivities() entering" ) );

    CancelCaptureStill();
 
    CloseVideoRecording();

    StopViewFinder();

    LOGTEXT( _L( "Cae: CCaeEngineImp::CancelAllActivities() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::ResetToPreviousSettingsL
// Resets the following settings to the previous values: 
// exposure mode, white balance mode, 
// zoom mode, zoom value, flash mode, brightness, and contrast.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ResetToPreviousSettingsL()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::ResetToPreviousSettingsL() entering" ) );

    SetExposureModeL( iExposureMode );
    SetWhiteBalanceModeL( iWhiteBalanceMode );
    SetZoomModeL( iZoomMode );
    SetZoomValueL( iZoomValue );
    SetFlashModeL( iFlashMode );

    // Try to reset this setting only if it is supported by Camera API.
    // Otherwise this could cause error when re-reserving camera.
    if ( iInfo->iOptionsSupported & TCameraInfo::EBrightnessSupported )
        {
        SetBrightnessL( iBrightness );
        }

    // Try to reset this setting only if it is supported by Camera API.
    // Otherwise this could cause error when re-reserving camera.
    if ( iInfo->iOptionsSupported & TCameraInfo::EContrastSupported )
        {
        SetContrastL( iContrast );
        }

 	// Process extensions
	RArray<TCaeExtensionInterfaceImplItem>* settingsImplementationList = 
		iExtInterfaceImplementationLists[KCaeExtSettingsInterfaceIndex].iImplementations;

	for ( TInt i = 0; i < settingsImplementationList->Count(); i++ )
		{
		TRAPD( err,STATIC_CAST( MCaeExtSettingsInterface*, 
		    (*settingsImplementationList)[i].iImplPtr )->ResetToPreviousSettingsL() );
		    
	    #ifdef _DEBUG
		if ( err )
		    {
			LOGTEXT3(_L("Cae: CCaeEngineImp::ResetToDefaultsL(). Error %d in ResetToDefaultsL() for Extension %x"),
			    err, (*settingsImplementationList)[i].iImplUid.iUid );
			}
		#endif
		
		User::LeaveIfError( err );
		}

    LOGTEXT( _L( "Cae: CCaeEngineImp::ResetToPreviousSettingsL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::CustomInterface
// Returns a custom interface  that matches to the given UID.
// -----------------------------------------------------------------------------
//
TAny* CCaeEngineImp::CustomInterface( TUid aInterface )
    {
	LOGTEXT2(_L("Cae: CCaeEngineImp::CustomInterface() entering, aIterface=%x"),aInterface.iUid );
	TAny* interfaceImp = NULL;

	// At first, try to find the interface in CAE extesions
	TCaeCustomInterfaceImplItem item;
	item.iInterfaceUid = aInterface;
	item.iImplPtr = NULL; // This is not used by FindInUnsignedKeyOrder()
	TInt index = iExtCustomInterfaceImplementations.FindInUnsignedKeyOrder( item ); 
	if ( index >= 0 )
		{
		interfaceImp = iExtCustomInterfaceImplementations[ index ].iImplPtr;
		}

	if ( !interfaceImp )
		{
		// Then, try to find the interface in Camera API
        interfaceImp = iCamera->CustomInterface( aInterface );
		}

    // Check if View Finder is stopped during burst sequency to optimize speed of burst
    // Detection of this feature is based to the fact that the CI UID (KUidBurstModeVFOptimization)
    // is asked and found from the CAPI.
    if ( aInterface == KUidBurstModeVFOptimization && interfaceImp )
        {
        iStillStatesActive->SetBurstModeVFOptimization( ETrue );
        }
        
	LOGTEXT2(_L("Cae: CCaeEngineImp::CustomInterface() returning, interfaceImp=%x"),interfaceImp );
    return interfaceImp;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::GetOrder
// Populate feature array
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::GetOrderL( RCaeOrderedFeatureList& aOrderedFeatureList )
    {
    TCaeOrderedFeatureListItem tmpItem;
    aOrderedFeatureList.Reset();
    
	// For all interface implementation lists
    for ( TInt implListIndex = 0; 
           implListIndex < iExtInterfaceImplementationLists.Count(); 
          implListIndex++ )
        {
		TCaeExtensionInterfaceImplListItem* implListItem = 
			&(iExtInterfaceImplementationLists[implListIndex]);
		RArray<TCaeExtensionInterfaceImplItem>& implementations 
		    = *(implListItem->iImplementations);

		// For all implementations
		for ( TInt implIndex = 0; implIndex < implementations.Count(); implIndex++ )
			{

			// Add to item to the feature array
			tmpItem.iServiceUid = implListItem->iInterfaceUid;
			tmpItem.iFeatureUid = implementations[implIndex].iImplUid;
			aOrderedFeatureList.AppendL( tmpItem );
			}
        }
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetOrderL
// The user must first call GetOrder() to get array. After that the user can 
// change the order of items and call this function. The user shall not remove 
// items or add new items to the array. 
//
// E.g. Typically the user finds two items from the array and switches
// the positions of these items to get desired effect.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetOrderL( const RCaeOrderedFeatureList& aOrderedFeatureList )
    {
	TInt status = KErrNone;

	// Check that aOrderedFeatureList is valid
	DoSetOrderL( aOrderedFeatureList, EFalse );
	
	// Check counts
	
	TInt totalImplCount = 0;

	// For all interface implementation lists
	for ( TInt implListIndex = 0; 
	      implListIndex < iExtInterfaceImplementationLists.Count(); 
	      implListIndex++ )
		{
		TCaeExtensionInterfaceImplListItem* implListItem = 
			&(iExtInterfaceImplementationLists[implListIndex]);
		RArray<TCaeExtensionInterfaceImplItem>& implementations 
		    = *(implListItem->iImplementations);

		totalImplCount += implementations.Count();
		}

	if ( totalImplCount != aOrderedFeatureList.Count() )
		{
		status = KErrArgument;
		}
	else
		{
		// Check duplicates
		status = KErrNone;
		for ( TInt i =  aOrderedFeatureList.Count() - 1; 
		      ( i >= 0 ) && ( !status ); 
		      i-- )
			{
			// Find always starts at index 0
			TInt index = aOrderedFeatureList.Find( 
				aOrderedFeatureList[i], CCaeEngineImp::Match ); 
			if (index != i)
				{
				status = KErrAlreadyExists;
				}
			}
		
		// Finally, update internal array
		if ( !status )
			{
			DoSetOrderL( aOrderedFeatureList, ETrue );
			}
		}

	User::LeaveIfError( status );
	}


// -----------------------------------------------------------------------------
// CCaeEngineImp::DoSetOrderL
//
// For each item the old item is removed and the new appended to the end of 
// iExtInterfaceImplementationLists[i].iImplementations.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::DoSetOrderL( 
	const RCaeOrderedFeatureList& aOrderedFeatureList, 
	TBool iUpdate )
    {
	TInt status = KErrNone;

	// For all items in given array
    for (TInt i = 0; i < aOrderedFeatureList.Count() && !status; i++ )
        {
        TUid interfaceUid = aOrderedFeatureList[i].iServiceUid;
        TUid itemImplUid = aOrderedFeatureList[i].iFeatureUid;

		// Find correct interface list

		TCaeExtensionInterfaceImplListItem listItem;
		listItem.iInterfaceUid = interfaceUid;
		listItem.iImplementations =  NULL;
		TInt index = iExtInterfaceImplementationLists.Find( listItem );
		if ( index < 0 )
			{
			status = index; // Invalid interface uid
			}
		else
			{
			
			// Find implementation by uid

			TCaeExtensionInterfaceImplItem item;
			item.iInitialPriority = 0;
			item.iImplUid.iUid = itemImplUid.iUid;
			item.iImplPtr = 0;
			item.iIsActive = ETrue;
			RArray<TCaeExtensionInterfaceImplItem>& implementations = 
				*( iExtInterfaceImplementationLists[index].iImplementations );

			TInt itemIndex = implementations.Find( item, CCaeEngineImp::MatchEqualImplUid );
			if ( itemIndex < 0 ) 
				{
				status = itemIndex; // Invalid feature/implementation uid
				}
			else
				{
				if ( iUpdate )
					{
					// Remove current item and add new implementation item to 
					// end of the array

					item = implementations[itemIndex];

					implementations.Remove( itemIndex );

					status = implementations.Append( item );
					}
				}
			}
        }

    User::LeaveIfError( status );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::MatchEqualImplUid 
// Compares two extension array items
// -----------------------------------------------------------------------------
//
TBool CCaeEngineImp::MatchEqualImplUid( 
    const TCaeExtensionInterfaceImplItem& aFirst, 
    const TCaeExtensionInterfaceImplItem& aSecond )
    {
	return ( aFirst.iImplUid.iUid == aSecond.iImplUid.iUid ); 
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::MatchEqualImplPtr 
// Compares two extension array items
// -----------------------------------------------------------------------------
//
TBool CCaeEngineImp::MatchEqualImplPtr( 
    const TCaeExtensionInterfaceImplItem& aFirst, 
    const TCaeExtensionInterfaceImplItem& aSecond )
    {
	return ( aFirst.iImplPtr == aSecond.iImplPtr ); 
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::MatchSmallerInitialPriorityAndImplUid 
// Compares two extension array items
// -----------------------------------------------------------------------------
//
TBool CCaeEngineImp::MatchSmallerInitialPriorityAndImplUid( 
    const TCaeExtensionInterfaceImplItem& aFirst, 
    const TCaeExtensionInterfaceImplItem& aSecond )
    {
	if (  ( aSecond.iInitialPriority == aFirst.iInitialPriority )
		  && ( aSecond.iImplUid.iUid < aFirst.iImplUid.iUid ) )
		{
		return ETrue;
		}

	else if ( aSecond.iInitialPriority < aFirst.iInitialPriority )
		{
		return ETrue;
		}

	else
		{
		return EFalse;
		}
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::Match 
// Compares two extension array items
// -----------------------------------------------------------------------------
//
TBool CCaeEngineImp::Match( 
    const TCaeOrderedFeatureListItem& aFirst, 
    const TCaeOrderedFeatureListItem& aSecond )
    {
	return ( ( aFirst.iServiceUid == aSecond.iServiceUid ) 
		     && ( aFirst.iFeatureUid == aSecond.iFeatureUid ) ); 
    }


// From MCaeExtEngineInfoCallbackInterface

// -----------------------------------------------------------------------------
// CCaeEngineImp::McaeExtStillImageSize
// Returns current still image size.
// -----------------------------------------------------------------------------
//
TSize CCaeEngineImp::McaeExtStillImageSize()
	{
	return( iStillStatesActive->StillImageSize() );
	}

// -----------------------------------------------------------------------------
// CCaeEngineImp::McaeExtSnapImageSize
// Returns current snap image size.
// -----------------------------------------------------------------------------
//
TSize CCaeEngineImp::McaeExtSnapImageSize()
	{
	return( iStillStatesActive->SnapImageSize() );
	}

// -----------------------------------------------------------------------------
// CCaeEngineImp::McaeExtViewFinderSize
// Returns current view finder size.
// -----------------------------------------------------------------------------
//
TSize CCaeEngineImp::McaeExtViewFinderSize()
	{
	return( iTrueViewFinderSize );
	}

// -----------------------------------------------------------------------------
// CCaeEngineImp::McaeExtCheckPowerL
// Check camera power.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::McaeExtCheckPowerL()
	{
	CheckPowerL();
	}

// -----------------------------------------------------------------------------
// CCaeEngineImp::McaeExtDisplayIndex
// Returns display index.
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::McaeExtDisplayIndex()
	{
    LOGTEXT2( _L( "Cae: CCaeEngineImp::McaeExtDisplayIndex() iDisplayIndex=%d" ), iDisplayIndex );
	return( iDisplayIndex );
	}

// From MExtensionCallbackInterface

// -----------------------------------------------------------------------------
// CCaeEngineImp::IsInterfaceSupported
// Check that the interface is supported. 
// -----------------------------------------------------------------------------
//
TBool CCaeEngineImp::IsInterfaceSupported( TUid aInterfaceUid )
	{
	TBool found = EFalse;
	TInt i = 0;

	TCaeExtensionInterfaceImplListItem listItem;
	listItem.iInterfaceUid = aInterfaceUid;
	listItem.iImplementations =  NULL;

	// Check extension interface uids
	if ( iExtInterfaceImplementationLists.Find( listItem ) >= 0 )
		{
		found = ETrue;
		}

	// Check callback interface uids
	for ( i = 0; (KCaeSupportedCallbackInterfaceUids[i].iUid != NULL) && !found; i++ )
		{
		if ( KCaeSupportedCallbackInterfaceUids[i] == aInterfaceUid )
			{
			found = ETrue;
			}
		}

	return found;
	}

// -----------------------------------------------------------------------------
// CCaeEngineImp::AddExtensionInterface
// Add an extension implemented interface to the list.
// Note: Returns KErrNotFound if the interface is not supported.
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::AddExtensionInterface( 
	TUid aInterfaceUid, 
	TUid aExtensionUid, 
	TAny* aImplementationPtr, 
	TInt aInitialPriority )
	{
    LOGTEXT3( _L( "Cae: CCaeEngineImp::AddExtensionInterface() entering: ifUid=%x, exUid=%x" ), aInterfaceUid.iUid, aExtensionUid.iUid );

	TInt status = KErrNone;

	// Check for invalid pointer
	if ( !aImplementationPtr )
		{
		status = KErrArgument;
		}
	else
		{

		// Add implementation to the list

		TCaeExtensionInterfaceImplListItem listItem;
		listItem.iInterfaceUid = aInterfaceUid;
		listItem.iImplementations =  NULL;

		TInt index = iExtInterfaceImplementationLists.Find( listItem );
		if ( index < 0 )
			{
			status = index; // Error code
			}
		else
			{

			// Add new implementation item to the array

			RArray<TCaeExtensionInterfaceImplItem>& implementations = 
				*( iExtInterfaceImplementationLists[index].iImplementations );

			TCaeExtensionInterfaceImplItem item;
			item.iInitialPriority = aInitialPriority;
			item.iImplUid  = aExtensionUid;
			item.iImplPtr  = aImplementationPtr;
			item.iIsActive = ETrue;

			// Check for duplicates
			TInt itemIndex = implementations.Find( item, CCaeEngineImp::MatchEqualImplUid );
			if ( itemIndex >= 0 ) 
				{
				status = KErrAlreadyExists; // Error code
				}
			else
				{
				itemIndex = implementations.Find( 
				    item, CCaeEngineImp::MatchSmallerInitialPriorityAndImplUid );
				if ( itemIndex < 0 ) 
					{
					// All have bigger or equal priority. Add to the end of list.
					itemIndex = implementations.Count();
					}

				// Finally, add item to the list
				status =  implementations.Insert( item, itemIndex ); 
				}
			}
		}

    LOGTEXT2( _L( "Cae: CCaeEngineImp::AddExtensionInterface() returning: status=%d" ), status );

	return( status );
	}

// -----------------------------------------------------------------------------
// CCaeEngineImp::RemoveExtensionInterface
// Remove an extension implemented interface from the list.
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::RemoveExtensionInterface( 
	TUid aInterfaceUid, 
	TAny* aImplementationPtr )
	{
    LOGTEXT3( _L( "Cae: CCaeEngineImp::RemoveExtensionInterface() entering: ifUid=%x, ptr=%d" ), aInterfaceUid.iUid, aImplementationPtr );
	TInt status = KErrNone;

	// Check for invalid pointer
	if ( !aImplementationPtr )
		{
		status = KErrArgument;
		}
	else
		{
		// Remove implementation to the list
		TCaeExtensionInterfaceImplListItem listItem;
		listItem.iInterfaceUid = aInterfaceUid;
		listItem.iImplementations =  NULL;

		TInt index = iExtInterfaceImplementationLists.Find( listItem );

		if ( index < 0 )
			{
			status = index; // Error code
			}
		else
			{
			TCaeExtensionInterfaceImplItem item;
			item.iInitialPriority = 0;
			item.iImplUid.iUid = 0;
			item.iImplPtr = aImplementationPtr;
			item.iIsActive = ETrue;
			
			RArray<TCaeExtensionInterfaceImplItem>& implementations = 
				*( iExtInterfaceImplementationLists[index].iImplementations );

			TInt itemIndex = implementations.Find( item, CCaeEngineImp::MatchEqualImplPtr );

			if ( itemIndex < 0 ) 
				{
				status = itemIndex; // Return error
				}
			else
				{
				implementations.Remove( itemIndex );
				}
			}
		}

    LOGTEXT2( _L( "Cae: CCaeEngineImp::RemoveExtensionInterface() returning: status=%d" ), status );

	return( status );
	}

// -----------------------------------------------------------------------------
// CCaeEngineImp::AddCustomInterface
// Add an extension implemented custom interface to the list.
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::AddCustomInterface( 
    TUid aInterfaceUid, 
    TAny* aImplementationPtr )
	{
    LOGTEXT3( _L( "Cae: CCaeEngineImp::AddCustomInterface() entering: ifUid=%x, ptr=%d" ), aInterfaceUid.iUid, aImplementationPtr );

	TInt status = KErrNone;

	if ( !aImplementationPtr )
		{
		status = KErrArgument;
		}
	else
		{
		TCaeCustomInterfaceImplItem item;
		item.iInterfaceUid = aInterfaceUid;
		item.iImplPtr = aImplementationPtr;
		
	    // Do not allow duplicate uid's
		status = iExtCustomInterfaceImplementations.InsertInUnsignedKeyOrder( item ); 	
		}
	
    LOGTEXT2( _L( "Cae: CCaeEngineImp::AddCustomInterface() returning: status=%d" ), status );

	return ( status );
	}

// -----------------------------------------------------------------------------
// CCaeEngineImp::GetCallbackInterface
// Return an engine implemented callback interface.
// -----------------------------------------------------------------------------
//
TAny* CCaeEngineImp::GetCallbackInterface( TUid aInterfaceUid )
	{
    LOGTEXT2( _L( "Cae: CCaeEngineImp::GetCallbackInterface() entering: ifUid=%x" ), aInterfaceUid.iUid );
	TAny* callbackInterface =  NULL;

	if ( aInterfaceUid == KCaeExtEngineInfoCallbackInterfaceUid )
		{
		callbackInterface = (MCaeExtEngineInfoCallbackInterface*) this;
		}

    LOGTEXT2( _L( "Cae: CCaeEngineImp::GetCallbackInterface() returning: callbackInterface=%d" ), callbackInterface );
	return ( callbackInterface );
	}

// -----------------------------------------------------------------------------
// CCaeEngineImp::RegisterFlags
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::RegisterFlags( TUint32 aFlags )
	{
    LOGTEXT2( _L( "Cae: CCaeEngineImp::RegisterFlags() aFlags=%x" ), aFlags );
    iStillStatesActive->RegisterFlags( aFlags );
	}


// -----------------------------------------------------------------------------
// CCaeEngineImp::DeregisterFlags
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::DeregisterFlags( TUint32 aFlags )
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::DeregisterFlags() aFlags=%x" ), aFlags );
    iStillStatesActive->DeregisterFlags( aFlags );    
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::CCameraHandle
// Gets handle of camera object owned by the CCaeEngine.
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::CCameraHandle() const
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::CCameraHandle(): %d" ), iCameraHandle );
    return ( iCameraHandle );
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::SetCaeExtensionModeL
// Sets CAE to Extension mode
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetCaeExtensionModeL( TBool aExtModeActive, TBool aCreateSnapImage )
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::SetCaeExtensionModeL(): %d, %d" ), aExtModeActive, aCreateSnapImage );

    iStillStatesActive->SetExtModeActiveL( aExtModeActive );
    iStillStatesActive->SetSnapImageCreation( aCreateSnapImage );
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::ProcessExtViewFinderFrameReadyL
// Process extension for view finder
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ProcessExtViewFinderFrameReadyL( CFbsBitmap& aFrame )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtViewFinderFrameReadyL()" ));

    // Leave if not in extension mode.
    if ( !iStillStatesActive->ExtModeActive() )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtViewFinderFrameReadyL() leaving KErrPermissionDenied" ) );
        User::Leave( KErrPermissionDenied );
        }

    ViewFinderFrameReady( aFrame );
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::ProcessExtSnapImageL
// Process extension for snap image
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ProcessExtSnapImageL( CFbsBitmap& aSnapImage )
    {
    TSize sizeEmpty;
    CFbsBitmap* bitmapSave = NULL;
    TSize snapSize = aSnapImage.SizeInPixels();
    
    LOGTEXT3( _L( "Cae: CCaeEngineImp::ProcessExtSnapImageL() size wxh=%dx%d" ),
            snapSize.iWidth, snapSize.iHeight );
        
    // Leave if not in extension mode.
    if ( !iStillStatesActive->ExtModeActive() )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtSnapImageL() leaving KErrPermissionDenied" ) );
        User::Leave( KErrPermissionDenied );
        }

    // Create a duplicate bitmap
    if ( snapSize != sizeEmpty ) 
        {
        bitmapSave = new CFbsBitmap;
        if ( bitmapSave )
            {
            TInt err = bitmapSave->Duplicate( aSnapImage.Handle() );
            if ( err ) 
                {
                    LOGTEXT2( _L( "Cae: CCaeEngineImp::ProcessExtSnapImageL(): Duplicate error:%d, leaving" ), err);
                    delete bitmapSave;
                    bitmapSave = NULL;
                    User::Leave( err );
                 }
            }
        else
            {
            LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtSnapImageL(): new CFbsBitmap failed, leaving KErrNoMemory" ));
            User::Leave( KErrNoMemory );
            }
        }
    else
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtSnapImageL(): got empty bitmap, ignore" ));
        }
        
    if ( bitmapSave )
        {
        // Send bitmap for extension handling
        TInt result = iStillStatesActive->ProcessExtSnapImage( bitmapSave );
        if ( result )
            {
            LOGTEXT2( _L( "Cae: CCaeEngineImp::ProcessExtSnapImageL(): ProcessExtSnapImage failed %d, leaving" ), result);
            delete bitmapSave;
            bitmapSave = NULL;
            User::Leave( result );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::ProcessExtCapturedImageL
// Process extension for captured image
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ProcessExtCapturedImageL( CFbsBitmap& aBitmap, TBool aLastImage )
    {
    TSize sizeEmpty;
    CFbsBitmap* bitmapSave = NULL;
    TSize bitmapSize = aBitmap.SizeInPixels();
    
    LOGTEXT3( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL() CFbsBitmap ExtMode=%d, last=%d" ), iStillStatesActive->ExtModeActive(), aLastImage);
    LOGTEXT3( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL() bitmap size WxH=%dx%d" ), bitmapSize.iWidth, bitmapSize.iHeight );
        
    // Leave if not in extension mode.
    if ( !iStillStatesActive->ExtModeActive() )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL leaving KErrPermissionDenied" ));
        User::Leave( KErrPermissionDenied );
        }

    // Create a duplicate bitmap
    if ( bitmapSize != sizeEmpty )
        {
        bitmapSave = new CFbsBitmap;
        if ( bitmapSave )
            {
            TInt err = bitmapSave->Duplicate( aBitmap.Handle() );
            if ( err ) 
                {
                    LOGTEXT2( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL(): Duplicate error:%d, leaving" ), err);
                    delete bitmapSave;
                    bitmapSave = NULL;
                    User::Leave( err );
                 }
            }
        else
            {
            LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL(): new CFbsBitmap failed, leaving KErrNoMemory" ));
            User::Leave( KErrNoMemory );
            }

        }
    else
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL(): got empty bitmap, ignore" ));
        }
        
    if ( bitmapSave )
        {
        // Send bitmap for extension handling
        TInt result = iStillStatesActive->ProcessExtCapturedImage( bitmapSave, aLastImage );
        if ( result )
            {
            LOGTEXT2( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL(): ProcessExtCapturedImage failed %d, leaving" ), result);
            delete bitmapSave;
            bitmapSave = NULL;
            User::Leave( result );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::ProcessExtCapturedImageL
// Process extension for captured image
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ProcessExtCapturedImageL( HBufC8* aImageData, TBool aLastImage )
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL() HBufC8 ExtMode=%d, last=%d" ), iStillStatesActive->ExtModeActive(), aLastImage);
    // Leave if not in extension mode.
    if ( !iStillStatesActive->ExtModeActive() )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL leaving KErrPermissionDenied" ));
        User::Leave( KErrPermissionDenied );
        }
    if ( aImageData )
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL() processing HBufC8 size=%d" ), aImageData->Size() );
        TInt result = iStillStatesActive->ProcessExtCapturedImage( aImageData, aLastImage );
        if ( result )
            {
            LOGTEXT2( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL(): ProcessExtCapturedImage failed %d, leaving" ), result);
            delete aImageData;
            aImageData = NULL;
            User::Leave( result );
            }
        }
    else
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL(): got empty HBufC8, ignore" ));
        }
    
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::ProcessExtCapturedImageL
// Process extension for captured image
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ProcessExtCapturedImageL( TDesC8& aImageDataDes, TBool aLastImage )
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL() TDesC8 ExtMode=%d, last=%d" ), iStillStatesActive->ExtModeActive(), aLastImage);
    // Leave if not in extension mode.
    if ( !iStillStatesActive->ExtModeActive() )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL leaving KErrPermissionDenied" ));
        User::Leave( KErrPermissionDenied );
        }
    // TDesC8 is used with C-API V2 features
    if ( aImageDataDes.Size() )
        {
        // Make a new HBufC8 copy from the TDesC8
        HBufC8* tempImageData = aImageDataDes.AllocL();
        // Do not delete aImageDataDes, it is part of the class MCameraBuffer
        LOGTEXT2( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL() New HBufC8 created, size=%d" ), tempImageData->Size() );
        
        TInt result = iStillStatesActive->ProcessExtCapturedImage( tempImageData, aLastImage );
        if ( result )
            {
            LOGTEXT2( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL(): ProcessExtCapturedImage failed %d, leaving" ), result);
            delete tempImageData;
            tempImageData = NULL;
            User::Leave( result );
            }
        }
    else
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtCapturedImageL(): got empty TDesC8, ignore" ));
        }
    
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::ProcessExtCancel
// Cancels processing
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ProcessExtCancel()
    {
    CancelCaptureStill();
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SkippedExtensionListL
// Get list of skipped extensions. Application may define which extensions are
// skipped during image processing. Only installed and skipped are listed.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SkippedExtensionListL( RCaeOrderedFeatureList& aDisabledExtensions )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SkippedExtensionListL() entering" ) );
    TCaeOrderedFeatureListItem tmpItem;
    aDisabledExtensions.Reset();
    
	// For all interface implementation lists
    for ( TInt implListIndex = 0; 
           implListIndex < iExtInterfaceImplementationLists.Count(); 
          implListIndex++ )
        {
		TCaeExtensionInterfaceImplListItem* implListItem = 
			&(iExtInterfaceImplementationLists[implListIndex]);
		RArray<TCaeExtensionInterfaceImplItem>& implementations 
		    = *(implListItem->iImplementations);

		// For all implementations
		for ( TInt implIndex = 0; implIndex < implementations.Count(); implIndex++ )
			{
			// Add the item to the skipped array if it is not active
			if ( !implementations[implIndex].iIsActive )
			    {
			    tmpItem.iServiceUid = implListItem->iInterfaceUid;
			    tmpItem.iFeatureUid = implementations[implIndex].iImplUid;
			    aDisabledExtensions.AppendL( tmpItem );
                LOGTEXT3( _L( "Cae: CCaeEngineImp::SkippedExtensionListL(), serviceUid %x, featureUid %x" ),
                    tmpItem.iServiceUid.iUid, tmpItem.iFeatureUid.iUid  );
			    }
			}
        }
    LOGTEXT2( _L( "Cae: CCaeEngineImp::SkippedExtensionListL() returning, count %d" ), aDisabledExtensions.Count() );
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::SetSkippedExtensionList
// Set list of skipped extensions. Application may define which extensions are
// skipped during image processing.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetSkippedExtensionList( RCaeOrderedFeatureList& aDisabledExtensions )
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::SetSkippedExtensionList() entering Count %d" ), aDisabledExtensions.Count() );

	// Set all extensions first active

	// For all interfaces
    for ( TInt implListIndex = 0; 
           implListIndex < iExtInterfaceImplementationLists.Count(); 
          implListIndex++ )
        {
		TCaeExtensionInterfaceImplListItem* implListItem = 
			&(iExtInterfaceImplementationLists[implListIndex]);
		RArray<TCaeExtensionInterfaceImplItem>& implementations 
		    = *(implListItem->iImplementations);

		// For all implementations
		for ( TInt implIndex = 0; implIndex < implementations.Count(); implIndex++ )
			{
			implementations[implIndex].iIsActive = ETrue;
			}
        }

    // Disable wanted extensions
    
	// For all items in given array
    for (TInt i = 0; i < aDisabledExtensions.Count(); i++ )
        {
        TUid interfaceUid = aDisabledExtensions[i].iServiceUid;
        TUid itemImplUid = aDisabledExtensions[i].iFeatureUid;

		// Find correct interface list

		TCaeExtensionInterfaceImplListItem listItem;
		listItem.iInterfaceUid = interfaceUid;
		listItem.iImplementations =  NULL;
		TInt index = iExtInterfaceImplementationLists.Find( listItem );
		if ( index >= 0 ) // Interface was found
			{
			// Find implementation by uid

			TCaeExtensionInterfaceImplItem item;
			item.iInitialPriority = 0;
			item.iImplUid.iUid = itemImplUid.iUid;
			item.iImplPtr = 0;
			item.iIsActive = ETrue;
			RArray<TCaeExtensionInterfaceImplItem>& implementations = 
				*( iExtInterfaceImplementationLists[index].iImplementations );

			TInt itemIndex = implementations.Find( item, CCaeEngineImp::MatchEqualImplUid );
			if ( itemIndex >= 0 ) 
				{
				implementations[itemIndex].iIsActive = EFalse;
                LOGTEXT3( _L( "Cae: CCaeEngineImp::SetSkippedExtensionList(), serviceUid %x, featureUid %x" ),
                    interfaceUid.iUid, itemImplUid.iUid  );
				}
			}
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::SetSkippedExtensionList() returning" ) );
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::EnableVideoRecording
// Set CAE internal camera status flags.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::EnableVideoRecording()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::EnableVideoRecording() entering" ) );

    // Set the flags to enable the video usage without InitL() call
    iReserved = ETrue;
    iPowerOn = ETrue;

    LOGTEXT( _L( "Cae: CCaeEngineImp::EnableVideoRecording() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::DisableVideoRecording
// Set CAE internal camera status flags.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::DisableVideoRecording()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::DisableVideoRecording() entering" ) );

    if ( iReserved ) 
        {
        if ( iPowerOn ) 
            {
            CancelAllActivities();
            iPowerOn = EFalse;
            }
        iReserved = EFalse;
        iStillPrepared = EFalse;
        iVideoPrepared = EFalse;
        iVideoOpened = EFalse;
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::DisableVideoRecording() returning" ) );
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::HandleEvent
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::HandleEvent( const TECAMEvent& aEvent)
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::HandleEvent() entering, type=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode );

    if ( iStillStatesActive->ExtModeActive() )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::HandleEvent() Extension mode active, skipping event handling" ) );
        }
    else
        {
        // Cae is in use when capturing images
        if ( aEvent.iEventType == KUidECamEventCameraNoLongerReserved )
            {
            LOGTEXT( _L( "Cae: CCaeEngineImp::HandleEvent() KUidECamEventCameraNoLongerReserved" ) );
            CancelAllActivities();
            iPowerOn = EFalse;
            iReserved = EFalse;
            iStillPrepared = EFalse;
            iVideoPrepared = EFalse;
            iVideoOpened = EFalse;
            iCaeObserver->McaeoInitComplete( KErrInUse ); // Tell the client that other application has taken the camera
            }
        else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
    	    {
            LOGTEXT( _L( "Cae: CCaeEngineImp::HandleEvent() KUidECamEventPowerOnComplete" ) );
            PowerOnComplete( aEvent.iErrorCode );
	        }
    	else if ( aEvent.iEventType == KUidECamEventReserveComplete )
    	    {
            LOGTEXT( _L( "Cae: CCaeEngineImp::HandleEvent() KUidECamEventReserveComplete" ) );
            ReserveComplete( aEvent.iErrorCode );
    	    }
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::HandleEvent() returning" ) );
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::ViewFinderReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ViewFinderReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::ViewFinderReady() entering, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() );

    if ( !aError )
        {
        if ( aCameraBuffer.NumFrames() > 0 )
	        {
            CFbsBitmap* bitmap = NULL;
	        TRAPD( err, bitmap = &aCameraBuffer.BitmapL( 0 ) );
	        if ( !err )
	            {
	            ViewFinderFrameReady( *bitmap );
	            }
	        else
	            {
                LOGTEXT2( _L( "Cae: CCaeEngineImp::ViewFinderReady() BitmapL failed err=%d" ), err );
	            }
	        }
	    aCameraBuffer.Release();
        }
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::ImageBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ImageBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::ImageBufferReady() entering, err=%d, NumFrames=%d" ), aError, aCameraBuffer.NumFrames() );
	
    if ( !aError )
        {
        TDesC8* data = NULL;
        TRAPD( err, data = aCameraBuffer.DataL( 0 ) );
        if ( err == KErrNotSupported )
            {
            // Got the image as a CFbsBitmap
            CFbsBitmap* bitmap = NULL;

            TRAP( err, bitmap = &aCameraBuffer.BitmapL( 0 ) );

            if ( !err )
                {
                // Create a duplicate bitmap
                CFbsBitmap* bitmapSave = NULL;
                bitmapSave = new CFbsBitmap;
                if ( bitmapSave )
                    {
                    err = bitmapSave->Duplicate( bitmap->Handle() );
                    if ( !err ) 
                        {
                        // All OK, send duplicated bitmap to old observer interface
                        ImageReady( bitmapSave, NULL, aError );
                        }
                    else
                        {
                        LOGTEXT2( _L( "Cae: CCaeEngineImp::ImageBufferReady(): Duplicate error:%d" ), err);
                        delete bitmapSave;
                        bitmapSave = NULL;
                        ImageReady( NULL, NULL, err );
                        }
                    }
                else
                    {
                    LOGTEXT( _L( "Cae: CCaeEngineImp::ImageBufferReady(): new CFbsBitmap failed" ));
                    ImageReady( NULL, NULL, KErrNoMemory );
                    }

                }
            else
                {
                LOGTEXT( _L( "Cae: CCaeEngineImp::ImageBufferReady(): BitmapL failed" ));
                ImageReady( NULL, NULL, err );
                }
            }
        else if ( !err )
            {
            // Got the image as a descriptor
            // Make a new HBufC8 copy from the TDesC8
            HBufC8* tempImageData = NULL;
            tempImageData = data->Alloc();
            
            if ( tempImageData )
                {
                // All OK, send buffer to old observer interface
                LOGTEXT2( _L( "Cae: CCaeEngineImp::ImageBufferReady() New HBufC8 created, size=%d" ), tempImageData->Size() );
                ImageReady( NULL, tempImageData, aError );
                }
            else
                {
                LOGTEXT( _L( "Cae: CCaeEngineImp::ImageBufferReady() Alloc of HBufC8 failed" ) );
                ImageReady( NULL, NULL, KErrNoMemory );
                }
            
            
            }
        else
            {
            LOGTEXT2( _L( "Cae: CCaeEngineImp::ImageBufferReady() DataL returned error code=%d" ), err );
            ImageReady( NULL, NULL, err );
            }

        // Release buffer only when no error
	    aCameraBuffer.Release();
        }
    else
        {
        ImageReady( NULL, NULL, aError );
        }
    }
    
// -----------------------------------------------------------------------------
// CCaeEngineImp::VideoBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::VideoBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::VideoBufferReady() entering, err=%d" ), aError );
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    }


//  End of File  
