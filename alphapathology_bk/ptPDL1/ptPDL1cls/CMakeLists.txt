project(PathologyPDL1)
################################################################################
# Include paths of header files 
################################################################################
set(PathologyPDL1_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
	${GLOB_PY_INCLUDE}
	${GLOB_TENSORFLOW_INCLUDE}
	${SOURCECODE_PATH}/common/base/inc
	#${SOURCECODE_PATH}/common/infer/inc
	${SOURCECODE_PATH}/common/utils/inc
	#${SOURCECODE_PATH}/common/imageProcess/inc
	${OpenCV_INCLUDE_DIRS})
include_directories(${PathologyPDL1_INCLUDES})

################################################################################
# Configure source files
################################################################################
set(PathologyPDL1_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelineElementPathologyPDL1Cls.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyPDL1.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/IFPipelinePathologyPDL1In.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/InferLocalPathologyPDL1.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyPDL1Factory.cpp
	)



################################################################################
# Configure lib files
################################################################################
set(UTIALS_LIB_ALL
	CommonUtils
	${COMM_UTIL_LIB})

if(BUILD_SHARED_LIBS)
	add_library(PipelinePathologyPDL1 SHARED ${PathologyPDL1_SOURCE})
	target_include_directories(PipelinePathologyPDL1 PUBLIC ${PathologyPDL1_INCLUDES})
	target_link_libraries(PipelinePathologyPDL1  ${ITK_LIBRARIES} ${OpenCV_LIBS} ${TF_LIB} ${UTIALS_LIB_ALL})
	generate_export_header(PipelinePathologyPDL1 EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelinePathologyPDL1_export.h)
	#add_library(PipelinePathologyPDL1Factory SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyPDL1Factory.cpp )
    #target_link_libraries(PipelinePathologyPDL1Factory PipelinePathologyPDL1 ${ITK_LIBRARIES} ${TFS_LIB_ALL} ${UTIALS_LIB_ALL} ${OpenCV_LIBS} ${TF_LIB})
	#generate_export_header(PipelinePathologyPDL1Factory EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelinePathologyPDL1Factory_export.h)
else()
	add_library(PipelinePathologyPDL1 STATIC ${PathologyPDL1_SOURCE} )
	target_link_libraries(PipelinePathologyPDL1 ${ITK_LIBRARIES} ${UTIALS_LIB_ALL} ${OpenCV_LIBS} ${TF_LIB})
	add_library(PipelinePathologyPDL1Factory STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyPDL1Factory.cpp )
    target_link_libraries(PipelinePathologyPDL1Factory PipelinePathologyPDL1 ${ITK_LIBRARIES} ${TFS_LIB_ALL} ${UTIALS_LIB_ALL} ${OpenCV_LIBS} ${TF_LIB})
endif()

################################################################################
# unit test
################################################################################
add_executable(PipelinePathologyPDL1Test 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/PipelinePathologyPDL1Test.cpp)
target_link_libraries(PipelinePathologyPDL1Test PipelinePathologyPDL1 ${ITK_LIBRARIES} ${UTIALS_LIB_ALL} ${OpenCV_LIBS} ${TF_LIB})
