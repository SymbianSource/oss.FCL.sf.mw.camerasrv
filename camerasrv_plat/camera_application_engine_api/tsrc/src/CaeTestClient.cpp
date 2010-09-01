/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: CaeTestClient.cpp
*
*/

#include <fbs.h>

#include "CaeTestClient.h"
#include "CaeTestVariation.h"

// Set 0
_LIT8(KVideoMimeType, "video/3gpp");
_LIT(KVideoPreferredSupplier, "Nokia");
_LIT8(KVideoType, "video/H263-2000");
_LIT8(KVideoAudioType, " AMR");
// Set 1 for 2.7 
_LIT8(KVideoMimeType1, "video/3gpp2");
_LIT(KVideoPreferredSupplier1, "Nokia");
_LIT8(KVideoType1, "video/H263-2000"); // H.263 video
_LIT8(KVideoAudioType1, " AMR");       // AMR audio
// Set 2 for 2.7 AAC
_LIT8(KVideoMimeType2, "video/3gpp2");
_LIT(KVideoPreferredSupplier2, "Nokia");
_LIT8(KVideoType2, "video/mp4v-es");   // MPEG-4 video
_LIT8(KVideoAudioType2, " AAC");       // AAC audio
// Set 3 for 2.8
_LIT8(KVideoMimeType3, "video/3gpp");
_LIT(KVideoPreferredSupplier3, "Nokia");
_LIT8(KVideoType3, "video/H263-2000; profile=0; level=45");
_LIT8(KVideoAudioType3, " AMR");


CCaeTestClient::CCaeTestClient () : 
    CTimer( CActive::EPriorityStandard )
    {
    }


CCaeTestClient::~CCaeTestClient()
    {
    Cancel();
    delete iCameraDup;
    delete iCamAppEngine;
    
    iFsSession.Close();
    RFbsSession::Disconnect();
    delete iBitmapSave;
    delete iDataSave;

    }


void CCaeTestClient::ConstructL()
    {
    // Initializing: FBS session, bitmaps and so on.
    User::LeaveIfError( FbsStartup() ); // Ensure Fbserv has started up OK
    User::LeaveIfError( RFbsSession::Connect() );
 
    // Connect and open output data file.
    #if ( WRITE_VIDEOTIMES_TO_FILE )
    User::LeaveIfError( iFsSession.Connect() );
    User::LeaveIfError( iFile.Replace( iFsSession, KProcessFileName, EFileShareAny ) );
    TInt aPos;
    User::LeaveIfError( iFile.Seek( ESeekEnd, aPos ) );
    iFile.Write( _L8( "CaeTestClient output data\n" ) );
    iFile.Close();
    #endif

    iSnapFileName  = KiSnapFileName;
    iStillFileName = KiStillFileName;
    iVideoFileName = KiVideoFileName;
    iVideoFileName2 = KiVideoFileName2;
    iVideoFileName3 = KiVideoFileName3;
    iVideoFileName4 = KiVideoFileName4;
    
    iCreateSnapImage = ETrue;
    
    TRect emptyRect( 0, 0, 0, 0 );
    iCropRect = emptyRect;

    TSize sizeEmpty;
    iSizeEmpty = sizeEmpty;

    TSize sizeSubQcif( KImgWidthSubQCIF, KImgHeightSubQCIF );
    iViewFinderSize    = sizeSubQcif;

    iStillFrameSize    = TSize( KDefaultStillImageWidth, KDefaultStillImageHeight );

	iSnapImageSize = TSize(0, 0);

    iSnapImageSource = CCaeEngine::ESnapImageSourceOriginal;
    iStillDataFormat = KDefaultStillDataFormat;
	iSnapImageColorMode = (TDisplayMode)0;

    iStillCompressionQuality = 95;

    iStillBurstLength = 1;

    iVideoFrameSize = sizeSubQcif;
    iBitmapSave = NULL;
    iDataSave = NULL;

    iCamAppEngine = CCaeEngine::NewL();

// If interface after S60 2.1.
#ifndef CAE_INTERFACE_21
    if ( iAction != ESpecifiedPrepareAndRecordVideoNoInit )
        {
        // For testing with the version inputting camera index and 
        // CCaeEngine::CamerasAvailable().
        delete iCamAppEngine;
        iCamAppEngine = NULL;
        if ( CCaeEngine::CamerasAvailable() > 0 ) 
            {
            iCamAppEngine = CCaeEngine::NewL( 0 );
            }
        else 
            {
            User::Leave( KErrHardwareNotAvailable );
            }
    
        }
#endif

    iCamAppEngine->SetCamAppEngineObserver( *this );
    iCamAppEngine->SetCaeStillBurstObserver( *this );

    iTimeout = 500000;

	iCamAppEngine->GetInfo( iInfo );

    iOpMode = EClientOpModeStill;

	iStopOnErrorInMcaeoSnapImageReady = ETrue;
	iStopOnErrorInMcaeoStillImageReady = ETrue;

    iVideoCodecSet = 0; // default set in use

    //set 0
    iMimeTypeSet[0]          = &KVideoMimeType;
    iPreferredSupplierSet[0] = &KVideoPreferredSupplier;
    iVideoTypeSet[0]         = &KVideoType;
    iAudioTypeSet[0]         = &KVideoAudioType;

    //set 1
    iMimeTypeSet[1]          = &KVideoMimeType1;
    iPreferredSupplierSet[1] = &KVideoPreferredSupplier1;
    iVideoTypeSet[1]         = &KVideoType1;
    iAudioTypeSet[1]         = &KVideoAudioType1;

    //set 2
    iMimeTypeSet[2]          = &KVideoMimeType2;
    iPreferredSupplierSet[2] = &KVideoPreferredSupplier2;
    iVideoTypeSet[2]         = &KVideoType2;
    iAudioTypeSet[2]         = &KVideoAudioType2;

    //set 3
    iMimeTypeSet[3]          = &KVideoMimeType3;
    iPreferredSupplierSet[3] = &KVideoPreferredSupplier3;
    iVideoTypeSet[3]         = &KVideoType3;
    iAudioTypeSet[3]         = &KVideoAudioType3;

    CTimer::ConstructL();
    CActiveScheduler::Add( this );
    }


void CCaeTestClient::InitStateVariables()
    {
    iError = KErrNone;

    iStop                   = EFalse;

    iStopWhenCompleted      = EFalse;
    iContinueWhenCompleted  = EFalse;

    iStopViewFinder         = EFalse;
    
    iRunlForSnapImage       = EFalse;
    iRunlForStillImage      = EFalse;

    iInitReady              = EFalse;
    iPowerOn                = EFalse;

    iViewFinderStarted      = EFalse;
    iViewFindingUsed        = EFalse;

    iStillPrepareReady      = EFalse;
    iSnapImageReady         = EFalse;
    iStillCaptureReady      = EFalse;
    iStillCapturingOn       = EFalse;
    iStillCaptured          = EFalse;

    iStillBurstCaptureMoment = EFalse;
    iStillBurstReady        = EFalse;
    iStopStillBurst         = EFalse;
    iCancelStillBurst       = EFalse;
    iCancelStillBurstAfterVfFrame = EFalse;
    
    iVideoPrepareReady      = EFalse;
    iVideoRecordingReady    = EFalse;
    iVideoRecordingPaused   = EFalse;
    iVideoRecordingResumed  = EFalse;
    iVideoRecordingOn       = EFalse;
    iVideoRecorded          = EFalse;

	iStoppedOnError			= EFalse;

    iAcceptVideoCompletionError = EFalse;    

    iVfFrameCount           = 0;
    
    iImageCountToCapture    = 1;
    
    iCancelCount            = 0;
    
    iCountInitComplete              = 0;
    iCountStillPrepareComplete      = 0;
    iCountStillBurstCaptureMoment   = 0;
    iCountStillBurstComplete        = 0;
    iCountVideoPrepareComplete      = 0;
    iCountViewFinderFrameReady      = 0;
    iCountSnapImageReady            = 0;
    iCountStillImageReady           = 0;
    iCountVideoRecordingOn          = 0;
    iCountVideoRecordingComplete    = 0;
    iCountVideoRecordingStopped     = 0;
    iCountVideoRecordingPaused      = 0;
    iCountVideoRecordingTimes       = 0;

    #ifdef CAE_TEST_VERSION
    CaeSetMcaeseoHBufC8ImageReadyError( KErrNone );
    CaeSetMcaeseoHBufC8ImageReadyError( KErrNone );
    CaeSetPowerOnCompleteError( KErrNone );
    CaeSetReserveCompleteError( KErrNone );
    CaeSetImageReadyError( KErrNone );
    CaeSetMvruoOpenCompleteError( KErrNone );
    CaeSetMvruoPrepareCompleteError( KErrNone );
    CaeSetMvruoRecordCompleteError( KErrNone );
    CaeSetPrepareVideoSettingsError( KErrNone );
    CaeSetCreateAndDeliverSnapImageError( KErrNone );
    CaeSetCreateAndDeliverStillImageError( KErrNone );
    CaeSetConvertCFbsBitmapToHBufC8Error( KErrNone );
    CaeSetConvertHBufC8ToCFbsBitmapError( KErrNone );
    CaeSetStillConvertError( KErrNone );
    #endif // CAE_TEST_VERSION

    iReleaseDone = EFalse;
    iReserveDone = EFalse;

    iNextRunLTime.HomeTime();

    iExtensionMode        = EFalse;
    iBitmapSendDone       = EFalse;

    }


void CCaeTestClient::SetSnapImageCreation( TBool aCreateSnapImage )
    {
    iCreateSnapImage = aCreateSnapImage;
    }


void CCaeTestClient::SetRunningWithViewFinder( TBool aRunWithViewFinder )
    {
    iRunWithViewFinder = aRunWithViewFinder;
    }


void CCaeTestClient::SetViewFinderSize( const TSize& aSize )
    {
    iViewFinderSize = aSize;
    }


void CCaeTestClient::SetStillQualityLevelIndex( TInt aQualityLevelIndex )
    {
    iStillQualityLevelIndex = aQualityLevelIndex;
    }


void CCaeTestClient::SetVideoQualityLevelIndex( TInt aQualityLevelIndex )
    {
    iVideoQualityLevelIndex = aQualityLevelIndex;
    }


void CCaeTestClient::SetStillFormat( CCamera::TFormat aFormat )
    {
    iStillDataFormat = aFormat;
    }


void CCaeTestClient::SetStillCompressionQuality( TInt aCompressionQuality )
    {
    iStillCompressionQuality = aCompressionQuality;
    }


void CCaeTestClient::SetStillFrameSize( const TSize& aSize )
    {
    iStillFrameSize = aSize;
    }


void CCaeTestClient::SetSnapImageSize( const TSize& aSize )
    {
    iSnapImageSize = aSize;
    }


void CCaeTestClient::SetSnapImageSource( CCaeEngine::TSnapImageSource aSnapImageSource )
    {
    iSnapImageSource = aSnapImageSource;
    }


void CCaeTestClient::SetSnapImageColorMode( TDisplayMode aMode )
    {
    iSnapImageColorMode = aMode;
    }


void CCaeTestClient::SetImageCodecs( TUid aDecoderUid, TUid aEncoderUid )
    {
    iImageDecoderUid = aDecoderUid;
    iImageEncoderUid = aEncoderUid;
    }


void CCaeTestClient::SetCropRect( const TRect& aRect )
    {
    iCropRect = aRect;
    }


void CCaeTestClient::SetStillBurstLength( TInt aLength )
    {
    iStillBurstLength = aLength;
    }


void CCaeTestClient::SetVideoFrameSize( const TSize& aSize )
    {
    iVideoFrameSize = aSize;
    }


void CCaeTestClient::SetVideoFrameRate( TReal32 aFrameRate )
    {
    iVideoFrameRate = aFrameRate;
    }


void CCaeTestClient::SetVideoBitRate( TInt aBitRate )
    {
    iVideoBitRate = aBitRate;
    }


void CCaeTestClient::SetVideoAudio( TBool aAudioEnabled )
    {
    iVideoAudioEnabled = aAudioEnabled;
    }


void CCaeTestClient::SetAudioBitRate( TInt aAudioBitRate )
    {
    iAudioBitRate = aAudioBitRate;
    }


void CCaeTestClient::SetFileNameBase( const TFileName& aFileNameBase ) 
    {
    iFileNameBase = aFileNameBase;
    }


void CCaeTestClient::SetVfFileName( const TFileName& aFileName ) 
    {
    iVfFileName = iFileNameBase;
    iVfFileName.Append ( aFileName );
#if ( WRITE_VF_TO_FILE )
    iVfFrameCountToSave = 1;
#else
    iVfFrameCountToSave = 0;
#endif
    }

void CCaeTestClient::SetVideoStopMode( TBool aAsyncVideoStopEnabled )
    {
    iVideoStopModeCommand = ETrue; // mark for new command
    iAsyncVideoStopEnabled = aAsyncVideoStopEnabled; // new stop mode
    }


void CCaeTestClient::SetVfFileCountToSave( TInt aCountOfVfFilesToSave ) 
    {
    iVfFrameCountToSave = aCountOfVfFilesToSave;
#if ( WRITE_VF_TO_FILE )
    iVfFrameCountToSave = aCountOfVfFilesToSave;
#else
    iVfFrameCountToSave = 0;
#endif
    }


void CCaeTestClient::SetSnapFileName( const TFileName& aFileName ) 
    {
    iSnapFileName = iFileNameBase;
    iSnapFileName.Append ( aFileName );
#if ( WRITE_SNAPIMAGE_TO_FILE )
    iSaveSnapImage = ETrue;
#else
    iSaveSnapImage = EFalse;
#endif
    }


void CCaeTestClient::SetStillFileName( const TFileName& aFileName ) 
    {
    iStillFileName = iFileNameBase;
    iStillFileName.Append ( aFileName );
#if ( WRITE_STILLIMAGE_TO_FILE )
    iSaveStillImage = ETrue;
#else
    iSaveStillImage = EFalse;
#endif
    }


void CCaeTestClient::SetVideoFileName( const TFileName& aFileName ) 
    {
    iVideoFileName = iFileNameBase;
    iVideoFileName.Append ( aFileName );
#if ( WRITE_VIDEOCLIP_TO_FILE )
    iSaveVideoClip = ETrue;
#else
    iSaveVideoClip = EFalse;
#endif
    }


void CCaeTestClient::SetTimeout( TTimeIntervalMicroSeconds32 aTimeout )
    {
	iTimeout = aTimeout;
    }


void CCaeTestClient::SetStillBurstCaptureInterval( TTimeIntervalMicroSeconds aInterval )
    {
    iStillBurstCaptureInterval = aInterval;
    }


void CCaeTestClient::SetExposureMode( TInt aExposureMode )
    {
    iExposureMode = aExposureMode;
    }


void CCaeTestClient::SetEvCompensation( TInt aEvCompensation )
    {
    iEvCompensation = aEvCompensation;
    }


void CCaeTestClient::SetWhiteBalanceMode( TInt aWhiteBalanceMode )
    {
    iWhiteBalanceMode = aWhiteBalanceMode;
    }


void CCaeTestClient::SetFlashMode( TInt aFlashMode )
    {
    iFlashMode = aFlashMode;
    }


void CCaeTestClient::SetBrightness( TInt aBrightness )
    {
    iBrightness = aBrightness;
    }


void CCaeTestClient::SetContrast( TInt aContrast )
    {
    iContrast = aContrast;
    }


void CCaeTestClient::SetZoomMode( TInt aZoomMode )
    {
    iZoomMode = aZoomMode;
    }


void CCaeTestClient::SetDigiZoomValue( TInt aDigiZoomValue )
    {
    iDigiZoomValue = aDigiZoomValue;
    }

void CCaeTestClient::SetVideoClipMaxSize( TInt aMaxClipSizeInBytes )
    {
    iVideoMaxClipSizeInBytes = aMaxClipSizeInBytes;
    }


void CCaeTestClient::SetVideoTimesInterval( TTimeIntervalMicroSeconds aInterval )
    {
    iVideoTimesInterval = aInterval;
    }


void CCaeTestClient::SetOpMode( TestClientOpMode aOpMode )
    {
    iOpMode = aOpMode;
    }
  

void CCaeTestClient::SetDisplayIndex( TInt aDisplayIndex )
    {
    iDisplayIndex = aDisplayIndex;
    }
  

void CCaeTestClient::SetStopOnErrorInMcaeoSnapImageReady( TBool aStopOnError )
    {
    iStopOnErrorInMcaeoSnapImageReady = aStopOnError;
    }


void CCaeTestClient::SetStopOnErrorInMcaeoStillImageReady( TBool aStopOnError )
    {
    iStopOnErrorInMcaeoStillImageReady = aStopOnError;
    }


void CCaeTestClient::GetInfo( TCamAppEngineInfo& aInfo ) const
    {
    aInfo = iInfo;
    }


void CCaeTestClient::GetStillFrameSize( TSize& aSize ) const
    {
    aSize = iStillFrameSizeSet;
    }


TInt CCaeTestClient::EstimatedStillSizeInBytes() const
    {
    return iEstimatedStillSizeInBytesSet;
    }


void CCaeTestClient::GetVideoFrameSize( TSize& aSize ) const
    {
    aSize = iVideoFrameSizeSet;
    }


TReal32 CCaeTestClient::VideoFrameRate() const
    {
    return iVideoFrameRateSet;
    }

TInt CCaeTestClient::EstimatedVideoRecordingBitRate() const
    {
    return iEstimatedVideoBitRateSet;
    }


TReal32 CCaeTestClient::ViewFinderFrameFrequency()
    {
    return 15.0;
    }


TInt CCaeTestClient::CountInitComplete()
    {
    return iCountInitComplete;
    }


TInt CCaeTestClient::CountStillPrepareComplete()
    {
    return iCountStillPrepareComplete;
    }


TInt CCaeTestClient::CountStillBurstCaptureMoment()
    {
    return iCountStillBurstCaptureMoment;
    }


TInt CCaeTestClient::CountStillBurstComplete()
    {
    return iCountStillBurstComplete;
    }

TInt CCaeTestClient::CountImageDeliveredStillBurstComplete()
    {
    return iImageCountDeliveredForStillBurst;
    }


TInt CCaeTestClient::CountVideoPrepareComplete()
    {
    return iCountVideoPrepareComplete;
    }


TInt CCaeTestClient::CountViewFinderFrameReady()
    {
    return iCountViewFinderFrameReady;
    }


TInt CCaeTestClient::CountSnapImageReady()
    {
    return iCountSnapImageReady;
    }


TInt CCaeTestClient::CountStillImageReady()
    {
    return iCountStillImageReady;
    }


TInt CCaeTestClient::CountVideoRecordingOn()
    {
    return iCountVideoRecordingOn;
    }


TInt CCaeTestClient::CountVideoRecordingComplete()
    {
    return iCountVideoRecordingComplete;
    }

TInt CCaeTestClient::CountVideoRecordingStopped()
    {
    return iCountVideoRecordingStopped;
    }


TInt CCaeTestClient::CountVideoRecordingPaused()
    {
    return iCountVideoRecordingPaused;
    }


TInt CCaeTestClient::CountVideoRecordingTimes()
    {
    return iCountVideoRecordingTimes;
    }

void CCaeTestClient::SetCustomInterfaceUid( TUid aCustomInterfaceUid )
    {
    iCustomInterfaceUid = aCustomInterfaceUid;
    }

TAny* CCaeTestClient::CustomInterfacePointer()
    {
    return iCustomInterfacePointer;
    }

void CCaeTestClient::SetColorToneValue( MCameraColorToneSettings::TColorTone aValue)
    {
    iColorToneValue = aValue;
    }

void CCaeTestClient::SetVideoCodecSet( TInt aValue)
    {
    iVideoCodecSet = aValue;
    }

// Trap RunL, to catch if the AO leaves.
// A Walkaround...
// RunError() should be used in this case, but
// Apparently, it is not possible to use the AO RunError() virtual function
// for that purpose, it is not called.
void CCaeTestClient::RunL()
    {
    TRAPD( error, RunTrappedL() );
	if ( error )
	    {
    	#ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::RunL() RunTrappedL leaved! %d" ), error );
        #endif
        StopOnError( error );
        // After the Stop, execution should continue at the end of RunTestAction()
        }
    }


void CCaeTestClient::DoCancel()
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::DoCancel() entering" ) );
    #endif
	
    if ( iCamAppEngine->IsViewFinding() )
	    {
        iCamAppEngine->StopViewFinder();
        iViewFinderStarted = EFalse;
        }
    
    iCamAppEngine->CancelCaptureStill();
    
    if ( iCamAppEngine->IsVideoRecording() ) 
        {
        iCamAppEngine->StopVideoRecording();
        }
    
    delete iCamAppEngine;
	iCamAppEngine = NULL;

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::DoCancel() returning" ) );
    #endif
    }


void CCaeTestClient::McaeoInitComplete( TInt aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoInitComplete() entering: %d" ), iCountInitComplete );
    #endif

    iCountInitComplete++;

    #if CAE_OOM_TESTING_HARNESS
    // For OOM testing in CppUnit.
    TInt* memTest = new TInt( 1 );
    if ( !memTest ) 
        {
        #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoInitComplete(): OOM test, calling CancelCaptureStill()" ) );
        #endif
        iCamAppEngine->CancelCaptureStill();
        StopOnError( KErrNoMemory );
        }
    else 
        {
        delete memTest;
        }
    #endif

    if ( aError ) 
        {
	    #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoInitComplete(): error detected: %d" ), aError );
        #endif
        StopOnError( aError );
        }
    else
        {
        if ( !(iAction == ECaptureStillAfterPowerOffOn && iStopWhenCompleted) ) 
            {
            iInitReady = ETrue;
            }
        iPowerOn   = ETrue;
        CTimer::After( KNormalTimingDelay );
        }

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoInitComplete() returning" ) );
    #endif
    }


void CCaeTestClient::McaeoStillPrepareComplete( TInt aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoStillPrepareComplete() entering: %d" ), iCountStillPrepareComplete );
    #endif

    iCountStillPrepareComplete++;

    #if CAE_OOM_TESTING_HARNESS
    // For OOM testing in CppUnit.
    TInt* memTest = new TInt( 1 );
    if ( !memTest ) 
        {
        #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoStillPrepareComplete(): OOM test, calling CancelCaptureStill()" ) );
        #endif
        iCamAppEngine->CancelCaptureStill();
        StopOnError( KErrNoMemory );
        }
    else 
        {
        delete memTest;
        }
    #endif

    if ( aError ) 
        {
	    #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoStillPrepareComplete(): error detected" ) );
        #endif
        StopOnError( aError );
        }
    else
        {
        iStillPrepareReady = ETrue;
        CTimer::After( KNormalTimingDelay );
        }

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoStillPrepareComplete() returning" ) );
    #endif
    }


void CCaeTestClient::McaeoVideoPrepareComplete( TInt aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoPrepareComplete() entering: %d" ), iCountVideoPrepareComplete );
    #endif

    iCountVideoPrepareComplete++;

    if ( aError ) 
        {
	    #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoPrepareComplete(): error detected: %d" ), aError );
        #endif
        StopOnError( aError );
        }
    else
        {
        iVideoPrepareReady = ETrue;

        if ( iVideoStopModeCommand ) // New command should be sent once
            {
            iVideoStopModeCommand = EFalse;
            TInt err = iCamAppEngine->SetAsyncVideoStopMode( iAsyncVideoStopEnabled );
            if ( err ) 
                {
	            #ifdef _DEBUG
                RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoPrepareComplete(): error detected in SetAsyncVideoStopMode: %d" ), err );
                #endif
                StopOnError( err );
                }
            }
        
        CTimer::After( KNormalTimingDelay );
        }

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoPrepareComplete() returning" ) );
    #endif
    }


void CCaeTestClient::McaeoViewFinderFrameReady( CFbsBitmap& aFrame, TInt aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoViewFinderFrameReady() entering: %d" ), iCountViewFinderFrameReady );
    #endif
    TSize sizeEmpty;

    iCountViewFinderFrameReady++;

    #if CAE_OOM_TESTING_HARNESS
    // For OOM testing in CppUnit.
    TInt* memTest = new TInt( 1 );
    if ( !memTest ) 
        {
        #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoViewFinderFrameReady(): OOM test, calling CancelCaptureStill()" ) );
        #endif
        iCamAppEngine->CancelCaptureStill();
        StopOnError( KErrNoMemory );
        }
    else 
        {
        delete memTest;
        }
    #endif

    if ( aError ) 
        {
	    #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoViewFinderFrameReady(): error detected: %d" ), aError );
        #endif
        StopOnError( aError );
        }
    else
        {
        iViewFinderStarted = ETrue;

        iVfFrameCount++;


		if ( iCancelStillBurstAfterVfFrame && ( iCountStillBurstCaptureMoment == 2 ) )
			{
			iCamAppEngine->CancelCaptureStill();
            iStop = ETrue;
			CTimer::After( KNormalTimingDelay );
			}
        
        // TSize viewFinderSize;
        // iCamAppEngine->GetViewFinderSize( viewFinderSize );
        // AsyncAssert( viewFinderSize==iViewFinderSize, KErrGeneral );

        if ( iVfFrameCount <= iVfFrameCountToSave ) 
            {
            TFileName fn( iVfFileName );
            TInt length = fn.Length();
            TBuf<4> ext;
            ext = fn.Right( 4 );
            fn.SetLength( length  - 4 );
            TBuf<10> num;
            num.Num( iVfFrameCount - 1 );
            fn.Append( _L("_") );
            fn.Append( num );
            fn.Append( ext );

            #ifdef _DEBUG
            RDebug::Print( fn );
            #endif

            iError = aFrame.Save( fn );
            if ( iError ) 
                {
                StopOnError( iError );
                }
            } 
            
        // Create a duplicate bitmap and save it
        if ( iAction == EProcessExtViewFinderFrameReady  &&
            !iBitmapSave &&
            aFrame.SizeInPixels() != sizeEmpty ) 
            {
            iBitmapSave = new CFbsBitmap;
            if ( iBitmapSave )
                {
                TInt err = iBitmapSave->Duplicate( aFrame.Handle() );
                if ( err ) 
                    {
//                        LOGTEXT2( _L( "Cae: CCaeEngineImp::ProcessExtSnapImageL(): Duplicate error:%d" ), err);
                        delete iBitmapSave;
                        iBitmapSave = NULL;
//                        User::Leave( KErrNoMemory );
                     }
                }
            else
                {
//                LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtSnapImageL(): new CFbsBitmap failed" ));
//                User::Leave( KErrNoMemory );
                }
            }
                       
        }

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoViewFinderFrameReady() returning" ) );
    #endif
    }


void CCaeTestClient::McaeoSnapImageReady( const CFbsBitmap& aBitmap, TInt aError ) 
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoSnapImageReady() entering: %d" ), iCountSnapImageReady );
    #endif
    TSize sizeEmpty;

    iCountSnapImageReady++;

    #if CAE_OOM_TESTING_HARNESS
    // For OOM testing in CppUnit.
    TInt* memTest = new TInt( 1 );
    if ( !memTest ) 
        {
        #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoSnapImageReady(): OOM test, calling CancelCaptureStill()" ) );
        #endif
        iCamAppEngine->CancelCaptureStill();
        StopOnError( KErrNoMemory );
        }
    else 
        {
        delete memTest;
        }
    #endif

    if ( aError ) 
        {
	    #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoSnapImageReady(): error detected: %d" ), aError );
        #endif
		if (iStopOnErrorInMcaeoSnapImageReady)
			{
			StopOnError( aError );
			}
        }
    else
        {
        if ( iSaveSnapImage && ( iCountSnapImageReady < 10 ) )
            {
            if ( aBitmap.SizeInPixels() != iSizeEmpty ) 
                {
                // create bitmap 3 as a duplicate of bitmap 2
                CFbsBitmap* bitmapSave = new CFbsBitmap;
                if ( bitmapSave )
                    {
                    iError = bitmapSave->Duplicate( aBitmap.Handle() );
                    if ( iError ) 
                        {
                        delete bitmapSave;
                        bitmapSave = NULL;
                        StopOnError( iError );
                        }

                    TFileName fn( iSnapFileName );
                    TInt length = fn.Length();
                    TBuf<4> ext;
                    ext = fn.Right( 4 );
                    fn.SetLength( length  - 4 );
                    TBuf<10> num;
                    num.Num( iCountSnapImageReady - 1 );
                    fn.Append( _L("_") );
                    fn.Append( num );
                    fn.Append( ext );

	                #ifdef _DEBUG
                    RDebug::Print( fn );
                    #endif

                    iError = bitmapSave->Save( fn );

                    delete bitmapSave;
                    bitmapSave = NULL;

                    if ( iError ) 
                        {
                        StopOnError( iError );
                        }
                    }
                }
            }
        // Extension mode
        // Create a duplicate bitmap and save it
        if ( (iAction == EProcessExtSnapImage || iAction == EProcessExtCapturedImageBurst ||
            iAction == EProcessExtCapturedImageBurstTDesC8 || iAction == EProcessExtCapturedImageCFbsBitmap) &&
            !iBitmapSave &&
            aBitmap.SizeInPixels() != sizeEmpty ) 
            {
            iBitmapSave = new CFbsBitmap;
            if ( iBitmapSave )
                {
                TInt err = iBitmapSave->Duplicate( aBitmap.Handle() );
                if ( err ) 
                    {
//                        LOGTEXT2( _L( "Cae: CCaeEngineImp::ProcessExtSnapImageL(): Duplicate error:%d" ), err);
                        delete iBitmapSave;
                        iBitmapSave = NULL;
//                        User::Leave( KErrNoMemory );
                     }
                }
            else
                {
//                LOGTEXT( _L( "Cae: CCaeEngineImp::ProcessExtSnapImageL(): new CFbsBitmap failed" ));
//                User::Leave( KErrNoMemory );
                }
            if ( iExtensionMode &&  // If CAE is switched to ext mode
                iAction == EProcessExtSnapImage )   // Only snap image is sent in this test case
                {
                CTimer::After( KNormalTimingDelay ); // start timer for next action
                }
            }

        if ( iRunlForSnapImage && ( iCountSnapImageReady < 2 ) )
            {
            iSnapImageReady = ETrue;
            CTimer::After( KNormalTimingDelay );
            }
        }

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoSnapImageReady() returning" ) );
    #endif
    }


void CCaeTestClient::McaeoStillImageReady( CFbsBitmap* aBitmap, HBufC8* aData, TInt aError ) 
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoStillImageReady() entering: %d" ), iCountStillImageReady );
    #endif

    iCountStillImageReady++;

    #if CAE_OOM_TESTING_HARNESS
    // For OOM testing in CppUnit.
    TInt* memTest = new TInt( 1 );
    if ( !memTest ) 
        {
        #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoStillImageReady(): OOM test, calling CancelCaptureStill()" ) );
        #endif
        iCamAppEngine->CancelCaptureStill();
        StopOnError( KErrNoMemory );
        }
    else 
        {
        delete memTest;
        }
    #endif

    if ( aError ) 
        {
        if ( aBitmap )
            {
            delete aBitmap;
            aBitmap = NULL;
            }
    
        if ( aData ) 
            {
            delete aData;
            aData = NULL;
            }

	    #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoStillImageReady(): error detected: %d" ), aError );
        #endif
		if (iStopOnErrorInMcaeoStillImageReady)
			{
			StopOnError( aError );
			}
        }
    else
        {
        if ( ( aBitmap == NULL ) && ( aData == NULL ) ) 
            {
            StopOnError( KErrBadHandle );
            }

        if ( iSaveStillImage  && ( iCountStillImageReady < 10 ) )
            {
            TFileName fn( iStillFileName );
            TInt length = fn.Length();
            TBuf<4> ext;
            ext = fn.Right( 4 );
            fn.SetLength( length  - 4 );
            TBuf<10> num;
            num.Num( iCountStillImageReady - 1 );
            fn.Append( _L("_") );
            fn.Append( num );
            fn.Append( ext );

	        #ifdef _DEBUG
            RDebug::Print( fn );
            #endif

            if ( aBitmap ) 
                {
                aBitmap->Save( fn );
                }
            else if ( aData ) 
                {
                RFs fsSession;
                RFile file;
                if ( fsSession.Connect() == KErrNone )
                    {
                    file.Replace( fsSession, fn, EFileWrite );
                    file.Write( aData->Des() );
                    file.Close();
                    fsSession.Close();
                    }
                }
            }
            
        // Extension mode handling, don't delete image data
        if ( (iAction == EProcessExtCapturedImage ||
            iAction == EProcessExtCapturedImageTDesC8 ||
            iAction == EProcessExtCapturedImageCFbsBitmap ||
            iAction == EProcessExtCancel2 ||
            iAction == EProcessExtCapturedImageBurst ||
            iAction == EProcessExtCapturedImageBurstTDesC8 ||
            iAction == EProcessExtCapturedImageNoInit)  &&
            !iDataSave )
            {
            // save captured image for extension testing
            iDataSave = aData;
            aData = NULL;
            }

    
        if ( aBitmap )
            {
            delete aBitmap;
            aBitmap = NULL;
            }
    
        if ( aData ) 
            {
            delete aData;
            aData = NULL;
            }

        if ( iRunlForStillImage || ( !iRunlForSnapImage && ( iImageCountToCapture == 1 ) ) )
            {
            iStillCaptureReady = ETrue;
            iStillCapturingOn = EFalse;
            CTimer::After( KNormalTimingDelay );
            }
        }

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoStillImageReady() returning" ) );
    #endif
    }


void CCaeTestClient::McaesboStillBurstCaptureMoment( TInt aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaesboStillBurstCaptureMoment() entering: %d" ), iCountStillBurstCaptureMoment );
    #endif

    iCountStillBurstCaptureMoment++;

    if ( aError ) 
        {
	    #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaesboStillBurstCaptureMoment(): error detected: %d" ), aError );
        #endif
        StopOnError( aError );
        }

    if ( iStopStillBurst && ( iCountStillBurstCaptureMoment >= 3 ) )
        {
        iCamAppEngine->StopStillBurstCapture();
        }
    else if ( iCancelStillBurst && ( iCountStillBurstCaptureMoment >= 2 ) ) 
        {
        iStillBurstCaptureMoment = ETrue;
        iCancelCount++;
        iCamAppEngine->CancelCaptureStill();
        CTimer::After( KNormalTimingDelay );
        }
	else if (iRunWithViewFinder && ( iCountStillBurstCaptureMoment == iStillBurstLength) )
		{
		// View finder is running and the last burst image will be taken
		if ( iCountViewFinderFrameReady < ( iStillBurstLength - 1 ) )
			{
			// Error! There should be at least one VF frame between every burst image
			#ifdef _DEBUG
			RDebug::Print( _L( "Cae: CCaeTestClient::McaesboStillBurstCaptureMoment() Error! Not enought VF frames during the burst! %d" ), iCountViewFinderFrameReady);
			#endif
			StopOnError( KErrGeneral );
			}
		}
    #if CAE_OOM_TESTING_HARNESS
    else
        {
        // For OOM testing in CppUnit.
        TInt* memTest = new TInt( 1 );
        if ( !memTest ) 
            {
            #ifdef _DEBUG
            RDebug::Print( _L( "Cae: CCaeTestClient::McaesboStillBurstCaptureMoment(): OOM test, calling CancelCaptureStill()" ) );
            #endif
            iCamAppEngine->CancelCaptureStill();
            StopOnError( KErrNoMemory );
            }
        else 
            {
            delete memTest;
            }
        }
    #endif

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaesboStillBurstCaptureMoment() returning" ) );
    #endif
    }


void CCaeTestClient::McaesboStillBurstComplete( TInt aImageCountDelivered, 
                                                TInt aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaesboStillBurstComplete() entering: %d" ), iCountStillBurstComplete );
    #endif

    iCountStillBurstComplete++;

    #if CAE_OOM_TESTING_HARNESS
    // For OOM testing in CppUnit.
    TInt* memTest = new TInt( 1 );
    if ( !memTest ) 
        {
        #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaesboStillBurstComplete(): OOM test, calling CancelCaptureStill()" ) );
        #endif
        iCamAppEngine->CancelCaptureStill();
        StopOnError( KErrNoMemory );
        }
    else 
        {
        delete memTest;
        }
    #endif

    iImageCountDeliveredForStillBurst = aImageCountDelivered;

    if ( aError ) 
        {
	    #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaesboStillBurstComplete(): error detected: %d" ), aError );
        #endif
        StopOnError( aError );
        }
    else
        {
        iStillBurstReady = ETrue;
        iStillCapturingOn = EFalse;
        CTimer::After( KNormalTimingDelay );
        }

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaesboStillBurstComplete() returning" ) );
    #endif
    }


void CCaeTestClient::McaeoVideoRecordingOn( TInt aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingOn() entering: %d" ), iCountVideoRecordingOn );
    #endif

    iCountVideoRecordingOn++;
    
    if ( aError ) 
        {
	    #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingOn(): error detected: %d" ), aError );
        #endif
        StopOnError( aError );
        }
    else
        {
        iVideoRecordingOn = ETrue;
        iVideoRecordingPaused = EFalse;
        CTimer::After( iTimeout );
        }

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingOn() returning" ) );
    #endif
    }


void CCaeTestClient::McaeoVideoRecordingComplete( TInt aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingComplete() entering: %d" ), iCountVideoRecordingComplete );
    #endif

    iCountVideoRecordingComplete++;

    if ( aError ) 
        {
        if ( !iAcceptVideoCompletionError || aError != KErrCompletion )
            {
	        #ifdef _DEBUG
            RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingComplete(): error detected: %d" ), aError );
            #endif
            StopOnError( aError );
            }
        else
            {
            iVideoRecordingReady = ETrue;
            iVideoRecordingOn = EFalse;
            // No CTimer::After call here because there's one pending  
            // from McaeoVideoRecordingOn.
            }
        }
    else
        {
        iVideoRecordingReady = ETrue;
        iVideoRecordingOn = EFalse;
        CTimer::After( KNormalTimingDelay );
        }

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingComplete() returning" ) );
    #endif
    }

void CCaeTestClient::McaeoVideoRecordingStopped()
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingStopped() entering: %d" ), iCountVideoRecordingStopped );
    #endif

    iCountVideoRecordingStopped++;

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingStopped() returning" ) );
    #endif
    }


void CCaeTestClient::McaeoVideoRecordingPaused( TInt aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingPaused() entering: %d" ), iCountVideoRecordingPaused );
    #endif

    iCountVideoRecordingPaused++;
    
    if ( aError && iAction != EPrepareRecordPausePauseStopVideo ) // Note double pause test exception
        {
	    #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingPaused(): error detected: %d" ), aError );
        #endif
        StopOnError( aError );
        }
    else
        {
        // Start CTimer only once, not two times for double pause test
        if ( !iVideoRecordingPaused )
            {
            iVideoRecordingPaused = ETrue;
            CTimer::After( KNormalTimingDelay );
            }
        }

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingPaused() returning" ) );
    #endif
    }


void CCaeTestClient::McaeoVideoRecordingTimes( TTimeIntervalMicroSeconds aTimeElapsed, 
                                               TTimeIntervalMicroSeconds aTimeRemaining, 
                                               TInt aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingTimes() entering: %d" ), iCountVideoRecordingTimes );
    #endif

    iCountVideoRecordingTimes++;

    if ( aError ) 
        {
	    #ifdef _DEBUG
        RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingTimes(): error detected: %d" ), aError );
        #endif
        StopOnError( aError );
        }

    TBuf8<128> textVideoTimes;
    
    _LIT(KVideoTimesTitle, "Video times (elapsed remaining):\n");
    _LIT(KSpaceSeparator, " ");

    textVideoTimes.Append( KVideoTimesTitle );
    textVideoTimes.AppendNum( aTimeElapsed.Int64() );
    textVideoTimes.Append( KSpaceSeparator );
    textVideoTimes.AppendNum( aTimeRemaining.Int64() );
    textVideoTimes.Append( _L("\n") );

    // Save video times info to data file. 
    #if ( WRITE_VIDEOTIMES_TO_FILE )
    const TDesC8& text = textVideoTimes;
    TInt err = iFile.Open( iFsSession, KProcessFileName, EFileWrite );
    TInt aPos;
    err = iFile.Seek( ESeekEnd, aPos );
    iFile.Write( text );
    iFile.Close();
    #endif

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::McaeoVideoRecordingTimes() returning" ) );
    #endif
    }


void CCaeTestClient::StopOnError( TInt aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::StopOnError(): Error: %d" ), aError );
    #endif

	__ASSERT_ALWAYS( !iStoppedOnError, User::Invariant());

    // Error => mark down the error code and stop.
    iError = aError;

    if ( iCamAppEngine->IsViewFinding() )
	    {
        iCamAppEngine->StopViewFinder();
        iViewFinderStarted = EFalse;
        }

    iCamAppEngine->CancelCaptureStill();

    if ( iCamAppEngine->IsVideoRecording() ) 
        {
        iCamAppEngine->StopVideoRecording();
        }
    
    CActiveScheduler::Stop();

	iStoppedOnError = ETrue;
    }

// -----------------------------------------------------------------------------
// CCaeTestClient::ReserveComplete
// Camera reservation is complete. 
// Called asynchronously when CCamera::Reserve() completes.
// From McameraObserver interface
// -----------------------------------------------------------------------------
//
void CCaeTestClient::ReserveComplete(                                 
    TInt aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::ReserveComplete() entering, aError=%d" ), aError );
    #endif

    if ( aError == KErrNone )
        {
        iCameraDup->PowerOn();
        }

	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::ReserveComplete() returning" ));
    #endif
    }

// -----------------------------------------------------------------------------
// CCaeEngineImp::PowerOnComplete
// Indicates camera power on is complete.
// Called on completion of CCamera:PowerOn().
// From McameraObserver interface
// -----------------------------------------------------------------------------
//
void CCaeTestClient::PowerOnComplete( 
    TInt aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::PowerOnComplete() entering, aError=%d" ), aError );
    #endif
    if ( !aError)
        {
        iInitReady = ETrue;
        iPowerOn   = ETrue;
        }
    CTimer::After( KNormalTimingDelay ); // This will start the test case AO to run
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::PowerOnComplete() returning" ) );
    #endif
    }

// -----------------------------------------------------------------------------
// CCaeTestClient::ViewFinderFrameReady
// Transfers the current view finder image from the camera. The frame is 
// From McameraObserver interface
// -----------------------------------------------------------------------------
//
void CCaeTestClient::ViewFinderFrameReady( 
    CFbsBitmap& /*aFrame*/ )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::ViewFinderFrameReady() entering" ) );
    #endif
    }

// -----------------------------------------------------------------------------
// CCaeTestClient::ImageReady
// Transfers the current image from the camera. 
// Called asynchronously when CCamera::CaptureImage() completes.
// From McameraObserver interface
// -----------------------------------------------------------------------------
//
void CCaeTestClient::ImageReady( 
    CFbsBitmap* aBitmap, 
    HBufC8*     aImageData, 
    TInt        aError )
    {
	#ifdef _DEBUG
    RDebug::Print( _L( "Cae: CCaeTestClient::ViewFinderFrameReady() entering %d" ), aError );
    #endif
    if ( aBitmap )
        delete aBitmap;
    if ( aImageData )
        delete aImageData;
    aError++;
    }

// -----------------------------------------------------------------------------
// CCaeTestClient::HandleEvent
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCaeTestClient::HandleEvent( const TECAMEvent& aEvent)
    {
    PRINT(( _L( "CCaeTestClient::HandleEvent() entering, type=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode ));

    if ( aEvent.iEventType == KUidECamEventCameraNoLongerReserved )
        {
        PRINT(( _L( "CCaeTestClient::HandleEvent() KUidECamEventCameraNoLongerReserved" ) ));
        }
    else if ( aEvent.iEventType == KUidECamEventPowerOnComplete )
        {
        PRINT(( _L( "CCaeTestClient::HandleEvent() KUidECamEventPowerOnComplete" ) ));
        PowerOnComplete( aEvent.iErrorCode );
        }
    else if ( aEvent.iEventType == KUidECamEventReserveComplete )
       {
       PRINT(( _L( "CCaeTestClient::HandleEvent() KUidECamEventReserveComplete" ) ));
       ReserveComplete( aEvent.iErrorCode );
       }

    PRINT(( _L( "CCaeTestClient::HandleEvent() returning" ) ));
    }

// -----------------------------------------------------------------------------
// CCaeTestClient::ViewFinderReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCaeTestClient::ViewFinderReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCaeTestClient::ViewFinderReady() NOT HANDLED err=%d"), aError ));

    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    }

// -----------------------------------------------------------------------------
// CCaeTestClient::ImageBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCaeTestClient::ImageBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCaeTestClient::ImageBufferReady() NOT HANDLED, err=%d" ), aError ));
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    }
    
// -----------------------------------------------------------------------------
// CCaeTestClient::VideoBufferReady
// MCameraObserver2 call-back handler
// -----------------------------------------------------------------------------
//
void CCaeTestClient::VideoBufferReady( MCameraBuffer& aCameraBuffer, TInt aError)
    {
    PRINT(( _L( "CCaeTestClient::VideoBufferReady() NOT HANDLED entering, err=%d" ), aError ));
	
    if ( !aError )
        {
	    aCameraBuffer.Release();
        }
    
    PRINT(( _L( "CCaeTestClient::VideoBufferReady() returning" )));
    }

