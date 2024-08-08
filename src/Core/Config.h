#pragma once
#include<pch/pch.h>
#include <filesystem>
#include <sstream>
namespace OBJ_Viewer {
    class ProjectPathHelper
    {
    public:
       static std::string GetCurrentExecutableFilePath() { return FetchCurrentExecutablePath(); }
    private:
        static inline std::string& FetchCurrentExecutablePath()
        {
            if (m_currentExecutablePath.empty())
            {
                std::string executable_path;
                executable_path.resize(255);
                GetModuleFileNameA(NULL, executable_path.data(), 255 + 1);
                m_currentExecutablePath = executable_path.substr(0, executable_path.find_last_of('\\') + 1);
                std::replace(m_currentExecutablePath.begin(), m_currentExecutablePath.end(), '\\', '/');
            }

            return m_currentExecutablePath;
        }
    private:
        static inline std::string m_currentExecutablePath;
    };
}
