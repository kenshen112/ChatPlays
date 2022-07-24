Project(UI)
cmake_minimum_required(VERSION 3.20.2)
message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")

set (SOURCES 
src/Window.cpp
src/SettingsWindow.cpp
src/settings.cpp 
src/MainWindow.cpp
src/TwitchWindow.cpp
)

add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty ${CMAKE_CURRENT_BINARY_DIR}/3rdparty)

add_library(UI STATIC ${SOURCES})
target_include_directories(UI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Header)
target_link_libraries(UI PUBLIC ThirdParty)