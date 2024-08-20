#pragma once
//STL and others
#include<unordered_map>
#include<memory>
#include<iostream>
#include<string>
#include<string_view>
#include<array>
#include<vector>
#include<algorithm>
#include<numeric>
#include<filesystem>
#include<cassert>
#include<future>
//OpenGL related includes
#include<GL/glew.h>
#include<GLFW/glfw3.h>

//GLM includes
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

//ImGui includes
#include"imgui_internal.h"
#include"imgui.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

//File includes
#include <nfd.h>
#include<fstream>

//Assimp includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 

//Spdlog includes
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

//Stb_imgage includes
#include "stb_image.h"
#include "stb_image_write.h"