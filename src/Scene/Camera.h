#pragma once
#include "pch.h"
#include "Core/WindowEvents.h"
#include "Controls/InputHandler.h"
#include "Core/WindowHandler.h"
#include "Core/AppEvent.h"
namespace OBJ_Viewer
{
    /**
     * @brief Named structure representing EulerAngles.
     * 
     * This structure is nothing specific is just named structure representing 2 angles.
     */
	struct EulerAngles {
		float yawAngle;     ///< Euler yaw angle describes rotations around the UP/Down axis.
		float pitchAngle;   ///< Euler pitch angle describes rotations around the Front/Back and Left/Right axis.

		EulerAngles& operator+=(const EulerAngles& other) {
			this->yawAngle += other.yawAngle;
			this->pitchAngle += other.pitchAngle;
			return *this;
		}
		EulerAngles& operator-(const EulerAngles& other)
		{
			this->yawAngle -= other.yawAngle;
			this->pitchAngle -= other.pitchAngle;
			return *this;
		}
	};

    /**
     * @brief Helper class for calculating and managing Euler angles.
     *
     * This class provides utilities for working with Euler angles, specifically adjusting angles, and constraining them
     * within valid bounds[-360;360]. Euler angles typically represent rotations in 3D space along the pitch,
     * yaw, and roll axes.
     *
     * The class initializes the pitch angle to 0.0f and the yaw angle to 90.0f by default.
     *
     * @note The class ensures that Euler angles are kept within the range of -360 to 360 degrees.
     */
	class EulerAngleHelper {
	public:
        /**
         * @brief Constructor that initializes default Euler angles.
         *
         * Initializes the pitch angle to 0.0f and the yaw angle to 90.0f.
         */
        EulerAngleHelper()
        {
            m_EulerAngles.pitchAngle = 0.0f;
            m_EulerAngles.yawAngle = 90.0f;
        }
        /**
         * @brief Retrieves the current Euler angles.
         *
         * @return The current Euler angles stored in the helper.
         */
        EulerAngles GetEulerAngles()const { return m_EulerAngles; }
        void SetEulerAngles(const EulerAngles& new_Euler_angles) { m_EulerAngles = new_Euler_angles; }
        /**
         * @brief A static cast from 1 data type to another since.
         *
         * Since 'Position2D' is a double and EulerAngles are float this function just do a simple static cast and 
         * creates a 'EulerAngles' from the provided position_to_convert.
         * 
         * @param position_to_convert The 2D position to be converted.
         * @return The staticly casted to EulerAngles position.
         */
        static EulerAngles ConvertMousePositionToAngles(Position2D position_to_convert);
        /**
         * @brief Increases the current Euler angles by the specified amounts.
         *
         * Adds the provided Euler angles to the current angles and then constrains them
         * within the valid range using the 'ConstrainAngles()'.
         *
         * @param angles The Euler angles to add.
         */
        void IncreaseEulerAngles(const EulerAngles& angles) { m_EulerAngles += angles; ConstrainAngles(); }
    private:
        /**
         * @brief Checks if an angle is within the valid range of -360 to 360 degrees.
         *
         * @param angle The angle to check.
         * @return True if the angle is within bounds, false otherwise.
         */
        bool IsAngleWithinBounds(float angle) { return angle >= -360.0f && angle <= 360.0f; }
        /**
         * @brief Constrains the Euler angles within the range of -360 to 360 degrees.
         *
         * Ensures that the pitch, yaw, and roll angles do not exceed the valid range.
         */
		void ConstrainAngles();
	private:
        EulerAngles m_EulerAngles {}; ///< Stores the current Euler angles.
	};

    /**
     * @brief Class for managing and controlling a camera in a 3D environment.
     *
     * This class provides functionality for both defining and controlling a camera.
     * It is designed to handle camera operations and update the camera's view and projection matrices based on user input.
     *
     * The `Camera` class is divided into two main parts:
     *
     * 1. **Camera Definition**:
     *    - Provides methods to retrieve various camera-related matrices, including:
     *      - `GetViewProjMatrix()`: Returns the combined view and projection matrix.
     *      - `GetViewProjNoTranslation()`: Returns the view-projection matrix without translation.
     *      - `GetViewAndProjectionSeparate(glm::mat4& pView, glm::mat4& pProj)`: Separately retrieves the view and projection matrices.
     *    - Provides access to the camera's projection type and world position.
     *
     * 2. **Camera Control**:
     *    - Implements the `Listener` interface and integrates with the `InputHandler` class.
     *    - Listens for user input events and updates the camera's position and orientation accordingly.
     *    - The camera movement is in blender style app movement where you use mouse to rotate around a pivot point.
     *
     * The class is designed to handle both the configuration of the camera (such as its view and projection) and its real-time adjustments based on user interactions.
     *
     * @note Ensure that the `InputHandler` and relevant input events are properly configured for the camera control to function correctly.
     * @note The class is not separated in 2 classes because this wont prove this much of a benefit for now. In future it can be 
     * camera + camera controller classes.
     * @note Because the camera is a event listener the class that uses the camera must add it as an listener to the Application event listeners
     * 
     */
	class Camera : public Listener
	{
	public:
        /**
         * @brief Initializes the camera and sets up the view and projection matrices.
         *
         * This constructor initializes the camera with the provided zoom level and input handler. It also sets up
         * the view and projection matrices based on the given `SceneViewport` and the specified camera zoom.
         *
         * @param application_inputHandlerRef A reference to the `InputHandler` used for processing user inputs.
         * @param kApplicationViewportManagerRef A reference to the `SceneViewport` that defines the viewport size and aspect ratio.
         *
         */
		Camera(InputHandler& application_inputHandlerRef,const SceneViewport& kApplicationViewportManagerRef);
        /**
         * @brief Retrieves both the view and projection matrices separately.
         *
         * This method assigns the current view matrix and projection matrix to the provided reference variables.
         *
         * @param pView A reference to a `glm::mat4` where the view matrix will be stored.
         * @param pProj A reference to a `glm::mat4` where the projection matrix will be stored.
         *
         * @note Ensure that the reference variables provided are properly initialized before passing them to this method.
         *
         * @example
         * glm::mat4 viewMatrix;
         * glm::mat4 projectionMatrix;
         * camera.GetViewAndProjectionSeparate(viewMatrix, projectionMatrix);
         * // Now viewMatrix and projectionMatrix contain the current view and projection matrices respectively.
         */
		void GetViewAndProjectionSeparate(glm::mat4& pView, glm::mat4& pProj)const { pView = m_ViewMatrix; pProj = m_ProjectionMatrix; }
        /**
         * @brief Returns the product of the projection*view matrix multiplication.
         *
         * @note A dumb note but its called 'ViewProj' and not 'ProjView' because matrix multiplication is from right to left.
         * @returns 4x4 matrix that is the product of projection*view matrix multiplication.
         */
		glm::mat4 GetViewProjMatrix()const { return m_ProjectionMatrix * m_ViewMatrix; }
        /**
         * @brief Returns the product of the projection*view(without translation) matrix multiplication.
         *
         * @note A dumb note but its called 'ViewProj' and not 'ProjView' because matrix multiplication is from right to left.
         * 
         * @returns 4x4 matrix that is the product of projection*view(without translation) matrix multiplication.
         */
		glm::mat4 GetViewProjNoTranslation()const { return m_ProjectionMatrix* glm::mat4(glm::mat3(m_ViewMatrix)); }
        /**
         * @brief Returns the camera position 
         * 
         * @returns The camera position in world space.
         */
		glm::vec3 GetCameraPos()const { return this->m_CurrentCameraPosition; }
        /**
         * @brief Returns true if projection is perspective.
         *
         * @returns Returns true if projection is perspective and false if its orthographic.
         */
        bool IsCameraProjectionPerspective()const { return m_IsProjectionPerspective; }
	private:
        /**
         * @brief Utility function for understanding if the Y vector is flipped.
         * 
         * This is more of a named action type of function instead of using comments to explain it, it uses function name.
         * Internally the function uses trigonometry to understand if the Y vector is flipped meaning we rotated more than 180 degrees
         * around the pivot.
         * 
         * @return Returns true if the Y dir vector should be flipped.
         */
        bool IsCameraYVectorFlipped(float pitch_angle);
        /**
         * @brief Utility function for calculating the aspect ratio of a screen. 
         * 
         * This function for calculates the aspect ratio of a screen where the aspect ratio is: 'width/height'.
         * 
         * @returns The aspect ration calculated by 'size'.
         */
        float GetAspectRatio(Size2D size) {return static_cast<float>(size.width) / static_cast<float>(size.height); }
        /**
         * @brief Calculates the view matrix based on the camera's position, origin, and zoom.
         *
         * This function computes the view matrix for the camera, which transforms world coordinates to camera coordinates.
         * It uses the current camera position, the origin of the camera view, and the zoom level to generate this matrix.
         *
         * Additionally, the function checks if the Y-axis is flipped using `IsCameraYVectorFlipped()`. If the Y-axis is flipped,
         * the function adjusts the view matrix calculation accordingly to ensure that the camera's orientation and projection
         * are correct.
         *
         * The new matrix is stored in 'm_viewMatrix'.
         * 
         */
		void CalculateViewMatrix();
        /**
         * @brief Calculates the camera position based on Euler angles.
         *
         * In this 3D modeling software-style camera, Euler angles are used to determine the camera's position, rather than its rotation
         * as in FPS-style cameras. This method computes the camera's position vector using these angles.
         *
         * After calculating the position, this method invokes `CalculateViewMatrix()` to update the view matrix with the new camera position.
         */
		void CalculatePositionVector();
        /**
         * @brief Calculates the orthographic projection matrix.
         *
         * This method computes the orthographic projection matrix based on the current window size and zoom level.
         * The orthographic projection matrix is scaled according to the zoom factor to simulate zooming effects.
         *
         * The `windowSize` parameter defines the dimensions of the viewport or window, which is used in the calculation of
         * the projection matrix. This method adjusts the orthographic projection to ensure that the rendered scene
         * remains correctly scaled and positioned as the zoom level changes.
         *
         * @param windowSize The dimensions of the window or viewport, used to calculate the projection matrix.
         *
         */
        void CalculateOthoProjection(Size2D windowSize);
        /**
         * @brief Recalculates the current projection matrix based on the window size.
         *
         * This method recalculates the projection matrix depending on the type of projection currently used (orthographic or perspective).
         * If the provided `windowSize` is `{0, 0}`, it is assumed that the window is minimized, and the projection matrix will not be recalculated.
         * This function is mainly called on viewport resizes and its called by the 'onViewportChanged()' event.
         * 
         * - For orthographic projection, the method calls `CalculateOthoProjection()` to adjust the projection matrix.
         * - For perspective projection, the method computes the projection matrix using the given window size.
         *
         * @param windowSize The dimensions of the window (width and height). Defaults to `{0, 0}` if not provided.
         *
         * @note Ensure the `windowSize` is correctly set before calling this method to avoid unintended behavior when the window is minimized.
         */
        void RecalculateProjection(Size2D windowSize = { 0,0 });
        /**
         * @brief Calculates the mouse offset from screen space to world space.
         *
         * This function computes the 3D world space offset vector based on the difference between two 2D screen space positions.
         *
         * **Implementation Details:**
         * 1. Calculates the screen space offset as the difference between the `current_mouse_position` and `previous_mouse_position`.
         * 2. Retrieves the camera matrix without any transformations applied.
         * 3. Inverts the camera matrix by transposing it (assuming it's orthogonal).
         * 4. Converts the 2D screen space offset vector to a 3D vector with a `z` value of `0`.
         * 5. Multiplies this 3D vector by the inverse camera matrix to obtain the corresponding world space vector.
         *
         * @param previous_mouse_position The 2D screen space coordinates of the previous mouse position.
         * @param current_mouse_position The 2D screen space coordinates of the current mouse position.
         *
         * @return A `glm::vec3` representing the mouse offset in world space.
         *
         */
        glm::vec3 CalculateMouseOffsetInWorldSpace(const Position2D& previous_mouse_position, const Position2D& current_mouse_position);

#pragma region On Event
        /**
         * @brief Handles events by delegating them to specific event handler functions.
         *
         * This function is called when an event occurs. It processes the event by dispatching it to the appropriate
         * handler function based on the type of event. This delegation allows for modular handling of various event types
         * and ensures that each type of event is processed by the corresponding handler function.
         *
         * The `OnEvent` function is overridden from the `Listener` interface, which is designed to handle different kinds
         * of events, such as input events, system events, or custom application events.
         *
         * @param e The event object that contains information about the event. The function uses this object to determine
         *          the event type and dispatch it to the appropriate handler.
         */
        void OnEvent(Event& e) override;
        /**
         * @brief Handles scroll change events to update the camera's zoom level.
         *
         * This method is triggered when a scroll event occurs. It calculates the new zoom level based on the event data
         * and updates the camera's zoom accordingly. Depending on the camera type, it also updates the camera and projection matrices:
         *
         * - For perspective cameras, it adjusts the projection matrix to reflect the new zoom level.
         * - For orthographic cameras, it directly updates the projection matrix to accommodate the zoom change.
         *
         * @param e A reference to a `ScrollPositionChanged` object that contains information about the scroll event.
         *
         * @note If the camera is orthographic, the projection matrix itself is modified to reflect the zoom change.
         */
		void onScrollChanged(ScrollPositionChanged& e);
        /**
         * @brief Handles the mouse position change event.
         *
         * This method processes updates to the mouse position whenever a change event occurs. It interacts with the application's input handler
         * to determine if mouse buttons are pressed and updates the internal state accordingly.
         *
         * **Functionality:**
         * - Checks if the mouse button is pressed using the `m_applicationInputHandler`.
         * - If the button is pressed, it saves the current mouse position to the appropriate internal variable.
         * - If the mouse button is held down, it calculates either a new position for the camera or updates the shift/pan position based on the movement.
         *
         *
         * @param e The `MousePositionEvent` containing details about the current mouse position and any relevant input states.
         *
         */
		void onMousePositionChanged(MousePositionEvent& e);
        /**
         * @brief Method of handling viewport changes.
         *
         * Changes the current projection(perspective or orthographic) using the new viewport size.
         * @param e A reference to a `SceneViewportResizeEvent` object that contains information about the viewport event.
         */
		void onViewportChanged(SceneViewportResizeEvent& e);
        /**
         * @brief Handles key press events to manage camera actions.
         *
         * This method processes key press events, specifically designed for actions related to camera control.
         * Currently, the implemented functionality includes resetting the camera pivot. When a key press event is detected,
         * this method checks the key and performs the corresponding action, such as resetting the camera's pivot point to
         * its default position. The keys are defined in this enum AppKeyBinding_.
         *
         * @param e The key press event containing information about the pressed key and other related data.
         *
         * @note Future implementations may extend this method to handle additional camera control actions triggered by different keys.
         */
		void onKeyPressedEvent(KeyboardKeyEvent& e);
        /**
         * @brief Handles the on projection change event.
         * 
         * On projection changed recalculates the projection matrix based of if its ortho or perspective and also changes
         * the 'm_isProjectionPerspective' to the current projection.
         * 
         * @param 'e' the event data for the current projection.
         */
		void OnProjectionModeChanged(EventCameraProjectionChanged& e);
#pragma endregion
	private:
		float m_CurrentCameraZoomAmount;                    ///< Current camera zoom amount.
		glm::mat4 m_ProjectionMatrix;                       ///< Current camera projection matrix(it can be both perspective or orthographic.)
		glm::mat4 m_ViewMatrix;                             ///< Current camera view matrix.
		EulerAngleHelper m_EulerAngleHelper;                ///< Helper class for calculating camera mouse offsets.
		glm::vec3 m_CurrentCameraPosition;                  ///< The current camera world position.
		InputHandler& m_ApplicationInputHandlerRef;         ///< Reference to the application input handler.
        const SceneViewport& m_ApplicationSceneViewportRef; ///< Const Reference to the application scene viewport manager.
		glm::vec3 m_CameraLookAtPivot;                      ///< Current camera look at pivot point(or world center).        
		Position2D m_lastMouseMovementPosition{};           ///< Last mouse position stored after a left mouse button was held or pressed.
        Position2D m_lastMouseShiftPosition{};              ///< Last mouse position stored after a right mouse button was held or pressed.
		bool m_IsProjectionPerspective = true;              ///< Is the camera projection perspective flag.
	};
}

