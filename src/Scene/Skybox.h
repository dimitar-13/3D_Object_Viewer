#pragma once
#include "pch.h"
#include "gpu_side/VertexAttributeObject.h"
#include "gpu_side/Texture.h"
#include "gpu_side/OpenGLBuffer.h"
#include "Helpers/TextureHelpers.h"
namespace OBJ_Viewer
{	
    /**
     * @brief Enum representing the index of a skybox face.
     *
     * This enum provides named indices for each face of a skybox, allowing you to refer to skybox faces by name
     * rather than by a numeric index. The enum values correspond to the six faces of a cubemap texture.
     *
     */
	enum SkyboxFace_
	{
		SkyboxFace_kRight  =  0,
		SkyboxFace_kLeft   =  1,
        SkyboxFace_kTop    =  2,
        SkyboxFace_kBottom =  3,
        SkyboxFace_kFront  =  4,
        SkyboxFace_kBack   =  5
	};

    /**
     * @brief A class for creating and managing a skybox.
     *
     * This class handles the creation and management of a skybox, including its textures and vertex attribute object.
     * It uses a texture array to store the faces of the skybox and implements Pixel Buffer Objects (PBO) for efficient
     * texture swapping.
     *
     * Key features include:
     * - Creation of a skybox cubemap texture for rendering.
     * - Management of skybox face textures with functionality to swap them.
     * - Efficient texture updates using PBO.
     * - Automatic cleanup of the PBO and skybox cubemap texture upon deletion.
     *
     * Class dependencies:
     * The class uses Texture class,Vertex attribute object and OpenGLBuffer classes as dependencies.
     * 
     * The class facilitates rendering a skybox in a 3D scene and supports dynamic updates to the skybox textures.
     *
     */
	class Skybox {
	public:
		static constexpr uint8_t SKYBOX_FACE_COUNT = 6; ///<Constant represents the skybox face count.
        /**
         * @brief Creates and manages skybox-related resources.
         *
         * This constructor initializes the skybox with the provided textures.
         * It expects an array of valid textures that have already been loaded into memory.
         * All textures must have the same dimensions to ensure correct rendering.
         *
         * The constructor performs the following operations:
         * - Creates a cubemap texture and initializes a pixel buffer object (PBO) array and a texture array for each texture.
         * The PBO are created using 'OpenGLBuffer' class and that class manages the resources.
         * - Constructs a normalized device coordinate (NDC) cube, which is used to render the skybox.
         *
         * @param textures An array of `TexturePixelReader` objects, each representing a face of the skybox.
         */
		Skybox(const std::array<TexturePixelReader,Skybox::SKYBOX_FACE_COUNT>& textures);
        /**
         * @brief Destroys the created GPU-based resource/cubemap.
         */
		~Skybox();
        /**
         * @brief Binds the cubemap texture for rendering.
         *
         * This method binds the cubemap texture associated with the skybox to the current OpenGL context.
         * It ensures that the skybox texture is active and ready for rendering operations.
         *
         * @note This function should be called before drawing and after setting the 
         *       active texture the skybox to ensure that the correct texture
         *       is used for rendering.
         * 
         * @example
         * ```cpp
         * glActiveTexture(GL_TEXTURE1);
         * skybox.BindSkyboxTexture();
         * 
         * //Do the rendering.
         * ```
         */
		void BindSkyboxTexture()const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapHandle); }
        /**
         * @brief Swaps two skybox faces.
         *
         * This method swaps the textures of two faces of the cubemap using a Pixel Buffer Object (PBO).
         * The swap of cubemap textures is more of a read from  the PBO.
         * It also swaps the associated PBOs and texture arrays.
         *
         * If the `toBeSwapped` face is the same as `swappedWith`, the method will return without performing any swap.
         *
         * @param toBeSwapped The enum value representing the face that is to be swapped.
         * @param swappedWith The enum value representing the face that will be swapped with `toBeSwapped`.
         */
		void SwapSkyboxFaceTextures(SkyboxFace_ toBeSwapped, SkyboxFace_ swappedWith);
        /**
         * @brief Returns readonly access to the skybox vertex attribute object.
         * 
         * This is used for rendering the skybox.
         * 
         * @returns Readonly ref to skybox vertex attribute object.
         */
		const VertexAttributeObject& GetSkyboxVAO()const { return *m_skyboxVAO; }
        /**
         * @brief Returns a read only reference to the skybox texture array.
         *
         * This is mainly used in the UI to display the textures.
         * 
         * @returns Readonly ref access to the skybox texture array.
         */
		const std::array<std::shared_ptr<Texture>, SKYBOX_FACE_COUNT>&  GetSkyboxFaceTextures()const { return m_faceTextures; }
    private:
		std::array<std::shared_ptr<Texture>, SKYBOX_FACE_COUNT> m_faceTextures;/// < 'Texture' array representing OpenGL 2D texture.Used for showing in the UI.
		std::array<std::unique_ptr<OpenGLBuffer>, SKYBOX_FACE_COUNT> m_PixelBuffers; /// < 'OpenGLBuffer' array representing the pixel buffer object.
		GLuint m_cubeMapHandle = 0;/// < GPU handle/ID of the cubemap texture resource.
		std::unique_ptr<VertexAttributeObject> m_skyboxVAO; /// < Normalize device coordinates vertex attribute object for rendering the cubemap on.
		Size2D m_CubeMapTextSize; /// < Size of a individual cubemap face texture. Used mainly in texture swapping.
		TextureFormat_ m_format; /// < Pixel format of a individual cubemap face texture. Used mainly in texture swapping. 
	};

}