#pragma once
#include "pch.h"
#include "gpu_side/VertexAttributeObject.h"
#include "gpu_side/Texture.h"
#include "gpu_side/OpenGLBuffer.h"
#include "Helpers/TextureHelpers.h"
namespace OBJ_Viewer
{
	
	enum SkyboxFace
	{
		SKYBOX_FACE_RIGHT = 0,
		SKYBOX_FACE_LEFT = 1,
		SKYBOX_FACE_TOP = 2,
		SKYBOX_FACE_BOTTOM = 3,
		SKYBOX_FACE_FRONT = 4,
		SKYBOX_FACE_BACK = 5
	};
	class Skybox {
	public:
		static constexpr uint8_t SKYBOX_FACE_COUNT = 6;
		Skybox(const std::array<TexturePixelReader,Skybox::SKYBOX_FACE_COUNT>& textures);
		~Skybox();
		void BindSkyboxTexture()const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle); }
		void SwapSkyboxFaceTextures(SkyboxFace toBeSwapped, SkyboxFace swappedWith);
		const VertexAttributeObject& GetSkyboxVAO()const { return *m_skyboxVAO; }
		const std::vector<std::shared_ptr<Texture>>& const GetSkyboxFaceTextures() { return m_faceTextures; }	
		//TODO:Hide away the pointers make the unique and return the reference to them if needed
	private:
		std::vector<std::shared_ptr<Texture>> m_faceTextures;
		std::vector<OpenGLBuffer> m_PixelBuffers;
		GLuint m_cubeMapHandle = 0;
		std::unique_ptr<VertexAttributeObject> m_skyboxVAO;
		Size2D m_CubeMapTextSize;
		TextureFormat_ m_format;
	};

}