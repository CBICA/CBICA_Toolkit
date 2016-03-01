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

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} 
                     ${CMAKE_CURRENT_SOURCE_DIR}/../../
)

SET( CBICA_ITK_LIB_NAME CBICAToolkit_ITK )
ADD_DEFINITIONS( -DCBICA_ITK_LIB_NAME="${CBICA_ITK_LIB_NAME}" )

FIND_PACKAGE(ITK REQUIRED)
INCLUDE( ${ITK_USE_FILE} )

FIND_PACKAGE(OpenMP REQUIRED)
IF(OPENMP_FOUND)
  SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}" )
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" )
ENDIF()

SET( ITK_Class_Headers 
  ${CMAKE_CURRENT_SOURCE_DIR}/../cbicaUtilities.h
	${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKImageInfo.h 
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKComputeAverageMap.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKComputeDtiScalars.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKComputeVarianceMap.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKCommonHolder.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKDtiRecon.h  
  #${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKReadImage.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKWriteImage.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKUtilities.h
    
  ${CMAKE_CURRENT_SOURCE_DIR}/itkDTILogEuclideanCalculator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/itkNaryMeanImageFilter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/itkNaryMeanDiffusionTensorImageFilter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/itkNaryVarianceImageFilter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/itkDiffusionTensor3DReconstructionImageFilter.h
  
)

SET( ITK_Class_Sources 
  ${CMAKE_CURRENT_SOURCE_DIR}/../cbicaUtilities.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKImageInfo.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKComputeAverageMap.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKComputeDtiScalars.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKComputeVarianceMap.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cbicaITKCommonHolder.cpp
    
  ${CMAKE_CURRENT_SOURCE_DIR}/itkDTILogEuclideanCalculator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/itkDiffusionTensor3DReconstructionImageFilter.cpp
)

SET( ITK_CLASSES_DIR    ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "ITK CLASS INCLUDES" FORCE )
SET( ITK_Class_Headers  ${ITK_Class_Headers}        CACHE STRING "ITK headers"        FORCE )
SET( ITK_Class_Sources  ${ITK_Class_Sources}        CACHE STRING "ITK sources"        FORCE )
SET( ITK_CLASS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "ITK classes directory" FORCE )
SET( ITK_CLASS_LIBRARY ${CBICA_ITK_LIB_NAME} CACHE STRING "ITK classes library" FORCE )

MARK_AS_ADVANCED ( ITK_CLASSES_DIR )
MARK_AS_ADVANCED ( ITK_Class_Headers )
MARK_AS_ADVANCED ( ITK_Class_Sources )

ADD_LIBRARY( 
  ${CBICA_ITK_LIB_NAME}
  ${ITK_Class_Headers}
  ${ITK_Class_Sources}  
)

SET(CMAKE_DEBUG_POSTFIX "d")
# OR set_target_properties(mylib PROPERTIES DEBUG_POSTFIX "d")

## Add c++11 flag to compilation if GCC is detected under UNIX: how does this translate to MinGW/Cygwin??
IF(CMAKE_COMPILER_IS_GNUCXX)
  
  ## string search
  #string(SUBSTRING <string> <begin> <length> <output variable>)
  #MESSAGE( STATUS "cxx_flags: ${CMAKE_CXX_FLAGS}" )
  #string(REPLACE " " ";" FLAGS ${CMAKE_CXX_FLAGS})
  #MESSAGE( STATUS "flags: ${FLAGS}" )
  #FOREACH( flag ${FLAGS} )
  #  MESSAGE( STATUS "current flag == ${flag}" )
  #  IF( ${flag} STREQUAL "-ftemplate-depth-50" )
  #    MESSAGE( STATUS "FOUND!" )
  #  ENDIF()
  #ENDFOREACH()
  #SET( CMAKE_CXX_FLAGS "-ftemplate-depth=900 -Wall -Wno-deprecated -msse2" )
  #MESSAGE( STATUS "flags: ${CMAKE_CXX_FLAGS}")
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth=900" )
	#INCLUDE( CheckCXXCompilerFlag )
	#CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	#CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	#IF( COMPILER_SUPPORTS_CXX11 )
	#	SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ftemplate-depth=900" )
	#ELSEIF(COMPILER_SUPPORTS_CXX0X )
	#	SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" )
	#ELSE()
	#	MESSAGE(ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	#ENDIF()
  
  #MESSAGE( STATUS "flags: ${CMAKE_CXX_FLAGS}")
  
  #SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ftemplate-depth=900 -fdiagnostics-color=always" )
ENDIF(CMAKE_COMPILER_IS_GNUCXX) 

FIND_PACKAGE( OpenCV 3.0 ) # this is required for the cbica::vectorizeImage function defined in cbicaITKUtilities
IF (OpenCV_FOUND)
  ADD_DEFINITIONS( -DOpenCV_FOUND=1)
  INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )
  TARGET_LINK_LIBRARIES(
	  ${CBICA_ITK_LIB_NAME} 
	  ${ITK_LIBRARIES}
    ${OpenCV_LIBS}
  )

ELSE( OpenCV_FOUND )
  TARGET_LINK_LIBRARIES(
	  ${CBICA_ITK_LIB_NAME} 
	  ${ITK_LIBRARIES}
  )
ENDIF (OpenCV_FOUND)

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
  #MESSAGE( STATUS "itk testing enabled" )
  ADD_SUBDIRECTORY( testing )
ENDIF()