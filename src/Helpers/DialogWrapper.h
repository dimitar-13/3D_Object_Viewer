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
        void OpenDialogSaveFile(const std::string_view& file_save_format);
		bool IsDialogClosed()const { return isDialogAborted; }
		const std::string& GetFirstDialogResult()const { return outPaths[0]; }
		const std::vector<std::string>& GetDialogResult() const { return outPaths; }
	private:
		std::vector<std::string> outPaths;
		bool isDialogAborted = false;
	};
}