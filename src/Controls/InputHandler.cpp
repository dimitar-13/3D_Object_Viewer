#include "pch.h"
#include "InputHandler.h"

OBJ_Viewer::KeyState_ OBJ_Viewer::InputHandler::KeyboardInputStateManager::GetKeyState(KeyboardKey_ key)
{
	if (m_keyHashes.find(key) == m_keyHashes.end())
		return KeyState_kUnknown;

	KeyState_ result = m_keyHashes.at(key);
	return result;
}
void OBJ_Viewer::InputHandler::KeyboardInputStateManager::onKeyStateChanged(KeyboardKeyEvent& e)
{
	if (m_keyHashes.find(e.GetKeyCode()) == m_keyHashes.end())
		return;

	KeyState_ state = static_cast<KeyState_>(e.GetKeyAction());
	m_keyHashes.at(e.GetKeyCode()) = state;
}
void OBJ_Viewer::InputHandler::MouseInputStateManager::onButtonStateChanged(MouseKeyEvent& e)
{
	if (m_mouseKeyHashes.find(e.GetKeyCode()) == m_mouseKeyHashes.end())
		return;

	KeyState_ state = static_cast<KeyState_>(e.GetKeyAction());
	m_mouseKeyHashes.at(e.GetKeyCode()) = state;
}

OBJ_Viewer::KeyState_ OBJ_Viewer::InputHandler::MouseInputStateManager::GetButtonState(MouseKey_ button)
{

    /**
     * Why don't just return the state?
     *
     * Well because GLFW doesn't really have state for the held mouse button it will only tell us if the mouse is pressed or released.
     * So that means in between these 2 stages that happens the mouse is being held.
     *
     * The user of this class is the one interested in the state so in a way we can make it so whenever he wants to get the state of the mouse
     * the first time he checks and it is pressed we can switch it on being held.This is in a way like a bool but
     * instead of recording bool we can use this trick to save us the trouble because now we don't have to define this for every key.
     * And the moment GLFW detects the key is released it will set it up back to released.
     *
     *
     * One possible problem is that if a class relies on knowing when a mouse button is held(not checking but beeing notified)
     * this might fail but this can be easily fixed but invoking an event within these classes.
     */

	if (m_mouseKeyHashes.find(button) == m_mouseKeyHashes.end())
		return KeyState_kUnknown;

	KeyState_ result = static_cast<KeyState_>(m_mouseKeyHashes.at(button));
 
	m_mouseKeyHashes.at(button) = result == KeyState_kPressed ? KeyState_kHeld : result;
	return result;
}

void OBJ_Viewer::InputHandler::OnEvent(Event& e)
{
	switch (e.GetEventType())
	{
	case EventType_kKeyPressed:
		this->m_keyboardInputManager.onKeyStateChanged(dynamic_cast<KeyboardKeyEvent&>(e));
		break;
	case EventType_kMouseButtonPressed:
		this->m_mouseInputManager.onButtonStateChanged(dynamic_cast<MouseKeyEvent&>(e));
		break;
	default:
		break;
	}
}
