#pragma once
#include "pch.h"
namespace OBJ_Viewer {
    /**
     * @brief Application structure for 2D sizes.
     *
     * Structure used across the application for sizes.
     */
	struct Size2D
	{
		int width;
		int height;

		int& operator [](size_t index)
		{
            //It sounded complex but when I look at glm impl I felt dumb.

			switch (index)
			{
			case 0:
				return width;
				break;
			case 1:
				return height;
				break;
			default:
				assert(false && "Index out of range.");
				break;
			}
		}
        const int& operator [](size_t index) const
		{
			switch (index)
			{
			case 0:
				return width;
				break;
			case 1:
				return height;
				break;
			default:
				assert("Index out of range;");
				break;
			}
		}
	};
    /**
     * @brief Application structure used for 2D positions.
     * 
     * Structure mainly used for mouse and scroll positions.
     */
	struct Position2D
	{
		double x; ///< The x position.
		double y; ///< The y position.
	};

    /**
     * @brief Namespace representing the UI panels and their names.  
     */
	namespace APP_FOCUS_REGIONS {
		typedef const char* AppWindowID;
		constexpr AppWindowID kUI_SceneWindowName = "Scene render window";
		constexpr AppWindowID kUI_ObjectLoadingWindowName = "Loading panel";
		constexpr AppWindowID kUI_Model_and_renderingSettingsWindowName = "ModelAndRenderingSettings";
		constexpr AppWindowID kUI_SceneSettingsWindowName = "Scene setting";
		constexpr AppWindowID kUI_WindowScreenshotSettings = "Screenshot settings";
		constexpr AppWindowID kUI_ApplicationControlInfoWindowName= "Application controls";
		constexpr AppWindowID kUI_WindowNameUnknown = "unknownWindow";
	}
}