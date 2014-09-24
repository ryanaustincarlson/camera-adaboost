# utils cmake
cmake_minimum_required(VERSION 2.6)
PROJECT( Summer_09 )

#SET( PROGRAMS )

ADD_LIBRARY(utils
            utils.cpp)

#FOREACH( program ${PROGRAMS} )
#  ADD_EXECUTABLE( ${program} ${program}.cpp )
#  TARGET_LINK_LIBRARIES( ${program} utils 
#    ${Boost_LIBRARIES} ${GDAL_LIBRARY})
#ENDFOREACH( program )
