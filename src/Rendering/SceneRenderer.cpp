#include "pch.h"
#include "SceneRenderer.h"
#include "Renderer.h"
#include "Core/Application.h"
#include "Helpers/ModelLoader.h"
#include "Scene/Skybox.h"
#include "ShaderPath.h"
#include "Helpers/MeshGeneratingMethods.h"
#include "Profiling/AppProfiler.h"
#include "Helpers/DialogWrapper.h"

#pragma region Constants
constexpr uint8_t kMatrixUniformBufferBindingPoint = 0;
constexpr uint8_t kLightUniformBufferBindingPoint = 1;

constexpr uint8_t kUniformBufferMatrixCount = 4;
constexpr size_t kUniformBufferMatrixSizeInBytes = kUniformBufferMatrixCount * sizeof(glm::mat4);
constexpr size_t kUniformBufferLightSIzeInBytes = OBJ_Viewer::APP_SETTINGS::MAX_LIGHT_COUNT * OBJ_Viewer::APP_SETTINGS::SIZE_OF_LIGHT_IN_BYTES;
#pragma endregion
OBJ_Viewer::SceneRenderer::SceneRenderer(Application& app,std::shared_ptr<RenderingMediator> mediator) :
#pragma region Buffer setup
	m_multiSampleSceneFrameBuffer(app.GetSceneViewport().GetViewportSize(), FramebufferAttachmentsFlags_kColorAttachment, true, 11),
	m_intermidiateFramebuffer(app.GetSceneViewport().GetViewportSize(), FramebufferAttachmentsFlags_kColorAttachment),
	m_uniformLightBuffer("LightInfo",kLightUniformBufferBindingPoint , kUniformBufferLightSIzeInBytes, nullptr),
	m_uniformMatrixBuffer("Matrices",kMatrixUniformBufferBindingPoint,kUniformBufferMatrixSizeInBytes, nullptr),
#pragma endregion
	//If this is causing problems see the comment where the definition of the 'GeneratePlaneVAOStack' is.
	m_screenQuad(GeneratePlaneVAOStack()),
	m_app(app)
{

	SetUpUniformBuffers();
	m_renderingMediator = mediator;
	m_sceneCamera = std::make_shared<Camera>(5.0f, app.GetSceneViewport().GetViewportSize(), app);
	m_app.AddEventListener(m_sceneCamera);

	ModelData data;
	std::unique_ptr<std::vector<Mesh>> meshVector = std::make_unique<std::vector<Mesh>>();
	meshVector->emplace_back((std::move(GenerateCubeVAO())));

	//These are not correct
	data.meshInfo.vertexCount = meshVector->at(0).GetMeshVAO().GetVertexCount();
	data.meshInfo.triangleCount = data.meshInfo.vertexCount/3;
	data.meshInfo.faceCount = data.meshInfo.triangleCount/2;

	m_sceneModel = std::make_shared<Model>(meshVector, glm::mat4(1), data);

	m_renderingMediator->SetSceneModel(m_sceneModel);
	m_renderingMediator->SetSceneMaterialRegistry(m_sceneRegistry);

	//TODO:Get this somewhere else
	

}
OBJ_Viewer::SceneRenderer::~SceneRenderer()
{
}
void OBJ_Viewer::SceneRenderer::RenderScene(const APP_SETTINGS::RenderStateSettings& renderSettings, Framebuffer* outputFrameBuffer)
{
	SetUniformMatrixBuffer();
   
	if (renderSettings.m_EnableAA)
		m_multiSampleSceneFrameBuffer.BindFramebuffer();
	else
		m_intermidiateFramebuffer.BindFramebuffer();


	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	ShaderClass* selected_shader_to_use {};

	for (const auto& mesh : m_sceneModel->GetModelMeshes())
	{
		switch (renderSettings.m_currentRenderingMode)
		{
		case APP_SETTINGS::RenderingMode_::RenderingMode_kWireframe:
			SetUpForWireframeRendering(mesh, renderSettings.wireframeSettings);
			break;

		case APP_SETTINGS::RenderingMode_::RenderingMode_kSolidColor:
			selected_shader_to_use = m_shaderLib.GetShaderPtr(ShaderLibarryShaderName_kStudioLightShader);
			selected_shader_to_use->UseShader();
			selected_shader_to_use->SetUniformSet3FloatVector("u_Color", renderSettings.m_colorRenderingColor);
			m_mainRenderer.RenderMesh(*selected_shader_to_use, mesh.GetMeshVAO(), *m_sceneCamera);
			break;

		case APP_SETTINGS::RenderingMode_::RenderingMode_kIndividualTexture:
			selected_shader_to_use = m_shaderLib.GetShaderPtr(ShaderLibarryShaderName_kSingleTextureShader);
			selected_shader_to_use->UseShader();
			if (auto material = mesh.GetMaterial().lock())
			{
				if(auto texture = material->GetMaterialTexture(renderSettings.m_curentIndividualTexture).lock())
					m_mainRenderer.BindMaterialTexture(*selected_shader_to_use, texture, GL_TEXTURE1, "textureToInspect");
			}
			m_mainRenderer.RenderMesh(*selected_shader_to_use, mesh.GetMeshVAO(), *m_sceneCamera);
			break;

		case APP_SETTINGS::RenderingMode_::RenderingMode_kUV:
			selected_shader_to_use = m_shaderLib.GetShaderPtr(ShaderLibarryShaderName_kUVShader);
			selected_shader_to_use->UseShader();
			selected_shader_to_use->SetUniformSet1Float("uvScale", renderSettings.m_uvViewSettings.UV_scaleFactor);
			m_mainRenderer.RenderMesh(*selected_shader_to_use, mesh.GetMeshVAO(), *m_sceneCamera);
			break;
		case APP_SETTINGS::RenderingMode_::RenderingMode_kLight:
			SetUpShaderForLightRendering(mesh, renderSettings.m_MaterialFlags, renderSettings.lightInfo);
			break;
		case APP_SETTINGS::RenderingMode_kNormalOrientation:
			selected_shader_to_use = m_shaderLib.GetShaderPtr(ShaderLibarryShaderName_kNormalShader);
			m_mainRenderer.RenderMesh(*selected_shader_to_use, mesh.GetMeshVAO(), *m_sceneCamera);
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
		auto& grid_shader = m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kGridShader);
		m_mainRenderer.RenderGrid(grid_shader, m_screenQuad, *m_sceneCamera, renderSettings.m_gridData);
	}

    if (renderSettings.m_isSkyboxOn && m_sceneSkybox != nullptr)
    {
        //glDisable(GL_CULL_FACE);
        glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
        glEnable(GL_BLEND);
        auto& skybox_shader = m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kSkyboxShader);
        m_mainRenderer.RenderSkybox(skybox_shader, *m_sceneSkybox, *m_sceneCamera);
        glDisable(GL_BLEND);

        //glEnable(GL_CULL_FACE);
    }

	m_intermidiateFramebuffer.UnbindFramebuffer();


	if (outputFrameBuffer == nullptr)
	{
		Framebuffer::BindDefaultFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		PostProcessScene(renderSettings.m_EnableAA);
	}
	else
	{
		outputFrameBuffer->BindFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		PostProcessScene(renderSettings.m_EnableAA);
		outputFrameBuffer->UnbindFramebuffer();
	}
}

void OBJ_Viewer::SceneRenderer::PostProcessScene(bool doFXAA)
{
	glActiveTexture(GL_TEXTURE1);
	Texture& framebuffer_texture_for_post_process = m_intermidiateFramebuffer.GetFramebufferTexture();

	const SceneViewport& kCurrent_scene_viewport = m_app.GetSceneViewport_ConstRef();

	const Size2D kViewport_size = kCurrent_scene_viewport.GetViewportSize();

	glm::vec2 shader_uniform_window_resolution = glm::vec2(kViewport_size.width, kViewport_size.height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	framebuffer_texture_for_post_process.BindTexture();
	auto& shader = m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kPostProcessShader);
	shader.UseShader();
	shader.SetUniformSet1Int("u_framebufferTexture", 1);
	shader.SetUniformSet2FloatVector("u_resolution", shader_uniform_window_resolution);
	shader.SetUniformSet1Int("u_useAA", doFXAA);

	m_mainRenderer.RenderMesh(shader, m_screenQuad, *m_sceneCamera);
	framebuffer_texture_for_post_process.UnbindTexture();
	glDisable(GL_BLEND);
}

void OBJ_Viewer::SceneRenderer::SetUpShaderForLightRendering(const Mesh& mesh, MaterialFlags materialFlags,
	APP_SETTINGS::SceneLightInfo lightInfo)
{
	auto& shader = m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kLightShader);

	shader.UseShader();
	m_uniformLightBuffer.SendBufferSubData(0, lightInfo.lights.size() * APP_SETTINGS::SIZE_OF_LIGHT_IN_BYTES, lightInfo.lights.data());

	shader.SetUniformSet1Int("isToonShadingOn",
		lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel_::LightShadingModel_kToonShading 
		|| lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel_::LightShadingModel_kRim_and_ToonShading);
	shader.SetUniformSet1Int("isRimLightOn",
		lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel_::LightShadingModel_kRimShading ||
		lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel_::LightShadingModel_kRim_and_ToonShading);

	shader.SetUniformSet3FloatVector("cameraPosition", m_sceneCamera->GetCameraPos());
	m_mainRenderer.RenderMeshMaterialWithLight(shader, mesh.GetMeshVAO(), *mesh.GetMaterial().lock(), materialFlags, *m_sceneCamera);

}

void OBJ_Viewer::SceneRenderer::SetUpForWireframeRendering(const Mesh& mesh,const APP_SETTINGS::WireFrameSettings& wireframeAppSettings)
{
	const glm::mat3& shader_uniform_viewport_matrix = m_app.GetSceneViewport_ConstRef().GetViewportMatrix();

	if (wireframeAppSettings.isPointRenderingOn)
	{

		auto& shader = m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kPointShader);

		constexpr static float pointSizeOffset = 1.;
		shader.UseShader();
		shader.SetUniformSet3FloatVector("u_Color", wireframeAppSettings.lineColor);
		shader.SetUniformSet3x3FloatMatrix("viewportMatrix", shader_uniform_viewport_matrix);
		shader.SetUniformSet1Float("pointSize", wireframeAppSettings.lineThickness + pointSizeOffset);
		m_mainRenderer.RenderMesh(shader, mesh.GetMeshVAO(), *m_sceneCamera);
	}
	else
	{
		auto& shader = m_shaderLib.GetShaderRef(ShaderLibarryShaderName_kWireframeShader);

		shader.UseShader();
		shader.SetUniformSet3FloatVector("u_frameColor", wireframeAppSettings.lineColor);
		shader.SetUniformSet1Float("frameThickness", wireframeAppSettings.lineThickness);

		shader.SetUniformSet3x3FloatMatrix("viewportMatrix", shader_uniform_viewport_matrix);
		m_mainRenderer.RenderMesh(shader, mesh.GetMeshVAO(), *m_sceneCamera);
	}
}

void OBJ_Viewer::SceneRenderer::LoadSkybox(std::vector<std::string>& paths)
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

void OBJ_Viewer::SceneRenderer::SwapSkyboxFaces(SkyboxFace_ toSwap, SkyboxFace_ with)
{
	this->m_sceneSkybox->SwapSkyboxFaceTextures(toSwap, with);
}
void OBJ_Viewer::SceneRenderer::LoadModel(const std::string& path)
{
	if (path.empty())
		return;

	LoadModelFileType_ model_file_format = FileFormatHelper::GetFileFormatFromPath(path);
	if (m_app.GetScene_RefSettings().m_disableFBXLoading && model_file_format == LoadModelFileType_::LoadModelFileType_kFBX)
	{
		LOGGER_LOG_WARN("File at path:{0} was detected to be an fbx and was not loaded.Enable fbx loading to be able to load it.", path);
		return;
	}

	ModelLoader model_data_reader (path.c_str(), model_file_format);
	

	if (!model_data_reader.isFileLoadedSuccessfully())
	{
		LOGGER_LOG_WARN("Failed to load model at path {0}", path);
		return;
	}
	m_sceneModel = model_data_reader.GetLoadedModel();
	m_sceneRegistry = model_data_reader.GetLoadedMaterialRegistry();
}

void OBJ_Viewer::SceneRenderer::SetUpUniformBuffers()
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

void OBJ_Viewer::SceneRenderer::SetUniformMatrixBuffer()
{
	glm::mat4 ubo_matrix_array [kUniformBufferMatrixCount] = {};
	m_sceneCamera->GetViewAndProjectionSeparate(ubo_matrix_array[0], ubo_matrix_array[1]);
	ubo_matrix_array[2] = m_sceneModel->GetModelMatrix();
	ubo_matrix_array[3] = m_sceneModel->GetNormalMatrix();
	m_uniformMatrixBuffer.SendBufferSubData(0, kUniformBufferMatrixSizeInBytes, ubo_matrix_array);
}

void OBJ_Viewer::SceneRenderer::OnEvent(Event& e)
{
	if (e.GetEventCategory() & EventCategory_kAppEvent && e.GetEventType() == EventType_kModelLoad)
		OnModelLoadEvent(dynamic_cast<EventOnModelLoaded&>(e));
	else if (e.GetEventCategory() & EventCategory_kAppEvent && e.GetEventType() == EventType_kSkyboxLoad)
		OnSkyboxLoadEvent(dynamic_cast<EventOnSkyboxLoaded&>(e));
	else if (e.GetEventCategory() & EventCategory_kAppEvent && e.GetEventType() == EventType_kViewportSizeChanged)
	{
		auto& sceneViewportEvent = dynamic_cast<SceneViewportResizeEvent&>(e);
		const Viewport& newViewport = sceneViewportEvent.GetViewport();
		glViewport(newViewport.x, newViewport.y, newViewport.width, newViewport.height);

		m_multiSampleSceneFrameBuffer.ResizeFramebuffer(sceneViewportEvent.GetViewportSize());
		m_intermidiateFramebuffer.ResizeFramebuffer(sceneViewportEvent.GetViewportSize());
	}
}

void OBJ_Viewer::SceneRenderer::OnSkyboxLoadEvent(EventOnSkyboxLoaded& e)
{
	DialogWrapper dialog;
	dialog.OpenDialogMultiple("png,jpeg,jpg");

	if (dialog.IsDialogClosed())
		return;

	auto& VecPaths = dialog.GetDialogResult();
	std::vector<std::string> m_stringVector(VecPaths.size());
	for (uint32_t i = 0; i < VecPaths.size(); i++)
	{
		m_stringVector[i] = std::string(VecPaths.at(i));
	}

	LoadSkybox(m_stringVector);
	m_renderingMediator->SetSkybox(m_sceneSkybox);
}

void OBJ_Viewer::SceneRenderer::OnModelLoadEvent(EventOnModelLoaded& e)
{
	DialogWrapper loadModelDialog;
	std::string_view filterList = "obj";

	if (!m_app.isFBXLoadingDisabled())
		filterList = "obj,fbx";

	loadModelDialog.OpenDialog(filterList);

	if (loadModelDialog.IsDialogClosed())
		return;

	LoadModel(loadModelDialog.GetFirstDialogResult());

	m_renderingMediator->SetSceneModel(m_sceneModel);
	m_renderingMediator->SetSceneMaterialRegistry(m_sceneRegistry);
}
