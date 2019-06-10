##############################################################################
# \file  CMakeLists.txt
#
# \brief Build configuration for Code/Source directory.
#
# https://www.cbica.upenn.edu/sbia/software/
# software@cbica.upenn.edu
# 
# Copyright (c) 2015 University of Pennsylvania. All rights reserved.
# See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html
##############################################################################

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} 
                     ${BASIC_CLASS_DIRECTORY}
)

# add current directory to list of cached include directories
SET( CACHED_INCLUDE_DIRS
  ${CACHED_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR} )

SET( CBICA_OPENCV_LIB_NAME CBICA_OPENCV )
ADD_DEFINITIONS( -DCBICA_OPENCV_LIB_NAME="${CBICA_OPENCV_LIB_NAME}" )

FIND_PACKAGE(OpenCV 3.0 REQUIRED QUIET)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

SET( OPENCV_Class_Headers 
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaOPENCVUtilities.h  
)

SET( OPENCV_Class_Sources 
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaOPENCVUtilities.cpp
)

SET( OPENCV_CLASSES_DIR    ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "OPENCV CLASS INCLUDES" FORCE )
SET( OPENCV_Class_Headers  ${OPENCV_Class_Headers}        CACHE STRING "OPENCV headers"        FORCE )
SET( OPENCV_Class_Sources  ${OPENCV_Class_Sources}        CACHE STRING "OPENCV sources"        FORCE )
SET( OPENCV_CLASS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "OPENCV classes directory" FORCE )
SET( OPENCV_CLASS_LIBRARY ${CBICA_OPENCV_LIB_NAME} CACHE STRING "OPENCV classes library" FORCE )

# keep source files structured as folders for clarity
SOURCE_GROUP( OPENCV-Class_Headers FILES ${OPENCV_Class_Headers})
SOURCE_GROUP( OPENCV-Class_Sources FILES ${OPENCV_Class_Sources})
SOURCE_GROUP( Basic_Headers FILES ${CBICA_Headers})
SOURCE_GROUP( Basic_Sources FILES ${CBICA_Sources})

MARK_AS_ADVANCED ( OPENCV_CLASSES_DIR )
MARK_AS_ADVANCED ( OPENCV_Class_Headers )
MARK_AS_ADVANCED ( OPENCV_Class_Sources )

ADD_LIBRARY( 
  ${CBICA_OPENCV_LIB_NAME}
  ${OPENCV_Class_Headers}
  ${OPENCV_Class_Sources}
  ${CBICA_Headers}
  ${CBICA_Sources}
)

SET_TARGET_PROPERTIES( ${CBICA_OPENCV_LIB_NAME} PROPERTIES FOLDER "CBICA_Toolkit")

SET(CMAKE_DEBUG_POSTFIX "d")
# OR set_target_properties(mylib PROPERTIES DEBUG_POSTFIX "d")

TARGET_LINK_LIBRARIES(
  ${CBICA_OPENCV_LIB_NAME} 
  ${OpenCV_LIBRARIES}
)

# install the library
INSTALL(
  FILES ${OPENCV_Class_Headers} 
  DESTINATION include
)

INSTALL( 
	TARGETS ${CBICA_OPENCV_LIB_NAME} 
	DESTINATION lib 
)

IF( BUILD_TESTING )
	ENABLE_TESTING()
  #INCLUDE(CTest)
  ADD_SUBDIRECTORY( ${CMAKE_CURRENT_SOURCE_DIR}/testing )
ENDIF()
