/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Camera Application Engine implementation video recording methods
*
*/


// INCLUDE FILES

#include <mmf/server/mmffile.h>                        // For MMF definitions (as TMMFFileConfig)
#include <AudioPreference.h>                // For MMF audio preference definitions.


#include "CaeEngineImp.h"                   // Engine implementation header.
#include "CaeVideoQualityLevels.h"          // For video recording quality levels.
#include "CaeVideoTimes.h"                  // For generating video recording time estimates.
#include "CaeStillStatesActive.h"                

#ifdef CAE_TEST_VERSION
#include "CaeEngineImpTestErrors.h"         // For TEST_VERSION compilation only
#endif


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCaeEngineImp::InitVideoRecorderL
// Initialise the engine for video recording using default parameters.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::InitVideoRecorderL()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::InitVideoRecorderL() entering" ) );

    iVideoInitialized = EFalse;

    // Close video recording if previously opened/prepared.
    CloseVideoRecording();

    // (Re-)initialize these.
    iVideoTimesInterval = KVideoTimesIntervalDefault; 
    iMaxClipSizeInBytes = KMMFNoMaxClipSize; 
    iMaxClipSizeInBytesPrep = KMMFNoMaxClipSize;

    // Create video quality levels container object.
    delete iVideoQualityLevelsCont;
    iVideoQualityLevelsCont = NULL;
    iVideoQualityLevelsCont = CCaeVideoQualityLevelsCont::NewL();

    // Destroy possible video clip file name. (Its allocated elsewhere when file name is set.)
    delete iVideoClipFileName;
    iVideoClipFileName = NULL;

    // Create video recorder.
    delete iVideoRecorder;
    iVideoRecorder = NULL;
    iVideoRecorder = CVideoRecorderUtility::NewL( *this , KAudioPriorityVideoRecording,
    	TMdaPriorityPreference( KAudioPrefVideoRecording ) );

    delete iVideoFrameSize;
    iVideoFrameSize = NULL;
    iVideoFrameSize = new( ELeave ) TSize();
    
    delete iVideoFrameSizePrep;
    iVideoFrameSizePrep = NULL;
    iVideoFrameSizePrep = new( ELeave ) TSize();

    // Create video times generator object.
    delete iVideoTimes;
    iVideoTimes = NULL;
    iVideoTimes = CCaeVideoTimes::NewL();

    // Create call-back function for video times generation.
    delete iVideoTimesCallback;
    iVideoTimesCallback = NULL;
    iVideoTimesCallback = new( ELeave ) TCallBack( VideoRecordingTimesCallback, this );

    // Initialize default video recording quality levels.
    iInfo->iNumVideoQualityLevelsSupported = 
        iVideoQualityLevelsCont->InitDefaultsL();

    iVideoInitialized = ETrue;

    LOGTEXT( _L( "Cae: CCaeEngineImp::InitVideoRecorderL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::InitVideoRecorderL
// Initialise the engine for video recording using either using default 
// parameters or parameters from ini-file.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::InitVideoRecorderL( 
    TUid aSdUidVideoQltyLevels )
    {
    if( aSdUidVideoQltyLevels == KNullUid )
        {
        InitVideoRecorderL();
        }
    else
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::InitVideoRecorderL(aSdUidVideoQltyLevels): NOT SUPPORTED, leaving" ) );
        User::Leave( KErrNotSupported );
        }
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetVideoRecordingFileNameL
// If video recording is not prepared then just memorizes the file name.
// If video recording is prepared, then calls variated ChangeVideoFileNameL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetVideoRecordingFileNameL( 
    const TDesC& aVideoClipFileName )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SetVideoRecordingFileNameL() entering" ) );

    if ( !iVideoInitialized || iVideoRecordingRunning ) 
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::SetVideoRecordingFileNameL leaving KErrNotReady" ));
        User::Leave( KErrNotReady );
        }

    if ( aVideoClipFileName.Length() > 0 )
        {
        // Memorize the video clip file name.
        delete iVideoClipFileName;
        iVideoClipFileName = NULL;
        iVideoClipFileName = aVideoClipFileName.AllocL();
        }
    else 
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::SetVideoRecordingFileNameL leaving KErrArgument (aVideoClipFileName)" ));
        User::Leave( KErrArgument );
        }

    if ( iVideoPrepared ) 
        {
        // Does the actual change of file name, only if video is prepared.
        // Note: Variated implementation
        ChangeVideoFileNameL();
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::SetVideoRecordingFileNameL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::PrepareVideoRecordingL
// Retrieves video recording parameters from the specified quality level and
// calls overloading PrepareVideoRecordingL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PrepareVideoRecordingL( 
    TInt aVideoQualityIndex )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::PrepareVideoRecordingL() entering" ) );

    if ( ( aVideoQualityIndex < 0 ) || 
         ( aVideoQualityIndex >= iVideoQualityLevelsCont->Count() ) ) 
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::PrepareVideoRecordingL() leaving KErrArgument" ) );
        User::Leave( KErrArgument );
        }
    
    PrepareVideoRecordingL( iVideoQualityLevelsCont->At( aVideoQualityIndex ).iFrameSize, 
                            iVideoQualityLevelsCont->At( aVideoQualityIndex ).iFrameRate, 
                            iVideoQualityLevelsCont->At( aVideoQualityIndex ).iBitRate, 
                            iVideoQualityLevelsCont->At( aVideoQualityIndex ).iAudioEnabled,
                            iVideoQualityLevelsCont->At( aVideoQualityIndex ).iAudioBitRate, 
                            iVideoQualityLevelsCont->At( aVideoQualityIndex ).iMimeType, 
                            iVideoQualityLevelsCont->At( aVideoQualityIndex ).iPreferredSupplier, 
                            iVideoQualityLevelsCont->At( aVideoQualityIndex ).iVideoType,    
                            iVideoQualityLevelsCont->At( aVideoQualityIndex ).iAudioType );
    
    iVideoQualityIndex = aVideoQualityIndex;

    LOGTEXT( _L( "Cae: CCaeEngineImp::PrepareVideoRecordingL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::PrepareVideoRecordingL
// Finds first the UIDs needed for opening a video recording, uses ECOM.
// Then gets camera handle from Camera API.
// Finally calls Video Recorder API OpenFileL().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PrepareVideoRecordingL( 
    const TSize&  aFrameSize, 
    TReal32       aFrameRate, 
    TInt          aBitRate, 
    TBool         aAudioEnabled,
    const TDesC8& aMimeType, 
    const TDesC&  aPreferredSupplier, 
    const TDesC8& aVideoType, 
    const TDesC8& aAudioType )
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::PrepareVideoRecordingL() entering aFrameSize wxh=%dx%d" ),
            aFrameSize.iWidth, aFrameSize.iHeight );

    // Leave if not initialized properly or busy doing something else.
    if ( !iVideoInitialized || 
         !iVideoClipFileName || 
         iStillStatesActive->IsRunning() || 
         iVideoRecordingRunning   ) 
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::PrepareVideoRecordingL leaving KErrNotReady, iVideoInitialized=%d" ), iVideoInitialized );
        User::Leave( KErrNotReady );
        }

    CheckPowerL();

    // Leave if video clip file name is not set properly.
    if ( iVideoClipFileName->Length() == 0 )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::PrepareVideoRecordingL() leaving KErrArgument (iVideoClipFileName)" ) );
        User::Leave( KErrArgument );
        }

    // Close if previously opened/prepared.
    CloseVideoRecording();

    // Find values for iVideoControllerUid and iVideoFormatUid.
    // Those are also needed if clip file name is changed when prepared.
    FindVideoUidsL( aMimeType, aPreferredSupplier );

    // Memorize video type.
    delete iVideoType;
    iVideoType = NULL;
    iVideoType = aVideoType.AllocL();

    // Convert audio type from TDesC8 to TFourCC.
    iVideoAudioType = ConvertAndSetVideoAudioTypeL( aAudioType );

    // Memorize the parameters to be prepared.
    *iVideoFrameSizePrep   = aFrameSize;
    iVideoFrameRatePrep    = aFrameRate;
    iVideoBitRatePrep      = aBitRate;
    iVideoAudioEnabledPrep = aAudioEnabled;

    // Open video recorder.
    iVideoOpened = ETrue; // This is always set to ETrue when 
                          // OpenFileL has been called to allow 
                          // freeing resources by CloseVideoRecording().
    iVideoRecorder->OpenFileL( iVideoClipFileName->Des(),
                               iCameraHandle,
                               iVideoControllerUid,
                               iVideoFormatUid, 
                               iVideoType->Des(),  
                               iVideoAudioType );
    
    LOGTEXT( _L( "Cae: CCaeEngineImp::PrepareVideoRecordingL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::CloseVideoRecording
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::CloseVideoRecording()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::CloseVideoRecording() entering" ) );

    if ( iVideoPrepared ) 
        {
        CancelVideoRecording();
        iVideoPrepared = EFalse;
        }

    if ( iVideoOpened )
        {
        iVideoRecorder->Close();
        iVideoOpened = EFalse;
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::CloseVideoRecording() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::VideoQualityIndex
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::VideoQualityIndex() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::VideoQualityIndex()" ) );

    TInt qualityIndex( -1 );
    if ( iVideoPrepared )
        {
        qualityIndex = iVideoQualityIndex;
        }
    return qualityIndex;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::GetVideoFrameSize
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::GetVideoFrameSize( 
    TInt aVideoQualityIndex, 
    TSize& aFrameSize ) const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::GetVideoFrameSize()" ) );
    
    if ( aVideoQualityIndex >= 0 &&  
         aVideoQualityIndex < iVideoQualityLevelsCont->Count() )
        {
        aFrameSize = iVideoQualityLevelsCont->At( aVideoQualityIndex ).iFrameSize;
        }
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::VideoFrameRate
// -----------------------------------------------------------------------------
//
TReal32 CCaeEngineImp::VideoFrameRate( 
    TInt aVideoQualityIndex ) const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::VideoFrameRate()" ) );

    TReal32 frameRate( 0.0 );
    if ( aVideoQualityIndex >= 0 &&  
         aVideoQualityIndex < iVideoQualityLevelsCont->Count() )
        {
        frameRate = iVideoQualityLevelsCont->At( aVideoQualityIndex ).iFrameRate;
        }
    return frameRate;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::EstimatedVideoRecordingBitRateL
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::EstimatedVideoRecordingBitRateL( 
    TInt aVideoQualityIndex ) const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::EstimatedVideoRecordingBitRateL()" ) );

    TInt storageRate( 0 );
    if ( aVideoQualityIndex >= 0 &&  
         aVideoQualityIndex < iVideoQualityLevelsCont->Count() )
        {
        storageRate = iVideoQualityLevelsCont->At( aVideoQualityIndex ).iStorageRate;
        }
    return storageRate;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetVideoClipMaxSizeL
// If video recording is not prepared then just memorizes the clip max size.
// If video recording is prepared, then calls Video Recorder API 
// SetMaxClipSizeL() and Prepare().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetVideoClipMaxSizeL( 
    TInt aMaxClipSizeInBytes )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SetVideoClipMaxSizeL() entering" ) );

    if ( !iVideoInitialized || iVideoRecordingRunning ) 
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::SetVideoClipMaxSizeL leaving KErrNotReady, iVideoInitialized=%d" ), iVideoInitialized );
        User::Leave( KErrNotReady );
        }

    if ( aMaxClipSizeInBytes > 0 ) 
        {
        iMaxClipSizeInBytesPrep = aMaxClipSizeInBytes;
        }
    else 
        {
        iMaxClipSizeInBytesPrep = KMMFNoMaxClipSize;
        }

    if ( iVideoPrepared )
        {
        iPrepPars = ETrue;
        iVideoRecorder->SetMaxClipSizeL( iMaxClipSizeInBytesPrep );
        iVideoRecorder->Prepare();
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::SetVideoClipMaxSizeL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::VideoClipMaxSize
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::VideoClipMaxSize() const 
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::VideoClipMaxSize()" ) );

    TInt maxClipSizeInBytes( 0 );
    if ( iMaxClipSizeInBytes != KMMFNoMaxClipSize ) 
        {
        maxClipSizeInBytes = iMaxClipSizeInBytes;
        }

    return maxClipSizeInBytes;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetVideoAudioL
// Calls Video Recorder API SetAudioEnabledL() and Prepare().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetVideoAudioL( 
    TBool aAudioEnabled )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SetVideoAudioL() entering" ) );

    if ( !iVideoInitialized || iVideoRecordingRunning ) 
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::SetVideoAudioL leaving KErrNotReady, iVideoInitialized=%d" ), iVideoInitialized );
        User::Leave( KErrNotReady );
        }

    iVideoAudioEnabledPrep = aAudioEnabled;
    iVideoRecorder->SetAudioEnabledL( iVideoAudioEnabledPrep );
    iPrepPars = ETrue;
    iVideoRecorder->Prepare();

    LOGTEXT( _L( "Cae: CCaeEngineImp::SetVideoAudioL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::VideoAudio
// Calls Video Recorder API AudioEnabledL().
// -----------------------------------------------------------------------------
//
TBool CCaeEngineImp::VideoAudio() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::VideoAudio()" ) );

    TBool audioEnabled( EFalse );
    if ( iVideoRecorder )
        {
        TRAPD( error, { audioEnabled = iVideoRecorder->AudioEnabledL(); } );
        if ( error != KErrNone ) 
            {
            audioEnabled = EFalse;
            }
        }
    return audioEnabled;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::SetVideoTimesIntervalL
// Converts from TTimeIntervalMicroSeconds used in the interface to 
// TTimeIntervalMicroSeconds32 used internally.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::SetVideoTimesIntervalL( 
    TTimeIntervalMicroSeconds aInterval )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::SetVideoTimesIntervalL()" ) );

    if ( iVideoRecordingRunning ) 
        {
        LOGTEXT2( _L( "Cae: CCaeEngineImp::SetVideoTimesIntervalL leaving KErrNotReady, iVideoRecordingRunning=%d" ), iVideoRecordingRunning );
        User::Leave( KErrNotReady );
        }

    // Note: KMaxTInt32 in microseconds is about 35 minutes
    if ( aInterval < TTimeIntervalMicroSeconds( 0 ) || 
         aInterval > TTimeIntervalMicroSeconds( KMaxTInt32 ) )
        {
        LOGTEXT( _L( "Cae: CCaeEngineImp::SetVideoTimesIntervalL leaving KErrArgument" ));
        User::Leave( KErrArgument );
        }

    iVideoTimesInterval = I64INT( aInterval.Int64() );
    }
        

// -----------------------------------------------------------------------------
// CCaeEngineImp::VideoTimesInterval
// Converts to TTimeIntervalMicroSeconds used in the interface from  
// TTimeIntervalMicroSeconds32 used internally.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CCaeEngineImp::VideoTimesInterval() const 
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::VideoTimesInterval()" ) );
    
    TInt64 intInterval = iVideoTimesInterval.Int();

    TTimeIntervalMicroSeconds interval( intInterval );

    return interval;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::StartVideoRecording
// Calls Video Recorder API Record().
// Starts generating video recording time information for the client.
// Finally calls McaeoVideoRecordingOn().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::StartVideoRecording()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::StartVideoRecording() entering" ) );

    TInt error( KErrNone );

    if ( iVideoPrepared && !iVideoRecordingRunning ) 
        {
        iVideoRecordingRunning = ETrue;
        iVideoRecordingPaused = EFalse;
        
 		// Reset previous time information
    	iPrevTimeElapsed = 0;
    	iPrevTimeRemaining = 0;
    	
       // Start video recording.
        iVideoRecorder->Record();
        if ( iVideoTimesInterval > TTimeIntervalMicroSeconds32( 0 ) ) 
            {
            // Start generating video recording time information for the client.
            iVideoTimes->Start( KVideoTimesDelay,       // delay
                                iVideoTimesInterval,    // interval
                                *iVideoTimesCallback ); // callback
            }
        }
    else 
        {
        error = KErrNotReady;
        }


    iCaeObserver->McaeoVideoRecordingOn( error );

    LOGTEXT( _L( "Cae: CCaeEngineImp::StartVideoRecording() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::StopVideoRecording
// Calls Video Recorder API Record(), DurationL(), and RecordTimeAvailable().
// Stops generating video recording time information for the client.
// Delivers video recording time info to the client once right after stopping, 
// by calling McaeoVideoRecordingTimes().
// Finally calls McaeoVideoRecordingComplete().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::StopVideoRecording()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::StopVideoRecording() entering" ) );
        
    TInt stoppingError( KErrNone );
    TInt asyncStopErr( KErrNone );
    
    if ( iVideoRecordingRunning ) 
        {
        TInt timesError( KErrNone );
        TTimeIntervalMicroSeconds timeElapsed( 0 );
                
        iVideoRecordingRunning = EFalse;
        
        // Stop generating video recording time information for the client. 
        iVideoTimes->Cancel();
        
        if ( iVideoTimesInterval > TTimeIntervalMicroSeconds32( 0 ) ) 
            {
            // Query elapsed time before stop (it is set to zero after stop).
            TRAP( timesError, { timeElapsed = iVideoRecorder->DurationL(); } );
            // Possible error is ignored locally because not so crucial. Error code is delivered by call-back.
            }
                    
        // Stop video recording.
        if ( iAsyncVideoStopEnabled )
            {
       		LOGTEXT( _L( "Cae: CCaeEngineImp::StopVideoRecording(). using custom command ECamCControllerCCVideoStopAsync" )); 
            asyncStopErr = iVideoRecorder->CustomCommandSync( TMMFMessageDestination( iVideoControllerUid, KMMFObjectHandleController ), 
                                                        ECamCControllerCCVideoStopAsync, 
                                                        KNullDesC8, 
                                                        KNullDesC8 );
       		if ( asyncStopErr ) // async stop was not supported, use sync stop
       		    {
           		LOGTEXT2( _L( "Cae: CCaeEngineImp::StopVideoRecording(). async stop err=%d, using sync stop" ), asyncStopErr); 
                stoppingError = iVideoRecorder->Stop();
                iCaeObserver->McaeoVideoRecordingStopped();
       		    }
            }
        else
            {
            // Synchronous, quaranteed that video recording is stopped when returning.
            stoppingError = iVideoRecorder->Stop();
            }
        
        // Can't be paused anymore.
        iVideoRecordingPaused = EFalse;

        /*
        #ifdef CAE_TEST_VERSION
        // For simulating errors when compiled as special "test version".
        CaeStopVideoRecordingError( stoppingError );
        #endif
        */
        
        if ( iVideoTimesInterval > TTimeIntervalMicroSeconds32( 0 ) ) 
            {    
            // Query remaining time after stop.
            TTimeIntervalMicroSeconds timeRemaining = iVideoRecorder->RecordTimeAvailable();
            
        	// The elapsed time could have been reseted to zero by the video recorder 
        	// if there is stopping ongoing in video recorder, even if we do 
        	// not have had the notification yet. In that case McaeoVideoRecordingTimes
        	// should be called with previous valid values.
        	if ( ( timeElapsed == 0 ) && ( iPrevTimeElapsed != 0 ) )
        		{
        		// Invalid time. Use previous time information
       			LOGTEXT2( _L( "Cae: CCaeEngineImp::StopVideoRecording(). Got invalid elapsed time from the video recorder: %f s." ), 
       				I64LOW( timeElapsed.Int64() ) * 1.0 / KOneSecond);
       			LOGTEXT3( _L( "Cae: CCaeEngineImp::StopVideoRecording(). Use previous valid elapsed time %f s. and remaining time %f s." ), 
       				I64LOW( iPrevTimeElapsed.Int64() ) * 1.0 / KOneSecond,  
       				I64LOW( iPrevTimeRemaining.Int64() ) * 1.0 / KOneSecond );
        		timeElapsed = iPrevTimeElapsed;
        		timeRemaining =iPrevTimeRemaining;
        		}
       
            // Deliver video times (elapsed, remaining) information.
            iCaeObserver->McaeoVideoRecordingTimes( timeElapsed, timeRemaining, timesError );
            }
        }
    else 
        {
        stoppingError = KErrNotReady;
        }

	// Reset previous time information
    iPrevTimeElapsed = 0;
    iPrevTimeRemaining = 0;

    if ( !iAsyncVideoStopEnabled || asyncStopErr )
        {
        iCaeObserver->McaeoVideoRecordingComplete( stoppingError );
        }


    LOGTEXT( _L( "Cae: CCaeEngineImp::StopVideoRecording() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::PauseVideoRecording
// Stops generating video recording time info for the client.
// Calls Video Recorder API PauseL().
// Delivers video recording time info to the client once right after pausing
// by calling VideoRecordingTimesCallback() that calls Video Recorder API 
// DurationL() and RecordTimeAvailable(), and then McaeoVideoRecordingTimes().
// Finally calls McaeoVideoRecordingPaused().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PauseVideoRecording()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::PauseVideoRecording() entering" ) );

    TInt error( KErrNone );

    if ( iVideoRecordingRunning && !iVideoRecordingPaused ) 
        {
        // Stop generating video recording time info for the client.
        iVideoTimes->Cancel();
        // Pause video recording.
        TRAP( error, iVideoRecorder->PauseL() );
        if ( iVideoTimesInterval > TTimeIntervalMicroSeconds32( 0 ) ) 
            {
            // Deliver video recording times (elapsed, remaining) info once.
            VideoRecordingTimesCallback( this );
            }
        // Intentionally setting iVideoRecordingPaused after (possibly) calling VideoRecordingTimesCallback().
        if ( error == KErrNone ) 
            {
            iVideoRecordingPaused = ETrue;
            }
        }
    else 
        {
        error = KErrNotReady;
        }

    iCaeObserver->McaeoVideoRecordingPaused( error );

    LOGTEXT( _L( "Cae: CCaeEngineImp::PauseVideoRecording() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::ResumeVideoRecording
// Calls Video Recorder API Record().
// Starts generating video recording time information for the client.
// Finally calls McaeoVideoRecordingOn().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::ResumeVideoRecording()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::ResumeVideoRecording() entering" ) );

    TInt error( KErrNone );

    if ( iVideoRecordingRunning && iVideoRecordingPaused ) 
        {
        // Start video recording.
        iVideoRecorder->Record();
        iVideoRecordingPaused = EFalse;
        if ( iVideoTimesInterval > TTimeIntervalMicroSeconds32( 0 ) ) 
            {
            // Start generating video recording time information for the client.
            iVideoTimes->Start( KVideoTimesDelay,       // delay
                                iVideoTimesInterval,    // interval
                                *iVideoTimesCallback ); // callback
            }
        }
    else 
        {
        error = KErrNotReady;
        }

    iCaeObserver->McaeoVideoRecordingOn( error );

    LOGTEXT( _L( "Cae: CCaeEngineImp::ResumeVideoRecording() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::RemainingVideoRecordingTime
// Calls Video Recorder API RecordTimeAvailable().
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CCaeEngineImp::RemainingVideoRecordingTime() const
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::RemainingVideoRecordingTime()" ) );
    
    TTimeIntervalMicroSeconds remaining( 0 );
    if ( iVideoRecorder )
        {
        remaining = iVideoRecorder->RecordTimeAvailable();
        }
    return remaining;
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::IsVideoRecording
// -----------------------------------------------------------------------------
//
TBool CCaeEngineImp::IsVideoRecording() const
    {
    return iVideoRecordingRunning;    
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::PrepareVideoRecordingL
// Overloading version with audio bit rate argument.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PrepareVideoRecordingL( 
    const TSize&  aFrameSize, 
    TReal32       aFrameRate, 
    TInt          aBitRate, 
    TBool         aAudioEnabled,
    TInt          aAudioBitRate, 
    const TDesC8& aMimeType, 
    const TDesC&  aPreferredSupplier, 
    const TDesC8& aVideoType, 
    const TDesC8& aAudioType )
    {
    // Memorize video audio bit rate value to be prepared.
    iVideoAudioBitRatePrep = aAudioBitRate;
    // Force audio bit rate preparation.
    iPrepareVideoAudioBitRate = ETrue;

    // Call the version without audio bit rate argument.
    // This is possible because the separate PrepareVideoSettingsL() is doing
    // settings after succesfull opening of video recording.
    PrepareVideoRecordingL( aFrameSize, 
                            aFrameRate, 
                            aBitRate, 
                            aAudioEnabled, 
                            aMimeType, 
                            aPreferredSupplier, 
                            aVideoType, 
                            aAudioType );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::MvruoOpenComplete
// The status of the video recorder after initialisation.
// This is either KErrNone if the open has completed successfully,
// or one of the system wide error codes
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::MvruoOpenComplete( 
    TInt aError )
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::MvruoOpenComplete() entering, aError=%d" ), aError );

    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaeMvruoOpenCompleteError( aError );
    #endif    

    if ( aError == KErrNone )
        {
        // To get default video audio bit rate.
        TRAP( aError, { iVideoAudioBitRate = iVideoRecorder->AudioBitRateL(); } );
        // (ignore possible error)
        // Prepare settings only if no errors in opening.
        TRAP( aError, PrepareVideoSettingsL() );
        if ( aError != KErrNone ) 
            {
            iCaeObserver->McaeoVideoPrepareComplete( aError );
            }
        }
    else
        {
        iCaeObserver->McaeoVideoPrepareComplete( aError );
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::MvruoOpenComplete() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::MvruoPrepareComplete
// Notification that video recorder is ready to begin recording. This callback
// is generated in response to a call to VR's Prepare.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::MvruoPrepareComplete( 
    TInt aError )
    {
    LOGTEXT2( _L( "Cae: CCaeEngineImp::MvruoPrepareComplete() entering, aError=%d" ), aError );

    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaeMvruoPrepareCompleteError( aError );
    #endif    

    if ( iVideoOpened && ( aError == KErrNone ) )
        {
        iVideoPrepared = ETrue; // Later errors with settings after 
                                // do not change the value.
        }

    if ( iPrepPars )
        {
        iPrepPars = EFalse;
        // If no error, then fresh parameters are valid.
        // Otherwise, old parameters are kept.
        if ( aError == KErrNone )
            {
            *iVideoFrameSize    = *iVideoFrameSizePrep;
            iVideoFrameRate     = iVideoFrameRatePrep;
            iVideoBitRate       = iVideoBitRatePrep;
            iVideoAudioEnabled  = iVideoAudioEnabledPrep;
            iVideoAudioBitRate  = iVideoAudioBitRatePrep;
            iMaxClipSizeInBytes = iMaxClipSizeInBytesPrep;
            }
        else 
            {
            *iVideoFrameSizePrep    = *iVideoFrameSize;
            iVideoFrameRatePrep     = iVideoFrameRate;
            iVideoBitRatePrep       = iVideoBitRate;
            iVideoAudioEnabledPrep  = iVideoAudioEnabled;
            iVideoAudioBitRatePrep  = iVideoAudioBitRate;
            iMaxClipSizeInBytesPrep = iMaxClipSizeInBytes;
            }
        }

    iCaeObserver->McaeoVideoPrepareComplete( aError );

    LOGTEXT( _L( "Cae: CCaeEngineImp::MvruoPrepareComplete() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::MvruoRecordComplete
// Notification that video recording has completed. This is not called if 
// recording is explicitly stopped by calling Stop.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::MvruoRecordComplete( 
    TInt aError )
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::MvruoRecordComplete() entering, aError=%d, iVideoRecordingRunning=%d" ), aError, iVideoRecordingRunning );
    
    // Stop generating video recording time information for the client. 
    iVideoTimes->Cancel();
    // Recording stopped: can't be paused anymore.
    iVideoRecordingPaused = EFalse; 
        
 	// Reset previous time information
    iPrevTimeElapsed = 0;
    iPrevTimeRemaining = 0;
    	
    if ( iVideoRecordingRunning || iAsyncVideoStopEnabled ) // To ensure that McaeoVideoRecordingComplete 
        {                                                   // gets called just once per recording.
        iVideoRecordingRunning = EFalse;
        
        #ifdef CAE_TEST_VERSION
        // For simulating errors when compiled as special "test version".
        CaeMvruoRecordCompleteError( aError );
        #endif
        
        // Close video recording always in error case. Otherwise the camcorder plugin would
        // be in indeterminated state. 
        // The code KErrCompletion means that video reocording has been completed by timer
        if ( aError && aError != KErrCompletion && aError != KErrDiskFull ) 
        	{ 
        	CloseVideoRecording();
        	}
        	
        // In async stop mode call also the other call-back to play stop sound
        if ( iAsyncVideoStopEnabled )
            {
            iCaeObserver->McaeoVideoRecordingStopped();
            }
        iCaeObserver->McaeoVideoRecordingComplete( aError );
        }
    
    LOGTEXT( _L( "Cae: CCaeEngineImp::MvruoRecordComplete() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::MvruoEvent
// Event Notification from controller 
// -----------------------------------------------------------------------------
//

void CCaeEngineImp::MvruoEvent( 
    const TMMFEvent& aEvent )
    {
    LOGTEXT3( _L( "Cae: CCaeEngineImp::MvruoEvent() entering, uid=%x, err=%d" ), aEvent.iEventType.iUid, aEvent.iErrorCode );
    
    if ( aEvent.iEventType.iUid == KCamCControllerCCVideoRecordStopped.iUid )
        {
        iCaeObserver->McaeoVideoRecordingStopped();
        }
    else if ( aEvent.iEventType.iUid == KCamCControllerCCVideoFileComposed.iUid )
        {
        iCaeObserver->McaeoVideoRecordingComplete( aEvent.iErrorCode );
        }
    else
        {
        // Send others events as error to UI 
        iVideoPrepared = EFalse;    // mark that re-preparation is needed 
        iCaeObserver->McaeoVideoPrepareComplete( aEvent.iErrorCode );
        }
    
    LOGTEXT( _L( "Cae: CCaeEngineImp::MvruoEvent() returning" ) );
    }



// -----------------------------------------------------------------------------
// CCaeEngineImp::CancelVideoRecording
// Calls Video Recorder API Stop().
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::CancelVideoRecording()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::CancelVideoRecording() entering" ) );
    
    if ( iVideoRecordingRunning ) 
        {
        // This also prevents McaeoVideoRecordingComplete() call from MvruoRecordComplete()  
        // and McaeoVideoRecordingTimes() call from VideoRecordingTimesCallback().
        iVideoRecordingRunning = EFalse;
        // Stop generating video recording time information for the client. 
        iVideoTimes->Cancel();
        
 		// Reset previous time information
    	iPrevTimeElapsed = 0;
    	iPrevTimeRemaining = 0;
    	
        // Stop video recording. Do not call McaeoVideoRecordingComplete()
        (void) iVideoRecorder->Stop();
        iVideoRecordingPaused = EFalse;
        }
    
    LOGTEXT( _L( "Cae: CCaeEngineImp::CancelVideoRecording() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::PrepareVideoSettingsL
// Calls Video Recorder API setting methods to set video recording paramaters.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::PrepareVideoSettingsL()
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::PrepareVideoSettingsL() entering" ) );

    #ifdef CAE_TEST_VERSION
    // For simulating errors when compiled as special "test version".
    CaePrepareVideoSettingsErrorL();
    #endif    

    iVideoRecorder->SetVideoFrameSizeL( *iVideoFrameSizePrep );
    iVideoRecorder->SetVideoFrameRateL( iVideoFrameRatePrep );
    iVideoRecorder->SetVideoBitRateL( iVideoBitRatePrep );
    iVideoRecorder->SetAudioEnabledL( iVideoAudioEnabledPrep );
    if ( iPrepareVideoAudioBitRate )
        {
        iVideoRecorder->SetAudioBitRateL( iVideoAudioBitRatePrep );
        iPrepareVideoAudioBitRate = EFalse;
        }
    iVideoRecorder->SetMaxClipSizeL( iMaxClipSizeInBytesPrep );

    // Set the recording gain to the maximum
    TInt gain = iVideoRecorder->GainL();
    LOGTEXT2( _L( "Cae: CCaeEngineImp::PrepareVideoSettingsL() GainL was %d" ), gain );
    gain = iVideoRecorder->MaxGainL();
    LOGTEXT2( _L( "Cae: CCaeEngineImp::PrepareVideoSettingsL() MaxGainL is %d" ), gain );
    iVideoRecorder->SetGainL( gain );
    gain = iVideoRecorder->GainL();
    LOGTEXT2( _L( "Cae: CCaeEngineImp::PrepareVideoSettingsL() GainL set to %d" ), gain );

    iPrepPars = ETrue;
    iVideoRecorder->Prepare();

    LOGTEXT( _L( "Cae: CCaeEngineImp::PrepareVideoSettingsL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::FindVideoUidsL
// Finds UIDs for opening video recording. Uses ECOM.
// -----------------------------------------------------------------------------
//
void CCaeEngineImp::FindVideoUidsL(
    const TDesC8& aMimeType, 
    const TDesC&  aPreferredSupplier )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::FindVideoUidsL() entering" ) );

    iVideoControllerUid.iUid = 0;
    iVideoFormatUid.iUid = 0; 

    // Retrieve a list of possible controllers from ECOM.
    //
    // Controller must support recording the requested mime type.
    // Controller must be provided by preferred supplier.

    CMMFControllerPluginSelectionParameters* cSelect = 
        CMMFControllerPluginSelectionParameters::NewLC();
    CMMFFormatSelectionParameters* fSelect = 
        CMMFFormatSelectionParameters::NewLC();

    fSelect->SetMatchToMimeTypeL( aMimeType );
    cSelect->SetRequiredRecordFormatSupportL( *fSelect );
    cSelect->SetPreferredSupplierL( aPreferredSupplier,
       CMMFPluginSelectionParameters::EOnlyPreferredSupplierPluginsReturned );

    RMMFControllerImplInfoArray controllers;
    CleanupResetAndDestroyPushL( controllers );
    cSelect->ListImplementationsL( controllers );

    if ( controllers.Count() < 1 )
        {
        // No appropriate controllers found.
        LOGTEXT( _L( "Cae: CCaeEngineImp::FindVideoUidsL() leaving KErrNotSupported (no controllers found)" ) );
        User::Leave( KErrNotSupported );
        }

    // Get the controller UID.
    iVideoControllerUid = controllers[0]->Uid();

    // Inquires the controller about supported formats.
    // We use the first controller found having index 0.
    RMMFFormatImplInfoArray formats;
    formats = controllers[0]->RecordFormats();

    // Get the first format that supports our mime type.
    
    TBool found( EFalse );

    for ( TInt i = 0; i < formats.Count(); i++ )
        {
        if ( formats[i]->SupportsMimeType( aMimeType ) )
            {
            iVideoFormatUid = formats[i]->Uid(); // set the UID
            found = ETrue;
            break;
            }
        }
    if ( !found )
        {
        // No appropriate video format found.
        LOGTEXT( _L( "Cae: CCaeEngineImp::FindVideoUidsL() leaving KErrNotSupported (no video format found)" ) );
        User::Leave( KErrNotSupported );
        }

    CleanupStack::PopAndDestroy( 3, cSelect ); // cselect, fselect, controllers

    LOGTEXT( _L( "Cae: CCaeEngineImp::FindVideoUidsL() returning" ) );
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::ConvertAndSetVideoAudioTypeL
// Convert audio type from TDesC8 to TFourCC, set to iVideoAudioType.
// -----------------------------------------------------------------------------
//
TFourCC CCaeEngineImp::ConvertAndSetVideoAudioTypeL(
    const TDesC8& aAudioType )
    {
    if ( aAudioType == KNullDesC8 )
        {
        return KMMFFourCCCodeNULL;
        }
    else
        {
        if ( aAudioType.Length() != 4 ) 
            {
            User::Leave( KErrArgument );
            }
        return TFourCC( aAudioType[0], aAudioType[1], aAudioType[2], aAudioType[3] );
        }
    }


// -----------------------------------------------------------------------------
// CCaeEngineImp::VideoRecordingTimesCallback
// Calls Video Recorder API DurationL() and RecordTimeAvailable().
// Finally calls McaeoVideoRecordingTimes().
// -----------------------------------------------------------------------------
//
TInt CCaeEngineImp::VideoRecordingTimesCallback( 
    TAny* aEngineImp )
    {
    LOGTEXT( _L( "Cae: CCaeEngineImp::VideoRecordingTimesCallback() entering" ) );

    CCaeEngineImp* thisEngineImp = static_cast<CCaeEngineImp*>( aEngineImp );

    if ( thisEngineImp->iVideoRecordingRunning && !thisEngineImp->iVideoRecordingPaused )
        {
        // Query elapsed recording time.
        TTimeIntervalMicroSeconds timeElapsed( 0 );
        TRAPD( error, { timeElapsed = thisEngineImp->iVideoRecorder->DurationL(); } );
        
        // Query remaining recording time.
        TTimeIntervalMicroSeconds timeRemaining = thisEngineImp->iVideoRecorder->RecordTimeAvailable();
        	
        // The elapsed time could have been reseted to zero by the video recorder 
        // if there is stopping ongoing in video recorder, even if we do 
        // not have had the notification yet. In that case McaeoVideoRecordingTimes
        // should be called with previous valid values.
        if ( ( timeElapsed == 0 ) && ( thisEngineImp->iPrevTimeElapsed != 0 ) )
        	{
        	// Invalid time. Use previous time information
   			LOGTEXT2( _L( "Cae: CCaeEngineImp::VideoRecordingTimesCallback(). Got invalid elapsed time from the video recorder: %f s." ), 
   				I64LOW( timeElapsed.Int64() ) * 1.0 / KOneSecond);
   			LOGTEXT3( _L( "Cae: CCaeEngineImp::VideoRecordingTimesCallback(). Use previous valid elapsed time %f s. and remaining time %f s." ), 
   				I64LOW( thisEngineImp->iPrevTimeElapsed.Int64() ) * 1.0 / KOneSecond,  
   				I64LOW( thisEngineImp->iPrevTimeRemaining.Int64() ) * 1.0 / KOneSecond );
        	timeElapsed = thisEngineImp->iPrevTimeElapsed;
        	timeRemaining = thisEngineImp->iPrevTimeRemaining;
        	}
        else
         	{
         	// Store current time information
        	thisEngineImp->iPrevTimeElapsed = timeElapsed;
        	thisEngineImp->iPrevTimeRemaining = timeRemaining;
        	}
       
       	// Deliver video recording times information.
       	thisEngineImp->iCaeObserver->McaeoVideoRecordingTimes( timeElapsed, timeRemaining, error );
        }

    LOGTEXT( _L( "Cae: CCaeEngineImp::VideoRecordingTimesCallback() returning" ) );

    return ETrue;
    }

// ---------------------------------------------------------
// CCaeEngineImp::EnumerateVideoFrameSizeL
// Checks that camera HW supports given framesize
// ---------------------------------------------------------
//
TInt CCaeEngineImp::EnumerateVideoFrameSizeL(const TSize& aSize)
    {

    LOGTEXT3( _L("Cae: CCaeEngineImp::EnumerateVideoFrameSizeL(), In, size: %dx%d"), aSize.iWidth, aSize.iHeight );

    TCameraInfo cameraInfo;
    iCamera->CameraInfo( cameraInfo );
    
    TSize size;
    TInt i = 0;
    TInt j = 0;
    TInt sizeIndex = -1;

    if ( cameraInfo.iVideoFrameFormatsSupported & CCamera::EFormatYUV420Planar )
        {
        // check YUV420 planar
        for ( i = 0; i < cameraInfo.iNumVideoFrameSizesSupported; i++ )
            {
            iCamera->EnumerateVideoFrameSizes(size, i, CCamera::EFormatYUV420Planar);
			LOGTEXT3(_L("Cae: CCaeEngineImp::EnumerateVideoFrameSizeL() Camera EFormatYUV420Planar %d x %d found from camera"), size.iWidth, size.iHeight ); 
        
            if ( size == aSize )
                {
				LOGTEXT3(_L("Cae: CCaeEngineImp::EnumerateVideoFrameSizeL() Camera EFormatYUV420Planar %d x %d Matched."), size.iWidth, size.iHeight ); 
                sizeIndex = i;
                break;
                }
            }
        }
    else
    	{
		LOGTEXT(_L("Cae: CCaeEngineImp::EnumerateVideoFrameSizeL() Camera doesn't support EFormatYUV420Planar"));
    	}

    if ( cameraInfo.iVideoFrameFormatsSupported & CCamera::EFormatYUV422 )
        {
        // check YUV422 interleaved
        for ( j = 0; j < cameraInfo.iNumVideoFrameSizesSupported; j++ )
            {
            iCamera->EnumerateVideoFrameSizes(size, j, CCamera::EFormatYUV422 );
			LOGTEXT3(_L("Cae: CCaeEngineImp::EnumerateVideoFrameSizeL() Camera EFormatYUV422 %d x %d found from camera"), size.iWidth, size.iHeight );
        
            if ( size == aSize )
                {
                LOGTEXT3(_L("Cae: CCaeEngineImp::EnumerateVideoFrameSizeL() Camera EFormatYUV422 %d x %d Matched."), size.iWidth, size.iHeight );
                sizeIndex = j;
                break;
                }
            }
        }
    else
    	{
		LOGTEXT(_L("Cae: CCaeEngineImp::EnumerateVideoFrameSizeL() Camera doesn't support EFormatYUV422"));	
    	}

    if ( sizeIndex == -1 )
        {
        LOGTEXT(_L("Cae: CCaeEngineImp::EnumerateVideoFrameSizeL(), Camera does not support this resolution"));
        }


    LOGTEXT(_L("Cae: CCaeEngineImp::EnumerateVideoFrameSizeL(), Out"));
    return ( sizeIndex );
    }

// ---------------------------------------------------------
// CCaeEngineImp::SetAsyncVideoStopMode
// Enables/disables async stopping
// ---------------------------------------------------------
//
TInt CCaeEngineImp::SetAsyncVideoStopMode( TBool aAsyncVideoStopEnabled )
    {
    LOGTEXT2( _L("Cae: CCaeEngineImp::SetAsyncVideoStop(%d) entering"), aAsyncVideoStopEnabled );
    TInt err;

    // Check if async stopping is supported
    if ( iVideoControllerUid == KCamCControllerImplementationUid ) 
        {
        iAsyncVideoStopEnabled = aAsyncVideoStopEnabled; 
        err = KErrNone;
        }
    else
        {
        err = KErrNotSupported;
        }

    LOGTEXT2(_L("Cae: CCaeEngineImp::SetAsyncVideoStop() err=%d, returning"), err);
    return ( err );
    }


//  End of File  
