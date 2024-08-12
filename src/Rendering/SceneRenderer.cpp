#include "pch.h"
#include "SceneRenderer.h"
#include "Renderer.h"
#include "Core/Application.h"
#include "Scene/Skybox.h"
#include "ShaderPath.h"
#include "Profiling/AppProfiler.h"
#include "Helpers/DialogWrapper.h"
#include "Helpers/ModelGenerator.h"

#pragma region Constants
constexpr uint8_t kMatrixUniformBufferBindingPoint = 0;
constexpr uint8_t kLightUniformBufferBindingPoint = 1;

constexpr uint8_t kUniformBufferMatrixCount = 4;
constexpr size_t kUniformBufferMatrixSizeInBytes = kUniformBufferMatrixCount * sizeof(glm::mat4);
constexpr size_t kUniformBufferLightSIzeInBytes = OBJ_Viewer::APP_SETTINGS::SceneLightInfo::kMaxLightCount 
* OBJ_Viewer::APP_SETTINGS::SceneLightInfo::kSizeOfLightInBytes;
#pragma endregion
OBJ_Viewer::SceneManager::SceneManager(Application& app) :
#pragma region Buffer setup
	m_multiSampleSceneFrameBuffer(app.GetSceneViewport().GetViewportSize(), FramebufferAttachmentsFlags_kColorAttachment, true, 11),
	m_intermidiateFramebuffer(app.GetSceneViewport().GetViewportSize(), FramebufferAttachmentsFlags_kColorAttachment),
	m_uniformLightBuffer("LightInfo",kLightUniformBufferBindingPoint , kUniformBufferLightSIzeInBytes, nullptr),
	m_uniformMatrixBuffer("Matrices",kMatrixUniformBufferBindingPoint,kUniformBufferMatrixSizeInBytes, nullptr),
#pragma endregion
	m_screenQuad(ModelGenerator::GeneratePlaneVAOStack()),
	m_app(app)
{
    constexpr float kDefaultCameraZoom = 5.0f;
	SetUpUniformBuffers();

	m_sceneCamera = std::make_shared<Camera>(kDefaultCameraZoom, app.GetGlobalInputHandler(), app.GetSceneViewport());
	m_app.AddEventListener(m_sceneCamera);

	m_sceneModel = ModelGenerator::GenerateCubeModel();
    std::shared_ptr<Material> default_application_material = std::make_shared<Material>("Application default material");
    m_sceneRegistry = std::make_shared<MaterialRegistry>(std::vector{ default_application_material });
}
OBJ_Viewer::SceneManager::~SceneManager()
{
}
void OBJ_Viewer::SceneManager::RenderScene(const APP_SETTINGS::SceneConfigurationSettings& renderSettings, Framebuffer* outputFrameBuffer)
{
    constexpr int kFramebufferClearBitMask = GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT;
	SetUniformMatrixBuffer();
   
	if (renderSettings.m_EnableAA)
		m_multiSampleSceneFrameBuffer.BindFramebuffer();
	else
		m_intermidiateFramebuffer.BindFramebuffer();


	glClear(kFramebufferClearBitMask);

	for (const auto& mesh : m_sceneModel->GetModelMeshes())
	{
		switch (renderSettings.m_currentRenderingMode)
		{
		case APP_SETTINGS::RenderingMode_::RenderingMode_kWireframe:
            if (renderSettings.wireframeSettings.isPointRenderingOn){
                m_mainRenderer.RenderMeshWireframePoint(m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kPointShader),
                    mesh.GetMeshVAO(), renderSettings.wireframeSettings, m_app.GetSceneViewport().GetViewportMatrix());
            }
            else
            {
                m_mainRenderer.RenderMeshWireframeLine(m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kWireframeShader),
                    mesh.GetMeshVAO(), renderSettings.wireframeSettings, m_app.GetSceneViewport().GetViewportMatrix());
            }
            break;
		case APP_SETTINGS::RenderingMode_::RenderingMode_kSolidColor:
            m_mainRenderer.RenderStudioWithLight(m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kStudioLightShader),
                mesh.GetMeshVAO(), renderSettings.m_colorRenderingColor);
			break;

		case APP_SETTINGS::RenderingMode_::RenderingMode_kIndividualTexture:
            m_mainRenderer.RenderMeshSingleTexture(m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kSingleTextureShader),
                mesh, m_sceneRegistry->GetMaterialAtIndex(mesh.GetMaterialIndex()), renderSettings.m_curentIndividualTexture);
			break;

		case APP_SETTINGS::RenderingMode_::RenderingMode_kUV:
            m_mainRenderer.RenderCheckboardPattern(m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kUVShader),
                mesh.GetMeshVAO(), renderSettings.m_uvViewSettings);
			break;

		case APP_SETTINGS::RenderingMode_::RenderingMode_kLight:
            m_uniformLightBuffer.SendBufferSubData(0, renderSettings.lightInfo.lights.size() * 
                APP_SETTINGS::SceneLightInfo::kSizeOfLightInBytes,renderSettings.lightInfo.lights.data());

            m_mainRenderer.RenderMeshWithLight(m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kLightShader),
                mesh.GetMeshVAO(), m_sceneRegistry->GetMaterialAtIndex(mesh.GetMaterialIndex()), renderSettings.m_MaterialFlags,
                renderSettings.lightInfo, m_sceneCamera->GetCameraPos());
			break;

		case APP_SETTINGS::RenderingMode_kNormalOrientation:
			m_mainRenderer.RenderMesh(m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kNormalShader), mesh.GetMeshVAO());
			break;
		default:
			break;
		}
	}

	if (renderSettings.m_EnableAA)
	{
		m_intermidiateFramebuffer.CopyFramebufferContent(m_multiSampleSceneFrameBuffer,
            static_cast<FramebufferBitMaskFlags_>(FramebufferBitMaskFlags_kColorBufferBit | FramebufferBitMaskFlags_kDepthBufferBit));

		m_multiSampleSceneFrameBuffer.UnbindFramebuffer();

		m_intermidiateFramebuffer.BindFramebuffer();
	}

	if (renderSettings.m_isWireGridOn)
	{
		m_mainRenderer.RenderInfiniteGrid(m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kGridShader),
            m_screenQuad, m_sceneCamera->GetCameraPos(), renderSettings.m_gridData);
	}

    if (renderSettings.m_isSkyboxOn && m_sceneSkybox != nullptr)
    {
        m_mainRenderer.RenderSkybox(m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kSkyboxShader), *m_sceneSkybox,
            m_sceneCamera->IsCameraProjectionPerspective());
    }

	m_intermidiateFramebuffer.UnbindFramebuffer();


	if (outputFrameBuffer == nullptr)
	{
		Framebuffer::BindDefaultFramebuffer();
		glClear(kFramebufferClearBitMask);
		PostProcessScene(renderSettings.m_EnableAA);
	}
	else
	{
		outputFrameBuffer->BindFramebuffer();
		glClear(kFramebufferClearBitMask);
		PostProcessScene(renderSettings.m_EnableAA);
		outputFrameBuffer->UnbindFramebuffer();
	}
}

void OBJ_Viewer::SceneManager::PostProcessScene(bool doFXAA)
{
	glActiveTexture(GL_TEXTURE1);
	Texture& framebuffer_texture_for_post_process = m_intermidiateFramebuffer.GetFramebufferTexture();

	const SceneViewport& kCurrent_scene_viewport = m_app.GetSceneViewport();

	const Size2D kViewport_size = kCurrent_scene_viewport.GetViewportSize();

	glm::vec2 shader_uniform_window_resolution = glm::vec2(kViewport_size.width, kViewport_size.height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_DST_ALPHA);
	framebuffer_texture_for_post_process.BindTexture();
	auto& shader = m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kPostProcessShader);
	shader.UseShader();
	shader.SetUniformSet1Int("u_framebufferTexture", 1);
	shader.SetUniformSet2FloatVector("u_resolution", shader_uniform_window_resolution);
	shader.SetUniformSet1Int("u_useAA", doFXAA);

	m_mainRenderer.RenderMesh(shader, m_screenQuad);
	framebuffer_texture_for_post_process.UnbindTexture();
	glDisable(GL_BLEND);
}

void OBJ_Viewer::SceneManager::LoadSkybox(std::vector<std::string>& paths)
{

	if (paths.empty())
	{
		LOGGER_LOG_ERROR("No paths provided for creating a skybox.");
		return;
	}
	if(paths.size() < Skybox::SKYBOX_FACE_COUNT)
	{
		LOGGER_LOG_ERROR("Not enough paths were provided for skybox creation.Provided count was {0} when {1} was expected",
			paths.size(), Skybox::SKYBOX_FACE_COUNT);
		return;
	}
    stbi_set_flip_vertically_on_load(false);

	std::array<TexturePixelReader,Skybox::SKYBOX_FACE_COUNT> skybox_texture_readers = {
		TexturePixelReader(paths[0].c_str()),
		TexturePixelReader(paths[1].c_str()),
		TexturePixelReader(paths[2].c_str()),
		TexturePixelReader(paths[3].c_str()),
		TexturePixelReader(paths[4].c_str()),
		TexturePixelReader(paths[5].c_str())
	};

    stbi_set_flip_vertically_on_load(true);

	for (const auto& reader : skybox_texture_readers)
	{
		if (!reader.isTextureValid())
		{
			LOGGER_LOG_ERROR("One or more textures were invalid.Can't construct skybox with invalid textures.");
			return;
		}
	}
	this->m_sceneSkybox.reset(new Skybox(skybox_texture_readers));
}

void OBJ_Viewer::SceneManager::LoadSceneFile(const std::string& path, LoadModelFileType_ model_file_format)
{
	ModelLoader model_data_reader (path.c_str(), model_file_format);
	
	if (!model_data_reader.isFileLoadedSuccessfully())
	{
		LOGGER_LOG_WARN("Failed to load model at path {0}", path);
		return;
	}
	m_sceneModel = model_data_reader.GetLoadedModel();
	m_sceneRegistry = model_data_reader.GetLoadedMaterialRegistry();
}

void OBJ_Viewer::SceneManager::SetUpUniformBuffers()
{
	m_uniformMatrixBuffer.BindBufferRange(0, kUniformBufferMatrixSizeInBytes);

	m_uniformLightBuffer.BindBufferRange(0, kUniformBufferLightSIzeInBytes);

	for (auto& item : m_shaderLib.GetShaderHash())
	{
		const ShaderClass& shader = item.second;
		const ShaderLibarryShaderName_& shaderKey = item.first;
		if (shaderKey == ShaderLibarryShaderName_kPostProcessShader)
			continue;

		shader.BindUniformBufferToShader(m_uniformMatrixBuffer);
	}
	m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kLightShader).BindUniformBufferToShader(m_uniformLightBuffer);
}

void OBJ_Viewer::SceneManager::SetUniformMatrixBuffer()
{
	glm::mat4 ubo_matrix_array [kUniformBufferMatrixCount] = {};
	m_sceneCamera->GetViewAndProjectionSeparate(ubo_matrix_array[0], ubo_matrix_array[1]);
	ubo_matrix_array[2] = m_sceneModel->GetModelMatrix();
	ubo_matrix_array[3] = m_sceneModel->GetNormalMatrix();
	m_uniformMatrixBuffer.SendBufferSubData(0, kUniformBufferMatrixSizeInBytes, ubo_matrix_array);
}

void OBJ_Viewer::SceneManager::OnEvent(Event& e)
{
	if (e.GetEventCategory() == EventCategory_kAppEvent && e.GetEventType() == EventType_kScenelLoad)
		OnSceneLoadEvent(dynamic_cast<EventOnSceneLoad&>(e));
	else if (e.GetEventCategory() == EventCategory_kAppEvent && e.GetEventType() == EventType_kSkyboxLoad)
		OnSkyboxLoadEvent(dynamic_cast<EventOnSkyboxLoaded&>(e));
	else if (e.GetEventCategory() == EventCategory_kAppEvent && e.GetEventType() == EventType_kViewportSizeChanged)
	{
		auto& sceneViewportEvent = dynamic_cast<SceneViewportResizeEvent&>(e);

		m_multiSampleSceneFrameBuffer.ResizeFramebuffer(sceneViewportEvent.GetViewportSize());
		m_intermidiateFramebuffer.ResizeFramebuffer(sceneViewportEvent.GetViewportSize());
	}
}

void OBJ_Viewer::SceneManager::OnSkyboxLoadEvent(EventOnSkyboxLoaded& e)
{
	DialogWrapper dialog;
	dialog.OpenDialogMultiple("png,jpeg,jpg");

	if (dialog.IsDialogClosed())
		return;

	auto& VecPaths = dialog.GetDialogPaths();
	std::vector<std::string> m_stringVector(VecPaths.size());
	for (uint32_t i = 0; i < VecPaths.size(); i++)
	{
		m_stringVector[i] = std::string(VecPaths.at(i));
	}

	LoadSkybox(m_stringVector);
}

void OBJ_Viewer::SceneManager::OnSceneLoadEvent(EventOnSceneLoad& e)
{
	DialogWrapper loadModelDialog;
	std::string_view filterList = "obj";

	if (!e.IsFBXDisabled())
		filterList = "obj,fbx";

	loadModelDialog.OpenDialog(filterList);

	if (loadModelDialog.IsDialogClosed())
		return;
    const std::string& model_path = loadModelDialog.GetFirstDialogResult();
    if (model_path.empty())
        return;

    LoadModelFileType_ model_file_format = FileFormatHelper::GetFileFormatFromPath(model_path);
    if (e.IsFBXDisabled() && model_file_format == LoadModelFileType_::LoadModelFileType_kFBX)
    {
        LOGGER_LOG_WARN("File at path:{0} was detected to be an fbx and was not loaded.Enable fbx loading to be able to load it.", model_path);
        return;
    }

	LoadSceneFile(model_path, model_file_format);
}
