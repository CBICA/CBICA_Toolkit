INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} )

#install all header files
FILE( GLOB Windows_Headers 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.hxx"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"

)

INSTALL(FILES ${Windows_Headers} DESTINATION include)
