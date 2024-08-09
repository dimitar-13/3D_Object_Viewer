set(INPUT_SOURCES
App_Logger.cpp
)

set(INPUT_HEADERS
App_Logger.h
)

target_sources(${PROJECT_NAME} PRIVATE ${INPUT_SOURCES} ${INPUT_HEADERS})