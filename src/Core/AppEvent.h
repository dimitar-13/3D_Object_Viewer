#pragma once
#include "pch.h"
#include "Event.h"
#include "WindowHandler.h"
namespace OBJ_Viewer {
    /**
     * @brief Application event that is called when the user want to import a skybox.
     * 
     * This event is trigged by the UI button but its not limited to it.Currently the user can trigger it 
     * by pressing the LoadSkybox button but at later point the event can be trigged using keyEvents or something else.
     * 
     * The event doesn't carry any information but instead the Listener interested in this event should open a dialog using the 
     * Helpers/DialogWrapper class and resolve the event in that way.
     * 
     * ```cpp
     * 
     * void FunctionForHandlingTheEvent(EventOnSkyboxLoaded& e)
     * {
	 *   DialogWrapper dialog;
	 *   dialog.OpenDialogMultiple("png,jpeg,jpg");
     *
	 *   if (dialog.IsDialogClosed())
	 *   	return;
     * 
     * 
     *  ...
     * }
     * ```
     * Other parts of the app might not want to load the skybox but do other things with the information ofc.
     * 
     */
	class EventOnSkyboxLoaded : public Event
	{
	public:
        EventOnSkyboxLoaded() = default;
        /**
         * @brief Returns the event type enum. See EventType_ for different types.
         * 
         * @returns Returns the type of event defined by the enum 'EventType_'.
         */
		EventType_ GetEventType()const override { return EventType_kSkyboxLoad; }
        /**
         * @brief Returns the category of event enum. See EventCategory_ for different types.
         *
         * @returns Returns the category of event defined by the enum 'EventCategory_'.
         */
		EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
	};
    /**
     * @brief Event that represents a scene beeing loaded.
     *
     * This event don't carry any information except the 'is_fbx_loading_disabled' flag.
     * The Listener interested in this event should open a dialog and handle the event and loading. See 'EventOnSkyboxLoaded' for example.
     * 
     */
	class EventOnSceneLoad : public Event
	{
	public:
		EventOnSceneLoad(bool is_fbx_loading_disabled):
            m_isFBXLoadingDisabled(is_fbx_loading_disabled)
		{}
        /**
         * @brief Returns the event type enum. See EventType_ for different types.
         *
         * @returns Returns the type of event defined by the enum 'EventType_'.
         */
		EventType_ GetEventType()const override { return EventType_kScenelLoad; }
        /**
         * @brief Returns the category of event enum. See EventCategory_ for different types.
         *
         * @returns Returns the category of event defined by the enum 'EventCategory_'.
         */
		EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
        /**
         * @brief Returns true if fbx loading is disabled.
         *
         * @returns Returns true if fbx loading is disabled and false if is enabled.
         */
        bool IsFBXDisabled()const { return m_isFBXLoadingDisabled; }
	private:
        bool m_isFBXLoadingDisabled; ///< Fbx disable flag.
	};

	class WindowStateChangedEvent : public Event
	{
	public:
        /**
          * @brief Constructor sets the event window size and the window state information.
          * 
          * @param metrics The new window size.
          * @param state The window state see WindowState_ for more information.
          */
		WindowStateChangedEvent(Size2D metrics,WindowState_ state) :m_metrics(metrics), m_state(state)
		{}
        /**
         * @brief Returns the event type enum. See EventType_ for different types.
         *
         * @returns Returns the type of event defined by the enum 'EventType_'.
         */
		EventType_ GetEventType()const override { return EventType_kWindowStateChanged; }
        /**
         * @brief Returns the category of event enum. See EventCategory_ for different types.
         *
         * @returns Returns the category of event defined by the enum 'EventCategory_'.
         */
		EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
        /**
         * @brief Returns the event window state.
         *
         * @returns Returns the event window state see 'WindowState_' for more information.
         */
		WindowState_ GetWindowState()const { return m_state; }
        /**
         * @brief Returns the window size.
         * 
         * @returns Returns the event window size.
         */
		Size2D GetWindowMetrics()const { return m_metrics; }

	private:
		Size2D m_metrics;    ///< Event window size.
		WindowState_ m_state;///< Event window state see 'WindowState_' for more information.
	};

    /**
     * @brief Application event that is called when the camera projection changes.
     *
     * This event is triggered when the camera's projection mode changes, for example,
     * switching between perspective and orthographic projection. The event carries
     * information about whether the new projection mode is perspective.
     *
     */
	class EventCameraProjectionChanged : public Event
	{
	public:
        /**
         * @brief Constructor sets the event isProjectionPerspective flag information.
         * 
         * @param isProjectionPerspective Flag of the new projection.
         */
		EventCameraProjectionChanged(bool isProjectionPerspective):m_isProjectionPerspective(isProjectionPerspective)
		{}
        /**
         * @brief Returns the event type enum. See EventType_ for different types.
         *
         * @returns Returns the type of event defined by the enum 'EventType_'.
         */
		EventType_ GetEventType()const override { return EventType_kCameraProjectionChanged; }
        /**
          * @brief Returns the category of event enum. See EventCategory_ for different types.
          *
          * @returns Returns the category of event defined by the enum 'EventCategory_'.
          */
		EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
        /**
         * @brief Returns perspective camera flag.
         *
         * @returns Returns true if the camera projection is perspective and false if orthographic.
         */
		bool isCameraProjectionPerspective()const { return m_isProjectionPerspective; }
	private:
		bool m_isProjectionPerspective; ///< Is camera perspective flag.
	};

    /**
     * @brief Application event that is called when the scene viewport is resized.
     *
     * This event is triggered when the scene's viewport is resized. The event carries
     * information about the new viewport size and offset.
     * 
     * By scene viewport is understood the scene rendering box between the UI. When UI is disabled the size of
     * the viewport is the same as the window.
     */
    class SceneViewportResizeEvent : public Event
    {
    public:
        /**
         * @brief Constructor sets the Event viewport information. 
         */
        SceneViewportResizeEvent(const Viewport& newSize) :m_viewport(newSize)
        { }
        /**
         * @brief Returns the event type enum. See EventType_ for different types.
         *
         * @returns Returns the type of event defined by the enum 'EventType_'.
         */
        EventType_ GetEventType()const override { return EventType_kViewportSizeChanged; }
        /**
         * @brief Returns the category of event enum. See EventCategory_ for different types.
         *
         * @returns Returns the category of event defined by the enum 'EventCategory_'.
         */
        EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
        /**
         * @brief Returns the new viewport size.
         *
         * @returns Returns the new viewport size as a 'Size2D' object.
         */
        Size2D GetViewportSize()const { return Size2D{ m_viewport.width,m_viewport.height }; }
        /**
         * @brief Returns the viewport offset.
         *
         * Most of the times this is {0,0}.
         * @returns Returns the viewport offset as a 'Size2D' object.
         */
        Size2D GetViewportOffset()const { return Size2D{ m_viewport.x,m_viewport.y }; }
        /**
         * @brief Returns the viewport object.
         *
         * @returns Returns the 'Viewport' object containing size and position.
         */
        const Viewport& GetViewport()const { return m_viewport; }
    private:
        Viewport m_viewport; ///< The new viewport metrics.
    };
    /**
     * @brief Structure used by the 'ScreenshotEvent'.
     * 
     * The structure carries information about:
     * - imgSize The size of the image to be stored(This is used to resize the framebuffer also).
     * - outImgFormat The file format of the image png,jpeg etc.See 'ImageFileFormat_' for all of the supported output formats.
     * - renderObjectOnly If this flag is set the final image will not include things like skybox or grid(or any other scene obscurities that are not the Model).
     * - allowTransparency If this flag is set the final image will be saved as RGBA.
     * @note If the format doesn't support transparency the image format will be RGB.
     */
    struct ImgOutputData
    {
        Size2D imgSize;                 ///< Size of the image that is to be saved on the disc.
        ImageFileFormat_ outImgFormat;  ///< File format of the image that is to be saved on the disc. 
        bool renderObjectOnly;          ///< If set to true the final image will contain only the currently loaded model/scene.
        bool allowTransparency;         ///< If set to true the final image will be in RGBA if the file format supports it.
    };
    /**
     * @brief Application event that is called when user wants to take a screenshot.
     *
     * This event carries the information submitted by the user via the UI about the how the screenshot.
     * The information is stored in a 'ImgOutputData' struct. You can see the struct for more information.
     * 
     * The event doesn't save the screenshot to an image the Listeners interested in this event or behavior needs to implement it.
     */
    class ScreenshotEvent : public Event
    {
    public:
        /**
         * @brief The constructor copies the 'ImgOutputData' submitted by the user via the UI.
         */
        ScreenshotEvent(const ImgOutputData& imgOutputData) :
            m_outputData(imgOutputData)
        {}
        /**
         * @brief Returns the event type enum. See EventType_ for different types.
         *
         * @returns Returns the type of event defined by the enum 'EventType_'.
         */
        EventType_ GetEventType()const override { return EventType_kScreenshotButtonPressed; }
        /**
         * @brief Returns the category of event enum. See EventCategory_ for different types.
         *
         * @returns Returns the category of event defined by the enum 'EventCategory_'.
         */
        EventCategory_ GetEventCategory()const override { return EventCategory_kAppEvent; }
        /**
         * @brief Returns the event 'ImgOutputData' struct.
         * 
         * @returns Returns a readonly ref to the event data via the 'ImgOutputData' struct.
         */
        const ImgOutputData& GetScreenshotEventData()const { return m_outputData; }
    private:
        ImgOutputData m_outputData; ///< Screenshot image data submitted by the user via the UI.
    };
}