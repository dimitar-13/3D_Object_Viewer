#pragma once
#include "pch.h"
#include "CommonAppData.h"
#include "Controls/KeyboardKeys.h"
#include "Controls/MouseKeys.h"
#include "Helpers/TextureHelpers.h"
#include "Core/SceneViewport.h"
namespace OBJ_Viewer {
	enum EventType_
	{
		EventType_kUknown,
		EventType_kKeyPressed,
		EventType_kMouseButtonPressed,
		EventType_kMousePositionChanged,
		EventType_kMouseScrollChanged,
		EventType_kWindowSizeChanged,
		EventType_kSkyboxLoad,
		EventType_kScenelLoad,
		EventType_kFocusedWindowChanged,
		EventType_kWindowStateChanged,
		EventType_kCameraProjectionChanged,
		EventType_kViewportSizeChanged,
		EventType_kScreenshotButtonPressed
	};

	enum EventCategory_
	{
		EventCategory_kAppEvent,
		EventCategory_kInputEvent,
		EventCategory_kWindowEvent
	};

	class Event
	{
	public:
		Event() = default;
		virtual EventType_ GetEventType()const = 0;
		virtual EventCategory_ GetEventCategory()const = 0;
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
		EventType_ GetEventType()const override { return EventType_kMousePositionChanged; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kInputEvent; }

		Position2D& GetMousePos() { return m_mousePos; }
	private:
		Position2D m_mousePos;
	};

	class MouseKeyEvent : public Event
	{
	public:
		MouseKeyEvent(MouseKey_ key, int action, int mod) {
			m_key = key;
			m_action = action;
			m_mod = mod;
		}
		EventType_ GetEventType()const override { return EventType_kMouseButtonPressed; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kInputEvent; }

		MouseKey_ GetKeyCode()const { return m_key; }
		int GetKeyAction()const { return m_action; }
		int GetKeyMod()const { return m_mod; }

	private:
		MouseKey_ m_key;
		int m_action;
		int m_mod;
	};

	class KeyboardKeyEvent : public Event
	{
	public:
		KeyboardKeyEvent(KeyboardKey_ key, int scanCode, int action, int mod) {
			m_key = key;
			m_scanCode = scanCode;
			m_action = action;
			m_mod = mod;
		}
		EventType_ GetEventType()const override { return EventType_kKeyPressed; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kInputEvent; }

		KeyboardKey_ GetKeyCode()const { return m_key; }
		int GetKeyAction()const { return m_action; }
		int GetKeyMod()const { return m_mod; }
		int GetKeyScanCode()const { return m_scanCode; }


	private:
		KeyboardKey_ m_key;
		int m_scanCode;
		int m_action;
		int m_mod;
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(Size2D newSize):m_windowNewSize(newSize)
		{ }
		EventType_ GetEventType()const override { return EventType_kWindowSizeChanged; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kWindowEvent; }

		Size2D GetWindowSize()const { return m_windowNewSize; }
	private:
		Size2D m_windowNewSize;
	};

    class ScrollPositionChanged : public Event
    {
    public:
        ScrollPositionChanged(Position2D mousePos) : m_mousePos(mousePos) {

        }
        EventType_ GetEventType()const override { return EventType_kMouseScrollChanged; }
        EventCategory_ GetEventCategory()const override { return EventCategory_kInputEvent; }

        Position2D& GetScrollPosition() { return m_mousePos; }
    private:
        Position2D m_mousePos;
    };

}