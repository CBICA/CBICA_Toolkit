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

SET( ExtraSources )

SET( CBICA_LIB_NAME CBICA_Basic )
ADD_DEFINITIONS( -DCBICA_LIB_NAME="${CBICA_LIB_NAME}" )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} )

FIND_PACKAGE(OpenMP REQUIRED)
SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}" )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" )

# If Windows is detected, add getopt for command line parsing
IF( WIN32 )
	#SET( WinHeaders ${PROJECT_SOURCE_DIR}/src )
	#SET( ExtraSources ${ExtraSources} ${WinHeaders}/windows/dirent.h )
	#INCLUDE_DIRECTORIES( ${WinHeaders} )
ENDIF( WIN32 )

#INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/src/thirdparty/yaml-cpp/include/ )
 
# select all headers. it is manual addition for the explicit reason of view-ability (can be made automatic in a future build)
SET( CBICA_Headers
	${CMAKE_CURRENT_SOURCE_DIR}/cbicaLogging.h
	${CMAKE_CURRENT_SOURCE_DIR}/cbicaUtilities.h
	${CMAKE_CURRENT_SOURCE_DIR}/cbicaCmdParser.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaStatistics.h
)

# select all implementations. it is manual addition for the explicit reason of view-ability (can be made automatic in a future build)
SET( CBICA_Sources
	${CMAKE_CURRENT_SOURCE_DIR}/cbicaLogging.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/cbicaUtilities.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/cbicaCmdParser.cpp
)

SET( CBICA_Headers ${CBICA_Headers} CACHE STRING "Basic headers" FORCE )
SET( CBICA_Sources ${CBICA_Sources} CACHE STRING "Basic sources" FORCE )
SET( BASIC_CLASS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Basic classes directory" FORCE )
SET( BASIC_CLASS_LIBRARY ${CBICA_LIB_NAME} CACHE STRING "Basic classes library" FORCE )

ADD_LIBRARY( ${CBICA_LIB_NAME}
  ${ExtraSources}
  ${CBICA_Headers}
  ${CBICA_Sources}
  ${YAMLCPP_Headers}
  ${YAMLCPP_Sources}
)

SET_PROPERTY( GLOBAL PROPERTY USE_FOLDERS ON)

SET_TARGET_PROPERTIES( ${CBICA_LIB_NAME} PROPERTIES FOLDER "CBICA_Toolkit")

SET(CMAKE_DEBUG_POSTFIX "d")
# OR set_target_properties(mylib PROPERTIES DEBUG_POSTFIX "d")

# keep source files structured as folders for clarity
SOURCE_GROUP( Basic_Headers FILES ${CBICA_Headers})
SOURCE_GROUP( Basic_Sources FILES ${CBICA_Sources})
SOURCE_GROUP( YAMLCPP_Headers FILES ${YAMLCPP_Headers})
SOURCE_GROUP( YAMLCPP_Sources FILES ${YAMLCPP_Sources})

# install the library
INSTALL(
  FILES ${CBICA_Headers} 
  DESTINATION include
)

INSTALL( 
	TARGETS ${CBICA_LIB_NAME} 
	DESTINATION lib 
)

IF( BUILD_TESTING )
	ENABLE_TESTING()
  INCLUDE(CTest)
  ADD_SUBDIRECTORY( ${CMAKE_CURRENT_SOURCE_DIR}/testing )
ENDIF()

# add ITK classes and implementations if BUILD_CBICA_ITK_CLASSES is enabled
IF( BUILD_CBICA_ITK_CLASSES )	
  ADD_SUBDIRECTORY( itk )
ENDIF( BUILD_CBICA_ITK_CLASSES )

# add ITK classes and implementations if BUILD_CBICA_ITK_CLASSES is enabled
IF( BUILD_CBICA_OPENCV_CLASSES )	
  ADD_SUBDIRECTORY( opencv )
ENDIF( BUILD_CBICA_OPENCV_CLASSES )

#SET( CBICA_TOOLKIT_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "CBICA_Toolkit build dir" FORCE )
#MARK_AS_ADVANCED ( CBICA_TOOLKIT_BIN_DIR )
#CONFIGURE_FILE(
#  ${CMAKE_CURRENT_BINARY_DIR}/${CBICA_LIB_NAME}
#  ${CMAKE_BINARY_DIR}/lib/${CBICA_LIB_NAME} COPYONLY )
