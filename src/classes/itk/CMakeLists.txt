##############################################################################
# \file  CMakeLists.txt
# \brief Build configuration for Code/Source directory.
#
# Copyright (c) 2011 University of Pennsylvania. All rights reserved.
# See COPYING file or https://www.rad.upenn.edu/sbia/software/license.html.
#
# Contact: SBIA Group <sbia-software at uphs.upenn.edu>
##############################################################################

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} )


SET( ITK_Class_Headers 
	cbicaITKImageInfo.h 
)
INSTALL(FILES ${Headers} DESTINATION include)
SET( ITK_Class_Sources 
	cbicaITKImageInfo.cpp 
)

SET( ITK_CLASSES_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "ITK CLASS INCLUDES" FORCE)
MARK_AS_ADVANCED ( ITK_CLASSES_DIR )

ADD_LIBRARY( ITK_Classes ${ITK_Class_Headers} ${ITK_Class_Sources} )

INSTALL( TARGETS ITK_Classes DESTINATION ${PROJECT_BINARY_DIR}/lib )