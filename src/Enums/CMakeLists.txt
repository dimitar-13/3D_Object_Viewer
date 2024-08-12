
set(INPUT_HEADERS
FIleImageEnums.h
GpuDataTypes.h
MaterialFlags.h
TextureEnums.h
OpenGLBufferType.h
)

target_sources(${PROJECT_NAME} PRIVATE  ${INPUT_HEADERS})