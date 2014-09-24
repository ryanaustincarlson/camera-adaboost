# Trainer Gui
cmake_minimum_required(VERSION 2.8)
project(AdaBoost-Webcam)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set ( trainer_sources trainergui.cpp main.cpp )
set ( trainer_headers trainergui.h )
set ( trainer_ui trainergui.ui )
qt4_wrap_cpp(trainer_mocs ${trainer_headers})
qt4_wrap_ui(trainer_ui ${trainer_ui})

set ( clicklabel_sources clicklabel.cpp )
set ( clicklabel_headers clicklabel.h )
qt4_wrap_cpp( clicklabel_mocs ${clicklabel_headers} )
add_library( clicklabel clicklabel.cpp ${clicklabel_mocs} )

add_executable( trainer ${trainer_sources} ${trainer_mocs} ${trainer_ui} )
target_link_libraries(trainer clicklabel ${QT_LIBRARIES})

# test program -- TODO remove
add_executable( testMain testMain.cpp )
target_link_libraries( testMain clicklabel ${QT_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY} )