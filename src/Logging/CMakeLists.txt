set(INPUT_SOURCES
App_Logger.cpp
)

set(INPUT_HEADERS
App_Logger.h
)
source_group("Source Files" FILES ${INPUT_SOURCES})
source_group("Header Files" FILES ${INPUT_HEADERS})

target_sources(3D_Object_Viewer PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})