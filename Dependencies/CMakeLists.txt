set(INPUT_SOURCES
stbi_image/stb_imp.cpp
stbi_image/stb_write_imp.cpp
imgui\imgui\imgui_demo.cpp
imgui\imgui\imgui_draw.cpp
imgui\imgui\imgui_tables.cpp
imgui\imgui\imgui_widgets.cpp
imgui\imgui\imgui.cpp
)

set(INPUT_HEADERS
imgui\imgui\imgui_internal.h
imgui\imgui\imstb_rectpack.h
imgui\imgui\imstb_textedit.h
imgui\imgui\imstb_truetype.h
imgui\imgui\imconfig.h
imgui\imgui\imgui.h
)

target_sources(3D_Object_Viewer PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})