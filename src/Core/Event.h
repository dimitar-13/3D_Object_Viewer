#pragma once
#include "Enums/EventTypeCategoryEnums.h"
namespace OBJ_Viewer {

    /**
     * @brief Event interface used by the application.
     *
     * This class is a interface for every application event. You must implement it
     * if you want to add your custom event to the app.
     *
     * Every event must implement the 2 pure virtual functions:
     * - GetEventType() : This function returns a enum 'EventType_' that represents the event type/class.
     * - GetEventCategory() : This function returns a enum 'EventCategory_' that categorizes the event see the enum for more info.
     *
     * Example use:
     *
     * ```cpp
     *
     * void OnEventListenerImpl(Event& e)
     * {
     *     if(e.GetEventType() == EventType_kKeyPressed)
     *     {
     *         MouseKeyEvent& casted_mouse_event = dynamic_cast<MouseKeyEvent&>(e);
     *     }
     * ...etc.
     * }
     * ```
     */
    class Event
    {
    public:
        Event() = default;
        /**
         * @brief Retrieves the specific type of the event.
         *
         * This function exist for 2 reasons:
         * - To identify the event class/type.
         * - To make other events a polymorphic types (for a polymorphic type a class must override a function
         * that's why we override instead of keeping the 'EventType_' and 'EventCategory_' as member variables. )
         *
         * @returns The type of the event as defined by the enum `EventType_`.
         */
        virtual EventType_ GetEventType()const = 0;
        /**
         * @brief Retrieves the specific type of the event.
         *
         * This function exist for 2 reasons:
         * - To identify the event class/type.
         * - To make other events a polymorphic types (for a polymorphic type a class must override a function
         * that's why we override instead of keeping the 'EventType_' and 'EventCategory_' as member variables. )
         *
         * @returns The type of the event as defined by the enum `EventCategory_`.
         */
        virtual EventCategory_ GetEventCategory()const = 0;
    };

    /**
     * @brief Interface for event listener.
     *
     * Implement this interface to be able to listen for application events.
     *
     * Every listener is notified by the application. See 'Application::AddEventListener()' on how to add
     * your class as a listener.
     */
    class Listener
    {
    public:
        /**
         * @brief Function that is called when an event is invoked.
         *
         * Currently every event listener submitted to the Application class is called no matter if they are interested
         * or not in the event.
         *
         * @param e Polymorphic type representing the event. See the Event class on more information on how to extract information about the
         * event.
         */
        virtual void OnEvent(Event& e) = 0;
    };
}