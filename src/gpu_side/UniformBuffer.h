#pragma once
#include "pch.h"
namespace OBJ_Viewer {
	class UniformBuffer
	{
	public:
		UniformBuffer(std::string bufferName,GLuint bindPoint,const uint32_t buffSize,void * data);
		~UniformBuffer();
		void SendBufferSubData(const uint32_t dataBufferOffset, const uint32_t buffSize, const void* data)const;
		void UnbindBuffer()const { glBindBuffer(GL_UNIFORM_BUFFER, 0); }
		void BindBuffer()const { glBindBuffer(GL_UNIFORM_BUFFER, m_bufferHandle); }
		//void BindBufferToShader(const ShaderClass& shader, GLuint m_bindingPoint);
		void BindBufferRange(const uint32_t dataBufferOffset, const uint32_t bufferRange)const;
		const std::string& GetBufferName()const { return m_BufferName; }
		GLuint GetBindingPoint()const { return m_bindingPoint; }
	private:
		GLuint m_bufferHandle;
		GLuint m_bindingPoint;
		std::string m_BufferName;
	};
}

