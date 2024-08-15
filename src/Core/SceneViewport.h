#pragma once
#include "pch.h"
namespace OBJ_Viewer {

    /**
     * @brief Defines an OpenGL viewport.
     *
     * This structure specifies the viewport parameters used in OpenGL to define the rectangular area of the window or
     * framebuffer where rendering will occur. It includes the position of the viewport's origin and its dimensions.
     *
     * The viewport is defined by its bottom-left corner (`x`, `y`) and its size top-right (`width`, `height`). These parameters are used
     * to set up the rendering area, ensuring that the rendered image is correctly positioned and sized within the window or framebuffer.
     *
     * The structure includes comparison operators to check for equality or inequality between two viewports.
     */
	struct Viewport
	{
		int x;        ///< The x-coordinate of the bottom-left corner of the viewport.
        int y;        ///< The y-coordinate of the bottom-left corner of the viewport.
        int width;    ///< The width of the viewport.
        int height;   ///< The height of the viewport.

		bool operator ==(const Viewport& other)const
		{
			return this->x == other.x         &&
				   this->y == other.y		  &&
				   this->width == other.width &&
				   this->height == other.height;
		}
		bool operator !=(const Viewport& other)const
		{
			return !Viewport::operator==(other);
		}
	};

    /**
     * @brief Manages the viewport for rendering in the scene.
     *
     * The `SceneViewport` class provides functionality to manage and update the viewport used for rendering. It stores
     * viewport dimensions and position, and calculates a transformation matrix based on these parameters.
     *
     * The viewport is defined by its position (x, y) and size (width, height). The class ensures that viewport values are valid
     * and updates the corresponding viewport matrix when changes occur.
     *
     * The viewport matrix is a 3x3 matrix used for transformations based on the viewport size. It is recalculated whenever
     * the viewport parameters are updated.
     */
	class SceneViewport
	{
	public:
        /**
         * @brief Constructs a `SceneViewport` with the given initial viewport parameters.
         *
         * The constructor initializes the viewport and calculates the initial viewport matrix.
         *
         * @param viewport The initial viewport parameters (position and size).
         */
		SceneViewport(const Viewport& viewport):m_viewport(viewport)
		{ RecalculateViewportMatrix();}
        /**
         * @brief Gets the offset of the viewport (top-left corner).
         *
         * Returns the (x, y) position of the viewport.
         *
         * @return A `Size2D` structure containing the x and y coordinates of the viewport offset.
         */
		Size2D GetViewportOffset()const { return Size2D{ m_viewport.x,m_viewport.y }; }
        /**
         * @brief Gets the size of the viewport.
         *
         * Returns the width and height of the viewport.
         *
         * @return A `Size2D` structure containing the width and height of the viewport.
         */
		Size2D GetViewportSize()const { return Size2D{ m_viewport.width,m_viewport.height }; }
        /**
         * @brief Gets a non-const reference to the viewport transformation matrix.
         *
         * Provides access to the matrix used for viewport transformations, allowing modifications.
         *
         * @return A reference to the `glm::mat3` matrix used for viewport transformations.
         */
		glm::mat3& GetViewportMatrix() { return m_viewPortMatrix; }
        /**
         * @brief Gets a const reference to the viewport transformation matrix.
         *
         * Provides access to the matrix used for viewport transformations without allowing modifications.
         *
         * @return A const reference to the `glm::mat3` matrix used for viewport transformations.
         */
		const glm::mat3& GetViewportMatrix()const { return m_viewPortMatrix; }
        /**
         * @brief Gets a const reference to the viewport struct.
         *
         * Provides access to the viewport struct.
         *
         * @return A const reference to the viewport struct.
         */
		const Viewport& GetViewport()const { return m_viewport; }
        /**
         * @brief Updates the viewport parameters and recalculates the viewport matrix.
         *
         * Sets new parameters for the viewport and ensures that they are valid. After updating the viewport, recalculates
         * the viewport matrix based on the new dimensions and position.
         *
         * @param newViewport The new viewport parameters (position and size).
         *
         * @note The function asserts if any of the viewport parameters are negative.
         */
		void UpdateSceneViewport(const Viewport& newViewport)
		{
			assert(!(newViewport.x < 0 || newViewport.y < 0 || newViewport.width < 0 || newViewport.height < 0));

			m_viewport = newViewport;
			RecalculateViewportMatrix();
		}
	private:
        /**
         * @brief Recalculates the viewport transformation matrix based on current viewport parameters.
         *
         * @note It doesn't recalculate the whole matrix only the x and y path not the Z/depth part.
         * 
         * Updates the viewport matrix to reflect the current size and position of the viewport.
         */
		void RecalculateViewportMatrix()
		{
			m_viewPortMatrix = glm::mat3(1);

			m_viewPortMatrix[0][0] = (float)m_viewport.width / 2.f;
			m_viewPortMatrix[1][1] = (float)m_viewport.height / 2.f;

			m_viewPortMatrix[2][0] = m_viewPortMatrix[0][0];
			m_viewPortMatrix[2][1] = m_viewPortMatrix[1][1];
		}
	private:
		Viewport m_viewport;                       ///< The current viewport parameters (position and size).
		glm::mat3 m_viewPortMatrix = glm::mat3(1); ///< The matrix used for transforming viewport coordinates.
	};
}