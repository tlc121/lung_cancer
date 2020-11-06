project(PathologyXS)
################################################################################
# Include paths of header files 
################################################################################
set(PathologyXS_INCLUDES
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
include_directories(${PathologyXS_INCLUDES})

################################################################################
# Configure source files
################################################################################
set(PathologyXS_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelineElementPathologyXSCls.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyXS.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/IFPipelinePathologyXSIn.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/InferLocalPathologyXS.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/AIThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/IoThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyXSFactory.cpp
	)



################################################################################
# Configure lib files
################################################################################
set(UTIALS_LIB_ALL
	CommonUtils
	${COMM_UTIL_LIB})

if(BUILD_SHARED_LIBS)
	add_library(PipelinePathologyXS SHARED ${PathologyXS_SOURCE})
	target_include_directories(PipelinePathologyXS PUBLIC ${PathologyXS_INCLUDES})
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyXS optimized ${_LIB} )
	endforeach()
	target_link_libraries(PipelinePathologyXS optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyXS debug ${_LIB})
	endforeach()
	target_link_libraries(PipelinePathologyXS debug ${UTIALS_LIB_ALL} )
	generate_export_header(PipelinePathologyXS EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelinePathologyXS_export.h)
else()
	add_library(PipelinePathologyXS STATIC ${PathologyXS_SOURCE} )
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyXS optimized ${_LIB} )
	endforeach()
	target_link_libraries(PipelinePathologyXS optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyXS debug ${_LIB})
	endforeach()
	target_link_libraries(PipelinePathologyXS debug ${UTIALS_LIB_ALL} )
	add_library(PipelinePathologyXSFactory STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyXSFactory.cpp )
    target_link_libraries(PipelinePathologyXSFactory PipelinePathologyXS ${ITK_LIBRARIES} ${TFS_LIB_ALL} ${UTIALS_LIB_ALL} ${OpenCV_LIBS} ${TF_LIB} ${Openslide_LIB})
endif()

################################################################################
# unit test
################################################################################
add_executable(PipelinePathologyXSTest 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/PipelinePathologyXSTest.cpp)
foreach(_LIB ${CONAN_LIBS_RELEASE})
	target_link_libraries(PipelinePathologyXSTest optimized ${_LIB} )
endforeach()
target_link_libraries(PipelinePathologyXSTest optimized ${UTIALS_LIB_ALL} )
foreach(_LIB ${CONAN_LIBS_DEBUG})
	target_link_libraries(PipelinePathologyXSTest debug ${_LIB})
endforeach()
target_link_libraries(PipelinePathologyXSTest debug ${UTIALS_LIB_ALL} )
target_link_libraries(PipelinePathologyXSTest PipelinePathologyXS )