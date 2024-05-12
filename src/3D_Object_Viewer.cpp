#include <iostream>
#include "Application.h"

int main()
{
	OBJ_Viewer::Application app;
	OBJ_Viewer::RendererCoordinator renderer(&app.GetAppWindow());
	renderer.RenderLoop();
	return 1;

}