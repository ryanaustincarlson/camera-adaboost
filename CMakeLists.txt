# Top level cmake
cmake_minimum_required(VERSION 2.8)
project(AdaBoost-Webcam)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(.)

set(CMAKE_CXX_FLAGS "-g -Wall")
add_subdirectory(adaboost)
add_subdirectory(features)
add_subdirectory(trainer)
add_subdirectory(tester)
add_subdirectory(classifiedviewer)
add_subdirectory(utils)

set ( PROGRAMS runadaboost )

FOREACH( program ${PROGRAMS} )
  ADD_EXECUTABLE( ${program} ${program}.cpp )
  TARGET_LINK_LIBRARIES( ${program} imageExtraction adaboost imageTesting grid utils
      ${QT_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY} )
ENDFOREACH( program )
