#include "pch.h"
#include "DialogWrapper.h"

void OBJ_Viewer::DialogWrapper::OpenDialog(const std::string_view& filterLIst)
{   
    nfdchar_t* out_path = nullptr;

    nfdresult_t result = NFD_OpenDialog(filterLIst.data(), NULL, &out_path);
    if (result != NFD_OKAY) {
        if (result == NFD_ERROR)
            LOGGER_LOG_ERROR("NDF error:{0}", NFD_GetError());
        isDialogAborted = true;
        return;
    }

    outPaths.push_back(out_path);
    free(out_path);
}
void OBJ_Viewer::DialogWrapper::OpenDialogSavePath(const std::string_view& filterLIst)
{
    nfdchar_t* out_path = nullptr;

    nfdresult_t result = NFD_SaveDialog(filterLIst.data(), NULL, &out_path);
    if (result != NFD_OKAY) {
        if (result == NFD_ERROR)
            LOGGER_LOG_ERROR("NDF error:{0}", NFD_GetError());
        isDialogAborted = true;
    }
    outPaths.push_back(out_path);
    free(out_path);
}
void OBJ_Viewer::DialogWrapper::OpenDialogSaveFile(const std::string_view& file_save_format)
{
    nfdchar_t* out_path = nullptr;

    nfdresult_t result = NFD_SaveDialog(file_save_format.data(), NULL, &out_path);
    if (result != NFD_OKAY) {
        if (result == NFD_ERROR)
            LOGGER_LOG_ERROR("NDF error:{0}", NFD_GetError());
        isDialogAborted = true;
        return;
    }

    outPaths.push_back(out_path);
    const size_t kCurrentElementIndex = outPaths.size() - 1;
    outPaths[kCurrentElementIndex] += '.';
    outPaths[kCurrentElementIndex].append(file_save_format.data());

    free(out_path);
}

void OBJ_Viewer::DialogWrapper::OpenDialogMultiple(const std::string_view& filterList)
{
    nfdpathset_t paths;
    nfdresult_t result = NFD_OpenDialogMultiple(filterList.data(), NULL, &paths);
    if (result != NFD_OKAY) {
        if (result == NFD_ERROR)
            LOGGER_LOG_ERROR("NDF error:{0}", NFD_GetError());
        isDialogAborted = true;
        return;
    }
    outPaths.resize(paths.count);
    nfdchar_t* pathPtr = paths.buf;
    for (size_t i = 0; i < outPaths.size(); i++) {
        outPaths[i] = pathPtr + paths.indices[i];
    }
    free(pathPtr);
}