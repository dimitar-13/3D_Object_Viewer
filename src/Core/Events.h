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
		EventType_kModelLoad,
		EventType_kFocusedWindowChanged,
		EventType_kWindowStateChanged,
		EventType_kCameraProjectionChanged,
		EventType_kViewportSizeChanged,
		EventType_kScreenshotButtonPressed
	};

	enum EventCategory_
	{
		EventCategory_kAppEvent = 1,
		EventCategory_kInputEvent = 2,
		EventCategory_kWindowEvent = 4
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

	class SceneViewportResizeEvent : public Event
	{
	public:
		SceneViewportResizeEvent(const Viewport& newSize) :m_viewport(newSize)
		{ }
		EventType_ GetEventType()const override { return EventType_kViewportSizeChanged; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }

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
		EventType_ GetEventType()const override { return EventType_kMouseScrollChanged; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kInputEvent; }

		Position2D& GetScrollPosition() { return m_mousePos; }
	private:
		Position2D m_mousePos;
	};

	struct ImgOutputData
	{
		Size2D imgSize;
		std::string outPath;
		ImageFileFormat_ outImgFormat;
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
		EventType_ GetEventType()const override { return EventType_kScreenshotButtonPressed; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
		const ImgOutputData& ImgData()const { return m_outputData; }
		Size2D GetImageSize()const { return m_outputData.imgSize; }
		const ImgOutputData& GetOutputData() { return m_outputData; }
	private:
		ImgOutputData m_outputData;
	};
}