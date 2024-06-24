#pragma once
#include<vector>

//TODO:Because some templates can share lets call it the same signature this can cause problems for example both onScrollChange and onMousePosChange return 
// x and y positions as offset and because of this the update methods overlap one way to fix this is the messageType but this also makes the templates almost pointless
// another way i thought is to make for example the scroll function return only the Y offset but this is a solution that is reling not that much on softwere design but
// more on tricks;
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
		EVENT_ON_FOCUSED_WINDOW_CHANGED
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
	struct MousePos
	{
		double x;
		double y;
	};

	class MousePositionEvent : public Event
	{
	public:
		MousePositionEvent(MousePos mousePos) : m_mousePos(mousePos) {

		}
		EventType GetEventType()const override { return EVENT_MOUSE_POSITION_CHANGED; }
		EventCategory GetEventCategory()const override { return INPUT_EVENT; }

		MousePos& GetMousePos() { return m_mousePos; }
	private:
		MousePos m_mousePos;
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
		WindowResizeEvent(int width, int height)
		{
			m_width = width;
			m_height = height;
		}
		EventType GetEventType()const override { return EVENT_WINDOW_SIZE_CHANGED; }
		EventCategory GetEventCategory()const override { return WINDOW_EVENT; }

		int GetWindowHeight()const { return m_height; }
		int GetWindowWidth()const { return m_width; }

	private:
		int m_width;
		int m_height;
	};


	class ScrollPositionChanged : public Event
	{
	public:
		ScrollPositionChanged(MousePos mousePos) : m_mousePos(mousePos) {

		}
		EventType GetEventType()const override { return EVENT_MOUSE_SCROLL_CHANGED; }
		EventCategory GetEventCategory()const override { return INPUT_EVENT; }

		MousePos& GetScrollPosition() { return m_mousePos; }
	private:
		MousePos m_mousePos;
	};

}