#include <iostream>
#include "Application.h"

int main()
{
	OBJ_Viewer::Application app;
	OBJ_Viewer::RenderingCoordinator renderer(&app.GetAppWindow());
	renderer.RenderLoop();
	return 1;

}