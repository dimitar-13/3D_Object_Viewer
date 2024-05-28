#pragma once
#include<nfd.h>
#include<iostream>
namespace OBJ_Viewer {
	class DialogWrapper {
	public:
		DialogWrapper() = default;
		void OpenDialog();
		void OpenDialogMultiple();
		~DialogWrapper();
		const std::vector<nfdchar_t*> GetDialogResult() const { return outPaths; }
	private:
		std::vector<nfdchar_t*> outPaths;
	};
	inline void DialogWrapper::OpenDialog()
	{
		outPaths.push_back(nullptr);
		nfdresult_t result = NFD_OpenDialog("obj", NULL, &outPaths[0]);
		if (result != NFD_OKAY && result != NFD_CANCEL) {
			std::cout << "[ERROR]:NFD:" << NFD_GetError() << '\n';
		}
	}
	inline void DialogWrapper::OpenDialogMultiple()
	{
		nfdpathset_t paths;
		nfdresult_t result = NFD_OpenDialogMultiple("png", NULL, &paths);
		if (result != NFD_OKAY && result != NFD_CANCEL) {
			std::cout << "[ERROR]:NFD:" << NFD_GetError() << '\n';
		}
		outPaths.resize(paths.count);
		nfdchar_t* pathPtr = paths.buf;
		for (size_t i = 0; i < outPaths.size(); i++){
			outPaths[i] = pathPtr + paths.indices[i];
		}
	}
	inline DialogWrapper::~DialogWrapper()
	{
		free(outPaths[0]);
	}
}