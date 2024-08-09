
set(INPUT_SOURCES
    InputHandler.cpp
)

set(INPUT_HEADERS
    AppKeyBindings.h
    InputHandler.h
    KeyboardKeys.h
    MouseKeys.h
)

target_sources(${PROJECT_NAME} PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})