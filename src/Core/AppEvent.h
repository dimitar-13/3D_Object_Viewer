#pragma once
#include "pch.h"
#include"Events.h"
#include"WindowHandler.h"
namespace OBJ_Viewer {
	class EventOnSkyboxLoaded : public Event
	{
	public:
		EventOnSkyboxLoaded(const std::vector<std::string>& paths):m_paths(paths)
		{}
		EventType GetEventType()const override { return EVENT_ON_SKYBOX_LOAD; }
		EventCategory GetEventCategory()const override { return APP_EVENT; }
		std::vector<std::string>& GetSkyboxPaths() { return m_paths; }
		const std::vector<std::string>& GetSkyboxPaths()const { return m_paths; }
	private:
		std::vector<std::string> m_paths;
	};
	class EventOnModelLoaded : public Event
	{
	public:
		EventOnModelLoaded(std::string modelPath):m_modelPath(modelPath)
		{}
		EventType GetEventType()const override { return EVENT_ON_MODEL_LOAD; }
		EventCategory GetEventCategory()const override { return APP_EVENT; }
		std::string GetModelPath()const { return m_modelPath; }
	private:
		std::string m_modelPath;
	};
	class WindowStateChangedEvent : public Event
	{
	public:
		WindowStateChangedEvent(Size2D metrics,WindowState state) :m_metrics(metrics), m_state(state)
		{}
		EventType GetEventType()const override { return EVENT_WINDOW_STATE_CHANGED; }
		EventCategory GetEventCategory()const override { return APP_EVENT; }
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
		EventType GetEventType()const override { return EVENT_CAMERA_PROJECTION_TYPE_CHANGED; }
		EventCategory GetEventCategory()const override { return APP_EVENT; }
		bool isCameraProjectionPerspective()const { return m_isProjectionPerspective; }
	private:
		bool m_isProjectionPerspective;
	};
}