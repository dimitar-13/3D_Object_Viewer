cmake_minimum_required(VERSION 3.13) 
project(3D_Object_Viewer)              
set(CMAKE_CXX_STANDARD 17)           

set(SOURCE_FILES 3D_Object_Viewer.cpp)

add_subdirectory(Controls)
add_subdirectory(Core)
add_subdirectory(Enums)
add_subdirectory(gpu_side)
add_subdirectory(Helpers)
add_subdirectory(Logging)
add_subdirectory(pch)
add_subdirectory(Profiling)
add_subdirectory(Rendering)
add_subdirectory(Scene)
add_subdirectory(UI)
add_subdirectory(../Dependencies)


target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/glfw/glfw/include)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/glew/glew-2.2.0/include)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/glm/glm)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/imgui/imgui)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/imgui/imgui/backends)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../src)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/nativefiledialog/nativefiledialog/src/include)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/nativefiledialog/nativefiledialog/src)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/assimp/assimp/include)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Shaders)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/stbi_image)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/assimp/assimp/build/include)
target_include_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/spdlog/spdlog/include)

target_link_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/glew/glew-2.2.0/lib/Debug/x64)
target_link_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/glfw/glfw/build/src/Debug)
target_link_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/nativefiledialog/nativefiledialog/build/lib/Debug/x64)
target_link_directories(3D_Object_Viewer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/assimp/assimp/build/lib/Debug)

add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/glfw/glfw)
add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/glew/glew-2.2.0)
add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/glm/glm)
add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/assimp/assimp)
add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/../Dependencies/spdlog/spdlog)





target_compile_definitions(3D_Object_Viewer PUBLIC
	-DGLEW_STATIC
	 $<$<CONFIG:Debug>:-DOBJ_VIEWER_LEVEL_DEBUG>
)

target_link_libraries(3D_Object_Viewer 
PUBLIC glfw3
PUBLIC opengl32
PUBLIC glew
PUBLIC nfd_d
PUBLIC assimp-vc143-mtd
)

add_dependencies(3D_Object_Viewer glfw glm glew_static nfd spdlog)