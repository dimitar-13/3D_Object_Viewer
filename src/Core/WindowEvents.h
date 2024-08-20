#pragma once
#include "pch.h"
#include "CommonAppData.h"
#include "Controls/KeyboardKeys.h"
#include "Controls/MouseKeys.h"
#include "Helpers/TextureHelpers.h"
#include "Core/SceneViewport.h"
#include "Event.h"
namespace OBJ_Viewer {

    /**
     * @brief Class representing a mouse movement input event.
     * 
     * The event carries information about the new mouse position and it's invoked when the mouse 
     * position changes. 
     * 
     * This event is created by the Window class.
     */
	class MousePositionEvent : public Event
	{
	public:
        /**
         * @brief Constructor sets the event data 
         *
         *@param mousePos The new mouse position.
         */
		MousePositionEvent(Position2D mousePos) : m_mousePos(mousePos) 
        { }
        /**
         * @brief Returns the event type enum. See EventType_ for different types.
         *
         * @returns Returns the type of event defined by the enum 'EventType_'.
         */
		EventType_ GetEventType()const override { return EventType_kMousePositionChanged; }
        /**
         * @brief Returns the category of event enum. See EventCategory_ for different types.
         *
         * @returns Returns the category of event defined by the enum 'EventCategory_'.
         */
		EventCategory_ GetEventCategory()const override { return EventCategory_kInputEvent; }
        /**
         * @brief Returns the mouse position.
         *
         * @returns Returns the new mouse position recorded by the event.
         */
		const Position2D& GetMousePos()const { return m_mousePos; }
	private:
		Position2D m_mousePos; ///< The new mouse position.
	};

    /**
      * @brief Class representing a mouse button input event.
      *
      * This class is triggered when a mouse button input is detected.
      * An input event occurs when the state of a mouse button changes.
      * Example:
      * If you press the left mouse button, an event is triggered indicating that
      * the button is pressed. Releasing the button triggers another event indicating
      * that the button is released.
      *
      * The event carries information about the mouse button that was involved,
      * represented by the `MouseKey_` enum. It also provides details about the action
      * performed (such as press or release) and any modifier keys (e.g., Shift, Control)
      * that were active at the time.
      *
      * The event is typically generated by the Window class or an input handling system.
      */
	class MouseKeyEvent : public Event
	{
	public:
        /**
         * @brief The constructor sets the event data.
         *
         * @param key The mouse button involved in the event, represented by the `MouseKey_` enum.
         * @param action The action performed on the mouse button (e.g., press, release).
         * @param mod The modifier keys (e.g., Shift, Control) that were active during the event.
         */
		MouseKeyEvent(MouseKey_ key, int action, int mod) {
			m_key = key;
			m_action = action;
			m_mod = mod;
		}
        /**
          * @brief Returns the event type enum. See `EventType_` for different types.
          *
          * @returns The type of event defined by the enum `EventType_`.
          */
		EventType_ GetEventType()const override { return EventType_kMouseButtonPressed; }
        /**
         * @brief Returns the category of event enum. See `EventCategory_` for different types.
         *
         * @returns The category of event defined by the enum `EventCategory_`.
         */
		EventCategory_ GetEventCategory()const override { return EventCategory_kInputEvent; }
        /**
         * @brief Returns the mouse button code associated with this event.
         *
         * This method retrieves the mouse button code that corresponds to the specific button
         * involved in the event. The button code is represented by the `MouseKey_` enum,
         * which includes values for all possible buttons on the mouse.
         *
         * @returns The mouse button code as an enum value of type `MouseKey_`.
         */
		MouseKey_ GetKeyCode()const { return m_key; }
        /**
         * @brief Returns the action performed on the mouse button.
         *
         * This method retrieves the action that was performed on the mouse button, such as
         * pressing or releasing the button. 
         * 
         * Because GLFW don't have hold for the mouse button this function wont return held event.
         * If you want to know if a mouse button is held see the InputHandler class.
         *
         * @returns The action as an integer.
         */
		int GetKeyAction()const { return m_action; }
        /**
         * @brief Returns the modifier keys that were active during the event.
         *
         * This method retrieves the state of modifier keys (e.g., Shift, Control, Alt)
         * at the time the mouse button event occurred.
         *
         * @returns The active modifier keys as an integer.
         */
		int GetKeyMod()const { return m_mod; }
	private:
		MouseKey_ m_key; ///< The mouse button code associated with this event.
        int m_action;    ///< The action performed on the mouse button.
        int m_mod;       ///< The modifier keys active during the event.
	};
   
    /**
     * @brief Class representing a keyboard input event.
     *
     * This class is called when a keyboard input is detected.
     * Where an input is understood as a change in a key state.
     * Example:
     * I press the key 'K' now 'K' is pressed.
     * Then I release it now 'K' is released.
     * 
     * The event carries information about the key that was pressed represented with the KeyboardKey_ enum.
     * The platform specific scanCode(currently not in used).
     * The action press,release etc.
     * The mod witch stands for modification key(thins like caps lock,shift,etc.)
     *
     * The event is created by the Window class.
     */
	class KeyboardKeyEvent : public Event
	{
	public:
        /**
         * @brief The constructor sets the event data. 
         */
		KeyboardKeyEvent(KeyboardKey_ key, int scanCode, int action, int mod) {
			m_key = key;
			m_scanCode = scanCode;
			m_action = action;
			m_mod = mod;
		}
        /**
         * @brief Returns the event type enum. See EventType_ for different types.
         *
         * @returns Returns the type of event defined by the enum 'EventType_'.
         */
		EventType_ GetEventType()const override { return EventType_kKeyPressed; }
        /**
         * @brief Returns the category of event enum. See EventCategory_ for different types.
         *
         * @returns Returns the category of event defined by the enum 'EventCategory_'.
         */
		EventCategory_ GetEventCategory()const override { return EventCategory_kInputEvent; }
        /**
         * @brief Returns the key code associated with this event.
         *
         * This method retrieves the key code that corresponds to the specific key
         * involved in the keyboard event. The key code is represented by the
         * `KeyboardKey_` enum, which includes values for all possible keys on the
         * keyboard.
         *
         * @returns The key code as an enum value of type `KeyboardKey_`.
         */
		KeyboardKey_ GetKeyCode()const { return m_key; }
        /**
         * @brief Returns the action performed on the key.
         *
         * This method retrieves the action that was performed on the key, such as
         * pressing, releasing, or repeating the key. 
         *
         * @returns The action as an integer.
         */
		int GetKeyAction()const { return m_action; }
        /**
         * @brief Returns the modifier keys that were active during the event.
         *
         * This method retrieves the state of modifier keys (e.g., Shift, Control, Alt)
         * at the time the keyboard event occurred.
         *
         * @returns The active modifier keys as an integer.
         */
		int GetKeyMod()const { return m_mod; }
        /**
         * @brief Returns the scan code of the key.
         *
         * This method retrieves the scan code of the key involved in the event. The
         * scan code is a hardware-specific code that identifies the physical key on
         * the keyboard, independent of the key's mapped character or function.
         *
         * @returns The scan code as an integer.
         */
		int GetKeyScanCode()const { return m_scanCode; }
	private:
		KeyboardKey_ m_key; ///< The key code associated with this event.
        int m_scanCode;     ///< The scan code of the key.
        int m_action;       ///< The action performed on the key.
        int m_mod;          ///< The modifier keys active during the event.
	};

    /**
     * @brief Class representing a window resize event.
     *
     * This class is called when the window is resized.
     * Unlike the 'SceneViewportResizeEvent' this is for the actual window instead of the rendering part of the application.
     * 
     * The event carries the new size of the window. And its created by the Window class.
     */
	class WindowResizeEvent : public Event
	{
	public:
        /**
         * @brief Constructor sets the event information data. 
         */
		WindowResizeEvent(Size2D newSize):m_windowNewSize(newSize)
		{}
        /**
         * @brief Returns the event type enum. See EventType_ for different types.
         *
         * @returns Returns the type of event defined by the enum 'EventType_'.
         */
		EventType_ GetEventType()const override { return EventType_kWindowSizeChanged; }
        /**
         * @brief Returns the category of event enum. See EventCategory_ for different types.
         *
         * @returns Returns the category of event defined by the enum 'EventCategory_'.
         */
		EventCategory_ GetEventCategory()const override { return EventCategory_kWindowEvent; }
        /**
         * @brief Returns the new window size carried by the event.
         * 
         * @returns Returns the new window size.
         */
		Size2D GetWindowSize()const { return m_windowNewSize; }
	private:
		Size2D m_windowNewSize; ///< The event new window size.
	};

    /**
     * @brief Event representing a scroll input.
     *
     * This event is invoked whenever the scroll position is changed.
     * By position I mean the offset of the scroll wheel.
     * 
     * The event carries the information only for the X and Y scroll offset.
     * 
     * This event is invoked by the Window class witch has a callback called by GLFW. Because of 
     * GLFW implementation the scroll event carries a X_offset and Y_offset. For regular mouse
     * use X_offset. See the GLFW docs for more information about the Y_offset: https://www.glfw.org/docs/3.3/input_guide.html#scrolling
     */
    class ScrollPositionChanged : public Event
    {
    public:
        /**
         * @brief Constructor sets the scroll offset information. 
         */
        ScrollPositionChanged(Position2D scrollChange) : m_scrollChange(scrollChange) 
        {}
        /**
         * @brief Returns the event type enum. See EventType_ for different types.
         *
         * @returns Returns the type of event defined by the enum 'EventType_'.
         */
        EventType_ GetEventType()const override { return EventType_kMouseScrollChanged; }
        /**
         * @brief Returns the category of event enum. See EventCategory_ for different types.
         *
         * @returns Returns the category of event defined by the enum 'EventCategory_'.
         */
        EventCategory_ GetEventCategory()const override { return EventCategory_kInputEvent; }
        /**
         * @brief Returns the scroll position readonly ref.
         *
         * @returns Returns the event scroll position data readonly ref.
         */
        const Position2D& GetScrollPosition()const { return m_scrollChange; }
    private:
        Position2D m_scrollChange; ///< Event scroll position.
    };

}