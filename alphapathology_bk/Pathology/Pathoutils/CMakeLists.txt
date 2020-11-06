project(PathologyUtils)
################################################################################
# Include paths of header files 
################################################################################
set(PathologyUtils_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
	${GLOB_PY_INCLUDE}
	${GLOB_TENSORFLOW_INCLUDE}
	${SOURCECODE_PATH}/common/base/inc
	#${SOURCECODE_PATH}/common/infer/inc
	${SOURCECODE_PATH}/common/utils/inc
	#${SOURCECODE_PATH}/common/imageProcess/inc
	${OpenCV_INCLUDE_DIRS})
include_directories(${PathologyUtils_INCLUDES})

################################################################################
# Configure source files
################################################################################
set(PathologyUtils_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelineElementPathologyUtilsCls.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyUtils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/IFPipelinePathologyUtilsIn.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/InferLocalPathologyUtils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyUtilsFactory.cpp
	)



################################################################################
# Configure lib files
################################################################################
set(UTIALS_LIB_ALL
	CommonUtils
	${COMM_UTIL_LIB})

if(BUILD_SHARED_LIBS)
	add_library(PipelinePathologyUtils SHARED ${PathologyUtils_SOURCE})
	target_include_directories(PipelinePathologyUtils PUBLIC ${PathologyUtils_INCLUDES})
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyUtils optimized ${_LIB} )
	endforeach()
	target_link_libraries(PipelinePathologyUtils optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyUtils debug ${_LIB})
	endforeach()
	target_link_libraries(PipelinePathologyUtils debug ${UTIALS_LIB_ALL} )
	generate_export_header(PipelinePathologyUtils EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelinePathologyUtils_export.h)
else()
	add_library(PipelinePathologyUtils STATIC ${PathologyUtils_SOURCE} )
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyUtils optimized ${_LIB} )
	endforeach()
	target_link_libraries(PipelinePathologyUtils optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyUtils debug ${_LIB})
	endforeach()
	target_link_libraries(PipelinePathologyUtils debug ${UTIALS_LIB_ALL} )
	add_library(PipelinePathologyUtilsFactory STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyUtilsFactory.cpp )
    target_link_libraries(PipelinePathologyUtilsFactory PipelinePathologyUtils ${ITK_LIBRARIES} ${TFS_LIB_ALL} ${UTIALS_LIB_ALL} ${OpenCV_LIBS} ${TF_LIB})
endif()


################################################################################
# unit test
################################################################################
add_executable(PipelinePathologyUtilsTest 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/PipelinePathologyUtilsTest.cpp)
foreach(_LIB ${CONAN_LIBS_RELEASE})
	target_link_libraries(PipelinePathologyUtilsTest optimized ${_LIB} )
endforeach()
target_link_libraries(PipelinePathologyUtilsTest optimized ${UTIALS_LIB_ALL} )
foreach(_LIB ${CONAN_LIBS_DEBUG})
	target_link_libraries(PipelinePathologyUtilsTest debug ${_LIB})
endforeach()
target_link_libraries(PipelinePathologyUtilsTest debug ${UTIALS_LIB_ALL} )
target_link_libraries(PipelinePathologyUtilsTest PipelinePathologyUtils )