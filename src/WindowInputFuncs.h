#pragma once
#include<GLFW/glfw3.h>
namespace OBJ_Viewer
{
	class WindowInputFuncs {
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};

}