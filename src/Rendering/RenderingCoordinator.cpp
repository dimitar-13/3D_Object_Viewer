#include "pch.h"
#include "RenderingCoordinator.h"
#include "Core/WindowHandler.h"
#include "gpu_side/Framebuffer.h"
#include "Profiling/AppProfiler.h"


void OBJ_Viewer::RenderingCoordinator::RenderLoop()
{
	GLFWwindow* application_window = this->m_application.GetGlobalAppWindow().GetGLFW_Window();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	//glCullFace(GL_BACK);

	while (!glfwWindowShouldClose(application_window))
	{
		if (m_currentWindowState != WINDOW_STATE_MINIMIZED) {
			if (!m_application.isUIHidden())
				m_UILayer->RenderUI();

			RenderScene();
			glfwSwapBuffers(application_window);
		}
		glfwPollEvents();

		//FPSLogger::log();
	}

}

void OBJ_Viewer::RenderingCoordinator::RenderScene()
{
	auto& application_rendering_config_settings = m_application.GetScene_RefSettings();

	//Set up the renderer based on the settings;
 
	//framebuffer.BindFramebuffer();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//I didnt thought of this but if you want to do blending and you render the grid last do yourself a favor and set the alpha here to 0;
	glClearColor(0, 0, 0, 0);

	if (m_application.isUIHidden())
	{
		m_sceneRenderer->RenderScene(application_rendering_config_settings);
	}
	else
		m_sceneRenderer->RenderScene(application_rendering_config_settings, &m_UILayer->GetInputFramebuffer());

}

void OBJ_Viewer::RenderingCoordinator::onEventTakeScreenshot(const ScreenshotEvent& e)
{
	auto& application_rendering_config_settings = m_application.GetScene_RefSettings();
	//App state saving
	const bool kPreviousSkyboxEnableState = application_rendering_config_settings.m_isSkyboxOn;
	const bool kPreviousGridEnableState = application_rendering_config_settings.m_isWireGridOn;

	const Viewport& kPreviousApplicationViewport = m_application.GetSceneViewport().GetViewport();
	const ImgOutputData& kEventDataImageData = e.ImgData();
	TextureFormat_ is_output_image_with_transparency = kEventDataImageData.allowTransparency ? TextureFormat_::TextureFormat_kRGBA :
		TextureFormat_::TextureFormat_kRGB;

	Viewport OutputImageViewport{};
	OutputImageViewport.width = kEventDataImageData.imgSize.width;
	OutputImageViewport.height = kEventDataImageData.imgSize.height;

	//Change app state
	application_rendering_config_settings.m_isSkyboxOn = 
		application_rendering_config_settings.m_isSkyboxOn && !kEventDataImageData.renderObjectOnly;
	application_rendering_config_settings.m_isWireGridOn = 
		application_rendering_config_settings.m_isWireGridOn && !kEventDataImageData.renderObjectOnly;

	m_application.SubmitSceneViewportSize(OutputImageViewport);

	m_sceneRenderer->RenderScene(application_rendering_config_settings, &m_UILayer->GetInputFramebuffer());

	glFinish();
	
	std::shared_ptr<std::vector<Framebuffer::pixel_component>> output_image_pixel_data = 
		std::make_shared<std::vector<Framebuffer::pixel_component>>(m_UILayer->GetInputFramebuffer().GetFramebufferPixels(is_output_image_with_transparency));

	m_saveImgResult = 
		std::async(std::launch::async, TexturePixelSaver::SavePicture, kEventDataImageData.outPath, kEventDataImageData.imgSize,
		is_output_image_with_transparency, output_image_pixel_data, kEventDataImageData.outImgFormat);

	//Restore original state
	m_application.SubmitSceneViewportSize(kPreviousApplicationViewport);

	application_rendering_config_settings.m_isSkyboxOn = kPreviousSkyboxEnableState;
	application_rendering_config_settings.m_isWireGridOn = kPreviousGridEnableState;

}

void OBJ_Viewer::RenderingCoordinator::OnEvent(Event& e)
{
	if (e.GetEventCategory() & EventCategory_kAppEvent && e.GetEventType() == EventType_kWindowStateChanged)
		m_currentWindowState = dynamic_cast<WindowStateChangedEvent&>(e).GetWindowState();
	else if (e.GetEventCategory() & EventCategory_kAppEvent && e.GetEventType() == EventType_kScreenshotButtonPressed)
		onEventTakeScreenshot(dynamic_cast<ScreenshotEvent&>(e));
}


OBJ_Viewer::RenderingCoordinator::RenderingCoordinator(Application& application):
	m_application(application)
{
	std::shared_ptr<RenderingMediator> mediator = std::make_shared<RenderingMediator>();
	m_sceneRenderer = std::make_shared<SceneRenderer>(application, mediator);
	m_application.AddEventListener(m_sceneRenderer);
	m_UILayer = std::make_unique<UILayer>(m_application, mediator);

}