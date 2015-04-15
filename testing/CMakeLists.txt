SET( TEST_EXE_NAME Test_Basic_Functions )

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CLASS_DIRECTORY}
	${SOURCE_DIRECTORY}
)

# Done for temporary testing
FILE( GLOB MainSources
  "${BASIC_CLASS_DIRECTORY}/*.h"
  "${BASIC_CLASS_DIRECTORY}/*.cpp"
)

ADD_EXECUTABLE( ${TEST_EXE_NAME} BasicFunctionTests.cxx ${MainSources} )

IF(UNIX)
	INCLUDE( CheckCXXCompilerFlag )
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	IF( COMPILER_SUPPORTS_CXX11 )
		SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	elseif(COMPILER_SUPPORTS_CXX0X )
		SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else()
		MESSAGE(ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif()
ENDIF(UNIX) 

# Test that usage prints properly
ADD_TEST( NAME Print_Usage_Test COMMAND ${TEST_EXE_NAME} -u "random text")

# Test for folder exist functions
ADD_TEST( NAME NoFolder_Test COMMAND ${TEST_EXE_NAME} -noFolder ${CMAKE_CURRENT_BINARY_DIR}/test_noFolder )

# Test for file exist functions
ADD_TEST( NAME NoFile_Test COMMAND ${TEST_EXE_NAME} -noFile ${CMAKE_CURRENT_BINARY_DIR}/test_noFile.png )

# Test for folder create functions
ADD_TEST( NAME CreateFolder_Test COMMAND ${TEST_EXE_NAME} -createFolder ${CMAKE_CURRENT_BINARY_DIR}/test_Folder )

# Test for file create functions
ADD_TEST( NAME CreateFile_Test COMMAND ${TEST_EXE_NAME} -createFile ${CMAKE_CURRENT_BINARY_DIR}/test_File.txt )

# Test for folder delete functions
ADD_TEST( NAME DeleteFolder_Test COMMAND ${TEST_EXE_NAME} -deleteFolder ${CMAKE_CURRENT_BINARY_DIR}/test_Folder )

# Test for file delete functions
ADD_TEST( NAME DeleteFile_Test COMMAND ${TEST_EXE_NAME} -deleteFile ${CMAKE_CURRENT_BINARY_DIR}/test_File.txt )

# Test for temporary folder creation
ADD_TEST( NAME TempFolder_Test COMMAND ${TEST_EXE_NAME} -makeTempDir "random")

# Test for Logger class
ADD_TEST( NAME LoggerClass_Test COMMAND ${TEST_EXE_NAME} -logging "testing_phrase")

# Test for String functions
ADD_TEST( NAME StringFunctions_Test COMMAND ${TEST_EXE_NAME} -logging "testing_phrase")

# Test for CmdParser class
ADD_TEST( NAME ParserClass_Test COMMAND ${TEST_EXE_NAME} -cmdParser "random")

# Test for SymbolicLink
ADD_TEST( NAME SymbolicLink_Test COMMAND ${TEST_EXE_NAME} -symbolic "random")

# Test for Environment Variables
ADD_TEST( NAME EnvironmentVar_Test COMMAND ${TEST_EXE_NAME} -envName "random")

# Test for Directory search
ADD_TEST( NAME DirSearch_Test COMMAND ${TEST_EXE_NAME} -dirSearch "random")
