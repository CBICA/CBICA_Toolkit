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

SET( CBICA_ITK_LIB_NAME CBICA_ITK )
ADD_DEFINITIONS( -DCBICA_ITK_LIB_NAME="${CBICA_ITK_LIB_NAME}" )

FIND_PACKAGE(ITK REQUIRED QUIET)
INCLUDE(${ITK_USE_FILE})

FIND_PACKAGE(OpenCV 3.0 REQUIRED QUIET)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

SET( ITK_Class_Headers 
  ${CMAKE_CURRENT_SOURCE_DIR}/../cbicaUtilities.h
	${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKImageInfo.h 
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKComputeAverageMap.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKComputeDtiScalars.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKComputeVarianceMap.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKCommonHolder.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKDtiRecon.h  
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKSafeImageIO.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKUtilities.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKOpenCVUtilities.h
    
  ${CMAKE_CURRENT_SOURCE_DIR}/itkNaryMeanImageFilter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/itkNaryMeanDiffusionTensorImageFilter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/itkNaryVarianceImageFilter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/itkDiffusionTensor3DReconstructionImageFilter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/itkDTILogEuclideanCalculator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/itkN3MRIBiasFieldCorrectionImageFilter.h
  
)

SET( ITK_Class_Sources 
	${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKImageInfo.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKComputeAverageMap.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKComputeDtiScalars.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKComputeVarianceMap.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKCommonHolder.cpp
    
  ${CMAKE_CURRENT_SOURCE_DIR}/itkDiffusionTensor3DReconstructionImageFilter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/itkDTILogEuclideanCalculator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/itkN3MRIBiasFieldCorrectionImageFilter.cpp
)

SET( ITK_CLASSES_DIR    ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "ITK CLASS INCLUDES" FORCE )
SET( ITK_Class_Headers  ${ITK_Class_Headers}        CACHE STRING "ITK headers"        FORCE )
SET( ITK_Class_Sources  ${ITK_Class_Sources}        CACHE STRING "ITK sources"        FORCE )
SET( ITK_CLASS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "ITK classes directory" FORCE )
SET( ITK_CLASS_LIBRARY ${CBICA_ITK_LIB_NAME} CACHE STRING "ITK classes library" FORCE )

# keep source files structured as folders for clarity
SOURCE_GROUP( ITK-Class_Headers FILES ${ITK_Class_Headers})
SOURCE_GROUP( ITK-Class_Sources FILES ${ITK_Class_Sources})
SOURCE_GROUP( Basic_Headers FILES ${CBICA_Headers})
SOURCE_GROUP( Basic_Sources FILES ${CBICA_Sources})

MARK_AS_ADVANCED ( ITK_CLASSES_DIR )
MARK_AS_ADVANCED ( ITK_Class_Headers )
MARK_AS_ADVANCED ( ITK_Class_Sources )

ADD_LIBRARY( 
  ${CBICA_ITK_LIB_NAME}
  ${ITK_Class_Headers}
  ${ITK_Class_Sources}
  ${CBICA_Headers}
  ${CBICA_Sources}
)

SET_TARGET_PROPERTIES( ${CBICA_ITK_LIB_NAME} PROPERTIES FOLDER "CBICA_Toolkit")

SET(CMAKE_DEBUG_POSTFIX "d")
# OR set_target_properties(mylib PROPERTIES DEBUG_POSTFIX "d")

TARGET_LINK_LIBRARIES(
  ${CBICA_ITK_LIB_NAME} 
  ${ITK_LIBRARIES}
  ${OpenCV_LIBRARIES}
)

# install the library
INSTALL(
  FILES ${ITK_Class_Headers} 
  DESTINATION include
)

INSTALL( 
	TARGETS ${CBICA_ITK_LIB_NAME} 
	DESTINATION lib 
)

IF( BUILD_TESTING )
	ENABLE_TESTING()
  INCLUDE(CTest)
  ADD_SUBDIRECTORY( ${CMAKE_CURRENT_SOURCE_DIR}/testing )
ENDIF()
