cmake_minimum_required(VERSION 3.24)
project(QtTimelineEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets WebEngineWidgets WebChannel Multimedia Gui)

add_subdirectory(src)
add_subdirectory(plugins/grayscale)
