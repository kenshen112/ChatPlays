Project(ChatPlays)
cmake_minimum_required(VERSION 3.20.2)
message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(CMAKE_BUILD_TYPE Debug)

find_package(Threads)
set(THREADS_PREFER_PTHREAD_FLAG ON) # Without this flag CMake may resort to just '-lpthread'

add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty)

if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
    find_package(PkgConfig REQUIRED)

    include_directories(${CMAKE_SOURCE_DIR}/Header ${EVDEV_INCLUDE_DIRS})
    link_directories(${CMAKE_SOURCE_DIR}/src ${EVDEV_LIBRARIES})

    pkg_search_module(EVDEV REQUIRED libevdev)
    set (SOURCES src/Linux/control.cpp src/main.cpp src/twitch.cpp src/Linux/connect.cpp src/settings.cpp src/PineClient.cpp src/message.cpp)

    add_executable(ChatPlays ${SOURCES})
    target_link_libraries(ChatPlays PRIVATE ${EVDEV_LIBRARIES} Threads::Threads ThirdParty)
endif()

if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    set (SOURCES src/Windows/control.cpp src/main.cpp src/twitch.cpp src/Windows/winConnect.cpp src/settings.cpp src/PineClient.cpp src/message.cpp)
    add_executable(ChatPlays ${SOURCES})
    target_include_directories(ChatPlays PRIVATE ${CMAKE_SOURCE_DIR}/Header)
    target_link_libraries(ChatPlays PRIVATE Threads::Threads ViGEmClient ThirdParty ws2_32)
endif()

