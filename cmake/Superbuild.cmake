## The Superbuild script is used to automatically download and build project dependencies. 

# Using GIT to download third party libraries. An SVN/BitBucket URL will also work the same way
FIND_PACKAGE( Git REQUIRED )

OPTION( USE_GIT_PROTOCOL "If behind a firewall turn this off to use http instead." ON)

SET(git_protocol "git")
if(NOT USE_GIT_PROTOCOL)
	SET(git_protocol "http")
endif()

SET( CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_MODULE_PATH}
)

INCLUDE( ExternalProject )

# Compute -G arg for configuring external projects with the same CMake generator:
if(CMAKE_EXTRA_GENERATOR)
	SET(gen "${CMAKE_EXTRA_GENERATOR} - ${CMAKE_GENERATOR}")
else()
	SET(gen "${CMAKE_GENERATOR}" )
endif()

SET( NewCore_DEPENDENCIES )

# Automatic VTK build and link
OPTION( USE_VTK "Build VTK v5.10.1" OFF )
if( ${USE_VTK} )
	FIND_PACKAGE( VTK REQUIRED )
	if( NOT VTK_DIR )
		MESSAGE( STATUS "VTK not found on system. Building from source..." )
		INCLUDE( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/External-VTK.cmake )
	endif(  )
	SET( NewCore_DEPENDENCIES ${NewCore_DEPENDENCIES} VTK )
endif()

# Automatic ITK build and link
OPTION( USE_ITK "Use ITK v4.4.2" ON )
if( ${USE_ITK} )
	FIND_PACKAGE( ITK REQUIRED )
	if( NOT ITK_DIR )
		MESSAGE( STATUS "ITK not found on system. Building from source..." )
		INCLUDE( ${CMAKE_CURRENT_SOURCE_DIR}/cmake/External-ITK.cmake )
	endif( )
	SET( NewCore_DEPENDENCIES ${NewCore_DEPENDENCIES} ITK )
endif( )
