# 3D_Object_Viewer

## About the project

This is a simple application for viewing 3D model/scene files with different options. 
The project aims to give you more ways to inspect your 3D model with various shading 
options and scene configurations. It's easy and fast to use if you just want to inspect
your model and not make changes to it. Currently the project is working only on windows.
You can download the pre-compiled binaries but I recommend building the project see inster buidung project

## Project Features

The project so far has these features:

- **Screenshot option:** The user can take a screenshot and save it.
- **Rendering/Viewing options:** The user can view their model with different options:
  - **Studio light:** This is the default rendering option you will see when you first open the app. This option shades the front pixels with the color set by the app and the back pixels in black.
  - **Wireframe:** This mode renders a wireframe of lines or points on top of the object.
  - **Light rendering:** This mode renders the object with light calculation in mind. Currently, the app has 4 light models to choose from (Blinn-Phong, Toon, Rim, and Toon + Rim shading models).
  - **Single texture:** This mode shows a single texture with no light calculation, etc. The textures are shown in the sRGB color space (Yes, that includes normals, but when they are used for calculations, they are in linear space). If a texture is not present, the mesh is shaded pink.
  - **Normal orientation:** This mode shades the mesh based on whether the normals are facing outwards or inwards. If they are facing outwards (meaning they are correct), the face is shaded blue; otherwise, it is shaded red.
  - **UV:** This mode renders a scalable checkerboard pattern onto your mesh. You can scale it up or down to see if there is any UV stretching.
- **Camera movement control:** The user can freely move, zoom, and shift/pan the camera.
- **Skybox loading:** Although it is a little bit annoying to set up, the project provides a way for a user to load a skybox by providing the app with 6 textures. If the provided textures are not in the correct order, the project provides a way to swap them to re-adjust the skybox.
- **Scene floor grid:** At any time, you can enable the floor grid. The application provides some control over how the grid looks, including options like color, grid scale, and whether the X and Z axes are shaded.
- **Scene lights:** The user can add or remove lights from the scene and change each light individually. The light settings that the user can tweak include the color and the light direction.
- **Model info:** The application displays the loaded file info. The info consists of vertex, triangle, face, and texture count, file path, and loaded materials with present textures in each material.
- **Supported file formats:** Currently, the application supports OBJ and FBX file formats (the FBX file format is disabled by default due to a security vulnerability; you can enable it from `Edit > Enable FBX loading`).
- **Different camera projections:** The project has 2 projection types: perspective and orthographic.
- **Scene Anti-aliasing (AA):** The project has an option for enabling and disabling scene AA. The AA is a combination of MSAA + FXAA.
- **Gamma:** Excluding the UI, every pixel shown on the screen is gamma corrected (this applies to the albedo/color texture too).


![Project wireframe rendering image](/gitImages/Project_Image1.png?raw=true)
![Project wireframe rendering image](/gitImages/Project_Image2.png?raw=true)
> [!NOTE]
> Skybox used in this picture was generated from this [website](https://tools.wwwtyro.net/space-3d/index.html).

![Project wireframe rendering image](/gitImages/Project_Image3.png?raw=true)
![Project wireframe rendering image](/gitImages/Project_Image4.png?raw=true)


## How to build
First clone the project using the following command:
```git
git clone https://github.com/dimitar-13/3D_Object_Viewer.git --recursive
```
The command will recursively clone the project and all the dependencies the project have(GLFW ,assimp etc.)

To build the project you will need to have Cmake(I don't recement the GUI version because the cmd input is only 1 command).
Then you open cmd in the cloned project folder (where the `src` folder is) and use this command:
```
cmake -S src/ -B build/
```
This will create a `build` folder where the Visual Studio project will be generated.

> [!IMPORTANT] 
>
> I haven't tested the build system on other compilers such as Clang and GCC, but I think it should work.  
> When the build is done, you can launch your compiler or IDE and build the project in Release or Debug mode. The project binary will be in the `bin/Configuration` folder.

## How the Project Works

The project has 4 main components/classes: `Application`, `RenderingCoordinator`, `UILayer`, and `SceneRenderer`.

- The `Application` class is the project’s global information class. Most of the other classes depend on the `Application` class.
- The `RenderingCoordinator` class contains the rendering loop. This class is responsible for managing the rendering pipeline and establishing the connection between the two main rendering classes, `UILayer` and `SceneRenderer`.
- The `UILayer` class is responsible for rendering the UI and retrieving the information submitted by the user via the UI.
- The `SceneRenderer` class doesn't render anything directly but knows the order of rendering for every element in the scene. This class uses another class for rendering called the `Renderer`. The `SceneRenderer` class is also where the scene post-processing happens.

## Rendering a Frame

The process of rendering a frame can be broken down into these simple steps:

- Render the UI and retrieve the input from the user.
- Render the scene using the input from the user.
- Perform scene post-processing.

At the start of every frame, we first render the UI. The `RenderingCoordinator` class calls the `RenderUI` function from the `UILayer` class and passes it a reference to a structure that represents the submitted user data. After the UI is rendered, the data that the user submitted is then passed to the `SceneRenderer` function `RenderScene`. In that function, we first render the model. If Anti-aliasing is enabled, we use the MSAA framebuffer. Based on the selected rendering mode, the `SceneRenderer` calls the `Renderer` class with the appropriate rendering function. After the model is rendered, we copy the MSAA buffer content into an intermediate framebuffer. Then we render the grid, if enabled, and finally, we render the skybox, if enabled (we don't use MSAA on the skybox and grid because they don't benefit from it). After everything is rendered, we use the intermediate framebuffer for post-processing. The post-processing consists of fast approximate anti-aliasing (FXAA) and then gamma correction at the end. If the `RenderScene` function is supplied with an output framebuffer, we use that to store the post-processed scene; if not, we use the default framebuffer.

## Features to Add in the Future

Some features I would like to add in the future include:

- **Object selection:** You can select an object/mesh and interact with it.
- **Hide all but selected object/mesh:** If one mesh is currently active/selected, by pressing a button, hide all of the other meshes and zoom in on the active one (similar to Blender when you select an object and press `/`).
- **Scene graph:** A scene graph where you can hide, unhide, and check all of the selected meshes in an organized way.
- **Scene saving:** A way to save the scene settings you’ve inputted (loaded file, skybox, etc.) and reopen them later.
- **UV on top of texture:** When someone selects a texture from the UI, a window will open with the texture and the mesh UVs on top (again, like in Blender where you edit your UVs).
- **Other:** There are other options that I can't think of right now.
