project(utils)
################################################################################
# Include paths of header files 
################################################################################
set(LIBCOMMON_INCLUDES
	${GLOB_PY_INCLUDE}
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
	${ITK_INCLUDE_DIR}	
    )
include_directories(${LIBCOMMON_INCLUDES})

################################################################################
# Configure source files
################################################################################
set(LIBCOMMON_SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CommonUtils.cpp)

	
if(BUILD_SHARED_LIBS)
	add_library(CommonUtils SHARED ${LIBCOMMON_SOURCE} )
	foreach(_LIB ${CONAN_LIBS_RELEASE})
		target_link_libraries(CommonUtils optimized ${_LIB})
	endforeach()
	foreach(_LIB ${CONAN_LIBS_DEBUG})
		target_link_libraries(CommonUtils debug ${_LIB})
	endforeach()
	generate_export_header(CommonUtils EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/inc/CommonUtils_export.h)
else()
	add_library(CommonUtils STATIC ${LIBCOMMON_SOURCE} )
	target_link_libraries(CommonUtils ${ITK_LIBRARIES} ${COMM_UTIL_LIB})
endif()	


################################################################################
# install include and lib for acl test
################################################################################
INSTALL(TARGETS CommonUtils
       RUNTIME DESTINATION bin
       LIBRARY DESTINATION lib
       ARCHIVE DESTINATION libstatic)
