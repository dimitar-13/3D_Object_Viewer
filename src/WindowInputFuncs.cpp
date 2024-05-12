#include "WindowInputFuncs.h"

void OBJ_Viewer::WindowInputFuncs::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
}

void OBJ_Viewer::WindowInputFuncs::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
}

void OBJ_Viewer::WindowInputFuncs::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

void OBJ_Viewer::WindowInputFuncs::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void OBJ_Viewer::WindowInputFuncs::window_size_callback(GLFWwindow* window, int width, int height)
{
	//Sets update the window size;
	glViewport(0, 0, width, height);
}
