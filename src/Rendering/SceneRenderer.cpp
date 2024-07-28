#include "pch.h"
#include "SceneRenderer.h"
#include "Renderer.h"
#include "Core/Application.h"
#include "Helpers/ModelLoader.h"
#include "Scene/Skybox.h"
#include "ShaderPath.h"
#include "Helpers/MeshGeneratingMethods.h"
#include "Profiling/AppProfiler.h"

#pragma region Constants
constexpr uint8_t MATRIX_UBO_BINDING_POINT = 0;
constexpr uint8_t LIGHT_UBO_BINDING_POINT = 1;

constexpr uint8_t UBO_MATRIX_COUNT = 4;
constexpr size_t UBO_MATRIX_BYTE_SIZE = UBO_MATRIX_COUNT * sizeof(glm::mat4);
constexpr size_t UBO_LIGHT_BYTE_SIZE = OBJ_Viewer::APP_SETTINGS::MAX_LIGHT_COUNT * OBJ_Viewer::APP_SETTINGS::SIZE_OF_LIGHT_IN_BYTES;
#pragma endregion
OBJ_Viewer::SceneRenderer::SceneRenderer(Application& app,std::shared_ptr<RenderingMediator> mediator) :
#pragma region Buffer setup
	m_multiSampleSceneFrameBuffer(app.GetSceneViewport().GetViewportSize(), FRAMEBUFFER_COLOR_ATTACHMENT, true, 11),
	m_intermidiateFramebuffer(app.GetSceneViewport().GetViewportSize(), FRAMEBUFFER_COLOR_ATTACHMENT),
	m_uniformLightBuffer("LightInfo",LIGHT_UBO_BINDING_POINT , UBO_LIGHT_BYTE_SIZE, nullptr),
	m_uniformMatrixBuffer("Matrices",MATRIX_UBO_BINDING_POINT,UBO_MATRIX_BYTE_SIZE, nullptr),
#pragma endregion
	//If this is causing problems see the comment where the definition of the 'GeneratePlaneVAOStack' is.
	m_screenQuad(GeneratePlaneVAOStack()),
#pragma region  Shader setup
	m_clearColorShader(ShaderAssetHelper::GetConcatShaderPath("ClearColorMeshShader.glsl").c_str()),
	m_gridShader(ShaderAssetHelper::GetConcatShaderPath("GridShader.glsl").c_str()),
	m_skyboxShader(ShaderAssetHelper::GetConcatShaderPath("SkyboxShader.glsl").c_str()),
	m_lightShader(ShaderAssetHelper::GetConcatShaderPath("LightShader.glsl").c_str()),
	m_materialShader(ShaderAssetHelper::GetConcatShaderPath("MaterialShader.glsl").c_str()),
	m_wireframeShader(ShaderAssetHelper::GetConcatShaderPath("WireframeShader.glsl").c_str()),
	m_wireframePointShader(ShaderAssetHelper::GetConcatShaderPath("WireframePointShader.glsl").c_str()),
	m_UVShader(ShaderAssetHelper::GetConcatShaderPath("UVShader.glsl").c_str()),
	m_singleTextureShader(ShaderAssetHelper::GetConcatShaderPath("SingleTextureInspectShader.glsl").c_str()),
	m_postProcessingShader(ShaderAssetHelper::GetConcatShaderPath("PostProcessShader.glsl").c_str()),
#pragma endregion
	m_app(app)
{

	SetUpUniformBuffers();
	m_renderingMediator = mediator;
	m_sceneCamera = std::make_shared<Camera>(5.0f, app.GetSceneViewport().GetViewportSize(), app);
	m_app.AddEventListener(m_sceneCamera);

	ModelData data;
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(std::move(GenerateCubeVAO()));
	//These are not correct
	data.vertexCount = mesh->GetMeshVAO().GetVertexCount();
	data.triangleCount = data.vertexCount/3;
	data.faceCount = data.triangleCount/2;

	m_sceneModel = std::make_shared<Model>(std::vector<std::shared_ptr<Mesh>>{ mesh }, glm::mat4(1), data);

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

	for (const auto& mesh : m_sceneModel->GetModelMeshes())
	{
		switch (renderSettings.m_currentRenderingMode)
		{
		case APP_SETTINGS::RenderingMode::RENDER_MODE_WIREFRAME:
			SetUpForWireframeRendering(*mesh, renderSettings.wireframeSettings);
			break;

		case APP_SETTINGS::RenderingMode::RENDER_MODE_SOLID_COLOR:
			m_clearColorShader.UseShader();
			m_clearColorShader.UniformSet3FloatVector("u_Color", renderSettings.m_colorRenderingColor);
			m_mainRenderer.RenderMesh(m_clearColorShader, mesh->GetMeshVAO(), *m_sceneCamera);
			break;

		case APP_SETTINGS::RenderingMode::RENDER_MODE_INDIVIDUAL_TEXTURES:
			m_singleTextureShader.UseShader();
			if (auto material = mesh->GetMaterial().lock())
			{
				if(auto texture = material->GetMaterialTexture(renderSettings.m_curentIndividualTexture).lock())
					m_mainRenderer.BindMaterialTexture(m_singleTextureShader, texture, GL_TEXTURE1, "textureToInspect");
			}
			m_mainRenderer.RenderMesh(m_singleTextureShader, mesh->GetMeshVAO(), *m_sceneCamera);
			break;

		case APP_SETTINGS::RenderingMode::RENDER_MODE_UV:
			m_UVShader.UseShader();
			m_UVShader.UniformSet1Float("uvScale", renderSettings.m_uvViewSettings.UV_scaleFactor);
			m_mainRenderer.RenderMesh(m_UVShader, mesh->GetMeshVAO(), *m_sceneCamera);
			break;
		case APP_SETTINGS::RenderingMode::RENDER_MODE_LIGHT:
			SetUpShaderForLightRendering(*mesh, renderSettings.m_MaterialFlags, renderSettings.lightInfo);
			break;
		default:
			break;
		}
	}

	if (renderSettings.m_EnableAA)
	{
		m_intermidiateFramebuffer.CopyFramebufferContent(m_multiSampleSceneFrameBuffer);
		m_multiSampleSceneFrameBuffer.UnbindFramebuffer();

		m_intermidiateFramebuffer.BindFramebuffer();
	}

	if (renderSettings.m_isSkyboxOn && m_sceneSkybox != nullptr)
	{
		glDisable(GL_CULL_FACE);

		m_mainRenderer.RenderSkybox(m_skyboxShader, *m_sceneSkybox, *m_sceneCamera);
		glEnable(GL_CULL_FACE);
	}

	if (renderSettings.m_isWireGridOn)
	{
		m_mainRenderer.RenderGrid(m_gridShader, m_screenQuad, *m_sceneCamera, renderSettings.m_gridData);
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
	Texture& framebufferTexture = m_intermidiateFramebuffer.GetFramebufferTexture();

	const SceneViewport& sceneViewPort = m_app.GetSceneViewport_ConstRef();

	const Size2D viewportSize = sceneViewPort.GetViewportSize();
	glm::vec2 uRes = glm::vec2(viewportSize.width, viewportSize.height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	framebufferTexture.BindTexture();
	m_postProcessingShader.UseShader();
	m_postProcessingShader.UniformSet1Int("u_framebufferTexture", 1);
	m_postProcessingShader.UniformSet2FloatVector("u_resolution", uRes);
	m_postProcessingShader.UniformSet1Int("u_useAA", doFXAA);

	m_mainRenderer.RenderMesh(m_postProcessingShader, m_screenQuad, *m_sceneCamera);
	framebufferTexture.UnbindTexture();
	glDisable(GL_BLEND);
}



void OBJ_Viewer::SceneRenderer::SetUpShaderForLightRendering(const Mesh& mesh, MaterialFlags materialFlags,
	APP_SETTINGS::SceneLightInfo lightInfo)
{
	m_uniformLightBuffer.SendBufferSubData(0, lightInfo.lights.size() * APP_SETTINGS::SIZE_OF_LIGHT_IN_BYTES, lightInfo.lights.data());

	m_lightShader.UniformSet1Int("isToonShadingOn", 
		lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel::LIGHT_MODEL_TOON_SHADING 
		|| lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel::LIGHT_MODEL_RIM_AND_TOON_SHADING);
	m_lightShader.UniformSet1Int("isRimLightOn",
		lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel::LIGHT_MODEL_RIM_SHADING ||
		lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel::LIGHT_MODEL_RIM_AND_TOON_SHADING);

	m_lightShader.UniformSet3FloatVector("cameraPosition", m_sceneCamera->GetCameraPos());
	m_mainRenderer.RenderMeshMaterialWithLight(m_lightShader, mesh.GetMeshVAO(), *mesh.GetMaterial().lock(), materialFlags, *m_sceneCamera);

}

void OBJ_Viewer::SceneRenderer::SetUpForWireframeRendering(const Mesh& mesh,const APP_SETTINGS::WireFrameSettings& wireframeAppSettings)
{
	const SceneViewport& sceneViewPort = m_app.GetSceneViewport_ConstRef();

	const glm::mat3& viewportTransform = sceneViewPort.GetViewportMatrix();

	if (wireframeAppSettings.isPointRenderingOn)
	{
		constexpr static float pointSizeOffset = 1.;
		m_wireframePointShader.UseShader();
		m_wireframePointShader.UniformSet3FloatVector("u_Color", wireframeAppSettings.lineColor);
		m_wireframePointShader.UniformSet3x3FloatMatrix("viewportMatrix", viewportTransform);
		m_wireframePointShader.UniformSet1Float("pointSize", wireframeAppSettings.lineThickness + pointSizeOffset);
		m_mainRenderer.RenderMesh(m_wireframePointShader, mesh.GetMeshVAO(), *m_sceneCamera);
	}
	else
	{
		m_wireframeShader.UseShader();
		m_wireframeShader.UniformSet3FloatVector("u_frameColor", wireframeAppSettings.lineColor);
		m_wireframeShader.UniformSet1Float("frameThickness", wireframeAppSettings.lineThickness);

		m_wireframeShader.UniformSet3x3FloatMatrix("viewportMatrix", viewportTransform);
		m_mainRenderer.RenderMesh(m_wireframeShader, mesh.GetMeshVAO(), *m_sceneCamera);
	}
}

void OBJ_Viewer::SceneRenderer::LoadSkybox(std::vector<std::string>& paths)
{
	//TODO: This can be async

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

	std::array<TexturePixelReader,Skybox::SKYBOX_FACE_COUNT> readers = {
		TexturePixelReader(paths[0].c_str()),
		TexturePixelReader(paths[1].c_str()),
		TexturePixelReader(paths[2].c_str()),
		TexturePixelReader(paths[3].c_str()),
		TexturePixelReader(paths[4].c_str()),
		TexturePixelReader(paths[5].c_str())
	};
	for (const auto& reader : readers)
	{
		if (!reader.isTextureValid())
		{
			LOGGER_LOG_ERROR("One or more textures were invalid.Can't construct skybox with invalid textures.");
			return;
		}
	}

	this->m_sceneSkybox.reset(new Skybox(readers));
}

void OBJ_Viewer::SceneRenderer::SwapSkyboxFaces(SkyboxFace toSwap, SkyboxFace with)
{
	this->m_sceneSkybox->SwapSkyboxFaceTextures(toSwap, with);
}
void OBJ_Viewer::SceneRenderer::LoadModel(const std::string& path)
{
	if (path.empty())
		return;
	LoadModelFileType modelFileFormat = FileFormatHelper::GetFileFormatFromPath(path);
	if (m_app.GetScene_RefSettings().m_disableFBXLoading && modelFileFormat == LoadModelFileType::MODEL_TYPE_FBX)
	{
		LOGGER_LOG_WARN("File at path:{0} was detected to be an fbx and was not loaded.Enable fbx loading to be able to load it.", path);
		return;
	}

	ModelLoader loader(path.c_str(), modelFileFormat);
	

	if (!loader.isFiledLoadedSuccessfully())
	{
		LOGGER_LOG_WARN("Failed to load model at path {0}", path);
		return;
	}
	m_sceneModel = std::move(loader.GetLoadedModel());
	m_sceneRegistry = std::move(loader.GetLoadedMaterialRegistry());
}

void OBJ_Viewer::SceneRenderer::SetUpUniformBuffers()
{
	m_uniformMatrixBuffer.BindBufferRange(0, UBO_MATRIX_BYTE_SIZE);

	m_uniformLightBuffer.BindBufferRange(0, UBO_LIGHT_BYTE_SIZE);

	m_clearColorShader.BindUBOToShader(m_uniformMatrixBuffer);
	m_skyboxShader.BindUBOToShader(m_uniformMatrixBuffer);
	m_gridShader.BindUBOToShader(m_uniformMatrixBuffer);
	m_lightShader.BindUBOToShader(m_uniformMatrixBuffer);
	m_materialShader.BindUBOToShader(m_uniformMatrixBuffer);
	m_wireframeShader.BindUBOToShader(m_uniformMatrixBuffer);
	m_wireframePointShader.BindUBOToShader(m_uniformMatrixBuffer);
	m_UVShader.BindUBOToShader(m_uniformMatrixBuffer);

	m_lightShader.BindUBOToShader(m_uniformLightBuffer);
}

void OBJ_Viewer::SceneRenderer::SetUniformMatrixBuffer()const
{
	glm::mat4 matrices[UBO_MATRIX_COUNT] = {};
	m_sceneCamera->GetViewAndProjectionSeparate(matrices[0], matrices[1]);
	matrices[2] = m_sceneModel->GetModelMatrix();
	matrices[3] = m_sceneModel->GetNormalMatrix();
	m_uniformMatrixBuffer.SendBufferSubData(0, UBO_MATRIX_BYTE_SIZE, matrices);
}

void OBJ_Viewer::SceneRenderer::OnEvent(Event& e)
{
	if (e.GetEventCategory() & APP_EVENT && e.GetEventType() == EVENT_ON_MODEL_LOAD)
		OnModelLoadEvent(dynamic_cast<EventOnModelLoaded&>(e));
	else if (e.GetEventCategory() & APP_EVENT && e.GetEventType() == EVENT_ON_SKYBOX_LOAD)
		OnSkyboxLoadEvent(dynamic_cast<EventOnSkyboxLoaded&>(e));
	else if (e.GetEventCategory() & APP_EVENT && e.GetEventType() == EVENT_SCENE_VIEWPORT_SIZE_CHANGED)
	{
		m_multiSampleSceneFrameBuffer.ResizeFramebuffer(m_app.GetSceneViewport().GetViewportSize());
		m_intermidiateFramebuffer.ResizeFramebuffer(m_app.GetSceneViewport().GetViewportSize());
	}
}

void OBJ_Viewer::SceneRenderer::OnSkyboxLoadEvent(EventOnSkyboxLoaded& e)
{
	LoadSkybox(e.GetSkyboxPaths());
	m_renderingMediator->SetSkybox(m_sceneSkybox);

}

void OBJ_Viewer::SceneRenderer::OnModelLoadEvent(EventOnModelLoaded& e)
{
	LoadModel(e.GetModelPath());
	m_renderingMediator->SetSceneModel(m_sceneModel);
	m_renderingMediator->SetSceneMaterialRegistry(m_sceneRegistry);
}
