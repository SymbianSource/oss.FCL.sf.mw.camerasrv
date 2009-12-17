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
* Description:  Camera Application Engine Shared Data UID and keys
*
*/



#ifndef CAESDKEYS_H
#define CAESDKEYS_H


// CONSTANTS

// -----------------------------------------------------------------------------
// Camera Application Engine 0x101f8569
// -----------------------------------------------------------------------------

const TUid KSDUidCamAppEngine = {0x101f8569};

/**
* Count of still image capturing quality levels, integer value
*
* Default value: 3
*/
_LIT( KCaeStillQltyLevelCount, "StillQltyC" );

/**
* Still image capturing quality level 0, multiparameter string
*
* String format: width height format quality imagesize
* width: frame/image width in pixels
* height: frame/image height in pixels
* format: Camera API CCamera::TFormat code (e.g. EFormatJpeg = 0x0010 ==> value 10 here)
* quality: compression quality 0...100 
* 0 corresponds to worst image quality (highest compression), 
* 100 corresponds to best image quality (lowest compression)
* bytesize: estimated storage size of an image in bytes (if 0 then no estimate is given)
*
* Default value: 640 480 10 90 0
*/
_LIT( KCaeStillQltyLevel0, "StillQlty0" );

/**
* Still image capturing quality level 1, multiparameter string
*
* String format: see the comment for level 0
*
* Default value: 640 480 10 50 0 
*/
_LIT( KCaeStillQltyLevel1, "StillQlty1" );

/**
* Still image capturing quality level 2, multiparameter string
*
* String format: see the comment for level 0
*
* Default value: 640 480 10 50 0 
*/
_LIT( KCaeStillQltyLevel2, "StillQlty2" );

/**
* Still image capturing quality level 3, multiparameter string
*
* String format: see the comment for level 0
*
* Default value: no default
*/
_LIT( KCaeStillQltyLevel3, "StillQlty3" );

/**
* Still image capturing quality level 4, multiparameter string
*
* String format: see the comment for level 0
*
* Default value: no default
*/
_LIT( KCaeStillQltyLevel4, "StillQlty4" );

/**
* Still image capturing quality level 5, multiparameter string
*
* String format: see the comment for level 0
*
* Default value: no default
*/
_LIT( KCaeStillQltyLevel5, "StillQlty5" );

/**
* Count of video recording quality levels, integer value
*
* Default value: 3
*/
_LIT( KCaeVideoQltyLevelCount, "VideoQltyC" );

/**
* Video recording quality level 0, multiparameter string
*
* String format: width height framerate bitrate audio mimetype supplier videotype audiotype byterate 
* width: frame/image width in pixels
* height: frame/image height in pixels
* framerate: encoded video frame rate (frames per second, fps)  
* bitrate: encoded video bit rate (bits per second, bps) 
* audio: is audio enabled? (0 = no, 1 = yes) 
* mimetype: video MIME type 
* supplier: supplier of video controller  
* videotype: video type
* audiotype: audio type (four character code)
* storagerate: estimated bit rate to storaged (bits per second, bps) (if 0 then no estimate is given)
*
* Default value: 176 144 10 50000 1 12200 video/3gpp Nokia video/H263-2000  AMR 0
*/
_LIT( KCaeVideoQltyLevel0, "VideoQlty0" );

/**
* Video image capturing quality level 1, multiparameter string
*
* String format: see the comment for level 0
*
* Default value: 128 96 10 30000 1 12200 video/3gpp Nokia video/H263-2000  AMR 0
*/
_LIT( KCaeVideoQltyLevel1, "VideoQlty1" );

/**
* Video image capturing quality level 2, multiparameter string
*
* String format: see the comment for level 0
*
* Default value: 128 96 5 15000 0 12200 video/3gpp Nokia video/H263-2000  AMR 0
*/
_LIT( KCaeVideoQltyLevel2, "VideoQlty2" );

/**
* Video image capturing quality level 3, multiparameter string
*
* String format: see the comment for level 0
*
* Default value: no default
*/
_LIT( KCaeVideoQltyLevel3, "VideoQlty3" );

/**
* Video image capturing quality level 4, multiparameter string
*
* String format: see the comment for level 0
*
* Default value: no default
*/
_LIT( KCaeVideoQltyLevel4, "VideoQlty4" );

/**
* Video image capturing quality level 5, multiparameter string
*
* String format: see the comment for level 0
*
* Default value: no default
*/
_LIT( KCaeVideoQltyLevel5, "VideoQlty5" );


#endif // CAESDKEYS_H   
            
// End of File
