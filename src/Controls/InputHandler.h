#pragma once
#include "pch.h"
#include "Controls/KeyboardKeys.h"
#include "Controls/MouseKeys.h"
#include "Core/Events.h"
namespace OBJ_Viewer
{
	enum KeyState_ {
		KeyState_kUnknown,
		KeyState_kPressed = GLFW_PRESS,
		KeyState_kHeld = GLFW_REPEAT,
		KeyState_kReleased = GLFW_RELEASE,
	};
	class MouseInputStateManager {
	public:
		KeyState_ GetButtonState(MouseKey_ button);
		void onButtonStateChanged(MouseKeyEvent& e);
	private:
		std::unordered_map<MouseKey_, KeyState_> m_mouseKeyHashes = {
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

	class KeyboardInputStateManager{
	public:
		KeyState_ GetKeyState(KeyboardKey_ key);
		void onKeyStateChanged(KeyboardKeyEvent& e);
	private:
		std::unordered_map<KeyboardKey_, KeyState_> m_keyHashes = {
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

	class InputHandler:public Listener
	{
	public:
        InputHandler() = default;
		bool isKeyboardPressed(KeyboardKey_ key) { return m_keyboardInputManager.GetKeyState(key) == KeyState_kPressed; }
		bool isKeyboardKeyHeld(KeyboardKey_ key) { return m_keyboardInputManager.GetKeyState(key) == KeyState_kHeld; }
		bool isMouseButtonPressed(MouseKey_ mouseButton) { return m_mouseInputManager.GetButtonState(mouseButton) == KeyState_kPressed; }
		bool isMouseButtonHeld(MouseKey_ mouseButton) { return m_mouseInputManager.GetButtonState(mouseButton) == KeyState_kHeld; }
		const char* GetCurrentlyFocusedWindow()const { return m_crrentlyFocusedWindowName; }
		void SetCurrentlyFocusedWindow(const char* windowName){ m_crrentlyFocusedWindowName = windowName; }
	private:
		InputHandler(const InputHandler& copy) = delete;

		MouseInputStateManager m_mouseInputManager;
		KeyboardInputStateManager m_keyboardInputManager;

		const char* m_crrentlyFocusedWindowName = APP_FOCUS_REGIONS::kUI_SceneWindowName;

		// Inherited via Listener
		void OnEvent(Event& e) override;
	};
}

