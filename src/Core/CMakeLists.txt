
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
ProjectPathHelper.h
)


target_sources(${PROJECT_NAME} PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})

