project(base)
################################################################################
# Include paths of header files 
################################################################################
set(LIBBASE_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
	${ITK_INCLUDE_DIRS}
	${GLOB_PY_INCLUDE}
)
include_directories(${LIBBASE_INCLUDES})

################################################################################
# install include and lib for acl test
################################################################################

#set(LIBBASE_SOURCE 
#    ${CMAKE_CURRENT_SOURCE_DIR}/src/CPipelineObj.cpp
#	${CMAKE_CURRENT_SOURCE_DIR}/src/CPipelineElementObj.cpp)

################################################################################
# Configure lib files
################################################################################
#add_library(COMMON ${LIBCOMMON_SOURCE} )
#target_link_libraries(COMMON ${ITK_LIBRARIES})

#install (TARGETS PipelineHemorrhagicStrokeFactory PipelineHemorrhagicStroke DESTINATION bin)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/inc/DataInferBase.h
         ${CMAKE_CURRENT_SOURCE_DIR}/inc/DataObj.h
         ${CMAKE_CURRENT_SOURCE_DIR}/inc/IFBase.h
		 ${CMAKE_CURRENT_SOURCE_DIR}/inc/IFPipelineImagesIn.h
		 ${CMAKE_CURRENT_SOURCE_DIR}/inc/InferBase.h
		 ${CMAKE_CURRENT_SOURCE_DIR}/inc/InferLocalBase.h
		 ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelineAlgorithmBase.h
		 ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelineAlgorithmExecutor.h
		 ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelineBase.h
		 ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelineElementAlgorithmBase.h
		 ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelineElementAlgorithmMachineLearningBase.h
		 ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelineElementBase.h
		 ${CMAKE_CURRENT_SOURCE_DIR}/inc/PipelineFactoryBase.h
         DESTINATION include)

#file(GLOB INFERHEADERFILES ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)
#install (FILES ${INFERHEADERFILES} DESTINATION include)
