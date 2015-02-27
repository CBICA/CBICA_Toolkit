CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

SET( PROJECT_NAME CBICA_Toolkit ) # change project name here

SET( PROJECT_VERSION "0.0.0" ) # change project version here
SET( RELEASE_ID "${PROJECT_VERSION}" )
SET( CONTACT_INFO "sbia-software@uphs.upenn.edu" )

PROJECT( ${PROJECT_NAME} )

ADD_DEFINITIONS( -DCONTACT_INFO="${CONTACT_INFO}" )
ADD_DEFINITIONS( -DPROJECT_NAME="${PROJECT_NAME}" )
ADD_DEFINITIONS( -DRELEASE_ID="${PROJECT_VERSION}" )
ADD_DEFINITIONS( -DPROJECT_VERSION="${PROJECT_VERSION}" )

INCLUDE( ${CMAKE_SOURCE_DIR}/cmake/Superbuild.cmake )

FIND_PACKAGE( ITK REQUIRED )
INCLUDE(${USE_ITK_FILE})

#SET( Boost_FIND_QUIETLY TRUE )
#SET( Boost_USE_MULTITHREADED TRUE )
#SET( Boost_USE_STATIC FALSE )
#FIND_PACKAGE( Boost REQUIRED 
#	COMPONENTS program_options
#	)
#ADD_DEFINITIONS("-DHAS_BOOST")
#LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

#FIND_PACKAGE( PythonLibs REQUIRED )

# Find all *.h and *.cpp files in a folder -> useful when the application is purely command line based
SET( SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src )
SET( BASIC_CLASS_DIRECTORY ${SOURCE_DIRECTORY}/classes )
SET( ITK_CLASS_DIRECTORY ${SOURCE_DIRECTORY}/classes/itk )

ADD_SUBDIRECTORY( ${BASIC_CLASS_DIRECTORY} )

# Done for temporary testing
FILE( GLOB MainSources
    "${SOURCE_DIRECTORY}/*.h"
    "${SOURCE_DIRECTORY}/*.cpp"
    "${BASIC_CLASS_DIRECTORY}/*.h"
    "${BASIC_CLASS_DIRECTORY}/*.cpp"
    "${ITK_CLASS_DIRECTORY}/*.h"
    "${ITK_CLASS_DIRECTORY}/*.cpp"
)

# If Windows is detected, add getopt for command line parsing
IF( WIN32 )
	SET( MainSources
		${MainSources}
		${SOURCE_DIRECTORY}/windows/getopt.h
		${SOURCE_DIRECTORY}/windows/getopt.cpp
		${SOURCE_DIRECTORY}/windows/dirent.h
	)
ENDIF( WIN32 )

SET( INCLUDE_DIR_CONFIG "\"${PROJECT_SOURCE_DIR}/src/classes\"" )
GET_TARGET_PROPERTY( LIBRARY_CONFIG CBICA_Classes LOCATION )
SET( LIBRARY_CONFIG "\"${LIBRARY_CONFIG}\"" )

### Building the documentation
## The following 4 lines are for generation of public documentation
SET( BUILD_DOCUMENTATION ON CACHE BOOL "Do you want to build Documentation (Needs Doxygen)." )
IF( BUILD_DOCUMENTATION )
    ADD_SUBDIRECTORY( docs )
ENDIF ()

## This documentation is for the SBIA website ONLY. Remove these lines AND the "docs_internal" folder before publishing
## under "tags".
#SET( BUILD_DOCUMENTATION_INTERNAL OFF CACHE BOOL "Do you want to build Documentation (Needs Doxygen)." )
#IF( BUILD_DOCUMENTATION_INTERNAL )
#    ADD_SUBDIRECTORY( docs_internal )
#ENDIF ()

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CLASS_DIRECTORY}
	${SOURCE_DIRECTORY}
	#${Boost_INCLUDE_DIRS}
)

ADD_EXECUTABLE( 
	${PROJECT_NAME} 
	${MainSources} 
)

LINK_DIRECTORIES( ${Boost_LIBRARY_DIR} )

TARGET_LINK_LIBRARIES( 
	${PROJECT_NAME} 
	${ITK_LIBRARIES}	
	#${Boost_LIBRARIES}
)

## Installation (optional)
SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE )


SET( PACKAGE_NAME ${PROJECT_NAME} )

IF( WIN32 )
  SET( INSTALL_CONFIG_DIR "cmake" )
ELSE()
  SET( INSTALL_CONFIG_DIR "lib" )
ENDIF()


# installation
FILE( RELATIVE_PATH DIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_CONFIG_DIR}" "${CMAKE_INSTALL_PREFIX}" )
STRING( REGEX REPLACE "/$|\\$" "" DIR "${DIR}" )

SET( INCLUDE_DIR_CONFIG "\"\${CMAKE_CURRENT_LIST_DIR}/${DIR}/include\"" )
GET_TARGET_PROPERTY( LIBRARY_CONFIG ${PROJECT_NAME} LOCATION )
GET_FILENAME_COMPONENT( LIBRARY_CONFIG "${LIBRARY_CONFIG}" NAME )
SET( LIBRARY_CONFIG "\"\${CMAKE_CURRENT_LIST_DIR}/${DIR}/lib/${LIBRARY_CONFIG}\"" )

CONFIGURE_FILE( "Config/Config.cmake.in" "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}InstallConfig.cmake" @ONLY )

INSTALL(
  FILES       "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}InstallConfig.cmake"
  DESTINATION "${INSTALL_CONFIG_DIR}"
  RENAME      "${PACKAGE_NAME}Config.cmake"
)

INSTALL(
	TARGETS	${PROJECT_NAME}
	RUNTIME DESTINATION bin
)