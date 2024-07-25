#include "pch.h"
#include "InputHandler.h"

OBJ_Viewer::KeyState OBJ_Viewer::KeyboardInputStateManager::GetKeyState(KeyboardKey key)
{
	if (m_keyHashes.find(key) == m_keyHashes.end())
		return KEY_STATE_UKNOWN;

	KeyState result = static_cast<KeyState>(m_keyHashes.find(key) != m_keyHashes.end() ? m_keyHashes.at(key) : KEY_PRESSED);
	return result;
}
void OBJ_Viewer::KeyboardInputStateManager::onKeyStateChanged(KeyboardKeyEvent& e)
{
	if (m_keyHashes.find(e.GetKeyCode()) == m_keyHashes.end())
		return;

	KeyState state = static_cast<KeyState>(e.GetKeyAction());
	m_keyHashes.at(e.GetKeyCode()) = state;
}
void OBJ_Viewer::MouseInputStateManager::onButtonStateChanged(MouseKeyEvent& e)
{
	if (m_mouseKeyHashes.find(e.GetKeyCode()) == m_mouseKeyHashes.end())
		return;

	KeyState state = static_cast<KeyState>(e.GetKeyAction());
	m_mouseKeyHashes.at(e.GetKeyCode()) = state;
}

OBJ_Viewer::KeyState OBJ_Viewer::MouseInputStateManager::GetButtonState(MouseKey button)
{
	if (m_mouseKeyHashes.find(button) == m_mouseKeyHashes.end())
		return KEY_STATE_UKNOWN;

	KeyState result = static_cast<KeyState>(m_mouseKeyHashes.at(button));
	//TODO:Add error checks;
	/*Why don't just return the state? Well because GLFW doesn't really have state for the held mouse button it will only tell us if the mouse is pressed or released.
	So that means in between these 2 stages that happens the mouse is being held.The user of this class is the one interested in the state so in a way we can make 
	it so whenever he wants to get the state of the mouse the first time he checks and it is pressed we can switch it on being held.This is in a way like a bool but 
	instead of recording bool we can use this trick to save us the trouble because now we don't have to define this for every key.And the moment GLFW detects the key 
	is released it will set it up back to released.You could say what if multiple classes need to access this information but does anything change the assumption is 
	still correct.In any means is not the most accurate way to test weather a key is held but for this project it satisfies the need well enough.*/
	m_mouseKeyHashes.at(button) = result == KEY_PRESSED ? KEY_HELD : result;
	return result;
}

void OBJ_Viewer::InputHandler::OnEvent(Event& e)
{
	switch (e.GetEventType())
	{
	case EVENT_KEY_PRESSES:
		this->m_keyboardInputManager.onKeyStateChanged(dynamic_cast<KeyboardKeyEvent&>(e));
		break;
	case EVENT_MOUSE_BUTTON_PRESSED:
		this->m_mouseInputManager.onButtonStateChanged(dynamic_cast<MouseKeyEvent&>(e));
		break;
	default:
		break;
	}
}
