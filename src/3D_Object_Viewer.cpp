#include <iostream>
#include "Application.h"

int main()
{
	if (!OBJ_Viewer::Application::Init())
	{
		std::cout << "[ERROR]:Application failed to initialize." << '\n';
		return -1;
	}
	OBJ_Viewer::Renderer renderer;
	renderer.RenderLoop();
	return 1;

}