# Object libraries require CMAKE 2.8.8 version 
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8) 
MESSAGE(STATUS "CMAKE VERSION ${CMAKE_VERSION}")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

FILE(GLOB HIPSRCS *.cpp) 

IF(${PLATFORM} MATCHES "hcc")
  # Find HCC compiler
  FIND_PACKAGE(HC++ 1.0 REQUIRED)

  EXECUTE_PROCESS(COMMAND ${HCC_CONFIG} --install --cxxflags
                          OUTPUT_VARIABLE HCC_CXXFLAGS)
  EXECUTE_PROCESS(COMMAND ${HCC_CONFIG}  --install --ldflags
                          OUTPUT_VARIABLE HCC_LDFLAGS)

  SET(HIPRNG_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/include/")
  SET(HIPRNG_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../build/lib/src")

  STRING(STRIP "${HCC_CXXFLAGS}" HCC_CXXFLAGS)
  SET(HCC_CXXFLAGS "${HCC_CXXFLAGS} -I${HIPRNG_INCLUDE_PATH}")
  STRING(STRIP "${HCC_LDFLAGS}" HCC_LDFLAGS)
  SET(HCC_LDFLAGS "${HCC_LDFLAGS} -L${HIPRNG_LIBRARY_PATH} -L${HIP_PATH}/lib -amdgpu-target=gfx803 -amdgpu-target=gfx900")
  INCLUDE_DIRECTORIES(${HIP_PATH}/include)
  
  SET(LINK "-lhiprng -lm -lhip_hcc")

  SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
  FOREACH(test_file ${HIPSRCS})
    SET_PROPERTY(SOURCE ${test_file} ../gtest_main.cpp ../gtest-all.cpp APPEND_STRING PROPERTY COMPILE_FLAGS " ${HCC_CXXFLAGS} -DGTEST_HAS_TR1_TUPLE=0")
    GET_FILENAME_COMPONENT(name_without_extension ${test_file} NAME_WE)
    ADD_EXECUTABLE(${name_without_extension} ${test_file} ../gtest_main.cpp ../gtest-all.cpp)
    SET_PROPERTY(TARGET ${name_without_extension} APPEND_STRING PROPERTY LINK_FLAGS " ${HCC_LDFLAGS} ${LINK}")
  ENDFOREACH()

ELSEIF(${PLATFORM} MATCHES "nvcc")
  SET(HIPRNG_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/include/")
  SET(HIPRNG_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../build/lib/src")

  SET(HIP_CXXFLAGS "${HIP_CXXFLAGS} -I${HIPRNG_INCLUDE_PATH} -I/usr/local/cuda/include/ -I${HIP_PATH}/include")
  SET(HIP_LDFLAGS "${HIP_LDFLAGS} -L${HIPRNG_LIBRARY_PATH} -L/usr/local/cuda/lib64")

  INCLUDE_DIRECTORIES(${HIP_PATH}/include)

  SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
  FOREACH(test_file ${HIPSRCS})
    SET_PROPERTY(SOURCE ${test_file} ../gtest_main.cpp ../gtest-all.cpp APPEND_STRING PROPERTY COMPILE_FLAGS " ${HIP_CXXFLAGS} -DGTEST_HAS_TR1_TUPLE=0 -D__HIP_PLATFORM_NVCC__")
    GET_FILENAME_COMPONENT(name_without_extension ${test_file} NAME_WE)
    ADD_EXECUTABLE(${name_without_extension} ${test_file} ../gtest_main.cpp ../gtest-all.cpp)
    SET_PROPERTY(TARGET ${name_without_extension} APPEND_STRING PROPERTY LINK_FLAGS " ${HIP_LDFLAGS} ${LINK}")
    TARGET_LINK_LIBRARIES(${name_without_extension} cudart hiprng pthread)
    ADD_TEST(NAME ${name_without_extension} COMMAND ./bin/${name_without_extension})
  ENDFOREACH()
ENDIF()
