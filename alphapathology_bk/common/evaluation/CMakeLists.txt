project(evaluation)
################################################################################
# Include paths of header files 
################################################################################
set(LIBCOMMON_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
	${CMAKE_CURRENT_SOURCE_DIR}/../utils/inc
	${ITK_INCLUDE_DIRS}
	${GLOB_PY_INCLUDE}
)
include_directories(${LIBCOMMON_INCLUDES})

################################################################################
# Configure source files
################################################################################
set(LIBCOMMON_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EvaluationAlg.cpp)

set(UTIALS_LIB_ALL
	CommonUtils
	${COMM_UTIL_LIB})

if(BUILD_SHARED_LIBS)
	add_library(EvaluationAlg SHARED ${LIBCOMMON_SOURCE} )
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(EvaluationAlg optimized ${_LIB} )
	endforeach()
	target_link_libraries(EvaluationAlg optimized ${UTIALS_LIB_ALL} )
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(EvaluationAlg debug ${_LIB})
	endforeach()
	target_link_libraries(EvaluationAlg debug ${UTIALS_LIB_ALL} )
	generate_export_header(EvaluationAlg EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/EvaluationAlg_export.h)
else()
	add_library(EvaluationAlg STATIC ${LIBCOMMON_SOURCE} )
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(EvaluationAlg optimized ${_LIB} )
	endforeach()
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(EvaluationAlg debug ${_LIB})
	endforeach()
endif()	

install (TARGETS EvaluationAlg DESTINATION bin)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/inc/EvaluationAlg.h DESTINATION include/common)
