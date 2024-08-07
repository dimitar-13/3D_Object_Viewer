
set(INPUT_HEADERS
FIleImageEnums.h
GpuDataTypes.h
MaterialFlags.h
TextureEnums.h
)
source_group("Header Files" FILES ${INPUT_HEADERS})
target_sources(3D_Object_Viewer PRIVATE  ${INPUT_HEADERS})