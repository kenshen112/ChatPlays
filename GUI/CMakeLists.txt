Project(UI)
cmake_minimum_required(VERSION 3.20.2)
message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")

set (SOURCES src/Window.cpp src/SettingsWindow.cpp)

add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty ${CMAKE_CURRENT_BINARY_DIR}/3rdparty)

add_library(UI EXCLUDE_FROM_ALL ${SOURCES})
target_include_directories(UI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Header)
target_link_libraries(UI PUBLIC ThirdParty)