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
		constexpr AppWindowID UI_LAYER_SCENE_WINDOW_NAME = "Scene";
		constexpr AppWindowID UI_LAYER_OBJECT_LOADING_WINDOW_NAME = "Loading panel";
		constexpr AppWindowID UI_LAYER_MODEL_AND_RENDERING_SETTINGS_WINDOW_NAME = "ModelAndRenderingSettings";
		constexpr AppWindowID UI_LAYER_SCENE_SETTINGS_WINDOW_NAME = "Scene setting";
		constexpr AppWindowID UI_WINDOW_SCREENSHOT_SETTINGS = "Screenshot settings";
		constexpr AppWindowID UI_WINDOW_APP_CONTROLINFO= "Application controls";
		constexpr AppWindowID UI_WINDOW_UNKNOWN = "unknownWindow";
	}
}