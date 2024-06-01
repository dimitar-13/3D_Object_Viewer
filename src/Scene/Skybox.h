#include<array>
#include<memory>
#include"Texture.h"
#include<iostream>
#include"Mesh.h"
#include"Rendering/OpenGLBuffer.h"
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
		Skybox(std::vector<char*>& textPaths);
		~Skybox();
		void BindSkyboxTexture()const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle); }
		void SwapSkyboxFaceTextures(SkyboxFace toBeSwapped, SkyboxFace swappedWith);
		const VertexAttributeObject& GetSkyboxVAO()const { return m_CubeMesh->GetMeshVAO(); }
		std::vector<std::shared_ptr<Texture>> m_faceTextures;
	private:
		std::vector<OpenGLBuffer*> m_PixelBuffers;
		GLuint m_cubeMapHandle = 0;
		Mesh* m_CubeMesh;
		TextureSize m_CubeMapTextSize;
		TextureFormat m_format;
	};

}