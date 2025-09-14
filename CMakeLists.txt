cmake_minimum_required(VERSION 3.10)
project(Gravity_simulator VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_executable(Gravity_simulator 
    src/main.cpp    
    src/glad.c
    src/config.cpp
    src/Object.cpp
    )

target_include_directories(Gravity_simulator PRIVATE dependencies/include)

target_link_directories(Gravity_simulator PRIVATE dependencies/lib-vc2022)

target_link_libraries(Gravity_simulator PRIVATE glfw3 OpenGL::GL)