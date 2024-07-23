#pragma once
#include<nfd.h>
#include<iostream>
namespace OBJ_Viewer {
	class DialogWrapper {
	public:
		DialogWrapper() = default;
		void OpenDialog(const std::string& filterLIst);
		void OpenDialogMultiple(const std::string& filterList);
		bool isDialogClosed()const { return isDialogAborted; }
		~DialogWrapper();
		const std::vector<nfdchar_t*> GetDialogResult() const { return outPaths; }
	private:
		std::vector<nfdchar_t*> outPaths;
		bool isDialogAborted = false;
	};
	inline void DialogWrapper::OpenDialog(const std::string& filterLIst)
	{
		outPaths.push_back(nullptr);
		nfdresult_t result = NFD_OpenDialog(filterLIst.c_str(), NULL, &outPaths[0]);
		if (result != NFD_OKAY) {
			if (result == NFD_ERROR)
				std::cout << "[ERROR]:NFD:" << NFD_GetError() << '\n';
			isDialogAborted = true;
			return;
		}
	}
	inline void DialogWrapper::OpenDialogMultiple(const std::string& filterList)
	{
		nfdpathset_t paths;
		nfdresult_t result = NFD_OpenDialogMultiple(filterList.c_str(), NULL, &paths);
		if (result != NFD_OKAY) {
			if(result == NFD_ERROR)
				std::cout << "[ERROR]:NFD:" << NFD_GetError() << '\n';
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