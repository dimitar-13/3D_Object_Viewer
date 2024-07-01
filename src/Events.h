#pragma once
#include<vector>
#include"CommonAppData.h"

namespace OBJ_Viewer {
	enum EventType
	{
		EVENT_NONE = 0,
		EVENT_KEY_PRESSES,
		EVENT_MOUSE_BUTTON_PRESSED,
		EVENT_MOUSE_POSITION_CHANGED,
		EVENT_MOUSE_SCROLL_CHANGED,
		EVENT_WINDOW_SIZE_CHANGED,
		EVENT_ON_SKYBOX_LOAD,
		EVENT_ON_MODEL_LOAD,
		EVENT_ON_FOCUSED_WINDOW_CHANGED,
		EVENT_WINDOW_STATE_CHANGED
	};

	enum EventCategory
	{
		APP_EVENT = 1,
		INPUT_EVENT = 2,
		WINDOW_EVENT = 4
	};

	class Event
	{
	public:
		Event() = default;
		virtual EventType GetEventType()const = 0;
		virtual EventCategory GetEventCategory()const = 0;

	private:

		//EventType m_eventType;
		//EventCategory m_eventCategory;

	};

	class Listener
	{
	public:
		virtual void OnEvent(Event& e) = 0;
	};
	class MousePositionEvent : public Event
	{
	public:
		MousePositionEvent(Position2D mousePos) : m_mousePos(mousePos) {

		}
		EventType GetEventType()const override { return EVENT_MOUSE_POSITION_CHANGED; }
		EventCategory GetEventCategory()const override { return INPUT_EVENT; }

		Position2D& GetMousePos() { return m_mousePos; }
	private:
		Position2D m_mousePos;
	};

	class MouseKeyEvent : public Event
	{
	public:
		MouseKeyEvent(int key, int action, int mod) {
			m_keyCode = key;
			m_action = action;
			m_mod = mod;
		}
		EventType GetEventType()const override { return EVENT_MOUSE_BUTTON_PRESSED; }
		EventCategory GetEventCategory()const override { return INPUT_EVENT; }

		int GetKeyCode()const { return m_keyCode; }
		int GetKeyAction()const { return m_action; }
		int GetKeyMod()const { return m_mod; }

	private:
		int m_keyCode;
		int m_action;
		int m_mod;
	};

	class KeyboardKeyEvent : public Event
	{
	public:
		KeyboardKeyEvent(int key, int scanCode, int action, int mod) {
			m_keyCode = key;
			m_scanCode = scanCode;
			m_action = action;
			m_mod = mod;
		}
		EventType GetEventType()const override { return EVENT_KEY_PRESSES; }
		EventCategory GetEventCategory()const override { return INPUT_EVENT; }

		int GetKeyCode()const { return m_keyCode; }
		int GetKeyAction()const { return m_action; }
		int GetKeyMod()const { return m_mod; }
		int GetKeyScanCode()const { return m_scanCode; }


	private:
		int m_keyCode;
		int m_scanCode;
		int m_action;
		int m_mod;
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(Size2D newSize):m_windowNewSize(newSize)
		{ }
		EventType GetEventType()const override { return EVENT_WINDOW_SIZE_CHANGED; }
		EventCategory GetEventCategory()const override { return WINDOW_EVENT; }

		Size2D GetWindowSize()const { return m_windowNewSize; }
	private:
		Size2D m_windowNewSize;
	};


	class ScrollPositionChanged : public Event
	{
	public:
		ScrollPositionChanged(Position2D mousePos) : m_mousePos(mousePos) {

		}
		EventType GetEventType()const override { return EVENT_MOUSE_SCROLL_CHANGED; }
		EventCategory GetEventCategory()const override { return INPUT_EVENT; }

		Position2D& GetScrollPosition() { return m_mousePos; }
	private:
		Position2D m_mousePos;
	};

}