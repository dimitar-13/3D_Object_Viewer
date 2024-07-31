#pragma once
#include "pch.h"
namespace OBJ_Viewer {
	class UniformBuffer
	{
	public:
		UniformBuffer(const std::string& buffer_name, GLuint buffer_binding_point,const uint32_t kBufferSizeInBytes, void* buffer_data);
		~UniformBuffer();
		void SendBufferSubData(const size_t kSubDataOffset, const size_t kNewDataSizeInBytes, const void* buffer_data_to_send);
		void UnbindBuffer()const { glBindBuffer(GL_UNIFORM_BUFFER, 0); }
		void BindBuffer()const { glBindBuffer(GL_UNIFORM_BUFFER, m_bufferHandle); }
		void BindBufferRange(const size_t kBufferDataOffset, const size_t kBufferSize)const;
		const std::string& GetBufferName()const { return m_BufferName; }
		GLuint GetBindingPoint()const { return m_bindingPoint; }
	private:
		GLuint m_bufferHandle;
		GLuint m_bindingPoint;
		std::string m_BufferName;
	};
}

