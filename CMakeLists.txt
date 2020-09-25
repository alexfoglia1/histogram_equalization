cmake_minimum_required(VERSION 2.6)
project(HistEq)

add_executable(
histeq

${CMAKE_SOURCE_DIR}/he.h
${CMAKE_SOURCE_DIR}/he.c
${CMAKE_SOURCE_DIR}/main.c
)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})
target_link_libraries(histeq ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} m)
