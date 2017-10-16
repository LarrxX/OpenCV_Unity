#pragma once

#include "IUnityGraphics.h"

extern "C"
{
	static void UNITY_INTERFACE_API OnGraphicsDeviceEvent( UnityGfxDeviceEventType eventType );
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad( IUnityInterfaces* unityInterfaces );
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload();

	//Not related to Unity but allows to create a console to show debug output
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API RedirectIOToConsole( bool createConsole );
}