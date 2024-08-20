#pragma once

#include "KeyboardKeys.h"
#include "MouseKeys.h"
namespace OBJ_Viewer {
	//TODO:Consider changing it to struct to support custom controls.

    /**
     * @brief Enum representing the application key/mouse bindings.
     * 
     * The enum is compatable with 'KeyboardKey_' and 'MouseKey_' via static_cast.
     * 
     * Example use:
     * ```cpp
     * 
     * // Is the UI hidden key pressed 
     * 
     * if(static_cast<AppKeyBinding_>(e.GetKey()) == AppKeyBinding_kHideUI)
     * {
     *      //Handle stuff here...
     * }
     * ```
     * 
     */
	enum AppKeyBinding_
	{
        AppKeyBinding_kUnknown,
		AppKeyBinding_kHideUI = KeyboardKey_kH,
		AppKeyBinding_kResetCameraPivot = KeyboardKey_kQ,
        AppKeyBinding_kCameraRotation = MouseKey_kLeftMouseButton,
        AppKeyBinding_kCameraShifting = MouseKey_kRightMouseButton
	};


}