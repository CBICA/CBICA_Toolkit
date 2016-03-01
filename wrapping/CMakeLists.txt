# Try to rebuild wrapping a little more often:
include_regular_expression("^(cbica).*$")
# TODO:
# SWIG is really a pain in the neck to use, a better alternative is Py++ which is using
# gccxml for the C++ parser and allow a full ANSI C++ support
# Note gcc has some issue with RTTI stuff:
# http://groups.google.com/group/comp.lang.c++.moderated/browse_thread/thread/ac889a7d9eac902f
# http://gcc.gnu.org/ml/gcc-help/2007-10/msg00239.html
# http://wiki.python.org/moin/boost.python/CrossExtensionModuleDependencies
# http://www.boost-consulting.com/writing/bpl.html
# http://gcc.gnu.org/ml/gcc/2002-05/msg00866.html
# http://mail.python.org/pipermail/c++-sig/2002-May/001021.html
# http://mail.python.org/pipermail/python-dev/2002-May/023923.html

FIND_PACKAGE( SWIG REQUIRED )
MARK_AS_ADVANCED( SWIG_DIR SWIG_EXECUTABLE SWIG_VERSION )
INCLUDE( ${SWIG_USE_FILE} )

FIND_PACKAGE( ITK REQUIRED )
INCLUDE( ${ITK_USE_FILE} )

#
# Do not cover this lib
#

CONFIGURE_FILE (${CMAKE_CURRENT_SOURCE_DIR}/.NoDartCoverage
                ${CMAKE_CURRENT_BINARY_DIR}/.NoDartCoverage)

# Note:
# python -c "from struct import pack; print  pack('5b', (41*len('99')), pow(8,2)+20, 4900**0.5, range(78)[-1], 10)"

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${BASIC_CLASS_DIRECTORY}
  ${ITK_CLASS_DIRECTORY}
	${SOURCE_DIRECTORY}
	${PYTHON_INCLUDE_DIR}
  ${ITK_INCLUDE_DIRS}
)

FIND_PACKAGE( PythonInterp REQUIRED )
FIND_PACKAGE( PythonLibs REQUIRED )
# TODO Need to check consistency python interp and python libs...
MARK_AS_ADVANCED( PYTHON_LIBRARY PYTHON_INCLUDE_DIR )


## Check if this is required or not
# Lamest excuse ever:
# http://mail.python.org/pipermail/python-list/2002-April/141189.html
# So here come craziest hack ever, since I cannot control the output of swig,
# I need to fake a Python.h file only for MSVC compilers...insane !
#IF(MSVC)
#  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Python.h.in
#    ${CMAKE_CURRENT_BINARY_DIR}/Python.h @ONLY
#  )
#ELSE()
## just plain including pyconfig.h is working...until one crazy python dev decide otherwise...
#  INCLUDE_DIRECTORIES(
#    ${PYTHON_INCLUDE_DIR}
#  )
#ENDIF()

SET_SOURCE_FILES_PROPERTIES( cbicaToolkit.i PROPERTIES CPLUSPLUS ON )
#SET_SOURCE_FILES_PROPERTIES( cbicaToolkit.i PROPERTIES SWIG_FLAGS "-includeall" )

SET(CBICA_PYTHON_IMPLEMENTATION_NAME cbicaToolkit)
SET(MODULE_NAME cbicaToolkit)
# BUG: DO NOT USE -interface flag it is NOT supported in cmake and in cmake > 2.6 will cause infinite rebuild
# Ref: http://www.cmake.org/pipermail/cmake/2008-August/023237.html
# UseSWIG and -interface flag (was: Re: CMake 2.6.1 available for download)
IF(${PYTHON_VERSION_MAJOR} EQUAL 3)
  # http://swig.org/Doc2.0/SWIGDocumentation.html#Python_python3support
  SET(CMAKE_SWIG_FLAGS "-py3")
ENDIF()
# TODO: PythonInterp and PythonLibs are not working together well
# see: http://bugs.debian.org/677598
#message(${PYTHONLIBS_VERSION_STRING})
#message(${PYTHON_VERSION_MAJOR})
#separate_arguments(CMAKE_SWIG_FLAGS)

# While trying to get rid of the compilation warning in swig generated c++ code, I thought I could
# simply do the following:
#set(CMAKE_CXX_FLAGS "")
# well no, you cannot, it get rid of some important flags, and make the _cbica.so incompatible with
# the other cbica lib. bad !!!
#set (SWIG_MODULE_${MODULE_NAME}_EXTRA_DEPS ${SWIG_MODULE_${MODULE_NAME}_EXTRA_DEPS} ${CMAKE_CURRENT_SOURCE_DIR}/docstrings.i)
SWIG_ADD_MODULE(${CBICA_PYTHON_IMPLEMENTATION_NAME} python cbicaToolkit.i 
#cbicaToolkit.cxx
#gdcmPythonFilter.cxx
)

SWIG_LINK_LIBRARIES(
  ${CBICA_PYTHON_IMPLEMENTATION_NAME} 
  ${ITK_CLASS_LIBRARY}
  ${BASIC_CLASS_LIBRARY}
)

# Apparently on my UNIX, python module (/usr/lib/pyshared/pythonX.Y/*/*.so) do not explicitely
# link to python libraries...Leave default to always link to python libraries since
# this is required at least on Apple & Win32, but leave the option to advanced user to explicitely
# refuse linking to python libs (set GDCM_NO_PYTHON_LIBS_LINKING to ON):
#if(NOT GDCM_NO_PYTHON_LIBS_LINKING)
  SWIG_LINK_LIBRARIES(${CBICA_PYTHON_IMPLEMENTATION_NAME} ${PYTHON_LIBRARY} ${ITK_LIBRARIES})
#endif()
SET_PROPERTY(TARGET ${SWIG_MODULE_${CBICA_PYTHON_IMPLEMENTATION_NAME}_REAL_NAME} PROPERTY NO_SONAME 1)

# Python extension modules on Windows must have the extension ".pyd"
# instead of ".dll" as of Python 2.5.  Older python versions do support
# this suffix.
# http://docs.python.org/whatsnew/ports.html#SECTION0001510000000000000000
# <quote>
# Windows: .dll is no longer supported as a filename extension for extension modules.
# .pyd is now the only filename extension that will be searched for.
# </quote>
IF(WIN32 AND NOT CYGWIN)
  SET_TARGET_PROPERTIES(${SWIG_MODULE_${CBICA_PYTHON_IMPLEMENTATION_NAME}_REAL_NAME} PROPERTIES SUFFIX ".pyd")
  # shared libs on windows needs to be fully resolved
  SWIG_LINK_LIBRARIES(${CBICA_PYTHON_IMPLEMENTATION_NAME} ${PYTHON_LIBRARIES})
ENDIF()
SET_TARGET_PROPERTIES(${SWIG_MODULE_${CBICA_PYTHON_IMPLEMENTATION_NAME}_REAL_NAME} PROPERTIES LINK_INTERFACE_LIBRARIES "")

# swig generates a _cbica.so and a cbica.py, we need to copy cbica.py to the proper place:
# cbica.py is the interface name != implementation name, so we need to keep 'cbica', so that 'import cbica'
# from a python script always work
#ADD_CUSTOM_COMMAND(
#  TARGET    ${SWIG_MODULE_${CBICA_PYTHON_IMPLEMENTATION_NAME}_REAL_NAME}
#  POST_BUILD
#  COMMAND   ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/cbicaToolkit.py ${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}
#  COMMAND   ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/cbica.py ${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}
#  DEPENDS 	${swig_generated_file_fullname} ${CMAKE_CURRENT_BINARY_DIR}/cbicaToolkit.py ${CMAKE_CURRENT_SOURCE_DIR}/cbica.py
#  COMMENT   "Copy cbicaToolkit.py into ${LIBRARY_OUTPUT_PATH}"
#)

#Module are always place in the library destination
#but for poor win32 user I decided to place them
# right next to the other dlls
#IF(NOT GDCM_INSTALL_NO_LIBRARIES)
#  INSTALL_SWIG_MODULE(${CBICA_PYTHON_IMPLEMENTATION_NAME} Python)
#  # the python file is not a dev file, but part of the cbica module...
  
  EXECUTE_PROCESS( COMMAND python -c "from distutils.sysconfig import get_python_lib; print get_python_lib()" 
					OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)
  
  INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cbicaToolkit.py
    ${CMAKE_CURRENT_SOURCE_DIR}/cbica.py
    DESTINATION ${PYTHON_SITE_PACKAGES} 
	COMPONENT PythonModule
  )
#ENDIF()

## Testing of Python module
# Test that will try to load any class in the target language: python
# it make sure swig was not broken accidentally
#IF(BUILD_TESTING)
#  ADD_PYTHON_TEST(TestWrapPython TestWrap.py ${GDCM_SOURCE_DIR}/Source)
#  IF(GDCM_DOCUMENTATION)
#    ADD_PYTHON_TEST(TestDoxy2SWIGPython doxy2swig.py ${GDCM_BINARY_DIR}/Utilities/doxygen/xml/index.xml ${GDCM_BINARY_DIR}/generated_docstrings.i)
#  ENDIF()
#ENDIF()

# TODO
# python -c "from distutils import sysconfig; print sysconfig.get_python_lib()"
# /usr/lib/python2.4/site-packages
