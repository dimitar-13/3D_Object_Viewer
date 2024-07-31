#pragma once
#include "pch.h"

namespace OBJ_Viewer {

	enum LoadModelFileType_
	{
		LoadModelFileType_kUknown,
		LoadModelFileType_kOBJ,
		LoadModelFileType_kFBX
	};

	struct FileFormatHelper {

		static LoadModelFileType_ GetFileFormatFromPath(const std::string& path)
		{
			const size_t fileFormatOffset = path.find_last_of(".");

			if(path.substr(fileFormatOffset + 1).compare("fbx") == 0)
				return LoadModelFileType_kFBX;
			else if(path.substr(fileFormatOffset + 1).compare("obj") == 0 )
				return LoadModelFileType_kOBJ;
			else
				return LoadModelFileType_kUknown;
		}
	};
}