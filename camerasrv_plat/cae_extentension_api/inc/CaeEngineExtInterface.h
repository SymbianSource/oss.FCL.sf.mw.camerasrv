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
* Description:  Camera Application Engine interface for extensions
*
*/



#ifndef CAEENGINEEXTINTERFACE_H
#define CAEENGINEEXTINTERFACE_H

//  INCLUDES

#include <e32std.h>
#include <fbs.h>
#include <ecom/ecom.h>

#include "CaeEngineExtInterface.hrh"

// CONSTANTS

// Common UID that identifies all Cae extensions
const TUid KExtensionInterfaceUid = {KExtensionInterfaceUidValue};

// Common string that identifies all Cae extensions
_LIT8( KExtensionTypeStringDesc, KExtensionTypeString);

// Global flags for extension handling. Affect how the engine handles extensions. 
enum TCaeExtFlags
        {
        ECaeExtFlagRequireFullColorSnapInputImage =        0x00000001
        /* \todo Not yet implemented
        ECaeExtFlagRequireStillImageAsBitmap =             0x00000002,
        ECaeExtFlagRequireImageFormatHeader =              0x00000004
        */
        };


// FORWARD DECLARATIONS

class MExtensionCallbackInterface;
class MExtension;


// CLASS DECLARATIONS

/**
* Main extension interface class.	
*/

class MExtension
	{
	public:
        /**
        * Create the extension. Create the object, get pointers to callback interfaces
		* and possibly add extension interfaces to the engine.
        * @since 2.8
        * @param aImplementationUid The uid for ECom to find the correct dll that implements this extension
        * @param aEngine Pointer to the callback interface in the engine. Delivered to the extension.
        * @return MExtension Pointer to the extension. Returned to the engine.
        */
 		static MExtension* NewExtensionL( TUid aImplementationUid, MExtensionCallbackInterface* aEngine );

        /**
        * Virtual destructor. 
        * @since 2.8
        */
		virtual ~MExtension() { REComSession::DestroyedImplementation (iDtor_ID_Key); };

	private:

		// The ID used by ECom for destroying this dll
		TUid	iDtor_ID_Key;
	};


/**
* Main callback interface implemented in the engine. The extension uses this
* interface to add to and remove own interface implementations from the engine 
* lists. Also, pointers to the callback interfaces implemented in the engine, 
* can be fetched.  The extension usually calls this interface during creation. 
*/

class MExtensionCallbackInterface
	{
	public:
        /**
        * Check that the interface is supported by the engine. 
        * @since 2.8
        * @param aInterfaceUid Uid for the interface.
        * @return TBool Whether the interface is supported.
        */
		virtual TBool IsInterfaceSupported( TUid aInterfaceUid ) = 0;

        /**
        * Add an extension implemented interface to the list in engine. 
		* The engine starts to call this interface.
        * @since 2.8
        * @param aInterfaceUid Uid for the interface.
        * @param aExtensionUid Uid for the extension that implements the interface.
        * @param aInterfacePtr Pointer to the interface implementation.
        * @param aPriority Priority of the interface. Interfaces with bigger priority value are called first.
        * @return TInt Error code.
        */
		virtual TInt AddExtensionInterface( TUid aInterfaceUid, TUid aExtensionUid, TAny* aImplementationPtr, TInt aInitialPriority = 0 ) = 0;

        /**
        * Remove an extension implemented interface from the list in engine. 
		* The engine stops calling this interface.
        * @since 2.8
        * @param aInterfaceUid Uid for the interface.
        * @param aInterfacePtr Pointer to the interface implementation.
        * @return TInt Error code.
        */
		virtual TInt RemoveExtensionInterface( TUid aInterfaceUid, TAny* aImplementationPtr ) = 0;

        /**
        * Add an extension implemented custom interface to the list in engine. 
		* The custom interface is not called by the engine. It is called by the application.
		* The engine gives the pointer to the application when requested.
		* Note that there is no remove method for a custom interface because there is not 
		* specified a way to tell to the application when the interface had been removed.
        * @since 2.8
        * @param aInterfaceUid Specific uid for the interface.
        * @param aInterfacePtr Pointer to the interface implementation.
        * @return TInt Error code.
        */
		virtual TInt AddCustomInterface( TUid aInterfaceUid, TAny* aImplementationPtr ) = 0;

        /**
        * Get an engine implemented callback interface. An extension can call this interface. 
        * @since 2.8
        * @param aInterfaceUid Specific uid for the interface.
        * @return TAny* Pointer to the callback interface.
        */
		virtual TAny* GetCallbackInterface( TUid aInterfaceUid ) = 0;

        /**
        * Register flags which affect general extension handling in the engine. 
        * Flags must be deregisterd when no longer needed as they can affect
        * to the engine performance. Each registered flag must be deregistered, 
        * and only once. All the flags that are in registered state when 
        * the class is deleted, must be deregistered in the class destructor
        * method. 
        * @since 2.8
        * @param aFlags Flags to register.
        * @return void.
        */
		virtual void RegisterFlags( TUint32 aFlags ) = 0;

        /**
        * Deregister flags which have been registered before. It is not allowed
        * to deregister flags that have not been registered earlier.  
        * @since 2.8
        * @param aFlags Flags to deregister.
        * @return void.
        */
		virtual void DeregisterFlags( TUint32 aFlags ) = 0;
	};


/**
* Image processing interface. The engine calls this interface when it gets 
* a new view finder or captured image from the camera. The extension that 
* implements this can manipulate the image before it is given to the 
* application. 
*/

const TInt KCaeExtProcessImageInterfaceUidValue = 0x101F856A;
const TUid KCaeExtProcessImageInterfaceUid = { KCaeExtProcessImageInterfaceUidValue };

class MCaeExtProcessImageInterface
	{
	public:
        /**
        * Process the view finder bitmap. 
        * @since 2.8
        * @param aFrame The VF frame bitmap.
        * @param aError Error code
        */
		virtual void ProcessViewFinderFrame( CFbsBitmap& aFrame, TInt& aError ) = 0;

        /**
        * Process the captured image from the camera. The function can be synchronous or 
		* asynchronous.
        * @since 2.8
        * @param aCaeStatus The status of the finished operation.
        * @param aBitmap The captured image, if it is a bitmap.
        * @param aImageData The captured image, if it is a formatted image.
        * @param aImageHeader The header of the captured image (e.g. Exif header).
        * @return TBool Whether the operation is asynchronous.
        */
		virtual TBool ProcessCapturedImage( TRequestStatus& aCaeStatus, CFbsBitmap*& aBitmap, HBufC8*& aImageData, HBufC8*& aImageHeader ) = 0;
		
        /**
        * Process the snap image. The function can be synchronous or asynchronous.
        * Modifications should be done on aSnapBitmap. If it is null the extension 
        * can create it using aStillBitmap as a source.
        * @since 2.8
        * @param aCaeStatus The status of the finished operation.
        * @param aBitmap The still image as a bitmap. Image can be NULL.
        * @param aSnapBitmap Returned snap bitmap. Image can be NULL.
        * @return TBool Whether the operation is asynchronous.
        */
		virtual TBool ProcessSnapImage( TRequestStatus& aCaeStatus, CFbsBitmap*& aStillBitmap, CFbsBitmap*& aSnapBitmap ) = 0;
		
        /**
        * Process the still image. The function can be synchronous or asynchronous.
        * @since 2.8
        * @param aCaeStatus The status of the finished operation.
        * @param aBitmap The still image, if it is a bitmap.
        * @param aImageData The still image, if it is a formatted image.
        * @param aImageHeader The header of the captured image (e.g. Exif header).
        * @return TBool Whether the operation is asynchronous.
        */
		virtual TBool ProcessStillImage( TRequestStatus& aCaeStatus, CFbsBitmap*& aBitmap, HBufC8*& aImageData, HBufC8*& aImageHeader ) = 0;
		
        /**
        * Cancel the asynchronous operation. 
        * @since 2.8
        */
		virtual void CaeExtensionCancel() = 0;
	};


/**
* Settings interface. The engine calls this when settings should be saved or 
* restored. Note that not all settings type of exttensions need to implement this
* interface.
*/

const TInt KCaeExtSettingsInterfaceUidValue = 0x101F8569;
const TUid KCaeExtSettingsInterfaceUid = { KCaeExtSettingsInterfaceUidValue };

class MCaeExtSettingsInterface
	{
	public:

        /**
        * Restore previously saved settings.
        * @since 2.8
        */
		virtual void ResetToPreviousSettingsL() = 0;				

        /**
        * Restore default settings.
        * @since 2.8
        */
		virtual void ResetToDefaultsL() = 0;
	};


/**
* Engine info interface. Implemented by the engine. The extension can get info 
* about the current engine state.
*/

const TInt KCaeExtEngineInfoCallbackInterfaceUidValue = 0x101F856B;
const TUid KCaeExtEngineInfoCallbackInterfaceUid = { KCaeExtEngineInfoCallbackInterfaceUidValue };

class MCaeExtEngineInfoCallbackInterface
    {
    public: 
        /**
        * Get current still image size. 
        * @since 2.8
        * @return TSize Still image size in pixels.
        */
		virtual TSize McaeExtStillImageSize() = 0;

        /**
        * Get current snap image size. 
        * @since 2.8
        * @return TSize Snap image size in pixels.
        */
		virtual TSize McaeExtSnapImageSize() = 0;

        /**
        * Get current view finder size. 
        * @since 2.8
        * @return TSize View finder size in pixels.
        */
		virtual TSize McaeExtViewFinderSize() = 0;

        /**
        * Checks that the camera power is on. If it is not, the function leaves with 
		* KErrNotReady.
        * @since 2.8
        * return void.
        */
		virtual void McaeExtCheckPowerL() = 0;

        /**
        * Get display index. 
        * @since 2.8
        * @return TInt Display index.
        */
		virtual TInt McaeExtDisplayIndex() = 0;
    };


#endif // CAEENGINEEXTINTERFACE_H