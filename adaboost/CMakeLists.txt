# adaboost cmake
cmake_minimum_required(VERSION 2.6)
PROJECT( Summer_09 )

#SET( PROGRAMS stuff test_adaboost)

ADD_LIBRARY(circles circles.cpp)
ADD_LIBRARY(adaboost 
  sorting.cpp 
  FeatureVector.cpp WeakClassifier.cpp 
  TrainingData.cpp AdaBooster.cpp
)

FOREACH( program ${PROGRAMS} )
  ADD_EXECUTABLE( ${program} ${program}.cpp )
  TARGET_LINK_LIBRARIES( ${program} grid circles adaboost utils)
ENDFOREACH( program )
