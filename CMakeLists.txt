cmake_minimum_required(VERSION 3.16)

project(QuizApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(QuizApp
    QuizApp_project.cpp
)

target_link_libraries(QuizApp PRIVATE Qt6::Widgets)



