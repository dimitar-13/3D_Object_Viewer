#include "pch.h"
#include "RenderingCoordinator.h"
#include "Core/WindowHandler.h"
#include "gpu_side/Framebuffer.h"
#include "Profiling/AppProfiler.h"
#include "Helpers/DialogWrapper.h"

void OBJ_Viewer::RenderingCoordinator::RenderLoop()
{
	GLFWwindow* application_GLFW_window = this->m_application.GetGlobalAppWindow().GetGLFW_Window();

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

	while (!glfwWindowShouldClose(application_GLFW_window))
	{
		if (m_currentWindowState != WindowState_kWindowMinimized) {
            if (!m_application.IsUIHidden())
            {
				m_UILayer->RenderUI(m_renderingConfigSettings,m_SceneManager->GetSceneModel(), m_SceneManager->GetSceneRegistry(),
                    m_SceneManager->GetSkyboxModel());
            }

			RenderScene();
			glfwSwapBuffers(application_GLFW_window);
		}
		glfwPollEvents();

		//FPSLogger::log();
	}

}

void OBJ_Viewer::RenderingCoordinator::RenderScene()
{
	//We set the alpha to 0 here in case we want to do blender with the grid.
	glClearColor(0, 0, 0, 0);

	if (m_application.IsUIHidden())
	{
		m_SceneManager->RenderScene(m_renderingConfigSettings);
	}
	else
		m_SceneManager->RenderScene(m_renderingConfigSettings, &m_UILayer->GetInputFramebuffer());

}

void OBJ_Viewer::RenderingCoordinator::onEventTakeScreenshot(const ScreenshotEvent& e)
{
    if (m_saveImgResult.valid())
        m_saveImgResult.wait();

    const ImgOutputData& kEventDataImageData = e.GetScreenshotEventData();
    //App state saving
    const bool kPreviousSkyboxEnableState = m_renderingConfigSettings.IsSkyboxOn;
    const bool kPreviousGridEnableState = m_renderingConfigSettings.IsWireGridOn;
    const Viewport& kPreviousApplicationViewport = m_application.GetSceneViewport().GetViewport();

    DialogWrapper dialogFolderPath;
    dialogFolderPath.OpenDialogSaveFile(TextureFileEnumConverter::GetStringTextureFormatFromEnum(kEventDataImageData.outImgFormat).data());

    if (dialogFolderPath.IsDialogClosed())
        return;

    const std::string& kOutputFilePath = dialogFolderPath.GetFirstDialogResult();

	TextureFormat_ is_output_image_with_transparency = kEventDataImageData.allowTransparency ? TextureFormat_::TextureFormat_kRGBA :
		TextureFormat_::TextureFormat_kRGB;

	Viewport OutputImageViewport{};
	OutputImageViewport.width = kEventDataImageData.imgSize.width;
	OutputImageViewport.height = kEventDataImageData.imgSize.height;

	//Change app state
    m_renderingConfigSettings.IsSkyboxOn =
        m_renderingConfigSettings.IsSkyboxOn && !kEventDataImageData.renderObjectOnly;
    m_renderingConfigSettings.IsWireGridOn =
        m_renderingConfigSettings.IsWireGridOn && !kEventDataImageData.renderObjectOnly;

	m_application.SubmitSceneViewportSize(OutputImageViewport);

	m_SceneManager->RenderScene(m_renderingConfigSettings, &m_UILayer->GetInputFramebuffer());

	glFinish();
	
	auto output_image_pixel_data = 
		std::make_shared<std::vector<Framebuffer::pixel_component>>(m_UILayer->GetInputFramebuffer().GetFramebufferPixels(is_output_image_with_transparency));

	m_saveImgResult = 
		std::async(std::launch::async, TexturePixelSaver::SavePicture, kOutputFilePath, kEventDataImageData.imgSize,
		is_output_image_with_transparency, output_image_pixel_data, kEventDataImageData.outImgFormat);
  
	//Restore original state
	m_application.SubmitSceneViewportSize(kPreviousApplicationViewport);

    m_renderingConfigSettings.IsSkyboxOn = kPreviousSkyboxEnableState;
    m_renderingConfigSettings.IsWireGridOn = kPreviousGridEnableState;

}

void OBJ_Viewer::RenderingCoordinator::OnEvent(Event& e)
{
	if (e.GetEventCategory() == EventCategory_kAppEvent && e.GetEventType() == EventType_kWindowStateChanged)
		m_currentWindowState = dynamic_cast<WindowStateChangedEvent&>(e).GetWindowState();
	else if (e.GetEventCategory() == EventCategory_kAppEvent && e.GetEventType() == EventType_kScreenshotButtonPressed)
		onEventTakeScreenshot(dynamic_cast<ScreenshotEvent&>(e));
}


OBJ_Viewer::RenderingCoordinator::RenderingCoordinator(Application& application):
	m_application(application)
{
	m_SceneManager = std::make_shared<SceneManager>(application);
	m_application.AddEventListener(m_SceneManager);
	m_UILayer = std::make_unique<UILayer>(m_application);
    m_application.AddEventListener(m_UILayer);

}