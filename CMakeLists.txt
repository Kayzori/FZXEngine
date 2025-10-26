cmake_minimum_required(VERSION 3.10.0)
project(FZXEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("C:/C++ libs/glfw-3.4" glfw_build)

add_subdirectory(Engine)
add_subdirectory(ThirdParties)

add_executable(${PROJECT_NAME}
    main.cpp
    ${SRC_FILES}
    ${THIRD_PARTIES_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/Engine
    ${CMAKE_SOURCE_DIR}/ThirdParties
)

target_link_libraries(${PROJECT_NAME} glfw opengl32)

