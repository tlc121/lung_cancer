project(PathologyLungCancer)
################################################################################
# Include paths of header files 
################################################################################
set(PathologyLungCancer_INCLUDES
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
include_directories(${PathologyLungCancer_INCLUDES})

################################################################################
# Configure source files
################################################################################
set(PathologyLungCancer_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelineElementPathologyLungCancerCls.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyLungCancer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/IFPipelinePathologyLungCancerIn.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/InferLocalPathologyLungCancer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/AIThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/IoThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyLungCancerFactory.cpp
	)



################################################################################
# Configure lib files
################################################################################
set(UTIALS_LIB_ALL
	CommonUtils
	${COMM_UTIL_LIB})

if(BUILD_SHARED_LIBS)
	add_library(PipelinePathologyLungCancer SHARED ${PathologyLungCancer_SOURCE})
	target_include_directories(PipelinePathologyLungCancer PUBLIC ${PathologyLungCancer_INCLUDES})
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyLungCancer optimized ${_LIB} )
	endforeach()
	target_link_libraries(PipelinePathologyLungCancer optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyLungCancer debug ${_LIB})
	endforeach()
	target_link_libraries(PipelinePathologyLungCancer debug ${UTIALS_LIB_ALL} )
	generate_export_header(PipelinePathologyLungCancer EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelinePathologyLungCancer_export.h)
else()
	add_library(PipelinePathologyLungCancer STATIC ${PathologyLungCancer_SOURCE} )
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyLungCancer optimized ${_LIB} )
	endforeach()
	target_link_libraries(PipelinePathologyLungCancer optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyLungCancer debug ${_LIB})
	endforeach()
	add_library(PipelinePathologyLungCancerFactory STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyLungCancerFactory.cpp )
    target_link_libraries(PipelinePathologyLungCancerFactory PipelinePathologyLungCancer ${ITK_LIBRARIES} ${TFS_LIB_ALL} ${UTIALS_LIB_ALL} ${OpenCV_LIBS} ${TF_LIB} ${Openslide_LIB})
endif()

################################################################################
# unit test
################################################################################
add_executable(PipelinePathologyLungCancerTest 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/PipelinePathologyLungCancerTest.cpp)
foreach(_LIB ${CONAN_LIBS_RELEASE})
	target_link_libraries(PipelinePathologyLungCancerTest optimized ${_LIB} )
endforeach()
target_link_libraries(PipelinePathologyLungCancerTest optimized ${UTIALS_LIB_ALL} )
foreach(_LIB ${CONAN_LIBS_DEBUG})
	target_link_libraries(PipelinePathologyLungCancerTest debug ${_LIB})
endforeach()
target_link_libraries(PipelinePathologyLungCancerTest PipelinePathologyLungCancer )
