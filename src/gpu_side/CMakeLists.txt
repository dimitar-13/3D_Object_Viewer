
set(INPUT_SOURCES
Framebuffer.cpp
OpenGLBuffer.cpp
ShaderClass.cpp
Texture.cpp
UniformBuffer.cpp
VertexAttributeObject.cpp
)

set(INPUT_HEADERS
Framebuffer.h
OpenGLBuffer.h
ShaderClass.h
Texture.h
UniformBuffer.h
VertexAttributeObject.h
)

target_sources(3D_Object_Viewer PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})