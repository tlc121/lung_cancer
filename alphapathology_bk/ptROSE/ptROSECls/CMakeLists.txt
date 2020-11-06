project(PathologyROSE)
################################################################################
# Include paths of header files 
################################################################################
set(PathologyROSE_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
	${GLOB_PY_INCLUDE}
	${GLOB_TENSORFLOW_INCLUDE}
	${SOURCECODE_PATH}/common/base/inc
	#${SOURCECODE_PATH}/common/infer/inc
	${SOURCECODE_PATH}/common/utils/inc
	#${SOURCECODE_PATH}/common/imageProcess/inc
	${SOURCECODE_PATH}/common/evaluation/inc
	${OpenCV_INCLUDE_DIRS}
	${Openslide_INC})
include_directories(${PathologyROSE_INCLUDES})

################################################################################
# Configure source files
################################################################################
set(PathologyROSE_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelineElementPathologyROSECls.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyROSE.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/IFPipelinePathologyIn.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/InferLocalPathologyROSE.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyROSEFactory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/AIThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/IoThread.cpp
	)



################################################################################
# Configure lib files
################################################################################
set(UTIALS_LIB_ALL
	CommonUtils
	EvaluationAlg
	${COMM_UTIL_LIB})

if(BUILD_SHARED_LIBS)
	add_library(PipelinePathologyROSE SHARED ${PathologyROSE_SOURCE})
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyROSE optimized ${_LIB} )
	endforeach()
	target_link_libraries(PipelinePathologyROSE optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyROSE debug ${_LIB})
	endforeach()
	target_link_libraries(PipelinePathologyROSE debug ${UTIALS_LIB_ALL} )
	generate_export_header(PipelinePathologyROSE EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelinePathologyROSE_export.h)
else()
	add_library(PipelinePathologyROSE STATIC ${PathologyROSE_SOURCE} )
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyROSE optimized ${_LIB} )
	endforeach()
	target_link_libraries(PipelinePathologyROSE optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyROSE debug ${_LIB})
	endforeach()
	target_link_libraries(PipelinePathologyROSE debug ${UTIALS_LIB_ALL} )
	add_library(PipelinePathologyROSEFactory STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyROSEFactory.cpp )
    target_link_libraries(PipelinePathologyROSEFactory PipelinePathologyROSE ${ITK_LIBRARIES} ${TFS_LIB_ALL} ${UTIALS_LIB_ALL} ${OpenCV_LIBS} ${TF_LIB} ${Openslide_LIB})
endif()

################################################################################
# unit test
################################################################################
add_executable(PipelinePathologyROSETest 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/PipelinePathologyROSETest.cpp)
foreach(_LIB ${CONAN_LIBS_RELEASE})
	target_link_libraries(PipelinePathologyROSETest optimized ${_LIB} )
endforeach()
target_link_libraries(PipelinePathologyROSETest optimized ${UTIALS_LIB_ALL} )
foreach(_LIB ${CONAN_LIBS_DEBUG})
	target_link_libraries(PipelinePathologyROSETest debug ${_LIB})
endforeach()
target_link_libraries(PipelinePathologyROSETest debug ${UTIALS_LIB_ALL} )
target_link_libraries(PipelinePathologyROSETest PipelinePathologyROSE )