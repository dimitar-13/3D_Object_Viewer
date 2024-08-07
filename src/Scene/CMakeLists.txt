set(INPUT_SOURCES
Material.cpp
MaterialRegistry.cpp
Mesh.cpp
Model.cpp
Skybox.cpp
Camera.cpp
)

set(INPUT_HEADERS
Model.h
Skybox.h
Camera.h
Material.h
MaterialRegistry.h
Mesh.h
)
source_group("Source Files" FILES ${INPUT_SOURCES})
source_group("Header Files" FILES ${INPUT_HEADERS})

target_sources(3D_Object_Viewer PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})