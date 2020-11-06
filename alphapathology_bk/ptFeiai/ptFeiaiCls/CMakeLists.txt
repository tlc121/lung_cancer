project(PathologyFEIAI)
################################################################################
# Include paths of header files 
################################################################################
set(PathologyFEIAI_INCLUDES
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
include_directories(${PathologyFEIAI_INCLUDES})

################################################################################
# Configure source files
################################################################################
set(PathologyFEIAI_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelineElementPathologyFEIAICls.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyFEIAI.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/IFPipelinePathologyFEIAIIn.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/InferLocalPathologyFEIAI.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/AIThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/IoThread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyFEIAIFactory.cpp
	)



################################################################################
# Configure lib files
################################################################################
set(UTIALS_LIB_ALL
	CommonUtils
	${COMM_UTIL_LIB})

if(BUILD_SHARED_LIBS)
	add_library(PipelinePathologyFEIAI SHARED ${PathologyFEIAI_SOURCE})
	target_include_directories(PipelinePathologyFEIAI PUBLIC ${PathologyFEIAI_INCLUDES})
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyFEIAI optimized ${_LIB} )
	endforeach()
	target_link_libraries(PipelinePathologyFEIAI optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyFEIAI debug ${_LIB})
	endforeach()
	target_link_libraries(PipelinePathologyFEIAI debug ${UTIALS_LIB_ALL} )
	generate_export_header(PipelinePathologyFEIAI EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelinePathologyFEIAI_export.h)
	#add_library(PipelinePathologyFEIAIFactory SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyFEIAIFactory.cpp )
    #target_link_libraries(PipelinePathologyFEIAIFactory PipelinePathologyFEIAI ${ITK_LIBRARIES} ${TFS_LIB_ALL} ${UTIALS_LIB_ALL} ${OpenCV_LIBS} ${TF_LIB} ${Openslide_LIB})
	#generate_export_header(PipelinePathologyFEIAIFactory EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelinePathologyFEIAIFactory_export.h)
else()
	add_library(PipelinePathologyFEIAI STATIC ${PathologyFEIAI_SOURCE} )
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyFEIAI optimized ${_LIB} )
	endforeach()
	target_link_libraries(PipelinePathologyFEIAI optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyFEIAI debug ${_LIB})
	endforeach()
	target_link_libraries(PipelinePathologyFEIAI debug ${UTIALS_LIB_ALL} )
	add_library(PipelinePathologyFEIAIFactory STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyFEIAIFactory.cpp )
    target_link_libraries(PipelinePathologyFEIAIFactory PipelinePathologyFEIAI ${ITK_LIBRARIES} ${TFS_LIB_ALL} ${UTIALS_LIB_ALL} ${OpenCV_LIBS} ${TF_LIB} ${Openslide_LIB})
endif()

################################################################################
# unit test
################################################################################
add_executable(PipelinePathologyFEIAITest 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/PipelinePathologyFEIAITest.cpp)
foreach(_LIB ${CONAN_LIBS_RELEASE})
	target_link_libraries(PipelinePathologyFEIAITest optimized ${_LIB} )
endforeach()
target_link_libraries(PipelinePathologyFEIAITest optimized ${UTIALS_LIB_ALL} )
foreach(_LIB ${CONAN_LIBS_DEBUG})
	target_link_libraries(PipelinePathologyFEIAITest debug ${_LIB})
endforeach()
target_link_libraries(PipelinePathologyFEIAITest debug ${UTIALS_LIB_ALL} )
target_link_libraries(PipelinePathologyFEIAITest PipelinePathologyFEIAI )