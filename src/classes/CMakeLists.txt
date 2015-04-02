##############################################################################
# \file  CMakeLists.txt
#
# \brief Build configuration for Code/Source directory.
#
# https://www.cbica.upenn.edu/sbia/software/
# sbia-software@uphs.upenn.edu
# 
# Copyright (c) 2015 University of Pennsylvania. All rights reserved.
# See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html
##############################################################################

SET( ExtraSources )

SET( CBICA_LIB_NAME CBICA_Classes )
ADD_DEFINITIONS( -DCBICA_LIB_NAME="${CBICA_LIB_NAME}" )

IF( @BUILD_CBICA_ITK_CLASSES@ )

  ADD_SUBDIRECTORY( itk )
  INCLUDE( ${USE_ITK_FILE} )
	INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/itk )
  
ENDIF( @BUILD_CBICA_ITK_CLASSES@ )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} )

# If Windows is detected, add getopt for command line parsing
IF( WIN32 )
	SET( WinHeaders ${PROJECT_SOURCE_DIR}/src )
	SET( ExtraSources ${ExtraSources} ${WinHeaders}/windows/dirent.h ${WinHeaders}/windows/getopt.h ${WinHeaders}/windows/getopt.cpp )
	INCLUDE_DIRECTORIES( ${WinHeaders} )
ENDIF( WIN32 )

# select all headers. it is manual addition for the explicit reason of view-ability (can be made automatic in a future build)
SET( CBICA_Headers
	cbicaLogging.h
	cbicaMacros.h
	cbicaUtilities.h
	cbicaCmdParser.h
)

# select all implementations. it is manual addition for the explicit reason of view-ability (can be made automatic in a future build)
SET( CBICA_Sources
	cbicaLogging.cpp
	cbicaUtilities.cpp
	cbicaCmdParser.cpp
)

# add ITK classes and implementations if BUILD_CBICA_ITK_CLASSES is enabled
IF( @BUILD_CBICA_ITK_CLASSES@ )	
	SET( CBICA_Headers
		${CBICA_Headers}
		${ITK_Class_Headers}
	)
  SET( CBICA_Sources
		${CBICA_Sources}
		${ITK_Class_Sources}
	)
ENDIF( @BUILD_CBICA_ITK_CLASSES@ )

# build static lib for Windows and shared lib for Unix: why?
IF(WIN32)
  ADD_LIBRARY( ${CBICA_LIB_NAME}
        ${ExtraSources}
        ${CBICA_Headers}
        ${CBICA_Sources}
  )
ELSE()
  ADD_LIBRARY( ${CBICA_LIB_NAME} SHARED
        ${ExtraSources}
        ${CBICA_Headers}
        ${CBICA_Sources}
  )
ENDIF()

# link ITK library to built classes if BUILD_CBICA_ITK_CLASSES is enabled
IF( @BUILD_CBICA_ITK_CLASSES@ )	
	TARGET_LINK_LIBRARIES( 
		${CBICA_LIB_NAME} 
		${ITK_LIBRARIES}
	)
ENDIF( @BUILD_CBICA_ITK_CLASSES@ )

## Add c++11 flag to compilation if GCC is detected under UNIX: how does this translate to MinGW/Cygwin??
IF(UNIX)
	INCLUDE( CheckCXXCompilerFlag )
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	IF( COMPILER_SUPPORTS_CXX11 )
		SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	ELSEIF(COMPILER_SUPPORTS_CXX0X )
		SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	ELSE()
		MESSAGE(ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	ENDIF()
ENDIF(UNIX) 

# install the library
INSTALL(
  FILES ${CBICA_Headers} 
  DESTINATION include
)

INSTALL( 
	TARGETS ${CBICA_LIB_NAME} 
	DESTINATION lib 
)

#MESSAGE( STATUS "cbica_class binary dir: " ${CMAKE_CURRENT_BINARY_DIR} )

#CONFIGURE_FILE(
#  ${CMAKE_CURRENT_BINARY_DIR}/${CBICA_LIB_NAME}
#  ${CMAKE_BINARY_DIR}/lib/${CBICA_LIB_NAME} COPYONLY )