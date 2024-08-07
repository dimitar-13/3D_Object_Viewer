
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
source_group("Source Files" FILES ${INPUT_SOURCES})
source_group("Header Files" FILES ${INPUT_HEADERS})

target_sources(3D_Object_Viewer PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})