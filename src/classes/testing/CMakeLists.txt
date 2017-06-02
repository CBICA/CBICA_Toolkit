SET( TEST_EXE_NAME Basic_Tests )

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CLASS_DIRECTORY}
	${SOURCE_DIRECTORY}
  #${CBICA_TOOLKIT_BIN_DIR}
)

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/testing/" )

# Done for temporary testing
FILE( GLOB MainSources
  "${BASIC_CLASS_DIRECTORY}/*.h"
  "${BASIC_CLASS_DIRECTORY}/*.cpp"
)

ADD_EXECUTABLE( 
  ${TEST_EXE_NAME} 
  ${CBICA_Headers}
  ${CBICA_Sources}
  BasicFunctionTests.cxx 
)
#MESSAGE( STATUS "cbica_class name: " ${CBICA_TOOLKIT_NAME} )

SET_TARGET_PROPERTIES( ${TEST_EXE_NAME} PROPERTIES FOLDER "Tests")

SET( DATA_DIR "${PROJECT_SOURCE_DIR}/data" )

# Test that usage prints properly
ADD_TEST( NAME Print_Usage_Test COMMAND ${TEST_EXE_NAME} -buffer "random text")

# Test for CmdParser class - done last because this class is anyway being used 
ADD_TEST( NAME ParserClass_Test COMMAND ${TEST_EXE_NAME} -cmdParser "${DATA_DIR}")

# Test for Config File Parser
ADD_TEST( NAME ConfigFileReader_Test COMMAND ${TEST_EXE_NAME} -configFileReader "${DATA_DIR}/ParserClassTests.txt")

# Test for Copy File
ADD_TEST( NAME CopyFile_Test COMMAND ${TEST_EXE_NAME} -copyFile "${DATA_DIR}/list.csv" )

# Test for Copy File
ADD_TEST( NAME CopyFolder_Test COMMAND ${TEST_EXE_NAME} -copyFolder "${DATA_DIR}/" )

# Test for file create functions
ADD_TEST( NAME CreateFile_Test COMMAND ${TEST_EXE_NAME} -createFile ${CMAKE_CURRENT_BINARY_DIR}/test_File.txt )

# Test for folder create functions
ADD_TEST( NAME CreateFolder_Test COMMAND ${TEST_EXE_NAME} -createFolder ${CMAKE_CURRENT_BINARY_DIR}/test_Folder )

# Test for CSV Parser
ADD_TEST( NAME CSVParser_Test COMMAND ${TEST_EXE_NAME} -csvParser "${DATA_DIR}/list.csv")

# Test for CSV Parser
ADD_TEST( NAME CSVParserRaw_Test COMMAND ${TEST_EXE_NAME} -csvParserRaw "${DATA_DIR}/roc.csv")

# Test for file delete functions
ADD_TEST( NAME DeleteFile_Test COMMAND ${TEST_EXE_NAME} -deleteFile ${CMAKE_CURRENT_BINARY_DIR}/test_File.txt )

# Test for folder delete functions
ADD_TEST( NAME DeleteFolder_Test COMMAND ${TEST_EXE_NAME} -deleteFolder ${CMAKE_CURRENT_BINARY_DIR}/test_Folder )

# Test for Environment Variables
ADD_TEST( NAME EnvironmentVar_Test COMMAND ${TEST_EXE_NAME} -envName "random")

# Test for Logger class
ADD_TEST( NAME LoggerClass_Test COMMAND ${TEST_EXE_NAME} -logging "testing_phrase")

# Test for temporary folder creation
ADD_TEST( NAME TempFolder_Test COMMAND ${TEST_EXE_NAME} -makeTempDir "random")

# Test for file exist functions
ADD_TEST( NAME NoFile_Test COMMAND ${TEST_EXE_NAME} -noFile ${CMAKE_CURRENT_BINARY_DIR}/test_noFile.png )

# Test for folder exist functions
ADD_TEST( NAME NoFolder_Test COMMAND ${TEST_EXE_NAME} -noFolder ${CMAKE_CURRENT_BINARY_DIR}/test_noFolder )

# Test for SymbolicLink
ADD_TEST( NAME SymbolicLink_Test COMMAND ${TEST_EXE_NAME} -symbolic "random")

# Test for temporary folder creation
ADD_TEST( NAME SubDir_Test COMMAND ${TEST_EXE_NAME} -subDir "random")

IF( (MSVC_VERSION GREATER 1700) OR (UNIX) )
  # Test for Variadic template
  ADD_TEST( NAME Variadic_Test COMMAND ${TEST_EXE_NAME} -variadic "random")
ENDIF()

# Test for temporary folder creation
#ADD_TEST( NAME ROC_Test COMMAND ${TEST_EXE_NAME} -roc "${DATA_DIR}/roc.csv")

# Test for Split File Name
ADD_TEST( NAME SplitFileName_Test COMMAND ${TEST_EXE_NAME} -buffer "random") # no test is needed since previous tests have used this function

# Test for File Name Extension
ADD_TEST( NAME FileNameExtension_Test COMMAND ${TEST_EXE_NAME} -buffer "random") # no test is needed since previous tests have used this function

## Tests for ITK classes
IF( BUILD_CBICA_ITK_CLASSES )
  #ADD_SUBDIRECTORY( itk )
ENDIF( BUILD_CBICA_ITK_CLASSES )

SET_TARGET_PROPERTIES( ${TEST_EXE_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/testing/" )
SET_TARGET_PROPERTIES( ${TEST_EXE_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/testing/" )
SET_TARGET_PROPERTIES( ${TEST_EXE_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/testing/" )
SET_TARGET_PROPERTIES( ${TEST_EXE_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/testing/" )

#execute_process(
#  COMMAND ${CMAKE_COMMAND} --build "${PROJECT_BINARY_DIR}"
#                           --target install
#                           --config "${CONFIGURATION}"
#  RESULT_VARIABLE failed
#  )

#MESSAGE( STATUS "basic test bin dir: ${CMAKE_CURRENT_BINARY_DIR}" )
