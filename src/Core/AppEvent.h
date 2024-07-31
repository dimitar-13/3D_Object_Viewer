#pragma once
#include "pch.h"
#include"Events.h"
#include"WindowHandler.h"
namespace OBJ_Viewer {
	class EventOnSkyboxLoaded : public Event
	{
	public:
		EventOnSkyboxLoaded()
		{}
		EventType_ GetEventType()const override { return EventType_kSkyboxLoad; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
	};
	class EventOnModelLoaded : public Event
	{
	public:
		EventOnModelLoaded()
		{}
		EventType_ GetEventType()const override { return EventType_kModelLoad; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
	private:
	};
	class WindowStateChangedEvent : public Event
	{
	public:
		WindowStateChangedEvent(Size2D metrics,WindowState state) :m_metrics(metrics), m_state(state)
		{}
		EventType_ GetEventType()const override { return EventType_kWindowStateChanged; }
		EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
		WindowState GetWindowState()const { return m_state; }
		Size2D GetWindowMetrics()const { return m_metrics; }

	private:
		Size2D m_metrics;
		WindowState m_state;
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
}