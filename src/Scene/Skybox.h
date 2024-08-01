#pragma once
#include "pch.h"
#include "gpu_side/VertexAttributeObject.h"
#include "gpu_side/Texture.h"
#include "gpu_side/OpenGLBuffer.h"
#include "Helpers/TextureHelpers.h"
namespace OBJ_Viewer
{	
	enum SkyboxFace_
	{
		SkyboxFace_kRight  =  0,
		SkyboxFace_kLeft   =  1,
        SkyboxFace_kTop    =  2,
        SkyboxFace_kBottom =  3,
        SkyboxFace_kFront  =  4,
        SkyboxFace_kBack   =  5
	};
	class Skybox {
	public:
		static constexpr uint8_t SKYBOX_FACE_COUNT = 6;
		Skybox(const std::array<TexturePixelReader,Skybox::SKYBOX_FACE_COUNT>& textures);
		~Skybox();
		void BindSkyboxTexture()const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle); }
		void SwapSkyboxFaceTextures(SkyboxFace_ toBeSwapped, SkyboxFace_ swappedWith);
		const VertexAttributeObject& GetSkyboxVAO()const { return *m_skyboxVAO; }
		const std::array<std::shared_ptr<Texture>, SKYBOX_FACE_COUNT>& const GetSkyboxFaceTextures() { return m_faceTextures; }
    private:
		std::array<std::shared_ptr<Texture>, SKYBOX_FACE_COUNT> m_faceTextures;
		std::array<std::unique_ptr<OpenGLBuffer>, SKYBOX_FACE_COUNT> m_PixelBuffers;
		GLuint m_cubeMapHandle = 0;
		std::unique_ptr<VertexAttributeObject> m_skyboxVAO;
		Size2D m_CubeMapTextSize;
		TextureFormat_ m_format;
	};

}