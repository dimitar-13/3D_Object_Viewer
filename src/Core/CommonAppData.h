#pragma once
#include "pch.h"
namespace OBJ_Viewer {
	struct Size2D
	{
		int width;
		int height;

		//It sounded complex but when I look at glm impl I felt dumb.
		int& operator [](size_t index)
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
				assert(false && "Index out of range.");
				break;
			}
		}

		/*int& const operator [](size_t index)
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
		}*/

	};
	struct Position2D
	{
		double x;
		double y;
	};

	namespace APP_FOCUS_REGIONS {
		typedef  const char* AppWindowID;
		constexpr AppWindowID kUI_SceneWindowName = "Scene render window";
		constexpr AppWindowID kUI_ObjectLoadingWindowName = "Loading panel";
		constexpr AppWindowID kUI_Model_and_renderingSettingsWindowName = "ModelAndRenderingSettings";
		constexpr AppWindowID kUI_SceneSettingsWindowName = "Scene setting";
		constexpr AppWindowID kUI_WindowScreenshotSettings = "Screenshot settings";
		constexpr AppWindowID kUI_ApplicationControlInfoWindowName= "Application controls";
		constexpr AppWindowID kUI_WindowNameUnknown = "unknownWindow";
	}
}