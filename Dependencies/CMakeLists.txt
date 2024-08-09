set(INPUT_SOURCES
stbi_image/stb_imp.cpp
stbi_image/stb_write_imp.cpp
imgui/imgui/imgui_demo.cpp
imgui/imgui/imgui_draw.cpp
imgui/imgui/imgui_tables.cpp
imgui/imgui/imgui_widgets.cpp
imgui/imgui/imgui.cpp
imgui/imgui/backends/imgui_impl_glfw.cpp
imgui/imgui/backends/imgui_impl_opengl3.cpp
)

set(INPUT_HEADERS
imgui/imgui/imgui_internal.h
imgui/imgui/imstb_rectpack.h
imgui/imgui/imstb_textedit.h
imgui/imgui/imstb_truetype.h
imgui/imgui/imconfig.h
imgui/imgui/imgui.h
imgui/imgui/backends/imgui_impl_glfw.h
imgui/imgui/backends/imgui_impl_opengl3.h
stbi_image/stb_image_write.h
stbi_image/stb_image.h
)

#set_source_files_properties(${INPUT_SOURCES}PROPERTIES COMPILE_FLAGS "/Y-")

target_sources(3D_Object_Viewer PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})

