#pragma once
#include "pch.h"
#include "Texture.h"
#include "Core/CommonAppData.h"

namespace OBJ_Viewer {

	enum FramebufferAttachmentsFlags_
	{
		FramebufferAttachmentsFlags_kColorAttachment = 1,
		FramebufferAttachmentsFlags_kStencilAttachment = 2,
		//FRAMEBUFFER_NORMAL_ATTACHMENT = 4,
		//FRAMEBUFFER_SPECULAR_ATTACHMENT = 8
	};
    enum FramebufferBitMaskFlags_
    {
        FramebufferBitMaskFlags_kColorBufferBit = GL_COLOR_BUFFER_BIT,
        FramebufferBitMaskFlags_kDepthBufferBit = GL_DEPTH_BUFFER_BIT
    };
    /**
     * @brief Class for creating and managing a framebuffer.
     *
     * The `Framebuffer` class manages a GPU-based framebuffer resource. It provides functionality
     * for creating, binding, unbinding, resizing, and copying framebuffer content. It also offers
     * access to the framebuffer's OpenGL handle and associated texture.
     *
     * This class supports both single-sample and multi-sample framebuffers, with configurable sample counts.
     */
	class Framebuffer
	{
	public:
		using pixel_component = unsigned char;
        /**
         * @brief Constructs a Framebuffer based on the given configuration.
         *
         * This constructor initializes a GPU-based framebuffer resource. Depending on the provided parameters,
         * the framebuffer can be either single-sampled or multi-sampled.
         * You can call 'isFramebufferValid' to check if the creation of the framebuffer succeeded.
         * 
         * @param size The initial size of the framebuffer in pixels.
         * @param attachmentFlags Currently not used, reserved for specifying framebuffer attachment types.
         * @param isMultiSampleBuffer Optional parameter; if set to `true`, the framebuffer will be multi-sampled.
         *                            Default is `false`, which means the framebuffer is single-sampled.
         * @param sampleCount Optional parameter specifying the number of samples for a multi-sampled framebuffer.
         *                    Default is 4. This parameter is ignored if `isMultiSampleBuffer` is `false`.
         */
		Framebuffer(Size2D size, FramebufferAttachmentsFlags_ attachmentFlags,bool isMultiSampleBuffer = false,uint8_t sampleCount = 4);
        /**
         * @brief Returns the OpenGL handle (ID) of the framebuffer.
         *
         * This method returns the OpenGL handle (also referred to as the ID) associated with this framebuffer.
         * The handle can be used in OpenGL calls to reference this specific framebuffer.
         *
         * @return The OpenGL handle (GLuint) of the framebuffer.
         */
		GLuint GetFramebufferHandle()const { return this->m_framebuffer; }
        /**
         * @brief Retrieves the texture associated with the framebuffer.
         *
         * This method returns a reference to the `Texture` object that is associated with the framebuffer.
         * The texture is used to access the content of the framebuffer, such as for rendering or reading pixel data.
         *
         * @return A reference to the `Texture` object associated with the framebuffer.
        */
		Texture& GetFramebufferTexture()const { return *this->m_texture; }
        /**
         * @brief Binds this framebuffer as the active framebuffer.
         *
         * This method binds the current framebuffer, making it the active framebuffer in the OpenGL state machine.
         * Subsequent framebuffer operations (e.g., reading or writing) will target this framebuffer until another
         * framebuffer is bound.
         */
		void BindFramebuffer()const { glBindFramebuffer(GL_FRAMEBUFFER, this->m_framebuffer); }
        /**
         * @brief Binds the default framebuffer (screen framebuffer).
         *
         * This method binds the default framebuffer, typically the screen framebuffer, making it the active
         * framebuffer in the OpenGL state machine. Although functionally equivalent to `UnbindFramebuffer`,
         * this method is named to provide clearer intent in code, indicating a return to the default framebuffer.
         *
         * Internally, it unbinds the currently bound framebuffer by binding framebuffer ID 0.
         */
		static void BindDefaultFramebuffer() {  glBindFramebuffer(GL_FRAMEBUFFER, 0); }
        /**
         * @brief Unbinds the current framebuffer.
         *
         * This method unbinds the currently bound framebuffer by binding the default framebuffer (ID 0).
         * After calling this method, the OpenGL state machine will use the default framebuffer for any
         * subsequent rendering operations until another framebuffer is bound.
         */
		void UnbindFramebuffer()const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
        /**
         * @brief Checks if the framebuffer is complete and valid.
         *
         * This method determines whether the framebuffer is valid based on the OpenGL completeness rules from the wiki.
         * A valid framebuffer is one that satisfies all the completeness rules outlined by OpenGL, including:
         *
         * - **Attachment Completeness**: Each attachment point must be complete according to its rules. Empty attachments are considered complete by default, while attached images must meet certain criteria:
         *   - The source object for the image must exist and match the type it was attached with.
         *   - The image must have non-zero width and height, and these dimensions must be within the maximum framebuffer width and height limits.
         *   - For 3D or array textures, the layer index must be less than the texture's depth and within the maximum number of framebuffer layers.
         *   - The number of samples must be less than the maximum number of framebuffer samples allowed.
         *   - The image's format must be compatible with the attachment point's requirements.
         *
         * - **Framebuffer Completeness**: The framebuffer as a whole must be complete:
         *   - If the default framebuffer (ID 0) is bound, it is always considered complete if it exists; otherwise, it is considered undefined.
         *   - All attachments must be complete. If any attachment is incomplete, the framebuffer is considered incomplete.
         *   - There must be at least one image attached to the framebuffer, or if OpenGL 4.3 or ARB_framebuffer_no_attachments is supported, default framebuffer width and height must be non-zero.
         *   - Each draw buffer must either have an attached image or be set to `GL_NONE`.
         *   - If a read buffer is set, it must have an attached image.
         *   - All attached images must use the same number of multi-sample samples and fixed sample layout settings.
         *   - If a layered image is attached to one attachment, all attachments must be layered images.
         *
         * @return `true` if the framebuffer is complete and valid according to OpenGL rules; otherwise, `false`.
         */
		bool isFramebufferValid()const;
        /**
        * @brief Returns the current size of the framebuffer.
        */
		Size2D GetFramebufferSize()const { return m_framebufferSize; }
        /**
         * @brief Resizes the framebuffer using the newSize.
         * 
         * Resizes all of the framebuffer texture attachments based on the new size.
         * Also sets the 'm_framebufferSize' to the new size.
         * If the new size is the same the framebuffer wont resize.
         * 
         * @param newSize New size of the framebuffer
        */
		void ResizeFramebuffer(Size2D newSize);
        /**
         * @brief Copies content from another framebuffer into this framebuffer based on the provided mask.
         *
         * This method copies the content from the specified source framebuffer (`framebufferToCopy`) into the current
         * framebuffer. The data copied can be controlled using the provided bitmask (`mask`). This bitmask determines
         * which types of data are copied (e.g., color, depth, stencil) according to the options defined in the
         * `FramebufferBitMaskFlags_` enumeration. By default, the mask is set to copy only color buffer information.
         *
         * @param framebufferToCopy The source/read framebuffer from which data will be copied.
         * @param mask A bitmask specifying which parts of the framebuffer data to copy. The default value is
         *             `FramebufferBitMaskFlags_kColorBufferBit`, which copies only the color buffer data.
         */
		void CopyFramebufferContent(const Framebuffer& framebufferToCopy, FramebufferBitMaskFlags_ mask = FramebufferBitMaskFlags_kColorBufferBit);
        /**
         * @brief Retrieves the pixel data from the framebuffer.
         *
         * This method reads the pixel data from the current framebuffer and returns it as a vector of pixel components.
         * The data is retrieved in the format specified by `retrieveFormat`. This method is useful for obtaining the
         * current content of the framebuffer for operations such as screenshot capturing or pixel-based analysis.
         *
         * @param retrieveFormat The format in which to retrieve the pixel data (RGBA RGB etc.). This should match the format of the
         *        framebuffer's texture or the desired format for the data.
         *
         * @return A `std::vector` containing the pixel data, where each element represents a pixel component.
         */
		std::vector<pixel_component> GetFramebufferPixels(TextureFormat_ retrieveFormat)const;
        /**
         * @brief Destroys the framebuffer and releases associated resources.
         *
         * This destructor cleans up and releases any resources associated with the framebuffer, including
         * the OpenGL framebuffer object and any related textures. It ensures that there are no memory leaks
         * or dangling references to GPU resources when the framebuffer object is no longer needed.
         */
		~Framebuffer();
	private:
		GLuint m_framebuffer;
		GLuint m_readBuffer;
		Size2D m_framebufferSize;
		bool m_isMultiSample = false;
		uint8_t m_sampleCount = 4;
		std::unique_ptr<Texture> m_texture;
	};
}

