#pragma once
namespace OBJ_Viewer {
	struct Size2D
	{
		int width;
		int height;
	};
	struct Position2D
	{
		double x;
		double y;
	};

	namespace APP_FOCUS_REGIONS {
		constexpr auto UI_LAYER_SCENE_WINDOW_NAME = "Scene";
		constexpr auto UI_LAYER_OBJECT_LOADING_WINDOW_NAME = "Loading panel";
		constexpr auto UI_LAYER_MODEL_AND_RENDERING_SETTINGS_WINDOW_NAME = "ModelAndRenderingSettings";
		constexpr auto UI_LAYER_SCENE_SETTINGS_WINDOW_NAME = "Scene setting";
		constexpr auto UI_WINDOW_UNKNOWN = "unknownWindow";
	}
}