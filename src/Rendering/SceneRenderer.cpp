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
	ShaderClass* shaderToUse {};

	for (const auto& mesh : m_sceneModel->GetModelMeshes())
	{
		switch (renderSettings.m_currentRenderingMode)
		{
		case APP_SETTINGS::RenderingMode::RENDER_MODE_WIREFRAME:
			SetUpForWireframeRendering(mesh, renderSettings.wireframeSettings);
			break;

		case APP_SETTINGS::RenderingMode::RENDER_MODE_SOLID_COLOR:
			shaderToUse = m_shaderLib.GetShaderPtr(ShaderLibarryShaderName_StudioLightShader);
			shaderToUse->UseShader();
			shaderToUse->UniformSet3FloatVector("u_Color", renderSettings.m_colorRenderingColor);
			m_mainRenderer.RenderMesh(*shaderToUse, mesh.GetMeshVAO(), *m_sceneCamera);
			break;

		case APP_SETTINGS::RenderingMode::RENDER_MODE_INDIVIDUAL_TEXTURES:
			shaderToUse = m_shaderLib.GetShaderPtr(ShaderLibarryShaderName_SingleTextureShader);
			shaderToUse->UseShader();
			if (auto material = mesh.GetMaterial().lock())
			{
				if(auto texture = material->GetMaterialTexture(renderSettings.m_curentIndividualTexture).lock())
					m_mainRenderer.BindMaterialTexture(*shaderToUse, texture, GL_TEXTURE1, "textureToInspect");
			}
			m_mainRenderer.RenderMesh(*shaderToUse, mesh.GetMeshVAO(), *m_sceneCamera);
			break;

		case APP_SETTINGS::RenderingMode::RENDER_MODE_UV:
			shaderToUse = m_shaderLib.GetShaderPtr(ShaderLibarryShaderName_UVShader);
			shaderToUse->UseShader();
			shaderToUse->UniformSet1Float("uvScale", renderSettings.m_uvViewSettings.UV_scaleFactor);
			m_mainRenderer.RenderMesh(*shaderToUse, mesh.GetMeshVAO(), *m_sceneCamera);
			break;
		case APP_SETTINGS::RenderingMode::RENDER_MODE_LIGHT:
			SetUpShaderForLightRendering(mesh, renderSettings.m_MaterialFlags, renderSettings.lightInfo);
			break;
		case APP_SETTINGS::RENDER_MODE_NORMAL_ORIENTATION:
			shaderToUse = m_shaderLib.GetShaderPtr(ShaderLibarryShaderName_NormalShader);
			m_mainRenderer.RenderMesh(*shaderToUse, mesh.GetMeshVAO(), *m_sceneCamera);
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
		//glDisable(GL_CULL_FACE);
		auto& shader = m_shaderLib.GetShaderRef(ShaderLibarryShaderName_SkyboxShader);
		m_mainRenderer.RenderSkybox(shader, *m_sceneSkybox, *m_sceneCamera);
		//glEnable(GL_CULL_FACE);
	}

	if (renderSettings.m_isWireGridOn)
	{
		auto& shader = m_shaderLib.GetShaderRef(ShaderLibarryShaderName_GridShader);
		m_mainRenderer.RenderGrid(shader, m_screenQuad, *m_sceneCamera, renderSettings.m_gridData);
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
	auto& shader = m_shaderLib.GetShaderRef(ShaderLibarryShaderName_PostProcessShader);
	shader.UseShader();
	shader.UniformSet1Int("u_framebufferTexture", 1);
	shader.UniformSet2FloatVector("u_resolution", uRes);
	shader.UniformSet1Int("u_useAA", doFXAA);

	m_mainRenderer.RenderMesh(shader, m_screenQuad, *m_sceneCamera);
	framebufferTexture.UnbindTexture();
	glDisable(GL_BLEND);
}

void OBJ_Viewer::SceneRenderer::SetUpShaderForLightRendering(const Mesh& mesh, MaterialFlags materialFlags,
	APP_SETTINGS::SceneLightInfo lightInfo)
{
	auto& shader = m_shaderLib.GetShaderRef(ShaderLibarryShaderName_LightShader);

	shader.UseShader();
	m_uniformLightBuffer.SendBufferSubData(0, lightInfo.lights.size() * APP_SETTINGS::SIZE_OF_LIGHT_IN_BYTES, lightInfo.lights.data());

	shader.UniformSet1Int("isToonShadingOn",
		lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel::LIGHT_MODEL_TOON_SHADING 
		|| lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel::LIGHT_MODEL_RIM_AND_TOON_SHADING);
	shader.UniformSet1Int("isRimLightOn",
		lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel::LIGHT_MODEL_RIM_SHADING ||
		lightInfo.currentLightModel == APP_SETTINGS::LightShadingModel::LIGHT_MODEL_RIM_AND_TOON_SHADING);

	shader.UniformSet3FloatVector("cameraPosition", m_sceneCamera->GetCameraPos());
	m_mainRenderer.RenderMeshMaterialWithLight(shader, mesh.GetMeshVAO(), *mesh.GetMaterial().lock(), materialFlags, *m_sceneCamera);

}

void OBJ_Viewer::SceneRenderer::SetUpForWireframeRendering(const Mesh& mesh,const APP_SETTINGS::WireFrameSettings& wireframeAppSettings)
{
	const SceneViewport& sceneViewPort = m_app.GetSceneViewport_ConstRef();

	const glm::mat3& viewportTransform = sceneViewPort.GetViewportMatrix();

	if (wireframeAppSettings.isPointRenderingOn)
	{

		auto& shader = m_shaderLib.GetShaderRef(ShaderLibarryShaderName_PointShader);

		constexpr static float pointSizeOffset = 1.;
		shader.UseShader();
		shader.UniformSet3FloatVector("u_Color", wireframeAppSettings.lineColor);
		shader.UniformSet3x3FloatMatrix("viewportMatrix", viewportTransform);
		shader.UniformSet1Float("pointSize", wireframeAppSettings.lineThickness + pointSizeOffset);
		m_mainRenderer.RenderMesh(shader, mesh.GetMeshVAO(), *m_sceneCamera);
	}
	else
	{
		auto& shader = m_shaderLib.GetShaderRef(ShaderLibarryShaderName_WireframeShader);

		shader.UseShader();
		shader.UniformSet3FloatVector("u_frameColor", wireframeAppSettings.lineColor);
		shader.UniformSet1Float("frameThickness", wireframeAppSettings.lineThickness);

		shader.UniformSet3x3FloatMatrix("viewportMatrix", viewportTransform);
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
	

	if (!loader.isFileLoadedSuccessfully())
	{
		LOGGER_LOG_WARN("Failed to load model at path {0}", path);
		return;
	}
	m_sceneModel =loader.GetLoadedModel();
	m_sceneRegistry = loader.GetLoadedMaterialRegistry();
}

void OBJ_Viewer::SceneRenderer::SetUpUniformBuffers()
{
	m_uniformMatrixBuffer.BindBufferRange(0, UBO_MATRIX_BYTE_SIZE);

	m_uniformLightBuffer.BindBufferRange(0, UBO_LIGHT_BYTE_SIZE);

	for (auto& item : m_shaderLib.GetShaderHash())
	{
		const ShaderClass& shader = item.second;
		const ShaderLibarryShaderName_& shaderKey = item.first;
		if (shaderKey == ShaderLibarryShaderName_PostProcessShader)
			continue;

		shader.BindUBOToShader(m_uniformMatrixBuffer);
	}
	m_shaderLib.GetShaderRef(ShaderLibarryShaderName_LightShader).BindUBOToShader(m_uniformLightBuffer);
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

	if (dialog.isDialogClosed())
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

	if (loadModelDialog.isDialogClosed())
		return;

	LoadModel(loadModelDialog.GetFirstDialogResult());

	m_renderingMediator->SetSceneModel(m_sceneModel);
	m_renderingMediator->SetSceneMaterialRegistry(m_sceneRegistry);
}
