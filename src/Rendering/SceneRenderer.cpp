#include "SceneRenderer.h"
#include "Renderer.h"
#include "Application.h"
#include "ModelLoader.h"
#include "Scene/Skybox.h"
#include<memory>
#include"ShaderPath.h"
#include"MeshGeneratingMethods.h"
OBJ_Viewer::SceneRenderer::SceneRenderer(Application& app,std::shared_ptr<RenderingMediator> mediator) : m_app(app)
{
	InitShaders();
	
	auto windowSize = m_app.GetGlobalAppWindow().GetWindowSize();
	m_sceneCamera = std::make_unique<Camera>(5.0f, windowSize,
		app);
	m_app.AddEventListener(m_sceneCamera);

	m_renderingMediator = mediator;

	std::vector <glm::vec3> planeVerts = {
	{ -1.0f,1.0f,0.0f},
	{-1.0f, -1.0f, 0.0},
	{1.0f,  1.0f, 0.0},
	{1.0f, -1.0f, 0.0}
	};
	std::vector<GLuint> planeIndexData =
	{
		 0, 1, 2,
		 2, 1, 3
	};

	ModelData data;
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(std::move(GenerateCubeVAO()));
	//Thise are not correct
	data.vertexCount = mesh->GetMeshVAO().GetVertexCount();
	data.triangleCount = data.vertexCount/3;
	data.faceCount = data.triangleCount/2;

	m_sceneModel = std::make_shared<Model>(std::vector<std::shared_ptr<Mesh>>{ mesh }, glm::mat4(1), data);

	m_gridVAO = std::make_unique<VertexAttributeObject>(planeVerts, planeIndexData);
	m_renderingMediator->SetSceneModel(m_sceneModel);
	//TODO:Get this somewhere else
	

}
OBJ_Viewer::SceneRenderer::~SceneRenderer()
{
}
void OBJ_Viewer::SceneRenderer::RenderScene(const RenderStateSettings& renderSettings)
{
	SetUniformMatrixBuffer();

	for (const auto& mesh : m_sceneModel->GetModelMeshes())
	{

		if (renderSettings.m_isWireFrameRenderingOn)
		{
			SetUpForWireframeRendering(*mesh,renderSettings.wireframeSettings);
		}
		else
		{
			if (!renderSettings.m_isRenderingLightOn)
			{
				mesh->GetMaterial().
					BindMaterialWithShader(*m_materialShader,
						renderSettings.m_isWireFrameRenderingOn || !renderSettings.currentlySetTextures.isRenderAlbedoTextureOn
						? static_cast<MaterialFlags>(0) : IS_ALBEDO_ON);
				Renderer::RenderMesh(*m_materialShader, *mesh, *m_sceneCamera);

			}
			else
			{
				SetUpShaderForLightRendering(*mesh,renderSettings.currentlySetTextures, renderSettings.lightInfo);
			}
		}
	}

	if (m_sceneSkybox != nullptr && renderSettings.m_isSkyboxOn)
	{
		glDisable(GL_CULL_FACE);

		Renderer::RenderSkybox(*m_skyboxShader, *m_sceneSkybox, *m_sceneCamera);
		glEnable(GL_CULL_FACE);
	}

	if (renderSettings.m_isWireGridOn)
	{
		RenderGrid(renderSettings.m_gridData);
	}

}

void OBJ_Viewer::SceneRenderer::RenderGrid(const GridData& appGridData)
{
	m_gridShader->UseShader();
	m_gridShader->UniformSet3FloatVector("cameraPosition", m_sceneCamera->GetCameraPos());
	Renderer::RenderGrid(*m_gridShader, *m_gridVAO, *m_sceneCamera, appGridData);
}

void OBJ_Viewer::SceneRenderer::SetUpShaderForLightRendering(const Mesh& mesh,TextureComposition textureFlags,SceneLightInfo lightInfo)
{
	MaterialFlags flags = static_cast<MaterialFlags>(
		textureFlags.isRenderAlbedoTextureOn * IS_ALBEDO_ON |
		textureFlags.isRenderNormalTextureOn * IS_CUSTOM_NORMALS_ON |
		textureFlags.isRenderSpecularTextureOn * IS_CUSTOM_SPECULAR_ON |
		textureFlags.isRenderAmbientOcclusionTextureOn * IS_AMBIENT_OCCLUSION_ON);

	mesh.GetMaterial().BindMaterialWithLightShader(*m_lightShader, flags);

	m_uniformLightBuffer->SendBufferSubData(0, lightInfo.lights.size() * sizeof(DirectionalLight), lightInfo.lights.data());

	m_lightShader->UniformSet1Int("isToonShadingOn", 
		lightInfo.currentLightModel == LIGHT_MODEL_TOON_SHADING || lightInfo.currentLightModel == LIGHT_MODEL_RIM_AND_TOON_SHADING);
	m_lightShader->UniformSet1Int("isRimLightOn",
		lightInfo.currentLightModel == LIGHT_MODEL_RIM_SHADING || lightInfo.currentLightModel == LIGHT_MODEL_RIM_AND_TOON_SHADING);

	m_lightShader->UniformSet3FloatVector("cameraPosition", m_sceneCamera->GetCameraPos());
	Renderer::RenderMesh(*m_lightShader, mesh, *m_sceneCamera);

}

void OBJ_Viewer::SceneRenderer::SetUpForWireframeRendering(const Mesh& mesh,const WireFrameSettings& wireframeAppSettings)
{
	const glm::mat3 viewportTransform = ConstructViewportMatrix();
	if (wireframeAppSettings.isPointRenderingOn)
	{
		constexpr static float pointSizeOffset = 1.;
		m_wireframePointShader->UseShader();
		m_wireframePointShader->UniformSet3FloatVector("u_Color", wireframeAppSettings.lineColor);
		m_wireframePointShader->UniformSet3x3FloatMatrix("viewportMatrix", viewportTransform);
		m_wireframePointShader->UniformSet1Float("pointSize", wireframeAppSettings.lineThickness + pointSizeOffset);
		Renderer::RenderMesh(*m_wireframePointShader, mesh, *m_sceneCamera);
	}
	else
	{
		m_wireframeShader->UseShader();
		m_wireframeShader->UniformSet3FloatVector("u_frameColor", wireframeAppSettings.lineColor);
		m_wireframeShader->UniformSet1Float("frameThickness", wireframeAppSettings.lineThickness);

		m_wireframeShader->UniformSet3x3FloatMatrix("viewportMatrix", viewportTransform);
		Renderer::RenderMesh(*m_wireframeShader, mesh, *m_sceneCamera);
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
	m_clearColorShader = std::make_unique<ShaderClass>(GetConcatShaderPath("ColorShader.glsl").c_str());
	m_skyboxShader = std::make_unique<ShaderClass>(GetConcatShaderPath("SkyboxShader.glsl").c_str());
	m_gridShader = std::make_unique<ShaderClass>(GetConcatShaderPath("GridShader.glsl").c_str());
	m_lightShader = std::make_unique<ShaderClass>(GetConcatShaderPath("LightShader.glsl").c_str());
	m_materialShader = std::make_unique<ShaderClass>(GetConcatShaderPath("MaterialShader.glsl").c_str());
	m_wireframeShader = std::make_unique<ShaderClass>(GetConcatShaderPath("WireframeShader.glsl").c_str());
	m_wireframePointShader = std::make_unique<ShaderClass>(GetConcatShaderPath("WireframePointShader.glsl").c_str());

	m_uniformMatrixBuffer = std::make_unique<UniformBuffer>("Matrices", 0, 3 * sizeof(glm::mat4), nullptr);
	m_uniformMatrixBuffer->BindBufferRange(0, 3 * sizeof(glm::mat4));

	m_uniformLightBuffer = std::make_unique<UniformBuffer>("LightInfo", 1, MAX_LIGHT_COUNT * 2* sizeof(glm::vec4), nullptr);
	m_uniformLightBuffer->BindBufferRange(0, MAX_LIGHT_COUNT * 2 * sizeof(glm::vec4));

	m_clearColorShader->BindUBOToShader(*m_uniformMatrixBuffer);
	m_skyboxShader->BindUBOToShader(*m_uniformMatrixBuffer);
	m_gridShader->BindUBOToShader(*m_uniformMatrixBuffer);
	m_lightShader->BindUBOToShader(*m_uniformMatrixBuffer);
	m_materialShader->BindUBOToShader(*m_uniformMatrixBuffer);
	m_wireframeShader->BindUBOToShader(*m_uniformMatrixBuffer);
	m_wireframePointShader->BindUBOToShader(*m_uniformMatrixBuffer);

	m_lightShader->BindUBOToShader(*m_uniformLightBuffer);

}

void OBJ_Viewer::SceneRenderer::SetUniformMatrixBuffer()const
{
	glm::mat4 matrices[3] = {};
	m_sceneCamera->GetViewAndProjectionSeparate(&matrices[0], &matrices[1]);
	matrices[2] = m_sceneModel->GetModelMatrix();
	m_uniformMatrixBuffer->SendBufferSubData(0, 3 * sizeof(glm::mat4), matrices);
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
