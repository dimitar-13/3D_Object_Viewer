cmake_minimum_required(VERSION 3.28.1) 
project(3D_Object_Viewer)

#Compiler/IDE settings.
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

add_executable(${PROJECT_NAME} "3D_Object_Viewer.cpp")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

#Project gloabal settings.
set_target_properties(${PROJECT_NAME}  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug")
set_target_properties(${PROJECT_NAME}  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release")

target_compile_definitions(${PROJECT_NAME} PUBLIC
	 GLEW_STATIC
	 "$<$<CONFIG:Debug>: OBJ_VIEWER_LEVEL_DEBUG>"
)
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Debug")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Release")

#Global cmake variables.
set(PROJECT_DEPENDENCY_FOLDER "${CMAKE_CURRENT_LIST_DIR}/../Dependencies")
set(PROJECT_SHADERS_FOLDER    "${CMAKE_CURRENT_LIST_DIR}/../Shaders")
set(PROJECT_INFO_FOLDER       "${CMAKE_SOURCE_DIR}/../project")

set(GLEW_RELEASE "${CMAKE_BINARY_DIR}/lib/Release")
set(GLEW_DEBUG   "${CMAKE_BINARY_DIR}/lib/Debug")

set(NFD_RELEASE  "${PROJECT_DEPENDENCY_FOLDER}/nativefiledialog/nativefiledialog/build/lib/Release/x64")
set(NFD_DEBUG    "${PROJECT_DEPENDENCY_FOLDER}/nativefiledialog/nativefiledialog/build/lib/Debug/x64")

set(GLFW_RELEASE "${PROJECT_DEPENDENCY_FOLDER}/glfw/glfw/build/src/Release")
set(GLFW_DEBUG   "${PROJECT_DEPENDENCY_FOLDER}/glfw/glfw/build/src/Debug")

set(ASSIMP_RELEASE "${PROJECT_DEPENDENCY_FOLDER}/assimp/assimp/build/lib/Release")
set(ASSIMP_DEBUG   "${PROJECT_DEPENDENCY_FOLDER}/assimp/assimp/build/lib/Debug")

set(ASSIMP_DLL_SOURCE_PATH_RELEASE "${PROJECT_DEPENDENCY_FOLDER}/assimp/assimp/build/bin/Release/assimp-vc143-mt.dll")
set(ASSIMP_DLL_SOURCE_PATH_DEBUG   "${PROJECT_DEPENDENCY_FOLDER}/assimp/assimp/build/bin/Debug/assimp-vc143-mtd.dll")

#Adding project files and subdirectories.
target_sources(${PROJECT_NAME} PRIVATE "${PROJECT_SHADERS_FOLDER}/ShaderPath.h")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/pch")
add_subdirectory("${PROJECT_SHADERS_FOLDER}" "${CMAKE_BINARY_DIR}/Shaders")
add_subdirectory("${PROJECT_DEPENDENCY_FOLDER}" "${CMAKE_BINARY_DIR}/Dependencies")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Controls")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Core")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Enums")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/gpu_side")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Helpers")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Logging")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Profiling")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Rendering")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Scene")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/UI")

#Adding project build dependencies
add_subdirectory("${PROJECT_DEPENDENCY_FOLDER}/assimp/assimp" "${PROJECT_DEPENDENCY_FOLDER}/assimp/assimp/build")
set(BUILD_SHARED_LIBS OFF)
add_subdirectory("${PROJECT_DEPENDENCY_FOLDER}/glm/glm" "${CMAKE_BINARY_DIR}/glm")
add_subdirectory("${PROJECT_DEPENDENCY_FOLDER}/glfw/glfw" "${PROJECT_DEPENDENCY_FOLDER}/glfw/glfw/build")
add_subdirectory("${PROJECT_DEPENDENCY_FOLDER}/glew/glew-2.2.0/build/cmake" "${PROJECT_DEPENDENCY_FOLDER}/glew/glew-2.2.0/build/cmake/build")
add_subdirectory("${PROJECT_DEPENDENCY_FOLDER}/spdlog/spdlog" "${PROJECT_DEPENDENCY_FOLDER}/spdlog/spdlog/build")
include_external_msproject(
    nfd
    "${PROJECT_DEPENDENCY_FOLDER}/nativefiledialog/nativefiledialog/build/vs2010/nfd.vcxproj"
    TYPE STATIC
)
#Project pch(pre-compiled header) setup.
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/pch/pch.h")

set_source_files_properties(
    "${PROJECT_DEPENDENCY_FOLDER}/imgui/imgui/imgui_demo.cpp"
    "${PROJECT_DEPENDENCY_FOLDER}/imgui/imgui/imgui_draw.cpp"
	"${PROJECT_DEPENDENCY_FOLDER}/imgui/imgui/imgui_tables.cpp"
	"${PROJECT_DEPENDENCY_FOLDER}/imgui/imgui/imgui_widgets.cpp"
	"${PROJECT_DEPENDENCY_FOLDER}/imgui/imgui/imgui.cpp"
	"${PROJECT_DEPENDENCY_FOLDER}/imgui/imgui/backends/imgui_impl_opengl3.cpp"
	"${PROJECT_DEPENDENCY_FOLDER}/imgui/imgui/backends/imgui_impl_glfw.cpp"
	PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)

#Project folder grouping.
source_group(src FILES 3D_Object_Viewer.cpp)
source_group(src FILES ${PROJECT_SHADERS_FOLDER}/ShaderPath.h)
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/*.h" )
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SOURCES})
file(GLOB SOURCES "${PROJECT_SHADERS_FOLDER}/*.glsl")
source_group("Shaders" FILES ${SOURCES})
file(GLOB SOURCES 
"${PROJECT_DEPENDENCY_FOLDER}/imgui/imgui/*.cpp" 
"${PROJECT_DEPENDENCY_FOLDER}/imgui/imgui/*.h" 
"${PROJECT_DEPENDENCY_FOLDER}/imgui/imgui/backends/*.cpp"
"${PROJECT_DEPENDENCY_FOLDER}/imgui/imgui/backends/*.h")
source_group("Dependencies/imgui" FILES ${SOURCES} )
file(GLOB SOURCES "${PROJECT_DEPENDENCY_FOLDER}/stbi_image/*.cpp" "${PROJECT_DEPENDENCY_FOLDER}/stbi_image/*.h")
source_group("Dependencies/stbi_image" FILES ${SOURCES} )

#Project include and link directories.
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DEPENDENCY_FOLDER}/glfw/glfw/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DEPENDENCY_FOLDER}/glew/glew-2.2.0/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DEPENDENCY_FOLDER}/glm/glm")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DEPENDENCY_FOLDER}/imgui/imgui")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DEPENDENCY_FOLDER}/imgui/imgui/backends")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DEPENDENCY_FOLDER}/nativefiledialog/nativefiledialog/src/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DEPENDENCY_FOLDER}/nativefiledialog/nativefiledialog/src")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DEPENDENCY_FOLDER}/assimp/assimp/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SHADERS_FOLDER}")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DEPENDENCY_FOLDER}/stbi_image")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DEPENDENCY_FOLDER}/assimp/assimp/build/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_DEPENDENCY_FOLDER}/spdlog/spdlog/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/pch")
target_link_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:Debug>:${GLEW_DEBUG}>"
    "$<$<CONFIG:Release>:${GLEW_RELEASE}>"
    "$<$<CONFIG:Debug>:${NFD_DEBUG}>"
    "$<$<CONFIG:Release>:${NFD_RELEASE}>"
    "$<$<CONFIG:Debug>:${GLFW_DEBUG}>"
    "$<$<CONFIG:Release>:${GLFW_RELEASE}>"
    "$<$<CONFIG:Debug>:${ASSIMP_DEBUG}>"
    "$<$<CONFIG:Release>:${ASSIMP_RELEASE}>"
)

#Project libraries and build dependencies.
target_link_libraries(${PROJECT_NAME}
	PUBLIC opengl32
   "$<$<CONFIG:Release>:glfw3.lib>"
   "$<$<CONFIG:Debug>:glfw3d.lib>"
	
   "$<$<CONFIG:Release>:libglew32.lib>"
   "$<$<CONFIG:Debug>:libglew32d.lib>"

   "$<$<CONFIG:Release>:nfd.lib>"
   "$<$<CONFIG:Debug>:nfd_d.lib>"

   "$<$<CONFIG:Release>:assimp-vc143-mt.lib>"
   "$<$<CONFIG:Debug>:assimp-vc143-mtd.lib>"
)
add_dependencies(${PROJECT_NAME} glfw glm spdlog assimp glew_s nfd copy_shaders)

#Project post build commands.
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<$<CONFIG:Debug>:${ASSIMP_DLL_SOURCE_PATH_DEBUG}>"
        "$<$<CONFIG:Release>:${ASSIMP_DLL_SOURCE_PATH_RELEASE}>"
        ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    COMMENT "Copying assimp dll into the $<CONFIG> directory"
)
add_custom_command(
	TARGET ${PROJECT_NAME}
    POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E make_directory 
		${CMAKE_BINARY_DIR}/bin/$<CONFIG>/project
    COMMAND ${CMAKE_COMMAND} -E copy
	   "${PROJECT_INFO_FOLDER}/imgui.ini"
	   "${PROJECT_INFO_FOLDER}/LICENSE.txt"
	   "${PROJECT_INFO_FOLDER}/README.md"

       ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/project/
    COMMENT "Creating Shaders directory and copying shader files to $<CONFIG> directory."
)
