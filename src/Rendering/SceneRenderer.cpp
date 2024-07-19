#include "SceneRenderer.h"
#include "Renderer.h"
#include "Application.h"
#include "ModelLoader.h"
#include "Scene/Skybox.h"
#include<memory>
#include"ShaderPath.h"
#include"MeshGeneratingMethods.h"

OBJ_Viewer::SceneRenderer::SceneRenderer(Application& app,std::shared_ptr<RenderingMediator> mediator) :
	m_multiSampleSceneFrameBuffer(app.GetSceneFrameBuffer().GetFramebufferSize(),FRAMEBUFFER_COLOR_ATTACHMENT,true,8),
	m_screenQuad({ { -1.0f,1.0f,0.0f },
				{ -1.0f, -1.0f, 0.0 },
				{ 1.0f,  1.0f, 0.0 },
				{ 1.0f, -1.0f, 0.0 } },
				{ 0, 1, 2,2, 1, 3 }),
	m_clearColorShader(GetConcatShaderPath("ColorShader.glsl").c_str()),
	m_gridShader(GetConcatShaderPath("GridShader.glsl").c_str()),
	m_skyboxShader(GetConcatShaderPath("SkyboxShader.glsl").c_str()),
	m_lightShader(GetConcatShaderPath("LightShader.glsl").c_str()),
	m_materialShader(GetConcatShaderPath("MaterialShader.glsl").c_str()),
	m_wireframeShader(GetConcatShaderPath("WireframeShader.glsl").c_str()),
	m_wireframePointShader(GetConcatShaderPath("WireframePointShader.glsl").c_str()),
	m_UVShader(GetConcatShaderPath("UVShader.glsl").c_str()),
	m_singleTextureShader(GetConcatShaderPath("SingleTextureInspectShader.glsl").c_str()),
	m_postProcessingShader(GetConcatShaderPath("SceneSampleShader.glsl").c_str()),
	m_app(app),
	m_uniformLightBuffer("LightInfo", 1, MAX_LIGHT_COUNT * 2 * sizeof(glm::vec4), nullptr),
	m_uniformMatrixBuffer("Matrices", 0, 3 * sizeof(glm::mat4), nullptr)

{

	InitShaders();
	m_renderingMediator = mediator;
	m_sceneCamera = std::make_shared<Camera>(5.0f, app.GetSceneFrameBuffer().GetFramebufferSize(), app);
	m_app.AddEventListener(m_sceneCamera);

	ModelData data;
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(std::move(GenerateCubeVAO()));
	//Thise are not correct
	data.vertexCount = mesh->GetMeshVAO().GetVertexCount();
	data.triangleCount = data.vertexCount/3;
	data.faceCount = data.triangleCount/2;

	m_sceneModel = std::make_shared<Model>(std::vector<std::shared_ptr<Mesh>>{ mesh }, glm::mat4(1), data);

	m_renderingMediator->SetSceneModel(m_sceneModel);
	//TODO:Get this somewhere else
	

}
OBJ_Viewer::SceneRenderer::~SceneRenderer()
{
}
void OBJ_Viewer::SceneRenderer::RenderScene(const RenderStateSettings& renderSettings)
{
	SetUniformMatrixBuffer();
	Framebuffer& appFramebuffer = m_app.GetSceneFrameBuffer();
	if (renderSettings.m_EnableAA)
		m_multiSampleSceneFrameBuffer.BindFramebuffer();
	else
		appFramebuffer.BindFramebuffer();


	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	for (const auto& mesh : m_sceneModel->GetModelMeshes())
	{
		switch (renderSettings.m_currentRenderingMode)
		{
		case RENDER_MODE_WIREFRAME:
			SetUpForWireframeRendering(*mesh, renderSettings.wireframeSettings);
			break;

		case RENDER_MODE_SOLID_COLOR:
			m_clearColorShader.UseShader();
			m_clearColorShader.UniformSet3FloatVector("u_Color", renderSettings.m_colorRenderingColor);
			m_mainRenderer.RenderMesh(m_clearColorShader, mesh->GetMeshVAO(), *m_sceneCamera);
			break;

		case RENDER_MODE_INDIVIDUAL_TEXTURES:
			m_singleTextureShader.UseShader();
			if (auto texture = mesh->GetMaterial().GetMaterialTexture(renderSettings.m_curentIndividualTexture).lock())
				m_mainRenderer.BindMaterialTexture(m_singleTextureShader, texture, GL_TEXTURE1, "textureToInspect");
			m_mainRenderer.RenderMesh(m_singleTextureShader, mesh->GetMeshVAO(), *m_sceneCamera);
			break;

		case RENDER_MODE_UV:
			m_UVShader.UseShader();
			m_UVShader.UniformSet1Float("uvScale", renderSettings.m_uvViewSettings.UV_scaleFactor);
			m_mainRenderer.RenderMesh(m_UVShader, mesh->GetMeshVAO(), *m_sceneCamera);
			break;
		case RENDER_MODE_LIGHT:
			SetUpShaderForLightRendering(*mesh, renderSettings.m_MaterialFlags, renderSettings.lightInfo);
			break;
		default:
			break;
		}
	}
	if (renderSettings.m_EnableAA)
	{
		appFramebuffer.CopyFramebufferContent(m_multiSampleSceneFrameBuffer);
		m_multiSampleSceneFrameBuffer.UnbindFramebuffer();
	}


	if (m_sceneSkybox != nullptr && renderSettings.m_isSkyboxOn)
	{
		glDisable(GL_CULL_FACE);

		m_mainRenderer.RenderSkybox(m_skyboxShader, *m_sceneSkybox, *m_sceneCamera);
		glEnable(GL_CULL_FACE);
	}

	if (renderSettings.m_isWireGridOn)
	{
		RenderGrid(renderSettings.m_gridData);
	}

	appFramebuffer.UnbindFramebuffer();

}

void OBJ_Viewer::SceneRenderer::RenderFramebufferSampledFullScreenQuad()
{
	const Framebuffer& mainFramebuffer = m_app.GetSceneFrameBuffer();

	glActiveTexture(GL_TEXTURE1);
	Texture& framebufferTexture = mainFramebuffer.GetFramebufferTexture();
	static Size2D winSize = m_app.GetGlobalAppWindow().GetWindowSize();
	glm::vec2 uRes = glm::vec2(winSize.width, winSize.height);

	glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		framebufferTexture.BindTexture();
		m_postProcessingShader.UseShader();
		m_postProcessingShader.UniformSet1Int("u_framebufferTexture", 1);
		m_postProcessingShader.UniformSet2FloatVector("u_resolution", uRes);

		m_mainRenderer.RenderMesh(m_postProcessingShader, m_screenQuad, *m_sceneCamera);
		framebufferTexture.UnbindTexture();
	glDisable(GL_BLEND);

}

void OBJ_Viewer::SceneRenderer::RenderGrid(const GridData& appGridData)
{
	m_gridShader.UseShader();
	m_gridShader.UniformSet3FloatVector("cameraPosition", m_sceneCamera->GetCameraPos());
	m_mainRenderer.RenderGrid(m_gridShader, m_screenQuad, *m_sceneCamera, appGridData);
}

void OBJ_Viewer::SceneRenderer::SetUpShaderForLightRendering(const Mesh& mesh, MaterialFlags materialFlags,SceneLightInfo lightInfo)
{
	m_uniformLightBuffer.SendBufferSubData(0, lightInfo.lights.size() * sizeof(DirectionalLight), lightInfo.lights.data());

	m_lightShader.UniformSet1Int("isToonShadingOn", 
		lightInfo.currentLightModel == LIGHT_MODEL_TOON_SHADING || lightInfo.currentLightModel == LIGHT_MODEL_RIM_AND_TOON_SHADING);
	m_lightShader.UniformSet1Int("isRimLightOn",
		lightInfo.currentLightModel == LIGHT_MODEL_RIM_SHADING || lightInfo.currentLightModel == LIGHT_MODEL_RIM_AND_TOON_SHADING);

	m_lightShader.UniformSet3FloatVector("cameraPosition", m_sceneCamera->GetCameraPos());
	m_mainRenderer.RenderMeshMaterialWithLight(m_lightShader, mesh.GetMeshVAO(), mesh.GetMaterial(), materialFlags, *m_sceneCamera);

}

void OBJ_Viewer::SceneRenderer::SetUpForWireframeRendering(const Mesh& mesh,const WireFrameSettings& wireframeAppSettings)
{
	const glm::mat3 viewportTransform = ConstructViewportMatrix();
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
	if (!paths.empty())
	{
		this->m_sceneSkybox.reset(new Skybox(paths));
	}
}

void OBJ_Viewer::SceneRenderer::SwapSkyboxFaces(SkyboxFace toSwap, SkyboxFace with)
{
	this->m_sceneSkybox->SwapSkyboxFaceTextures(toSwap, with);
}
void OBJ_Viewer::SceneRenderer::LoadModel(const std::string& path)
{
	if (path.empty())
		return;

	ModelLoader loader;
	Model* newModel = loader.LoadModel(path.c_str());
	if (newModel == nullptr)
	{
		std::cout << "Failed to load the model." << '\n';
		return;
	}
	m_sceneModel.reset(newModel);
}

void OBJ_Viewer::SceneRenderer::InitShaders()
{
	

	m_uniformMatrixBuffer.BindBufferRange(0, 3 * sizeof(glm::mat4));

	m_uniformLightBuffer.BindBufferRange(0, MAX_LIGHT_COUNT * 2 * sizeof(glm::vec4));

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
	glm::mat4 matrices[3] = {};
	m_sceneCamera->GetViewAndProjectionSeparate(&matrices[0], &matrices[1]);
	matrices[2] = m_sceneModel->GetModelMatrix();
	m_uniformMatrixBuffer.SendBufferSubData(0, 3 * sizeof(glm::mat4), matrices);
}

glm::mat3 OBJ_Viewer::SceneRenderer::ConstructViewportMatrix() const
{
	glm::mat3 result(1);
	auto winSize = m_app.GetSceneViewport();
	result[0][0] = (float)winSize.width / 2.f;
	result[1][1] = (float)winSize.height / 2.f;

	result[2][0] = result[0][0];
	result[2][1] = result[1][1];

	return result;
}

void OBJ_Viewer::SceneRenderer::OnEvent(Event& e)
{
	if (e.GetEventCategory() & APP_EVENT && e.GetEventType() == EVENT_ON_MODEL_LOAD)
		OnModelLoadEvent(dynamic_cast<EventOnModelLoaded&>(e));
	else if (e.GetEventCategory() & APP_EVENT && e.GetEventType() == EVENT_ON_SKYBOX_LOAD)
		OnSkyboxLoadEvent(dynamic_cast<EventOnSkyboxLoaded&>(e));
	else if (e.GetEventCategory() & APP_EVENT && e.GetEventType() == EVENT_FRAMEBUFFER_SIZE_CHANGED)
		m_multiSampleSceneFrameBuffer.ResizeFramebuffer(m_app.GetSceneFrameBuffer().GetFramebufferSize());
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
}
