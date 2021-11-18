cmake_minimum_required(VERSION 3.7)
project(EulerOperation-zwk)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework OpenGL")

set(SOURCE_FILES ../src/main.hpp ../src/main.cpp 
../src/EulerOperation.hpp ../src/EulerOperation.cpp 
../src/userInteraction.hpp ../src/userInteraction.cpp 
../src/display.hpp ../src/display.cpp
../src/CreateSolid.hpp ../src/CreateSolid.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})


if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    SET(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif (APPLE)

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if (OPENGL_FOUND)
    message("OPENGL_FOUND=${OPENGL_INCLUDE_DIR}")
    link_libraries(${OPENGL_LIBRARIES})
endif()

# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
if (glfw3_FOUND)
    message("GLFW_FOUND=${GLFW_INCLUDE_DIRS}")
    link_libraries(${GLFW_LIBRARIES})
endif()

# glm
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
if (glm_FOUND)
    message("GLM_FOUND=${GLM_INCLUDE_DIRS}")
    link_libraries(${glm_LIBRARIES})
endif()


target_link_libraries(
        ${PROJECT_NAME}
        ${GLFW_LIBRARIES}
        glfw
        ${OPENGL_LIBRARIES}
        ${GLM_LIBRARIES}
        ${EXTRA_LIBS}
)
