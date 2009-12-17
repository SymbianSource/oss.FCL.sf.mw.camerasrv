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
* Description:  Camera Application Engine implementation information getting method
*
*/



// INCLUDE FILES

#include "CaeEngineImp.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaeEngineImp::InitializeInfo
// Initialize Camera Application Engine information class.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::InitializeInfo( 
    const CCamera& aCamera )
    {

    // Initialise Camera (API) info.

    TCameraInfo cameraInfo;
    aCamera.CameraInfo( cameraInfo );

    iInfo->iCaeOptionsSupported = 0;

    iInfo->iHardwareVersion = cameraInfo.iHardwareVersion;
    iInfo->iSoftwareVersion = cameraInfo.iSoftwareVersion;

    iInfo->iOrientation = cameraInfo.iOrientation;

    iInfo->iOptionsSupported = cameraInfo.iOptionsSupported;
    
    iInfo->iFlashModesSupported = cameraInfo.iFlashModesSupported;

    iInfo->iExposureModesSupported = cameraInfo.iExposureModesSupported;

    iInfo->iWhiteBalanceModesSupported = cameraInfo.iWhiteBalanceModesSupported;

    iInfo->iMinZoom = cameraInfo.iMinZoom;
    iInfo->iMaxZoom = cameraInfo.iMaxZoom;
    iInfo->iMaxDigitalZoom = cameraInfo.iMaxDigitalZoom;

    iInfo->iMinZoomFactor = cameraInfo.iMinZoomFactor;
    iInfo->iMaxZoomFactor = cameraInfo.iMaxZoomFactor;
    iInfo->iMaxDigitalZoomFactor = cameraInfo.iMaxDigitalZoomFactor;

    iInfo->iNumImageSizesSupported = cameraInfo.iNumImageSizesSupported;
    iInfo->iImageFormatsSupported = cameraInfo.iImageFormatsSupported;

    // Initialize EV compensation info (no supported in this version)
    iInfo->iMinEvCompensation = 0;
    iInfo->iMaxEvCompensation = 0;
    iInfo->iMinEvCompensationValue = 0;
    iInfo->iMaxEvCompensationValue = 0;

    // Initialize engine info.

    iInfo->iNumStillQualityLevelsSupported = 0; // Quality levels are initialized separately.
    iInfo->iNumVideoQualityLevelsSupported = 0; // Quality levels are initialized separately.

    iInfo->iCaeOptionsSupported |= StillBurstSupport();
    }

//  End of File  
