SET( ITK_TEST_EXE_NAME ITK_Tests )

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CLASS_DIRECTORY}
	${SOURCE_DIRECTORY}
  #${CBICA_TOOLKIT_BIN_DIR}
)

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/testing/" )

FIND_PACKAGE( ITK REQUIRED QUIET )
IF( WIN32 )
  INCLUDE(${ITK_USE_FILE})
ELSE()
  INCLUDE_DIRECTORIES(${ITK_INCLUDE_DIRS})
ENDIF()

ADD_EXECUTABLE( ITK_Tests ItkFunctionTests.cxx )

SET_TARGET_PROPERTIES( ITK_Tests PROPERTIES FOLDER "Tests")

TARGET_LINK_LIBRARIES( 
	ITK_Tests
  ${CBICA_ITK_LIB_NAME} 
  ${CBICA_LIB_NAME}
  CmdParser
)

ENABLE_TESTING()

SET( DATA_DIR "${PROJECT_SOURCE_DIR}/data/nifti1/" )

# Test for the imageInfo class
ADD_TEST( NAME ItkImageInfoClass_Test COMMAND ITK_Tests -imageInfo "${DATA_DIR}/1.nii.gz")

#Test for the ReadImage function
ADD_TEST( NAME ItkReadUnknownImage_Test COMMAND ITK_Tests -readImage "${DATA_DIR}/1.nii.gz")

#Test for the ReadImage function
ADD_TEST( NAME ItkWriteUnknownImage_Test COMMAND ITK_Tests -writeImage "${DATA_DIR}/1.nii.gz" "${DATA_DIR}/1_test.nii.gz")

##Test for the ReadImage function
#ADD_TEST( NAME ItkDeformReg_Test COMMAND ITK_Tests -deform "${DATA_DIR}/deform/ref.nii.gz ${DATA_DIR}/deform/mov.nii.gz ${DATA_DIR}/deform/expected.nii.gz")

##Test for the ReadImage function
#ADD_TEST( NAME ItkDeformReg_Test COMMAND ITK_Tests -skullStrip "${DATA_DIR}/deform/input.nii.gz ${DATA_DIR}/deform/atlasImage.nii.gz ${DATA_DIR}/deform/atlasLabel.nii.gz ${DATA_DIR}/deform/expected.nii.gz")

SET_TARGET_PROPERTIES( ITK_Tests PROPERTIES ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/testing/" )
SET_TARGET_PROPERTIES( ITK_Tests PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/testing/" )
SET_TARGET_PROPERTIES( ITK_Tests PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/testing/" )
SET_TARGET_PROPERTIES( ITK_Tests PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/testing/" )

#MESSAGE( STATUS "itk test bin dir: ${CMAKE_CURRENT_BINARY_DIR}" )
