##############################################################################
# \file  CMakeLists.txt
#
# \brief Build configuration for Code/Source directory.
#
# http://www.cbica.upenn.edu/sbia/software/
# sbia-software@uphs.upenn.edu
# 
# Copyright (c) 2015 University of Pennsylvania. All rights reserved.
# See COPYING file or http://www.cbica.upenn.edu/sbia/software/license.html
##############################################################################

CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

SET( PROJECT_NAME CBICA_Toolkit ) # change project name here

SET( PROJECT_VERSION_MAJOR "0" ) # change project major version here
SET( PROJECT_VERSION_MINOR "1" ) # change project minor version here
SET( PROJECT_VERSION_BUILD "0" ) # change project build version here
SET( PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_BUILD}" ) 
SET( RELEASE_ID "${PROJECT_VERSION}" )
SET( CONTACT_INFO "sbia-software@uphs.upenn.edu" )

PROJECT( ${PROJECT_NAME} )

ADD_DEFINITIONS( -DCONTACT_INFO="${CONTACT_INFO}" )
ADD_DEFINITIONS( -DPROJECT_NAME="${PROJECT_NAME}" )
ADD_DEFINITIONS( -DRELEASE_ID="${PROJECT_VERSION}" )
ADD_DEFINITIONS( -DPROJECT_VERSION="${PROJECT_VERSION}" )
ADD_DEFINITIONS( -DPROJECT_VERSION_MAJOR="${PROJECT_VERSION_MAJOR}" )
ADD_DEFINITIONS( -DPROJECT_VERSION_MINOR="${PROJECT_VERSION_MINOR}" )

## Super build
#INCLUDE( ${CMAKE_SOURCE_DIR}/cmake/Superbuild.cmake )

# Find all *.h and *.cpp files in a folder -> useful when the application is purely command line based
SET( SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src )
SET( BASIC_CLASS_DIRECTORY ${SOURCE_DIRECTORY}/classes )

SET( SOURCE_DIRECTORY  ${PROJECT_SOURCE_DIR}/src CACHE STRING "Source Directory" FORCE )
SET( BASIC_CLASS_DIRECTORY  ${SOURCE_DIRECTORY}/classes CACHE STRING "Source Directory" FORCE )

ADD_SUBDIRECTORY( ${BASIC_CLASS_DIRECTORY} )

GET_TARGET_PROPERTY( LIBRARY_CONFIG CBICA_Classes LOCATION )
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
  IF(MSVC)
    SET(CMAKE_CONFIGURATION_TYPES Release CACHE TYPE INTERNAL FORCE)
    MESSAGE( STATUS "NOTE: Building package in ${CMAKE_CONFIGURATION_TYPES} mode(s) only." )
  ENDIF()
  ADD_SUBDIRECTORY( wrapping )
ENDIF ()

## Installation (optional)
#SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE )

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
GET_TARGET_PROPERTY( LIBRARY_CONFIG ${PROJECT_NAME} LOCATION )
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
	ADD_SUBDIRECTORY( testing )
ENDIF( BUILD_TESTING )
