##############################################################################
# \file  CMakeLists.txt
# \brief Build configuration for Code/Source directory.
#
# Copyright (c) 2011 University of Pennsylvania. All rights reserved.
# See COPYING file or https://www.rad.upenn.edu/sbia/software/license.html.
#
# Contact: SBIA Group <sbia-software at uphs.upenn.edu>
##############################################################################
SET( ExtraSources )
ADD_SUBDIRECTORY( itk )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${ITK_CLASSES_DIR} )

# If Windows is detected, add getopt for command line parsing
IF( WIN32 )
	SET( WinHeaders ${PROJECT_SOURCE_DIR}/src )
	SET( ExtraSources ${ExtraSources} ${WinHeaders}/windows/dirent.h ${WinHeaders}/windows/getopt.h ${WinHeaders}/windows/getopt.cpp )
	INCLUDE_DIRECTORIES( ${WinHeaders} )
ENDIF( WIN32 )

#install all header files
SET( CBICA_Headers
	cbicaLogging.h
	cbicaMacros.h
	cbicaUtilities.h
	${ITK_Class_Headers}
)
INSTALL(FILES ${CBICA_Headers} DESTINATION include)

SET( CBICA_Sources
	cbicaUtilities.cpp
	${ITK_Class_Sources}
)

ADD_LIBRARY( CBICA_Basic_Classes
			${ExtraSources}
			${CBICA_Headers}
			${CBICA_Sources}
)

INSTALL( 
	TARGETS CBICA_Basic_Classes 
	DESTINATION lib 
)