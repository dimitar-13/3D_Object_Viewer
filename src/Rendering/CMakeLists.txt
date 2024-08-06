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

target_sources(3D_Object_Viewer PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})