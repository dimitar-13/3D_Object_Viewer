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

target_sources(${PROJECT_NAME} PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})