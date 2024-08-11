#pragma once
#include "pch.h"

namespace OBJ_Viewer {

	enum LoadModelFileType_
	{
		LoadModelFileType_kUnknown,
		LoadModelFileType_kOBJ,
		LoadModelFileType_kFBX
	};
    /**
     * @brief Utility class for working with the  LoadModelFileType_ enum.
     */
	struct FileFormatHelper {
        /**
         * @brief Function that determines the format based on the file extension.
         * 
         * Utility function that gets a file name or path and returns the correct enum from LoadModelFileType_.
         * If the format is not supported or it cant deduce it it will return 'LoadModelFileType_kUnknown'.
         * 
         * @param path The file name or path whose extension is to be examined. This string should include the file extension.
         * @returns Returns a enum LoadModelFileType_ based on the file format.
         */
		static LoadModelFileType_ GetFileFormatFromPath(const std::string& path)
		{
			const size_t fileFormatOffset = path.find_last_of(".");

			if(path.substr(fileFormatOffset + 1).compare("fbx") == 0)
				return LoadModelFileType_kFBX;
			else if(path.substr(fileFormatOffset + 1).compare("obj") == 0 )
				return LoadModelFileType_kOBJ;
			else
				return LoadModelFileType_kUnknown;
		}
	};
}