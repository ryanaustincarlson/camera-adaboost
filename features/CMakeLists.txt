# Feature Extraction
cmake_minimum_required(VERSION 2.8)
project(AdaBoost-Webcam)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_library( imageExtraction
             ImageTransforms.cpp
             TrainingParser.cpp
             ImageFeatures.cpp
             HaarFeature.cpp)
             
