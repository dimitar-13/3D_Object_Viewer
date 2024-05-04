#pragma once
namespace OBJ_Viewer
{
	static class RenderingSettings
	{
	public:
		static bool isWireFrameRenderingOn;
		static bool isRenderAlbedoTextureOn;
		static bool isRenderSpecularTextureOn;
		static bool isRenderNormalTextureOn;
		static bool isSkyboxOn;
		static bool isWireGridOn;
		//TODO:change from void to a Model struct representing a model.
		static void* currentlyLoadedModel;
	public:
		static void RenderLoop();
		static void RenderImGui();
	};
}

