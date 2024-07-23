#pragma once
#include<string>
namespace OBJ_Viewer {

	enum LoadModelFileType
	{
		MODEL_TYPE_UKNOWN,
		MODEL_TYPE_OBJ,
		MODEL_TYPE_FBX
	};

	struct FileFormatHelper {

		static LoadModelFileType GetFileFormatFromPath(const std::string& path)
		{
			const size_t fileFormatOffset = path.find_last_of(".");

			if(path.substr(fileFormatOffset + 1).compare("fbx") == 0)
				return MODEL_TYPE_FBX;
			else if(path.substr(fileFormatOffset + 1).compare("obj") == 0 )
				return MODEL_TYPE_OBJ;
			else
				return MODEL_TYPE_UKNOWN;
		}
	};
}