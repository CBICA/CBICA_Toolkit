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

CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

SET( 
  PROJECT_NAME CBICA-TK_Trunk # change this for branches, etc.
  #VERSION 0.0.2 
  ) # change project name here

SET( RELEASE_ID "${PROJECT_VERSION}" )
SET( CONTACT_INFO "sbia-software@uphs.upenn.edu" )

PROJECT( ${PROJECT_NAME} )

ADD_DEFINITIONS( -DCONTACT_INFO="${CONTACT_INFO}" )
ADD_DEFINITIONS( -DPROJECT_NAME="${PROJECT_NAME}" )
ADD_DEFINITIONS( -DRELEASE_ID="${PROJECT_VERSION}" )
ADD_DEFINITIONS( -DPROJECT_VERSION="${PROJECT_VERSION}" )
ADD_DEFINITIONS( -DBUILD_CBICA_ITK=0 )

## Super build
SET( BUILD_ITK OFF CACHE BOOL "Do you want to build ITK from source (needs Git)?" )
IF( BUILD_ITK )
  INCLUDE( ${CMAKE_SOURCE_DIR}/cmake/Superbuild.cmake )
ENDIF()

#SET( Boost_FIND_QUIETLY TRUE )
#SET( Boost_USE_MULTITHREADED TRUE )
#SET( Boost_USE_STATIC FALSE )
#FIND_PACKAGE( Boost REQUIRED 
#	COMPONENTS program_options
#	)
#ADD_DEFINITIONS("-DHAS_BOOST")
#LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})


# Find all *.h and *.cpp files in a folder -> useful when the application is purely command line based
SET( SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src )
SET( BASIC_CLASS_DIRECTORY ${SOURCE_DIRECTORY}/classes )

ADD_SUBDIRECTORY( ${BASIC_CLASS_DIRECTORY} )

FIND_PACKAGE(OpenMP REQUIRED)
SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}" )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" )

# Done for temporary testing
FILE( GLOB 
  MainSources
  "${SOURCE_DIRECTORY}/*.h"
  "${SOURCE_DIRECTORY}/*.cpp"
  "${BASIC_CLASS_DIRECTORY}/*.h"
  "${BASIC_CLASS_DIRECTORY}/*.cpp"
)

## Do you want to build the ITK classes as well?
SET( BUILD_CBICA_ITK_CLASSES ON CACHE BOOL "Do you want to build the CBICA ITK classes (needs ITK 4.7.0+)?" )

ADD_DEFINITIONS( -DBUILD_CBICA_ITK_CLASSES="${${BUILD_CBICA_ITK_CLASSES}}" )
IF( BUILD_CBICA_ITK_CLASSES )

	#ADD_DEFINITIONS( -DBUILD_CBICA_ITK=1 )	
	FIND_PACKAGE( ITK REQUIRED )
	#INCLUDE(${USE_ITK_FILE})
	#SET( ITK_CLASS_DIRECTORY ${SOURCE_DIRECTORY}/classes/itk )
  #ADD_SUBDIRECTORY( ${SOURCE_DIRECTORY}/classes/itk )
	# Done for temporary testing
	#FILE( GLOB CBICAITKSources
	#    "${ITK_CLASS_DIRECTORY}/*.h"
	#    "${ITK_CLASS_DIRECTORY}/*.cpp"
	#)
  
  #SET( MainSources ${MainSources} ${ITK_Class_Headers} ${ITK_Class_Sources} )

ENDIF( BUILD_CBICA_ITK_CLASSES ) 

# If Windows is detected, add getopt for command line parsing. Will be deprecated in favor of cbica::CmdParser
IF( WIN32 )
	SET( MainSources
		${MainSources}
		${SOURCE_DIRECTORY}/windows/getopt.h
		${SOURCE_DIRECTORY}/windows/getopt.cpp
		${SOURCE_DIRECTORY}/windows/dirent.h
	)
ENDIF( WIN32 )

SET( INCLUDE_DIR_CONFIG "\"${PROJECT_SOURCE_DIR}/src/classes\"" )
#GET_TARGET_PROPERTY( LIBRARY_CONFIG CBICA_Classes LOCATION )
SET( LIBRARY_CONFIG "\"${LIBRARY_CONFIG}\"" )

### Building the documentation
## The following 4 lines are for generation of public documentation
SET( BUILD_DOCUMENTATION ON CACHE BOOL "Do you want to build Documentation (needs Doxygen)?" )
IF( BUILD_DOCUMENTATION )
  ADD_SUBDIRECTORY( docs )
ENDIF ()

## This documentation is for the SBIA website ONLY. Remove these lines AND the "docs_internal" folder before publishing
## under "tags".
#SET( BUILD_DOCUMENTATION_INTERNAL OFF CACHE BOOL "Do you want to build Documentation (Needs Doxygen)." )
#IF( BUILD_DOCUMENTATION_INTERNAL )
#    ADD_SUBDIRECTORY( docs_internal )
#ENDIF ()

### Python wrapping
SET( BUILD_PYTHON_WRAP OFF CACHE BOOL "Do you want to wrap classes (needs SWIG 3.0.4)? Ensure build option matches Python lib build." )
IF( BUILD_PYTHON_WRAP )
  IF(WIN32)
    MESSAGE( STATUS "If Python has not been built on Debug mode (i.e., pythonXX_d.lib is NOT present), then only build library in Release." )
  ENDIF()
  ADD_SUBDIRECTORY( wrapping )
ENDIF ()

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${BASIC_CLASS_DIRECTORY}
	${SOURCE_DIRECTORY}
	#${Boost_INCLUDE_DIRS}
)

## Keep all executables in a single directory
#IF( WIN32 )
#	SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/executable )
#	SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/executable )
#ELSE( WIN32 )
#	SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/executable )
#ENDIF( WIN32 )

SET( EXE_NAME ${PROJECT_NAME} CACHE STRING "Executable name" FORCE)

#ADD_EXECUTABLE( 
#	${EXE_NAME} 
#	${MainSources} 
#	${SOURCE_DIRECTORY}/main.cxx
#)

## Add c++11 flag to compilation if GCC is detected under UNIX: how does this translate to MinGW/Cygwin??
IF(CMAKE_COMPILER_IS_GNUCXX)
	#INCLUDE( CheckCXXCompilerFlag )
	#CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	#CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	#IF( COMPILER_SUPPORTS_CXX11 )
	#	SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	#ELSEIF(COMPILER_SUPPORTS_CXX0X )
	#	SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	#ELSE()
	#	MESSAGE(ERROR " The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	#ENDIF()
ENDIF(CMAKE_COMPILER_IS_GNUCXX) 

## Link libraries
#IF( BUILD_CBICA_ITK_CLASSES )
#	TARGET_LINK_LIBRARIES( 
#		${EXE_NAME} 
#		${ITK_LIBRARIES}
#	)
#ENDIF( BUILD_CBICA_ITK_CLASSES )

## Installation (optional)
#SET_TARGET_PROPERTIES( ${EXE_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE )

SET( PACKAGE_NAME ${PROJECT_NAME} )

IF( WIN32 )
  SET( INSTALL_CONFIG_DIR "cmake" )
ELSE()
  SET( INSTALL_CONFIG_DIR "lib" )
ENDIF()

FILE( RELATIVE_PATH DIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_CONFIG_DIR}" "${CMAKE_INSTALL_PREFIX}" )
STRING( REGEX REPLACE "/$|\\$" "" DIR "${DIR}" )

SET( INSTALL_DIR_CONFIG "${CMAKE_INSTALL_PREFIX}" CACHE INTERNAL "Install location")

#SET( INCLUDE_DIR_CONFIG "\"\${CMAKE_CURRENT_LIST_DIR}/include\"" )
SET( INCLUDE_DIR_CONFIG "\"${INSTALL_DIR_CONFIG}/include/\"" )
#GET_TARGET_PROPERTY( LIBRARY_CONFIG ${PROJECT_NAME} LOCATION )
GET_FILENAME_COMPONENT( LIBRARY_CONFIG "${LIBRARY_CONFIG}" NAME )
SET( LIBRARY_CONFIG "\"${INSTALL_DIR_CONFIG}/lib/${LIBRARY_CONFIG}\"" )

CONFIGURE_FILE( "config/Config.cmake.in" "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}InstallConfig.cmake" @ONLY )
CONFIGURE_FILE( "config/Use.cmake.in" "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}Use.cmake" @ONLY )
CONFIGURE_FILE( "config/ConfigVersion.cmake.in" "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}ConfigVersion.cmake" @ONLY )

INSTALL(
  FILES       ${PROJECT_BINARY_DIR}/${PACKAGE_NAME}InstallConfig.cmake 
  DESTINATION ${INSTALL_DIR_CONFIG}
  RENAME      ${PACKAGE_NAME}Config.cmake
)

INSTALL(
  FILES       ${PROJECT_BINARY_DIR}/${PACKAGE_NAME}Use.cmake
  DESTINATION ${INSTALL_DIR_CONFIG}
)

INSTALL(
  FILES       ${PROJECT_BINARY_DIR}/${PACKAGE_NAME}ConfigVersion.cmake
  DESTINATION ${INSTALL_DIR_CONFIG}
)


## Build test(s)
SET( BUILD_TESTING TRUE CACHE BOOL "Build testing" )
IF( BUILD_TESTING )
	ENABLE_TESTING()
	#ADD_SUBDIRECTORY( testing )
ENDIF( BUILD_TESTING )
