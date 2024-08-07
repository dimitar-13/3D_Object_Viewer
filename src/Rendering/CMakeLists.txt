set(INPUT_SOURCES
SceneRenderer.cpp
ShaderLibrary.cpp
Renderer.cpp
RenderingCoordinator.cpp
)

set(INPUT_HEADERS
RenderingCoordinator.h
SceneConfigurationSettingsStruct.h
SceneRenderer.h
ShaderLibrary.h
Renderer.h
)
source_group("Source Files" FILES ${INPUT_SOURCES})
source_group("Header Files" FILES ${INPUT_HEADERS})

target_sources(3D_Object_Viewer PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})