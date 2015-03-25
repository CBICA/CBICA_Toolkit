## Superbuild module for building ITK externally.

MESSAGE( "External project - ITK" )

SET( ITK_DEPENDENCIES )

ExternalProject_Add( ITK
	DEPENDS ${ITK_DEPENDENCIES}
	GIT_REPOSITORY ${git_protocol}://itk.org/ITK.git #  url from where to download
	GIT_TAG v4.4.2
	SOURCE_DIR ITK
	BINARY_DIR ITK-build
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	CMAKE_GENERATOR ${gen}
	CMAKE_ARGS
		${ep_common_args}
		-DBUILD_EXAMPLES:BOOL=OFF # examples are not needed
		-DBUILD_SHARED_LIBS:BOOL=ON # no static builds
		-DBUILD_TESTING:BOOL=OFF # testing the ITK build is not required
		-DITK_BUILD_ALL_MODULES:BOOL=ON
		-DITK_LEGACY_REMOVE:BOOL=ON 
		-DITK_USE_REVIEW:BOOL=ON # new modules
		#-DModule_ITKVideoBridgeOpenCV:BOOL=ON # [OPENCV] dependency
		#-DOpenCV_DIR:PATH=${OpenCV_DIR} # [OPENCV] dependency
		#-DModule_ITKVtkGlue:BOOL=ON # [VTK] dependency
		#-DVTK_DIR:PATH=${VTK_DIR} # [VTK] dependency
		-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE} # toggle for type of build if something different that 
		-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/INSTALL
	)

SET( ITK_DIR ${CMAKE_BINARY_DIR}/ITK-build )