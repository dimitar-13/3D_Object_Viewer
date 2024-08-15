#pragma once
namespace OBJ_Viewer {
    /**
     * @brief Enum representing application event types.
     *
     *
     * - EventType_kUnknown :                For debugging.
     * - EventType_kKeyPressed:              Event of type/class 'KeyboardKeyEvent'.
     * - EventType_kMouseButtonPressed:      Event of type/class 'MouseKeyEvent'.
     * - EventType_kMousePositionChanged:    Event of type/class 'MousePositionEvent'.
     * - EventType_kMouseScrollChanged:      Event of type/class 'ScrollPositionChanged'.
     * - EventType_kWindowSizeChanged:       Event of type/class 'WindowResizeEvent'.
     * - EventType_kSkyboxLoad:              Event of type/class 'EventOnSkyboxLoaded'.
     * - EventType_kScenelLoad:              Event of type/class 'EventOnSceneLoad'.
     * - EventType_kFocusedWindowChanged:    Currently not used.
     * - EventType_kWindowStateChanged:      Event of type/class 'WindowStateChangedEvent'.
     * - EventType_kCameraProjectionChanged: Event of type/class 'EventCameraProjectionChanged'.
     * - EventType_kViewportSizeChanged:     Event of type/class 'SceneViewportResizeEvent'.
     * - EventType_kScreenshotButtonPresse:  Event of type/class 'ScreenshotEvent'.
     */
    enum EventType_
    {
        EventType_kUnknown,                       //< For debugging.
        EventType_kKeyPressed,                    //< Event of type/class 'KeyboardKeyEvent'.
        EventType_kMouseButtonPressed,            //< Event of type/class 'MouseKeyEvent'.
        EventType_kMousePositionChanged,          //< Event of type/class 'MousePositionEvent'.
        EventType_kMouseScrollChanged,            //< Event of type/class 'ScrollPositionChanged'.
        EventType_kWindowSizeChanged,             //< Event of type/class 'WindowResizeEvent'.
        EventType_kSkyboxLoad,                    //< Event of type/class 'EventOnSkyboxLoaded'.
        EventType_kScenelLoad,                    //< Event of type/class 'EventOnSceneLoad'.
        EventType_kFocusedWindowChanged,          //< Currently not used.
        EventType_kWindowStateChanged,            //< Event of type/class 'WindowStateChangedEvent'.
        EventType_kCameraProjectionChanged,       //< Event of type/class 'EventCameraProjectionChanged'.
        EventType_kViewportSizeChanged,           //< Event of type/class 'SceneViewportResizeEvent'.
        EventType_kScreenshotButtonPressed        //< Event of type/class 'ScreenshotEvent'.
    };

    /**
     * @brief Enum for categorizing the events.
     *
     * - EventCategory_kUnknown: For debugging.
     * - EventCategory_kAppEvent : Application event like: ViewportResize,Loading event etc.
     * - EventCategory_kInputEvent : Keyboard or mouse input.
     * - EventCategory_kWindowEvent : Window resize or state change event.
     */
    enum EventCategory_
    {
        EventCategory_kUnknown,     ///< For debugging.
        EventCategory_kAppEvent,    ///< Viewport resize event ,loading event etc.
        EventCategory_kInputEvent,  ///< Keyboard or mouse input.
        EventCategory_kWindowEvent  ///< Window resize or state change event.
    };
}