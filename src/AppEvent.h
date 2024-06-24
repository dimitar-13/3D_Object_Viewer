#pragma once
#include<vector>
#include<string>
#include"../src/IObserver.h"
namespace OBJ_Viewer {
	class EventOnSkyboxLoaded : public Event
	{
	public:
		EventOnSkyboxLoaded(std::vector<std::string> paths):m_paths(paths)
		{}
		EventType GetEventType()const override { return EVENT_ON_SKYBOX_LOAD; }
		EventCategory GetEventCategory()const override { return APP_EVENT; }
		std::vector<std::string>& GetSkyboxPaths() { return m_paths; }
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
}