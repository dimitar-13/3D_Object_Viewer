#pragma once
#include "pch.h"
#include "Logging/App_Logger.h"

namespace OBJ_Viewer {
	class DialogWrapper {
	public:
		DialogWrapper() = default;
		void OpenDialog(const std::string_view& filterLIst);
		void OpenDialogMultiple(const std::string_view& filterList);
		void OpenDialogSavePath(const std::string_view& filterLIst);
		bool isDialogClosed()const { return isDialogAborted; }
		~DialogWrapper();
		const std::string GetFirstDialogResult()const { return outPaths[0]; }
		const std::vector<nfdchar_t*>& GetDialogResult() const { return outPaths; }
	private:
		std::vector<nfdchar_t*> outPaths;
		bool isDialogAborted = false;
	};
	inline void DialogWrapper::OpenDialog(const std::string_view& filterLIst)
	{
		outPaths.push_back(nullptr);
		nfdresult_t result = NFD_OpenDialog(filterLIst.data(), NULL, &outPaths[0]);
		if (result != NFD_OKAY) {
			if (result == NFD_ERROR)
				LOGGER_LOG_ERROR("NDF error:{0}", NFD_GetError());
			isDialogAborted = true;
			return;
		}
	}
	inline void DialogWrapper::OpenDialogSavePath(const std::string_view& filterLIst)
	{
		outPaths.push_back(nullptr);
		
		nfdresult_t result = NFD_SaveDialog(filterLIst.data() , NULL, &outPaths[0]);
		if (result != NFD_OKAY) {
			if (result == NFD_ERROR)
				LOGGER_LOG_ERROR("NDF error:{0}", NFD_GetError());
			isDialogAborted = true;
			return;
		}
	}
	inline void DialogWrapper::OpenDialogMultiple(const std::string_view& filterList)
	{
		nfdpathset_t paths;
		nfdresult_t result = NFD_OpenDialogMultiple(filterList.data(), NULL, &paths);
		if (result != NFD_OKAY) {
			if(result == NFD_ERROR)
				LOGGER_LOG_ERROR("NDF error:{0}", NFD_GetError());
			isDialogAborted = true;
			return;
		}
		outPaths.resize(paths.count);
		nfdchar_t* pathPtr = paths.buf;
		for (size_t i = 0; i < outPaths.size(); i++){
			outPaths[i] = pathPtr + paths.indices[i];
		}
	}
	inline DialogWrapper::~DialogWrapper()
	{
		if(!outPaths.empty())free(outPaths[0]);
	}
}