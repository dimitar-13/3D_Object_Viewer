set(INPUT_SOURCES
AppProfiler.cpp
)

set(INPUT_HEADERS
AppProfiler.h
)
source_group("Source Files" FILES ${INPUT_SOURCES})
source_group("Header Files" FILES ${INPUT_HEADERS})

target_sources(3D_Object_Viewer PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})