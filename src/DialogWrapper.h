#pragma once
#include<nfd.h>
#include<iostream>
namespace OBJ_Viewer {
	class DialogWrapper {
	public:
		DialogWrapper() { OpenDialog(); }
		void OpenDialog();
		~DialogWrapper() { if(outPath != nullptr) free(outPath); }
		char* GetDialogPath() { return outPath; }
	private:
		nfdchar_t* outPath =nullptr;
	};
	inline void DialogWrapper::OpenDialog()
	{
		nfdresult_t result = NFD_OpenDialog("obj", NULL, &outPath);
		if (result != NFD_OKAY && result != NFD_CANCEL) {
			std::cout << "[ERROR]:NFD:" << NFD_GetError() << '\n';
		}
	}
}