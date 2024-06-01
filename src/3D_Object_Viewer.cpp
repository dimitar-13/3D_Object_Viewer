#include <iostream>
#include "Application.h"
#include"InputHandler.h"
#include"Rendering/RenderingCoordinator.h"
#include"AppState.h"
int main()
{
	OBJ_Viewer::Application app;
	OBJ_Viewer::InputHandler inputHandler(app.GetAppWindow().GetMouseButtonNotifier(), app.GetAppWindow().GetKeyNotifier());
	OBJ_Viewer::AppState appState(&app.GetAppWindow(),&inputHandler);
	OBJ_Viewer::RenderingCoordinator renderer(&appState);
	renderer.RenderLoop();
	return 1;
}