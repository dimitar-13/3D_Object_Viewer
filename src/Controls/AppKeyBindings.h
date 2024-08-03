#pragma once

#include "KeyboardKeys.h"
#include "MouseKeys.h"
namespace OBJ_Viewer {
	//Consider changing it to struct to support custom controls.
	enum AppKeyBinding_
	{
		AppKeyBinding_kHideUI = KeyboardKey_kH,
		AppKeyBinding_kResetCameraPivot = KeyboardKey_kQ,
        AppKeyBinding_kCameraRotation = MouseKey_kLeftMouseButton,
        AppKeyBinding_kCameraShifting = MouseKey_kRightMouseButton
	};


}