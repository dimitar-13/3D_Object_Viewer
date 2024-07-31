#pragma once
#include "pch.h"
#include "CommonAppData.h"
#include "Controls/KeyboardKeys.h"
#include "Controls/MouseKeys.h"
#include "Helpers/TextureHelpers.h"
#include "Core/SceneViewport.h"
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
		EVENT_SCENE_VIEWPORT_SIZE_CHANGED,
		EVENT_SCREENSHOT_BUTTON_PRESSED
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

	class SceneViewportResizeEvent : public Event
	{
	public:
		SceneViewportResizeEvent(const Viewport& newSize) :m_viewport(newSize)
		{ }
		EventType GetEventType()const override { return EVENT_SCENE_VIEWPORT_SIZE_CHANGED; }
		EventCategory GetEventCategory()const override { return APP_EVENT; }

		Size2D GetViewportSize()const { return Size2D{ m_viewport.width,m_viewport.height}; }
		Size2D GetViewportOffset()const { return Size2D{ m_viewport.x,m_viewport.y }; }
		const Viewport& GetViewport()const { return m_viewport; }
	private:
		Viewport m_viewport;
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

	struct ImgOutputData
	{
		Size2D imgSize;
		std::string outPath;
		ImageFileFormat outImgFormat;
		bool renderObjectOnly;
		bool allowTransparency;
	};

	class ScreenshotEvent : public Event
	{
	public:
		ScreenshotEvent(const ImgOutputData& imgOutputData) :
			m_outputData(imgOutputData)
		{
			std::string_view formatString = TextureFileEnumConverter::GetStringTextureFileExtensionFormatFromEnum(m_outputData.outImgFormat);
			m_outputData.outPath.append(formatString.data());
			
		}
		EventType GetEventType()const override { return EVENT_SCREENSHOT_BUTTON_PRESSED; }
		EventCategory GetEventCategory()const override { return APP_EVENT; }
		const ImgOutputData& ImgData()const { return m_outputData; }
		Size2D GetImageSize()const { return m_outputData.imgSize; }
		const ImgOutputData& GetOutputData() { return m_outputData; }
	private:
		ImgOutputData m_outputData;
	};
}