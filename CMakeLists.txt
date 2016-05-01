project(raw2any)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

FIND_PACKAGE( Qt4 REQUIRED )

SET( QT_USE_QTCORE TRUE)
SET( QT_DEBUG TRUE)

INCLUDE(${QT_USE_FILE})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
