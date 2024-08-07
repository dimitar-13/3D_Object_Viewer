set(INPUT_SOURCES
pch.cpp
)

set(INPUT_HEADERS
pch.h
)
source_group("Source Files" FILES ${INPUT_SOURCES})
source_group("Header Files" FILES ${INPUT_HEADERS})

target_sources(3D_Object_Viewer PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})

#target_precompile_headers(3D_Object_Viewer PRIVATE pch.h)