#pragma once
#include<unordered_map>
#include"Controls/KeyboardKeys.h"
#include"Controls/MouseKeys.h"
#include<GLFW/glfw3.h>
#include"Events.h"
#include<iostream>
namespace OBJ_Viewer
{
	class Application;
	enum KeyState {
		KEY_STATE_UKNOWN = -1,
		KEY_PRESSED = GLFW_PRESS,
		KEY_HELD = GLFW_REPEAT,
		KEY_RELEASED = GLFW_RELEASE,
	};
	class MouseInputStateManager {
	public:
		KeyState GetButtonState(MouseKey button);
		void onButtonStateChanged(MouseKeyEvent& e);
	private:
		std::unordered_map<MouseKey, KeyState> m_mouseKeyHashes = {
					{MOUSE_KEY_1, KEY_RELEASED },
					{MOUSE_KEY_2, KEY_RELEASED },
					{MOUSE_KEY_3, KEY_RELEASED },
					{MOUSE_KEY_4, KEY_RELEASED },
					{MOUSE_KEY_5, KEY_RELEASED },
					{MOUSE_KEY_6, KEY_RELEASED },
					{MOUSE_KEY_7, KEY_RELEASED },
					{MOUSE_KEY_8, KEY_RELEASED }
		};
	};

	class KeyboardInputStateManager{
	public:
		KeyState GetKeyState(KeyboardKey key);
		void onKeyStateChanged(KeyboardKeyEvent& e);
	private:
	std::unordered_map<KeyboardKey, KeyState> m_keyHashes = {
	{KEYBOARD_KEY_END, KEY_RELEASED },
	{KEYBOARD_KEY_SPACE, KEY_RELEASED},
	{KEYBOARD_KEY_APOSTROPHE, KEY_RELEASED},
	{KEYBOARD_KEY_COMMA, KEY_RELEASED},
	{KEYBOARD_KEY_MINUS, KEY_RELEASED},
	{KEYBOARD_KEY_PERIOD, KEY_RELEASED},
	{KEYBOARD_KEY_SLASH, KEY_RELEASED},
	{KEYBOARD_KEY_0, KEY_RELEASED},
	{KEYBOARD_KEY_1, KEY_RELEASED},
	{KEYBOARD_KEY_2, KEY_RELEASED},
	{KEYBOARD_KEY_3, KEY_RELEASED},
	{KEYBOARD_KEY_4, KEY_RELEASED},
	{KEYBOARD_KEY_5, KEY_RELEASED},
	{KEYBOARD_KEY_6, KEY_RELEASED},
	{KEYBOARD_KEY_7, KEY_RELEASED},
	{KEYBOARD_KEY_8, KEY_RELEASED},
	{KEYBOARD_KEY_9, KEY_RELEASED},
	{KEYBOARD_KEY_SEMICOLON, KEY_RELEASED},
	{KEYBOARD_KEY_EQUAL, KEY_RELEASED},
	{KEYBOARD_KEY_A, KEY_RELEASED},
	{KEYBOARD_KEY_B, KEY_RELEASED},
	{KEYBOARD_KEY_C, KEY_RELEASED},
	{KEYBOARD_KEY_D, KEY_RELEASED},
	{KEYBOARD_KEY_E, KEY_RELEASED},
	{KEYBOARD_KEY_F, KEY_RELEASED},
	{KEYBOARD_KEY_G, KEY_RELEASED},
	{KEYBOARD_KEY_H, KEY_RELEASED},
	{KEYBOARD_KEY_I, KEY_RELEASED},
	{KEYBOARD_KEY_J, KEY_RELEASED},
	{KEYBOARD_KEY_K, KEY_RELEASED},
	{KEYBOARD_KEY_L, KEY_RELEASED},
	{KEYBOARD_KEY_M, KEY_RELEASED},
	{KEYBOARD_KEY_N, KEY_RELEASED},
	{KEYBOARD_KEY_O, KEY_RELEASED},
	{KEYBOARD_KEY_P, KEY_RELEASED},
	{KEYBOARD_KEY_Q, KEY_RELEASED},
	{KEYBOARD_KEY_R, KEY_RELEASED},
	{KEYBOARD_KEY_S, KEY_RELEASED},
	{KEYBOARD_KEY_T, KEY_RELEASED},
	{KEYBOARD_KEY_U, KEY_RELEASED},
	{KEYBOARD_KEY_V, KEY_RELEASED},
	{KEYBOARD_KEY_W, KEY_RELEASED},
	{KEYBOARD_KEY_X, KEY_RELEASED},
	{KEYBOARD_KEY_Y, KEY_RELEASED},
	{KEYBOARD_KEY_Z, KEY_RELEASED},
	{KEYBOARD_KEY_LEFT_BRACKET, KEY_RELEASED},
	{KEYBOARD_KEY_BACKSLASH, KEY_RELEASED},
	{KEYBOARD_KEY_RIGHT_BRACKET, KEY_RELEASED},
	{KEYBOARD_KEY_GRAVE_ACCENT, KEY_RELEASED},
	{KEYBOARD_KEY_WORLD_1, KEY_RELEASED},
	{KEYBOARD_KEY_WORLD_2, KEY_RELEASED},
	{KEYBOARD_KEY_ESCAPE, KEY_RELEASED},
	{KEYBOARD_KEY_ENTER, KEY_RELEASED},
	{KEYBOARD_KEY_TAB, KEY_RELEASED},
	{KEYBOARD_KEY_BACKSPACE, KEY_RELEASED},
	{KEYBOARD_KEY_INSERT, KEY_RELEASED},
	{KEYBOARD_KEY_DELETE, KEY_RELEASED},
	{KEYBOARD_KEY_RIGHT, KEY_RELEASED},
	{KEYBOARD_KEY_LEFT, KEY_RELEASED},
	{KEYBOARD_KEY_DOWN, KEY_RELEASED},
	{KEYBOARD_KEY_UP, KEY_RELEASED},
	{KEYBOARD_KEY_PAGE_UP, KEY_RELEASED},
	{KEYBOARD_KEY_PAGE_DOWN, KEY_RELEASED},
	{KEYBOARD_KEY_HOME, KEY_RELEASED},
	{KEYBOARD_KEY_END, KEY_RELEASED},
	{KEYBOARD_KEY_CAPS_LOCK, KEY_RELEASED},
	{KEYBOARD_KEY_SCROLL_LOCK, KEY_RELEASED},
	{KEYBOARD_KEY_NUM_LOCK, KEY_RELEASED},
	{KEYBOARD_KEY_PRINT_SCREEN, KEY_RELEASED},
	{KEYBOARD_KEY_PAUSE, KEY_RELEASED},
	{KEYBOARD_KEY_F1, KEY_RELEASED},
	{KEYBOARD_KEY_F2, KEY_RELEASED},
	{KEYBOARD_KEY_F3, KEY_RELEASED},
	{KEYBOARD_KEY_F4, KEY_RELEASED},
	{KEYBOARD_KEY_F5, KEY_RELEASED},
	{KEYBOARD_KEY_F6, KEY_RELEASED},
	{KEYBOARD_KEY_F7, KEY_RELEASED},
	{KEYBOARD_KEY_F8, KEY_RELEASED},
	{KEYBOARD_KEY_F9, KEY_RELEASED},
	{KEYBOARD_KEY_F10, KEY_RELEASED},
	{KEYBOARD_KEY_F11, KEY_RELEASED},
	{KEYBOARD_KEY_F12, KEY_RELEASED},
	{KEYBOARD_KEY_F13, KEY_RELEASED},
	{KEYBOARD_KEY_F14, KEY_RELEASED},
	{KEYBOARD_KEY_F15, KEY_RELEASED},
	{KEYBOARD_KEY_F16, KEY_RELEASED},
	{KEYBOARD_KEY_F17, KEY_RELEASED},
	{KEYBOARD_KEY_F18, KEY_RELEASED},
	{KEYBOARD_KEY_F19, KEY_RELEASED},
	{KEYBOARD_KEY_F20, KEY_RELEASED},
	{KEYBOARD_KEY_F21, KEY_RELEASED},
	{KEYBOARD_KEY_F22, KEY_RELEASED},
	{KEYBOARD_KEY_F23, KEY_RELEASED},
	{KEYBOARD_KEY_F24, KEY_RELEASED},
	{KEYBOARD_KEY_F25, KEY_RELEASED},
	{KEYBOARD_KEY_KP_0, KEY_RELEASED},
	{KEYBOARD_KEY_KP_1, KEY_RELEASED},
	{KEYBOARD_KEY_KP_2, KEY_RELEASED},
	{KEYBOARD_KEY_KP_3, KEY_RELEASED},
	{KEYBOARD_KEY_KP_4, KEY_RELEASED},
	{KEYBOARD_KEY_KP_5, KEY_RELEASED},
	{KEYBOARD_KEY_KP_6, KEY_RELEASED},
	{KEYBOARD_KEY_KP_7, KEY_RELEASED},
	{KEYBOARD_KEY_KP_8, KEY_RELEASED},
	{KEYBOARD_KEY_KP_9, KEY_RELEASED},
	{KEYBOARD_KEY_KP_DECIMAL, KEY_RELEASED},
	{KEYBOARD_KEY_KP_DIVIDE, KEY_RELEASED},
	{KEYBOARD_KEY_KP_MULTIPLY, KEY_RELEASED},
	{KEYBOARD_KEY_KP_SUBTRACT, KEY_RELEASED},
	{KEYBOARD_KEY_KP_ADD, KEY_RELEASED},
	{KEYBOARD_KEY_KP_ENTER, KEY_RELEASED},
	{KEYBOARD_KEY_KP_EQUAL, KEY_RELEASED},
	{KEYBOARD_KEY_LEFT_SHIFT, KEY_RELEASED},
	{KEYBOARD_KEY_LEFT_CONTROL, KEY_RELEASED},
	{KEYBOARD_KEY_LEFT_ALT, KEY_RELEASED},
	{KEYBOARD_KEY_LEFT_SUPER, KEY_RELEASED},
	{KEYBOARD_KEY_RIGHT_SHIFT, KEY_RELEASED},
	{KEYBOARD_KEY_RIGHT_CONTROL, KEY_RELEASED},
	{KEYBOARD_KEY_RIGHT_ALT, KEY_RELEASED},
	{KEYBOARD_KEY_RIGHT_SUPER, KEY_RELEASED},
	{KEYBOARD_KEY_MENU, KEY_RELEASED}};
	};

	class InputHandler:public Listener
	{
	public:
		bool isKeyboardPressed(KeyboardKey key) { return m_keyboardInputManager.GetKeyState(key) == KEY_PRESSED; }
		bool isKeyboardKeyHeld(KeyboardKey key) { return m_keyboardInputManager.GetKeyState(key) == KEY_HELD; }
		bool isMouseButtonPressed(MouseKey mouseButton) { return m_mouseInputManager.GetButtonState(mouseButton) == KEY_PRESSED; }
		bool isMouseButtonHeld(MouseKey mouseButton) { return m_mouseInputManager.GetButtonState(mouseButton) == KEY_HELD; }
		const char* GetCurrentlyFocusedWindow()const { return m_crrentlyFocusedWindowName; }
		void SetCurrentlyFocusedWindow(const char* windowName){ m_crrentlyFocusedWindowName = windowName; }

		KeyState GetKeyState(int keycode);
	private:
		friend class Application;
		InputHandler() = default;
		InputHandler(InputHandler& copy) = delete;

		MouseInputStateManager m_mouseInputManager;
		KeyboardInputStateManager m_keyboardInputManager;

		const char* m_crrentlyFocusedWindowName = "SceneWindow";

		// Inherited via Listener
		void OnEvent(Event& e) override;
	};
}

