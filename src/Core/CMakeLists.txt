
set(INPUT_SOURCES
Application.cpp
WindowHandler.cpp
)

set(INPUT_HEADERS
AppEvent.h
Application.h
CommonAppData.h
Events.h
SceneViewport.h
WindowHandler.h
Config.h
)

source_group("Source Files" FILES ${INPUT_SOURCES})
source_group("Header Files" FILES ${INPUT_HEADERS})

target_sources(3D_Object_Viewer PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})

