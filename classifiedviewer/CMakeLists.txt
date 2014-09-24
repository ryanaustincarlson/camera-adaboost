# Classified Viewer 
cmake_minimum_required(VERSION 2.8)
project(AdaBoost-Webcam)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set ( cviewer_sources classifiedviewer.cpp main.cpp )
set ( cviewer_headers classifiedviewer.h )
set ( cviewer_ui classifiedviewer.ui )
qt4_wrap_cpp(cviewer_mocs ${cviewer_headers})
qt4_wrap_ui(cviewer_ui ${cviewer_ui})

add_executable( classifiedviewer ${cviewer_sources} ${cviewer_mocs} ${cviewer_ui} )
target_link_libraries(classifiedviewer grid ${QT_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})
