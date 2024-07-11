#pragma once
#include<vector>
#include"CommonAppData.h"
#include"Controls/KeyboardKeys.h"
#include"Controls/MouseKeys.h"

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
		EVENT_WINDOW_STATE_CHANGED,
		EVENT_CAMERA_PROJECTION_TYPE_CHANGED,
		EVENT_FRAMEBUFFER_SIZE_CHANGED
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
		MouseKeyEvent(MouseKey key, int action, int mod) {
			m_key = key;
			m_action = action;
			m_mod = mod;
		}
		EventType GetEventType()const override { return EVENT_MOUSE_BUTTON_PRESSED; }
		EventCategory GetEventCategory()const override { return INPUT_EVENT; }

		MouseKey GetKeyCode()const { return m_key; }
		int GetKeyAction()const { return m_action; }
		int GetKeyMod()const { return m_mod; }

	private:
		MouseKey m_key;
		int m_action;
		int m_mod;
	};

	class KeyboardKeyEvent : public Event
	{
	public:
		KeyboardKeyEvent(KeyboardKey key, int scanCode, int action, int mod) {
			m_key = key;
			m_scanCode = scanCode;
			m_action = action;
			m_mod = mod;
		}
		EventType GetEventType()const override { return EVENT_KEY_PRESSES; }
		EventCategory GetEventCategory()const override { return INPUT_EVENT; }

		KeyboardKey GetKeyCode()const { return m_key; }
		int GetKeyAction()const { return m_action; }
		int GetKeyMod()const { return m_mod; }
		int GetKeyScanCode()const { return m_scanCode; }


	private:
		KeyboardKey m_key;
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

	class FramebufferResizeEvent : public Event
	{
	public:
		FramebufferResizeEvent(Size2D newSize) :m_framebufferSize(newSize)
		{ }
		EventType GetEventType()const override { return EVENT_FRAMEBUFFER_SIZE_CHANGED; }
		EventCategory GetEventCategory()const override { return APP_EVENT; }

		Size2D GetNewFramebufferSize()const { return m_framebufferSize; }
	private:
		Size2D m_framebufferSize;
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