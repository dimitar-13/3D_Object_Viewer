set(INPUT_SOURCES
DialogWrapper.cpp
ModelGenerator.cpp
ModelLoader.cpp
TextureHelpers.cpp
)

set(INPUT_HEADERS
ModelLoader.h
TextureHelpers.h
UnitTransformer.h
DialogWrapper.h
FileFromatStruct.h
ModelGenerator.h
)

target_sources(${PROJECT_NAME} PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})