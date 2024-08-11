#pragma once
#include "pch.h"
#include "Logging/App_Logger.h"

namespace OBJ_Viewer {
    /**
     * @brief A class for managing and opening file dialog windows.
     *
     * This class utilizes the `nfd` library (https://github.com/mlabbe/nativefiledialog) to provide a cross-platform
     * solution for file dialogs. Instead of using a constructor, you can call one of the `OpenDialogXXX` methods to open
     * a specific type of dialog (e.g., file open, file save).
     *
     * To determine if the dialog was successfully opened or if the user has closed it without selecting a file, use
     * `IsDialogClosed()`. This method returns `true` if the dialog was closed or an error occurred, and `false` if
     * the operation was successful and a file path is available.
     *
     * To retrieve the selected file path(s), use `GetFirstDialogResult()` for a single result or `GetDialogResult()`
     * for multiple results. `GetFirstDialogResult()` provides a straightforward way to obtain the first selected file path,
     * while `GetDialogResult()` returns a vector of all selected paths.
     *
     * When opening a dialog one can specify a file filters there are a few rules of specifying them:
     * - To list filters you have to separate them by ','.
     * - To start a new filter you have to use ';'.
     * 
     * Example use of the class:
     * ````
     * DialogWrapper dialog;
     * 
     * dialog.OpenDialog("png,jpeg");
     * 
     * if(dialog.IsDialogClosed())
     * {
     *  //Handle the closing of the dialog.
     * }
     * else
     * {
     *  const std::string& result_of_the_dialog = dialog.GetFirstDialogResult();
     * 
     * }
     * 
     * ````
     */
	class DialogWrapper {
	public:
		DialogWrapper() = default;
        /**
         * @brief Opens a file selection dialog and retrieves the selected file's path, name, and extension.
         *
         * This method opens a file dialog where the displayed files can be filtered based on the provided `filterList`.
         * The `filterList` specifies which file types are shown in the dialog, allowing users to filter files by extension.
         *
         * If the user selects a file, this method retrieves the absolute path, file name, and extension of the selected file.
         * If the dialog is closed by the user without making a selection or if an error occurs during the dialog operation,
         * the `isDialogAborted` flag is set to `true`, and an error message is logged.
         *
         * @param filterList A string view containing the file filter patterns, such as `"*.txt;*.cpp"`. The filter list determines
         *                    which file types are visible in the dialog.
         * 
         * @note Ensure that filters are separated by commas in the `filterList`. For more information on how to use the filter patterns,
         *       refer to the explanation above.
         */
		void OpenDialog(const std::string_view& filterList);
        /**
         * @brief Opens a file selection dialog for multiple files and retrieves the paths, names, and extensions of selected files.
         *
         * This method opens a file dialog that allows users to select multiple files. The files displayed in the dialog can be filtered
         * based on the provided `filterList`. This list determines which file types are visible in the dialog, allowing users to filter
         * files by their extensions.
         *
         * If the user selects one or more files, this method retrieves the absolute paths, names, and extensions of the selected files.
         * If the dialog is closed by the user without making a selection or if an error occurs during the dialog operation, the
         * `isDialogAborted` flag is set to `true`, and an error message is logged.
         * After all file paths are retrieved this method constructs a vector of paths. Use 'GetDialogPaths()' to retrieve the paths.
         *
         * @param filterList A string view containing file filter patterns, such as `"*.txt,*.cpp"`. The filter list determines
         *                    which file types are visible in the dialog. Filters should be separated by commas.
         *
         * @note Ensure that filters are separated by commas in the `filterList`. For more information on how to use the filter patterns,
         *       refer to the explanation above.
         */
		void OpenDialogMultiple(const std::string_view& filterList);
        /**
         * @brief Opens a dialog for retrieving a file save path.
         *
         * This method opens a file dialog that allows the user to specify a path for saving a file. The dialog enables
         * filtering of file types based on the provided `filterList`, which determines which file extensions are visible
         * and selectable in the dialog.
         *
         * Note: This method is currently not used in the implementation.
         *
         * @param filterList A string view containing the file filter patterns, such as `"*.txt;*.png"`. The filter list
         *                    specifies which file types are visible in the dialog, allowing users to filter file options.
         *
         * @note As this method is not currently used, it might not be functional or integrated into the rest of the codebase.
         */
		void OpenDialogSavePath(const std::string_view& filterLIst);
        /**
         * @brief Opens a file save dialog for selecting a file location and name.
         *
         * This method opens a dialog that allows the user to specify a file location and name for saving a file.
         * The dialog facilitates saving or creating new files. Due to how the 'nfd' library handles file dialogs,
         * the retrieved file path does not include the file extension. Therefore, this method appends the provided
         * file extension to the end of the file path.
         *
         * @param file_save_format A string view representing the desired file extension (e.g., `".txt"` or `".jpg"`).
         *                         This extension is appended to the file path selected by the user in the save dialog.
         *
         * @note Ensure that filters are separated by commas in the `filterList`. For more information on how to use the filter patterns,
         *       refer to the explanation above.
         */
        void OpenDialogSaveFile(const std::string_view& file_save_format);
        /**
         * @brief Checks whether the dialog operation was successful or aborted.
         *
         * This method returns `true` if the dialog was closed by the user or if an error occurred during the dialog operation.
         * If the dialog operation was successful and the user selected a file, this method returns `false`.
         *
         * @return `true` if the dialog was closed or an error occurred; `false` if the dialog was successful and a selection was made.
         */
		bool IsDialogClosed()const { return m_IsDialogFailedOrClosed; }
        /**
         * @brief Retrieves the path of the first file selected in the dialog.
         *
         * This method returns the absolute path, including the file name and extension, of the first file selected
         * in the dialog. If multiple files were selected, only the path of the first file is returned.
         *
         * To ensure that a valid path is returned, check the result of `IsDialogClosed()`. If `IsDialogClosed()` returns `true`,
         * it indicates that the dialog was closed or an error occurred, and the path may not be valid.
         *
         * @return The absolute path of the first or the only file selected by the dialog.
         */
		const std::string& GetFirstDialogResult()const { return m_dialogResultOutputPaths[0]; }
        /**
         * @brief Returns a vector containing all file paths retrieved by the dialog.
         *
         * This method returns a `std::vector<std::string>` containing the absolute paths of all files selected in the dialog.
         * Use this method when you have opened the dialog using `OpenDialogMultiple()`, which allows for multiple file selections.
         *
         * To ensure that the retrieved paths are valid, verify that `IsDialogClosed()` returns `false`, indicating that the
         * dialog was not closed prematurely or with an error.
         *
         * @return A constant reference to a vector of strings, each representing a file path retrieved by the dialog.
         */
		const std::vector<std::string>& GetDialogPaths() const { return m_dialogResultOutputPaths; }
	private:
		std::vector<std::string> m_dialogResultOutputPaths; ///< Vector that represents the dialog result output paths.
		bool m_IsDialogFailedOrClosed = false; ///< Represents if the dialog was closed due to error or user closed it.
	};
}