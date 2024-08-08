cmake_minimum_required(VERSION 3.28.1) 
project(3D_Object_Viewer)              
set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
endif()

add_executable(3D_Object_Viewer 3D_Object_Viewer.cpp)
target_sources(3D_Object_Viewer PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../Shaders/ShaderPath.h)

source_group("Source Files" FILES 3D_Object_Viewer.cpp)
source_group("Header Files" FILES ${CMAKE_CURRENT_LIST_DIR}/../Shaders/ShaderPath.h)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Debug)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Release)

set_target_properties(3D_Object_Viewer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
)
set_target_properties(3D_Object_Viewer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release
)


set(GLEW_RELEASE ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/glew/glew-2.2.0/lib/Release/x64)
set(GLEW_DEBUG   ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/glew/glew-2.2.0/lib/Debug/x64)
set(NFD_RELEASE  ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/nativefiledialog/nativefiledialog/build/lib/Release/x64)
set(NFD_DEBUG    ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/nativefiledialog/nativefiledialog/build/lib/Debug/x64)

set(GLFW_RELEASE ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/glfw/glfw/build/src/Release)
set(GLFW_DEBUG   ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/glfw/glfw/build/src/Debug)

set(ASSIMP_DEBUG   ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/assimp/assimp/build/lib/Debug)
set(ASSIMP_RELEASE ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/assimp/assimp/build/lib/Release)



set(DLL_SOURCE_PATH_DEBUG ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/assimp/assimp/build/bin/Debug/assimp-vc143-mtd.dll)
set(DLL_SOURCE_PATH_RELEASE ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/assimp/assimp/build/bin/Release/assimp-vc143-mt.dll)


add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/pch)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../Shaders  ${CMAKE_BINARY_DIR}/Shaders)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../Dependencies ${CMAKE_BINARY_DIR}/Dependencies)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Controls)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Core)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Enums)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gpu_side)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Helpers)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Logging)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Profiling)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Rendering)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Scene)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/UI)

add_dependencies(3D_Object_Viewer copy_shaders)

target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/glfw/glfw/include)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/glew/glew-2.2.0/include)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/glm/glm)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/imgui/imgui)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/imgui/imgui/backends)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/nativefiledialog/nativefiledialog/src/include)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/nativefiledialog/nativefiledialog/src)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/assimp/assimp/include)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Shaders)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/stbi_image)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/assimp/assimp/build/include)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/spdlog/spdlog/include)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/pch)


target_link_directories(3D_Object_Viewer PUBLIC
    $<$<CONFIG:Debug>:${GLEW_DEBUG}>
    $<$<CONFIG:Release>:${GLEW_RELEASE}>
    $<$<CONFIG:Debug>:${NFD_DEBUG}>
    $<$<CONFIG:Release>:${NFD_RELEASE}>
    $<$<CONFIG:Debug>:${GLFW_DEBUG}>
    $<$<CONFIG:Release>:${GLFW_RELEASE}>
    $<$<CONFIG:Debug>:${ASSIMP_DEBUG}>
    $<$<CONFIG:Release>:${ASSIMP_RELEASE}>
)
add_subdirectory(${CMAKE_SOURCE_DIR}/../Dependencies/glfw/glfw ${CMAKE_BINARY_DIR}/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/../Dependencies/glew/glew-2.2.0/build/cmake ${CMAKE_BINARY_DIR}/glew)
add_subdirectory(${CMAKE_SOURCE_DIR}/../Dependencies/glm/glm ${CMAKE_BINARY_DIR}/glm)
add_subdirectory(${CMAKE_SOURCE_DIR}/../Dependencies/assimp/assimp ${CMAKE_BINARY_DIR}/assimp)
add_subdirectory(${CMAKE_SOURCE_DIR}/../Dependencies/spdlog/spdlog ${CMAKE_BINARY_DIR}/spdlog)




target_compile_definitions(3D_Object_Viewer PUBLIC
	 GLEW_STATIC
	 $<$<CONFIG:Debug>: OBJ_VIEWER_LEVEL_DEBUG>
)

target_link_libraries(3D_Object_Viewer 
	PUBLIC opengl32	
	PUBLIC glfw3.lib
   "$<$<CONFIG:Release>:glew32s.lib>"
   "$<$<CONFIG:Debug>:glew32sd.lib>"

   "$<$<CONFIG:Release>:nfd.lib>"
   "$<$<CONFIG:Debug>:nfd_d.lib>"

   "$<$<CONFIG:Release>:assimp-vc143-mt.lib>"
   "$<$<CONFIG:Debug>:assimp-vc143-mtd.lib>"
)


add_custom_command(
    TARGET 3D_Object_Viewer
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${DLL_SOURCE_PATH_DEBUG}>
        $<$<CONFIG:Release>:${DLL_SOURCE_PATH_RELEASE}>
        ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    COMMENT "Copying DLL to the current bin output directory"
)
add_custom_target(nfd_project ALL DEPENDS ${CMAKE_SOURCE_DIR}/../Dependencies/nativefiledialog/nativefiledialog/build/vs2010/nfd.vcxproj)
add_custom_target(glew_static ALL DEPENDS ${CMAKE_SOURCE_DIR}/../Dependencies/glew/glew-2.2.0/build/vc15/glew_static.vcxproj)

add_dependencies(3D_Object_Viewer glfw glm spdlog assimp glew_static nfd_project )
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 3D_Object_Viewer)
