#include "InputHandler.h"
#include<iostream>
OBJ_Viewer::KeyState OBJ_Viewer::KeyboardInputStateManager::GetKeyState(int key)
{
	KeyState result = static_cast<KeyState>(m_keyHashes.find(key) != m_keyHashes.end() ? m_keyHashes.at(key) : KEY_PRESSES);
	return result;
}
void OBJ_Viewer::KeyboardInputStateManager::Update(MessageType type, int key, int scancode, int action, int mods)
{
	KeyState state = static_cast<KeyState>(action);
	m_keyHashes.at(key) = state;
}
void OBJ_Viewer::MouseInputStateManager::Update(MessageType type, int button, int action, int mods)
{
	KeyState state = static_cast<KeyState>(action);
	m_mouseKeyHashes.at(button) = state;
}

OBJ_Viewer::KeyState OBJ_Viewer::MouseInputStateManager::GetButtonState(int button)
{
	KeyState result = static_cast<KeyState>(m_mouseKeyHashes.find(button) != m_mouseKeyHashes.end() ? m_mouseKeyHashes.at(button) : KEY_PRESSES);
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

OBJ_Viewer::InputHandler::InputHandler(Notifier<int, int, int>* mouseButtonNotifier, Notifier<int, int, int, int>* keyboardKeyNotifier)
{
	mouseButtonNotifier->Attach(dynamic_cast<IObserver<int, int, int>*>(&this->m_mouseInputManager));
	keyboardKeyNotifier->Attach(dynamic_cast<IObserver<int, int, int, int>*>(&this->m_keyboardInputManager));
}
