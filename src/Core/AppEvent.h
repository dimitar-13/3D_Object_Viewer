#pragma once
#include "pch.h"
#include "Events.h"
#include "WindowHandler.h"
namespace OBJ_Viewer {
	class EventOnSkyboxLoaded : public Event
	{
	public:
		EventOnSkyboxLoaded()
		{}
		EventType_ GetEventType()const override { return EventType_kSkyboxLoad; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
	};

	class EventOnSceneLoad : public Event
	{
	public:
		EventOnSceneLoad(bool is_fbx_loading_disabled):
            m_isFBXLoadingDisabled(is_fbx_loading_disabled)
		{}
		EventType_ GetEventType()const override { return EventType_kScenelLoad; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
        bool IsFBXDisabled()const { return m_isFBXLoadingDisabled; }
	private:
        bool m_isFBXLoadingDisabled;
	};

	class WindowStateChangedEvent : public Event
	{
	public:
		WindowStateChangedEvent(Size2D metrics,WindowState_ state) :m_metrics(metrics), m_state(state)
		{}
		EventType_ GetEventType()const override { return EventType_kWindowStateChanged; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
		WindowState_ GetWindowState()const { return m_state; }
		Size2D GetWindowMetrics()const { return m_metrics; }

	private:
		Size2D m_metrics;
		WindowState_ m_state;
	};

	class EventCameraProjectionChanged : public Event
	{
	public:
		EventCameraProjectionChanged(bool isProjectionPerspective):m_isProjectionPerspective(isProjectionPerspective)
		{}
		EventType_ GetEventType()const override { return EventType_kCameraProjectionChanged; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
		bool isCameraProjectionPerspective()const { return m_isProjectionPerspective; }
	private:
		bool m_isProjectionPerspective;
	};

    class SceneViewportResizeEvent : public Event
    {
    public:
        SceneViewportResizeEvent(const Viewport& newSize) :m_viewport(newSize)
        { }
        EventType_ GetEventType()const override { return EventType_kViewportSizeChanged; }
        EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }

        Size2D GetViewportSize()const { return Size2D{ m_viewport.width,m_viewport.height }; }
        Size2D GetViewportOffset()const { return Size2D{ m_viewport.x,m_viewport.y }; }
        const Viewport& GetViewport()const { return m_viewport; }
    private:
        Viewport m_viewport;
    };

    struct ImgOutputData
    {
        Size2D imgSize;
        ImageFileFormat_ outImgFormat;
        bool renderObjectOnly;
        bool allowTransparency;
    };

    class ScreenshotEvent : public Event
    {
    public:
        ScreenshotEvent(const ImgOutputData& imgOutputData) :
            m_outputData(imgOutputData)
        {}      
        EventType_ GetEventType()const override { return EventType_kScreenshotButtonPressed; }
        EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
        const ImgOutputData& ImgData()const { return m_outputData; }
        Size2D GetImageSize()const { return m_outputData.imgSize; }
        const ImgOutputData& GetOutputData() { return m_outputData; }
    private:
        ImgOutputData m_outputData;
    };
}