cmake_minimum_required(VERSION 3.5)

project(OS-project1 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Charts REQUIRED)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/node.cpp
        src/node.h
        src/drilldownchart.cpp
        src/drilldownchart.h
        src/drilldownslice.cpp
        src/drilldownslice.h
        src/drillChartView.cpp
        src/drillChartView.h
        )

find_package(Qt5Charts)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Charts)