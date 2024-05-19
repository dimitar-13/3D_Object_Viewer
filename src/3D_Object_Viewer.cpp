#include <iostream>
#include "Application.h"
#include"InputHandler.h"
int main()
{
	OBJ_Viewer::Application app;
	OBJ_Viewer::InputHandler inputHandler(app.GetAppWindow().GetMouseButtonNotifier(), app.GetAppWindow().GetKeyNotifier());
	OBJ_Viewer::RenderingCoordinator renderer(&app.GetAppWindow(), &inputHandler);
	renderer.RenderLoop();
	return 1;

}