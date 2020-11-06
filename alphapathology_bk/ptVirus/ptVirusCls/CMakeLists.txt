project(ptVirusCls)
#add_definitions (-D_GLIBCXX_USE_CXX11_ABI=0) ## opencv

################################################################################
# Include paths of header files 
################################################################################
set(LIBPipelineElementPathologyVirus_INCLUDES  ${CMAKE_CURRENT_SOURCE_DIR}/inc
				${CMAKE_CURRENT_SOURCE_DIR}/inc
				${GLOB_PY_INCLUDE}
			    ${GLOB_TENSORFLOW_INCLUDE}
				${SOURCECODE_PATH}/common/base/inc
				${SOURCECODE_PATH}/common/infer/inc
				${SOURCECODE_PATH}/common/utils/inc
				${SOURCECODE_PATH}/common/imageProcess/inc
				${ITK_INCLUDE_DIRS}
				${OPENCV_INCLUDE_DIRS})

include_directories(${LIBPipelineElementPathologyVirus_INCLUDES})
link_directories(${OpenCV_LIBS}  ${TFS_LIB_ALL}) 
aux_source_directory(src LIBPipelineElementVirus_SRCS)

################################################################################
# Configure lib files
################################################################################
set(UTIALS_LIB_ALL CommonUtils ${COMM_UTIL_LIB})



if(BUILD_SHARED_LIBS)
	add_library(PipelinePathologyVirus SHARED ${LIBPipelineElementVirus_SRCS})
	target_include_directories(PipelinePathologyVirus PUBLIC ${LIBPipelineElementPathologyVirus_INCLUDES})
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyVirus optimized ${_LIB} )
	endforeach()
	target_link_libraries(PipelinePathologyVirus optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyVirus debug ${_LIB})
	endforeach()
	target_link_libraries(PipelinePathologyVirus debug ${UTIALS_LIB_ALL} )
	generate_export_header(PipelinePathologyVirus EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelinePathologyVirus_export.h)
else()
	add_library(PipelinePathologyVirus STATIC ${LIBPipelineElementVirus_SRCS})
	target_include_directories(PipelinePathologyVirus PUBLIC ${LIBPipelineElementPathologyVirus_INCLUDES})
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(PipelinePathologyVirus optimized ${_LIB} )
	endforeach()
	target_link_libraries(PipelinePathologyVirus optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(PipelinePathologyVirus debug ${_LIB})
	endforeach()
	target_link_libraries(PipelinePathologyVirus debug ${UTIALS_LIB_ALL} )
	generate_export_header(PipelinePathologyVirus EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelinePathologyVirus_export.h)
	add_library(PipelinePathologyVirusFactory STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/PipelinePathologyVirusFactory.cpp )
    target_link_libraries(PipelinePathologyVirusFactory PipelinePathologyVirus ${ITK_LIBRARIES} ${TFS_LIB_ALL} ${UTIALS_LIB_ALL} ${OpenCV_LIBS} ${TF_LIB})
	generate_export_header(PipelinePathologyVirusFactory EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelinePathologyVirusFactory_export.h)
endif()



################################################################################
# Configure unit test
################################################################################
add_executable(PipelinePathologyVirusTest ${CMAKE_CURRENT_SOURCE_DIR}/test/PipelinePathologyVirusTest.cpp)
target_include_directories(PipelinePathologyVirusTest PUBLIC ${LIBPipelineElementPathologyVirus_INCLUDES})
foreach(_LIB ${CONAN_LIBS_RELEASE})
	target_link_libraries(PipelinePathologyVirusTest optimized ${_LIB} )
endforeach()
target_link_libraries(PipelinePathologyVirusTest optimized ${UTIALS_LIB_ALL} )
foreach(_LIB ${CONAN_LIBS_DEBUG})
	target_link_libraries(PipelinePathologyVirusTest debug ${_LIB})
endforeach()
target_link_libraries(PipelinePathologyVirusTest debug ${UTIALS_LIB_ALL} )
target_link_libraries(PipelinePathologyVirusTest PipelinePathologyVirus )