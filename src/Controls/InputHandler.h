#pragma once
#include "pch.h"
#include "Controls/KeyboardKeys.h"
#include "Controls/MouseKeys.h"
#include "Core/WindowEvents.h"
namespace OBJ_Viewer
{
	enum KeyState_ {
		KeyState_kUnknown,
		KeyState_kPressed = GLFW_PRESS,
		KeyState_kHeld = GLFW_REPEAT,
		KeyState_kReleased = GLFW_RELEASE,
	};


    /**
     * @brief Manages and polls user input events.
     *
     * This class is responsible for handling and polling input events, including both mouse and keyboard inputs.
     * It listens for input events and maintains the current state of each input.
     *
     * The class works with two other subclasses: `MouseInputStateManager` and `KeyboardInputStateManager`, which are responsible
     * for polling mouse and keyboard inputs, respectively. It provides access to the poll state via the
     * IsKeyboard<state>, IsMouseButton<state>. 
     * 
     * Additionally, the class monitors and tracks the currently focused window.
     */
	class InputHandler : public Listener
	{
    private:
        /**
         * @brief Manages and polls the state of mouse button inputs.
         *
         * This class is responsible for tracking the state of various mouse buttons. It maintains a hash map where each entry
         * associates a mouse button (specified by the `MouseKey_` enum) with its current state (specified by the `KeyState_` enum).
         *
         * The class provides methods to query the current state of a mouse button and to handle state changes when they occur.
         *
         */
        class MouseInputStateManager {
        public:
            /**
             * @brief Retrieves the current state of a specified mouse button.
             *
             * This function returns the current state of a mouse button, indicating whether it is released, pressed, or held.
             *
             * @note GLFW does not natively support detecting the "held" state for mouse buttons. To work around this limitation,
             *       the function uses an internal mechanism to simulate the "held" state. If a button was previously detected as
             *       pressed and is still being polled as pressed, it will be reported as held. This approach provides a practical
             *       solution for tracking the held state as seen in the implementation.
             *
             * @param button The mouse button whose state is to be retrieved, specified by the `MouseKey_` enum.
             * @return The current state of the specified mouse button, represented by the `KeyState_` enum.
             */
            KeyState_ GetButtonState(MouseKey_ button);
            /**
             * @brief Handles a mouse button state change event.
             *
             * This function processes a mouse button state change event by updating the internal state of the mouse button
             * in the poll hash map. It adjusts the button's state based on the information provided in the event.
             *
             * @param e The event object containing details about the mouse button state change, including the button and its new state.
             */
            void onButtonStateChanged(MouseKeyEvent& e);
        private:
            std::unordered_map<MouseKey_, KeyState_> m_mouseKeyHashes = { ///< Mouse button state poll hash map.
                        {MouseKey_kLeftMouseButton, KeyState_kReleased },
                        {MouseKey_kRightMouseButton, KeyState_kReleased },
                        {MouseKey_kScrollWheelButton, KeyState_kReleased },
                        {MouseKey_kX1, KeyState_kReleased },
                        {MouseKey_kX2, KeyState_kReleased },
                        {MouseKey_kAdditionalButton_1, KeyState_kReleased },
                        {MouseKey_kAdditionalButton_2, KeyState_kReleased },
                        {MouseKey_kAdditionalButton_3, KeyState_kReleased }
            };
        };

        /**
         * @brief Manages and polls the state of keyboard keys.
         *
         * This class is responsible for tracking the state of keyboard keys. It maintains a hash map where each entry
         * associates a keyboard key (specified by the `KeyboardKey_` enum) with its current state (specified by the `KeyState_` enum).
         *
         * The class provides methods to update key states based on changes and to retrieve the current state of any key.
         *
         */
        class KeyboardInputStateManager {
        public:
            /**
             * @brief Retrieves the current state of a specified keyboard key.
             *
             * This method returns the state of a given keyboard key, such as whether it is pressed, held, or released.
             *
             * @param key The keyboard key whose state is to be retrieved, specified by the `KeyboardKey_` enum.
             * @return The current state of the specified keyboard key, represented by the `KeyState_` enum.
             */
            KeyState_ GetKeyState(KeyboardKey_ key);
            /**
             * @brief Handles changes in keyboard key state.
             *
             * This method processes events related to keyboard key state changes. It updates the internal state based on the
             * information provided in the `KeyboardKeyEvent`.
             *
             * @param e The event object containing details about the keyboard key state change.
             */
            void onKeyStateChanged(KeyboardKeyEvent& e);
        private:
            std::unordered_map<KeyboardKey_, KeyState_> m_keyHashes = { ///< Keyboard state poll hash map.
            {KeyboardKey_kSpace, KeyState_kReleased },
            {KeyboardKey_kApostrophe, KeyState_kReleased},
            {KeyboardKey_kComma, KeyState_kReleased},
            {KeyboardKey_kMinus, KeyState_kReleased},
            {KeyboardKey_kPeriod, KeyState_kReleased},
            {KeyboardKey_kSlash, KeyState_kReleased},
            {KeyboardKey_k0, KeyState_kReleased},
            {KeyboardKey_k1, KeyState_kReleased},
            {KeyboardKey_k2, KeyState_kReleased},
            {KeyboardKey_k3, KeyState_kReleased},
            {KeyboardKey_k4, KeyState_kReleased},
            {KeyboardKey_k5, KeyState_kReleased},
            {KeyboardKey_k6, KeyState_kReleased},
            {KeyboardKey_k7, KeyState_kReleased},
            {KeyboardKey_k8, KeyState_kReleased},
            {KeyboardKey_k9, KeyState_kReleased},
            {KeyboardKey_kSemicolon, KeyState_kReleased},
            {KeyboardKey_kEqual, KeyState_kReleased},
            {KeyboardKey_kA, KeyState_kReleased},
            {KeyboardKey_kB, KeyState_kReleased},
            {KeyboardKey_kC, KeyState_kReleased},
            {KeyboardKey_kD, KeyState_kReleased},
            {KeyboardKey_kE, KeyState_kReleased},
            {KeyboardKey_kF, KeyState_kReleased},
            {KeyboardKey_kG, KeyState_kReleased},
            {KeyboardKey_kH, KeyState_kReleased},
            {KeyboardKey_kI, KeyState_kReleased},
            {KeyboardKey_kJ, KeyState_kReleased},
            {KeyboardKey_kK, KeyState_kReleased},
            {KeyboardKey_kL, KeyState_kReleased},
            {KeyboardKey_kM, KeyState_kReleased},
            {KeyboardKey_kN, KeyState_kReleased},
            {KeyboardKey_kO, KeyState_kReleased},
            {KeyboardKey_kP, KeyState_kReleased},
            {KeyboardKey_kQ, KeyState_kReleased},
            {KeyboardKey_kR, KeyState_kReleased},
            {KeyboardKey_kS, KeyState_kReleased},
            {KeyboardKey_kT, KeyState_kReleased},
            {KeyboardKey_kU, KeyState_kReleased},
            {KeyboardKey_kV, KeyState_kReleased},
            {KeyboardKey_kW, KeyState_kReleased},
            {KeyboardKey_kX, KeyState_kReleased},
            {KeyboardKey_kY, KeyState_kReleased},
            {KeyboardKey_kZ, KeyState_kReleased},
            {KeyboardKey_kLeftBracket, KeyState_kReleased},
            {KeyboardKey_kBackslash, KeyState_kReleased},
            {KeyboardKey_kRightBracket, KeyState_kReleased},
            {KeyboardKey_kGraveAccent, KeyState_kReleased},
            {KeyboardKey_kWorld1, KeyState_kReleased},
            {KeyboardKey_kWorld2, KeyState_kReleased},
            {KeyboardKey_kEscape, KeyState_kReleased},
            {KeyboardKey_kEnter, KeyState_kReleased},
            {KeyboardKey_kTab, KeyState_kReleased},
            {KeyboardKey_kBackspace, KeyState_kReleased},
            {KeyboardKey_kInsert, KeyState_kReleased},
            {KeyboardKey_kDelete, KeyState_kReleased},
            {KeyboardKey_kRight, KeyState_kReleased},
            {KeyboardKey_kLeft, KeyState_kReleased},
            {KeyboardKey_kDown, KeyState_kReleased},
            {KeyboardKey_kUp, KeyState_kReleased},
            {KeyboardKey_kPageUp, KeyState_kReleased},
            {KeyboardKey_kPageDown, KeyState_kReleased},
            {KeyboardKey_kHome, KeyState_kReleased},
            {KeyboardKey_kEnd, KeyState_kReleased},
            {KeyboardKey_kCapsLock, KeyState_kReleased},
            {KeyboardKey_kScrollLock, KeyState_kReleased},
            {KeyboardKey_kNumLock, KeyState_kReleased},
            {KeyboardKey_kPrintScreen, KeyState_kReleased},
            {KeyboardKey_kPause, KeyState_kReleased},
            {KeyboardKey_kF1, KeyState_kReleased},
            {KeyboardKey_kF2, KeyState_kReleased},
            {KeyboardKey_kF3, KeyState_kReleased},
            {KeyboardKey_kF4, KeyState_kReleased},
            {KeyboardKey_kF5, KeyState_kReleased},
            {KeyboardKey_kF6, KeyState_kReleased},
            {KeyboardKey_kF7, KeyState_kReleased},
            {KeyboardKey_kF8, KeyState_kReleased},
            {KeyboardKey_kF9, KeyState_kReleased},
            {KeyboardKey_kF25, KeyState_kReleased},
            {KeyboardKey_kF10, KeyState_kReleased},
            {KeyboardKey_kF11, KeyState_kReleased},
            {KeyboardKey_kF12, KeyState_kReleased},
            {KeyboardKey_kF13, KeyState_kReleased},
            {KeyboardKey_kF14, KeyState_kReleased},
            {KeyboardKey_kF15, KeyState_kReleased},
            {KeyboardKey_kF16, KeyState_kReleased},
            {KeyboardKey_kF17, KeyState_kReleased},
            {KeyboardKey_kF18, KeyState_kReleased},
            {KeyboardKey_kF19, KeyState_kReleased},
            {KeyboardKey_kF20, KeyState_kReleased},
            {KeyboardKey_kF21, KeyState_kReleased},
            {KeyboardKey_kF22, KeyState_kReleased},
            {KeyboardKey_kF23, KeyState_kReleased},
            {KeyboardKey_kF24, KeyState_kReleased},
            {KeyboardKey_kKp0, KeyState_kReleased},
            {KeyboardKey_kKp1, KeyState_kReleased},
            {KeyboardKey_kKp2, KeyState_kReleased},
            {KeyboardKey_kKp3, KeyState_kReleased},
            {KeyboardKey_kKp4, KeyState_kReleased},
            {KeyboardKey_kKp5, KeyState_kReleased},
            {KeyboardKey_kKp6, KeyState_kReleased},
            {KeyboardKey_kKp7, KeyState_kReleased},
            {KeyboardKey_kKp8, KeyState_kReleased},
            {KeyboardKey_kKp9, KeyState_kReleased},
            {KeyboardKey_kKpDecimal, KeyState_kReleased},
            {KeyboardKey_kKpDivide, KeyState_kReleased},
            {KeyboardKey_kKpMultiply, KeyState_kReleased},
            {KeyboardKey_kKpSubtract, KeyState_kReleased},
            {KeyboardKey_kKpAdd, KeyState_kReleased},
            {KeyboardKey_kKpEnter, KeyState_kReleased},
            {KeyboardKey_kKpEqual, KeyState_kReleased},
            {KeyboardKey_kLeftShift, KeyState_kReleased},
            {KeyboardKey_kLeftControl, KeyState_kReleased},
            {KeyboardKey_kLeftAlt, KeyState_kReleased},
            {KeyboardKey_kLeftSuper, KeyState_kReleased},
            {KeyboardKey_kRightShift, KeyState_kReleased},
            {KeyboardKey_kRightControl, KeyState_kReleased},
            {KeyboardKey_kRightAlt, KeyState_kReleased},
            {KeyboardKey_kRightSuper, KeyState_kReleased},
            {KeyboardKey_kMenu, KeyState_kReleased} };
        };

	public:
        InputHandler() = default;
        InputHandler(const InputHandler& copy) = delete;
        InputHandler(InputHandler&& move) = delete;
        InputHandler& operator=(const InputHandler& other) = delete;
        InputHandler& operator=(InputHandler&& other) = delete;

        /**
         * @brief Checks if a specific keyboard key is currently pressed.
         *
         * This function checks the current state of a specified keyboard key and returns `true` if the key is in the `Pressed` state.
         *
         * @param key The keyboard key to check, specified by the `KeyboardKey_` enum.
         * @return `true` if the key is pressed, `false` otherwise.
         */
		bool IsKeyboardPressed(KeyboardKey_ key) { return m_keyboardInputManager.GetKeyState(key) == KeyState_kPressed; }
        /**
         * @brief Checks if a specific keyboard key is currently being held down.
         *
         * This function checks the current state of a specified keyboard key and returns `true` if the key is in the `Held` state.
         *
         * @param key The keyboard key to check, specified by the `KeyboardKey_` enum.
         * @return `true` if the key is held down, `false` otherwise.
         */
		bool IsKeyboardKeyHeld(KeyboardKey_ key) { return m_keyboardInputManager.GetKeyState(key) == KeyState_kHeld; }
        /**
         * @brief Checks if a specific mouse button is currently pressed.
         *
         * This function checks the current state of a specified mouse button and returns `true` if the button is in the `Pressed` state.
         *
         * @param mouseButton The mouse button to check, specified by the `MouseKey_` enum.
         * @return `true` if the button is pressed, `false` otherwise.
         */
		bool IsMouseButtonPressed(MouseKey_ mouseButton) { return m_mouseInputManager.GetButtonState(mouseButton) == KeyState_kPressed; }
        /**
         * @brief Checks if a specific mouse button is currently being held down.
         *
         * This function checks the current state of a specified mouse button and returns `true` if the button is in the `Held` state.
         *
         * @param mouseButton The mouse button to check, specified by the `MouseKey_` enum.
         * @return `true` if the button is held down, `false` otherwise.
         */
		bool IsMouseButtonHeld(MouseKey_ mouseButton) { return m_mouseInputManager.GetButtonState(mouseButton) == KeyState_kHeld; }
        /**
         * @brief Retrieves the name of the currently focused window.
         *
         * This function returns a pointer to a C-style string containing the name of the window that currently has focus.
         *
         * @return A `const char*` pointing to the name of the currently focused window.
         */
		const char* GetCurrentlyFocusedWindow()const { return m_currentlyFocusedWindowName; }
        /**
         * @brief Sets the name of the currently focused window.
         *
         * This function updates the name of the window that currently has focus.
         *
         * @param windowName A `const char*` containing the name of the window to set as currently focused.
         */
		void SetCurrentlyFocusedWindow(const char* windowName){ m_currentlyFocusedWindowName = windowName; }
	private:
		MouseInputStateManager m_mouseInputManager;       ///< Polls the mouse button states.
		KeyboardInputStateManager m_keyboardInputManager; ///< Polls the keyboard key states.

		const char* m_currentlyFocusedWindowName = APP_FOCUS_REGIONS::kUI_SceneWindowName; ///< Currently focused window poll.

        /**
         * @brief Handles input events by delegating them to the appropriate helper classes.
         *
         * This function, inherited from the `Listener` interface, processes incoming events related to mouse and keyboard inputs.
         * It determines the type of event (mouse or keyboard) and forwards the event to the `MouseInputStateManager` or
         * `KeyboardInputStateManager` as appropriate. This delegation allows the respective managers to handle the event and
         * update their states accordingly.
         *
         * @param e The event object containing details about the input event, including the type of event and relevant data.
         */
		void OnEvent(Event& e) override;
	};
}

