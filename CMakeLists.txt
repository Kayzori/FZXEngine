cmake_minimum_required(VERSION 3.10.0)
project(FZXEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("C:/C++ libs/glfw-3.4" glfw_build) # Drag GLFW source code path here
add_subdirectory("C:/C++ libs/glm-master" glm_build) # Drag GLM source code path here

add_subdirectory(src)
add_subdirectory(vendor)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${VENDOR_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/vendor
)

target_link_libraries(${PROJECT_NAME} glfw glm opengl32)

