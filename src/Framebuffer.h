#pragma once
#include<GL/glew.h>
namespace OBJ_Viewer {
	class Framebuffer
	{
	private:
		GLuint m_framebuffer, m_readBuffer, m_framebufferTextureID;
	};
}

