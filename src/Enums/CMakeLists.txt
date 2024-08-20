
set(INPUT_HEADERS
FIleImageEnums.h
GpuDataTypes.h
MaterialFlags.h
TextureEnums.h
OpenGLBufferType.h
EventTypeCategoryEnums.h
)

target_sources(${PROJECT_NAME} PRIVATE  ${INPUT_HEADERS})