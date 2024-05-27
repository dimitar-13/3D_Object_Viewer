#include<array>
#include<memory>
#include"Texture.h"
#include<iostream>
#include"Mesh.h"
namespace OBJ_Viewer
{
	class Skybox {
	public:
		Skybox(std::array<const char*,6> textPaths);
		~Skybox();
		void BindSkyboxTexture()const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle); }
		const VertexAttributeObject& GetSkyboxVAO()const { return m_CubeMesh->GetMeshVAO(); }
	private:
		std::array<std::unique_ptr<Texture>,6> m_faceTextures;
		GLuint m_cubeMapHandle = 0;
		Mesh* m_CubeMesh;
	};

}