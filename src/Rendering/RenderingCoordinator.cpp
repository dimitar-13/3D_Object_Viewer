#include "pch.h"
#include "RenderingCoordinator.h"
#include "Core/WindowHandler.h"
#include "gpu_side/Framebuffer.h"
#include "Profiling/AppProfiler.h"


void OBJ_Viewer::RenderingCoordinator::RenderLoop()
{
	GLFWwindow* window = this->m_application.GetGlobalAppWindow().GetGLFW_Window();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glCullFace(GL_BACK);

	while (!glfwWindowShouldClose(window))
	{
		if (m_currentWindowState != WINDOW_STATE_MINIMIZED) {
			if (!m_application.isUIHidden())
				m_UILayer->RenderUI();

			RenderScene();
			glfwSwapBuffers(window);
		}
		glfwPollEvents();

		//FPSLogger::log();
	}

}

void OBJ_Viewer::RenderingCoordinator::RenderScene()
{
	auto& appSettings =	m_application.GetScene_RefSettings();

	//Set up the renderer based on the settings;
 
	//framebuffer.BindFramebuffer();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//I didnt thought of this but if you want to do blending and you render the grid last do yourself a favor and set the alpha here to 0;
	glClearColor(0, 0, 0, 0);

	if (m_application.isUIHidden())
	{
		m_sceneRenderer->RenderScene(appSettings);
	}
	else
		m_sceneRenderer->RenderScene(appSettings, &m_UILayer->GetInputFramebuffer());

}

void OBJ_Viewer::RenderingCoordinator::onEventTakeScreenshot(const ScreenshotEvent& e)
{
	auto& renderingConfig = m_application.GetScene_RefSettings();
	//App state saving
	const bool APP_PREVIOUS_SKYBOX_STATE = renderingConfig.m_isSkyboxOn;
	const bool APP_PREVIOUS_GRID_STATE = renderingConfig.m_isWireGridOn;
	const Viewport& APP_PREVIOUS_VIEWPORT = m_application.GetSceneViewport().GetViewport();
	const ImgOutputData& EVENT_DATA = e.ImgData();
	const TextureFormat USE_TRANSPERANT_FORMAT =EVENT_DATA.allowTransparency ? TextureFormat::TEXTURE_FORMAT_RGBA :
		TextureFormat::TEXTURE_FORMAT_RGB;

	Viewport OutputImageViewport{};
	OutputImageViewport.x = 0;
	OutputImageViewport.y = 0;
	OutputImageViewport.width = EVENT_DATA.imgSize.width;
	OutputImageViewport.height = EVENT_DATA.imgSize.height;

	//Change app state
	renderingConfig.m_isSkyboxOn = renderingConfig.m_isSkyboxOn   && !EVENT_DATA.renderObjectOnly;
	renderingConfig.m_isWireGridOn = renderingConfig.m_isWireGridOn && !EVENT_DATA.renderObjectOnly;
	m_application.SubmitSceneViewportSize(OutputImageViewport);

	m_sceneRenderer->RenderScene(renderingConfig, &m_UILayer->GetInputFramebuffer());

	glFinish();
	
	const std::shared_ptr<std::vector<Framebuffer::pixel_component>> renderScenePixelData = 
		std::make_shared<std::vector<Framebuffer::pixel_component>>(m_UILayer->GetInputFramebuffer().GetFramebufferPixels(USE_TRANSPERANT_FORMAT));

	m_saveImgResult = std::async(std::launch::async, TexturePixelSaver::SavePicture, EVENT_DATA.outPath, EVENT_DATA.imgSize,
		USE_TRANSPERANT_FORMAT,renderScenePixelData, EVENT_DATA.outImgFormat);

	//Restore original state
	m_application.SubmitSceneViewportSize(APP_PREVIOUS_VIEWPORT);
	renderingConfig.m_isSkyboxOn = APP_PREVIOUS_SKYBOX_STATE;
	renderingConfig.m_isWireGridOn = APP_PREVIOUS_GRID_STATE;

}

void OBJ_Viewer::RenderingCoordinator::OnEvent(Event& e)
{
	if (e.GetEventCategory() & APP_EVENT && e.GetEventType() == EVENT_WINDOW_STATE_CHANGED)
		m_currentWindowState = dynamic_cast<WindowStateChangedEvent&>(e).GetWindowState();
	else if (e.GetEventCategory() & APP_EVENT && e.GetEventType() == EVENT_SCREENSHOT_BUTTON_PRESSED)
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