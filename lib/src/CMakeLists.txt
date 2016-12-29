
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8) 
MESSAGE(STATUS "CMAKE VERSION ${CMAKE_VERSION}")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#Setting a Project name
SET(PROJECT_NAME "hcrng")
SET(PROJECT_NAME_EXT "hiprng")
PROJECT (${PROJECT_NAME})
MESSAGE(STATUS "PROJECT NAME    : ${PROJECT_NAME}")

# Build hcblas and hipblas from source on AMD platform
IF (${PLATFORM} MATCHES "hcc")

  # Find HCC compiler
  FIND_PACKAGE(HC++ 1.0 REQUIRED)

  # Find HIP
  set (HIP_PATH $ENV{HIP_PATH})
  if (NOT DEFINED HIP_PATH)
        set (HIP_PATH /opt/rocm/hip)
  endif()

  execute_process(COMMAND ${HIP_PATH}/bin/hipconfig --platform OUTPUT_VARIABLE HIP_PLATFORM)
  MESSAGE ("HIP_PATH=" ${HIP_PATH})
  SET(HIP_SHARED_OBJ $ENV{HIP_SHARED_OBJ})

  ADD_SUBDIRECTORY(generators)

  #Setting a variable for source files
  SET (SRCS ${RNGSRC})

  execute_process(COMMAND ${HCC_CONFIG} --install --cxxflags
                            OUTPUT_VARIABLE HCC_CXXFLAGS)
  execute_process(COMMAND ${HCC_CONFIG} --install --ldflags --shared
                            OUTPUT_VARIABLE HCC_LDFLAGS)
  
  string(STRIP "${HCC_CXXFLAGS}" HCC_CXXFLAGS)
  string(STRIP "${HCC_LDFLAGS}" HCC_LDFLAGS)
  set (HCC_CXXFLAGS "${HCC_CXXFLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}/../include -I${HIP_PATH}/include -D__HIP_PLATFORM_HCC__=")
  set (HCC_LDFLAGS "${HCC_LDFLAGS} -L${HIP_PATH}/lib -lhip_hcc")

  FOREACH(src_file ${SRCS})
          SET_PROPERTY(SOURCE ${src_file} APPEND_STRING PROPERTY COMPILE_FLAGS " ${HCC_CXXFLAGS} ")
  ENDFOREACH()

  ADD_LIBRARY("${PROJECT_NAME}" SHARED ${SRCS})
  SET_PROPERTY(TARGET "${PROJECT_NAME}" APPEND_STRING PROPERTY LINK_FLAGS " ${HCC_LDFLAGS} ")
  TARGET_LINK_LIBRARIES("${PROJECT_NAME}" hc_am)
  INSTALL(TARGETS "${PROJECT_NAME}" 
    RUNTIME DESTINATION lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )

  file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.h") 
  INSTALL(FILES ${headers} DESTINATION include)
  

  IF(DEFINED HIP_SHARED_OBJ)
    SET(HIPRNGSRCS ${RNGSRC} ${CMAKE_CURRENT_SOURCE_DIR}/hcc_detail/hiprng.cpp)

    # Setting header dependencies for hiprng library generation
    FOREACH(src_file ${HIPRNGSRCS})
     SET_PROPERTY(SOURCE ${src_file} APPEND_STRING PROPERTY COMPILE_FLAGS " ${HCC_CXXFLAGS} ")
    ENDFOREACH()

    #Generating hiprng shared object
    ADD_LIBRARY("${PROJECT_NAME_EXT}" SHARED ${HIPRNGSRCS})
    SET_PROPERTY(TARGET "${PROJECT_NAME_EXT}" APPEND_STRING PROPERTY LINK_FLAGS " ${HCC_LDFLAGS} ")
    TARGET_LINK_LIBRARIES("${PROJECT_NAME_EXT}" hc_am)

    INSTALL(TARGETS "${PROJECT_NAME_EXT}" 
      RUNTIME DESTINATION lib
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib
    )

    file(GLOB hcc_detail "${CMAKE_CURRENT_SOURCE_DIR}/../include/hcc_detail/*.h")
    file(GLOB nvcc_detail "${CMAKE_CURRENT_SOURCE_DIR}/../include/nvcc_detail/*.h")
    INSTALL(FILES ${hcc_detail} DESTINATION include/hcc_detail)
    INSTALL(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../include/" DESTINATION include)
  ENDIF()
ELSEIF (${PLATFORM} MATCHES "nvcc")
  MESSAGE(STATUS "Building hiprng.cpp")
  SET(HIPRNGSRCS ${CMAKE_CURRENT_SOURCE_DIR}/nvcc_detail/hiprng.cpp)

  set (CXXFLAGS "-I${CMAKE_CURRENT_SOURCE_DIR}/../include -I${HIP_PATH}/include -I/usr/local/cuda/include -D__HIP_PLATFORM_NVCC__=")
  set (LDFLAGS "-L/usr/local/cuda/lib64 -L/usr/local/cuda/lib")

  FOREACH(src_file ${HIPRNGSRCS})
          SET_PROPERTY(SOURCE ${src_file} APPEND_STRING PROPERTY COMPILE_FLAGS " ${CXXFLAGS} ")
  ENDFOREACH()
  ADD_LIBRARY("${PROJECT_NAME_EXT}" SHARED ${HIPRNGSRCS})
  SET_PROPERTY(TARGET "${PROJECT_NAME_EXT}" APPEND_STRING PROPERTY LINK_FLAGS " ${LDFLAGS} ")
  TARGET_LINK_LIBRARIES("${PROJECT_NAME_EXT}" cudart curand)
  INSTALL(TARGETS "${PROJECT_NAME_EXT}" 
    RUNTIME DESTINATION lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )
  file(GLOB hcc_detail "${CMAKE_CURRENT_SOURCE_DIR}/../include/hcc_detail/*.h")
  file(GLOB nvcc_detail "${CMAKE_CURRENT_SOURCE_DIR}/../include/nvcc_detail/*.h")
  INSTALL(FILES ${hcc_detail} DESTINATION include/hcc_detail)
  INSTALL(FILES ${nvcc_detail} DESTINATION include/nvcc_detail)
ENDIF()

