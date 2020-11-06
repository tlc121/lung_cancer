project(PathologyTCT)
################################################################################
# Include paths of header files 
################################################################################
set(PathologyTCT_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
	${GLOB_PY_INCLUDE}
	${GLOB_TENSORFLOW_INCLUDE}
	${SOURCECODE_PATH}/common/base/inc
	#${SOURCECODE_PATH}/common/infer/inc
	${SOURCECODE_PATH}/common/utils/inc
	#${SOURCECODE_PATH}/common/imageProcess/inc
	${OpenCV_INCLUDE_DIRS}
	${Openslide_INC}
	)

include_directories(${PathologyTCT_INCLUDES})

################################################################################
# Configure source files
################################################################################
set(PathologyTCT_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelineElementPathologyTCTCls.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyTCT.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/IFPipelinePathologyTCTIn.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/InferLocalPathologyTCT.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/AIThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/IoThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyTCTFactory.cpp
	)



################################################################################
# Configure lib files
################################################################################
set(UTIALS_LIB_ALL
	CommonUtils
	${COMM_UTIL_LIB})

if(BUILD_SHARED_LIBS)
	add_library(PipelinePathologyTCT SHARED ${PathologyTCT_SOURCE})
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyTCT optimized ${_LIB} )
	endforeach()
	target_link_libraries(PipelinePathologyTCT optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyTCT debug ${_LIB})
	endforeach()
	target_link_libraries(PipelinePathologyTCT debug ${UTIALS_LIB_ALL} )
	generate_export_header(PipelinePathologyTCT EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelinePathologyTCT_export.h)
else()
	add_library(PipelinePathologyTCT STATIC ${PathologyTCT_SOURCE} )
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyTCT optimized ${_LIB})
	endforeach()
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyTCT debug ${_LIB})
	endforeach()
	add_library(PipelinePathologyTCTFactory STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyTCTFactory.cpp)
    target_link_libraries(PipelinePathologyTCTFactory PipelinePathologyTCT ${ITK_LIBRARIES} ${TFS_LIB_ALL} ${UTIALS_LIB_ALL} ${OpenCV_LIBS} ${TF_LIB} ${Openslide_LIB})
endif()

################################################################################
# unit test
################################################################################
add_executable(PipelinePathologyTCTTest 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/PipelinePathologyTCTTest.cpp)
    
foreach(_LIB ${CONAN_LIBS_RELEASE})
	target_link_libraries(PipelinePathologyTCTTest optimized ${_LIB})
endforeach()
target_link_libraries(PipelinePathologyTCTTest optimized ${UTIALS_LIB_ALL} )
foreach(_LIB ${CONAN_LIBS_DEBUG})
	target_link_libraries(PipelinePathologyTCTTest debug ${_LIB})
endforeach()
target_link_libraries(PipelinePathologyTCTTest debug ${UTIALS_LIB_ALL} )
target_link_libraries(PipelinePathologyTCTTest PipelinePathologyTCT )

