# Object libraries require CMAKE 2.8.8 version 
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8) 
MESSAGE(STATUS "CMAKE VERSION ${CMAKE_VERSION}")

#Setting a Project name
SET(PROJECT_NAME "libhcrngtest")
PROJECT (${PROJECT_NAME})
MESSAGE(STATUS "PROJECT NAME    : ${PROJECT_NAME}")
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
FIND_PACKAGE(HC++ 1.0 REQUIRED)


#Setting a variable for source files
SET (TESTSRCS
    mrg31k3pSingle_test.cpp  mrg32k3aSingle_test.cpp  lfsr113Single_test.cpp  philox432Single_test.cpp
    mrg31k3pDouble_test.cpp  mrg32k3aDouble_test.cpp  lfsr113Double_test.cpp  philox432Double_test.cpp
    )

IF(WIN32)
  #include the additional libraries 
  #settings->c/c++->general->additional directories 
  SET(TESTSRCS ${TESTSRCS})
  ADD_DEFINITIONS(-D__LITTLE_ENDIAN__)
  ADD_DEFINITIONS(-DTARGET_OS_WIN32=1)
    
  IF(MSVC)
    ADD_DEFINITIONS(/W3)
    SET (DEBUG_PATH "../../Build/vc11-x86_64/Debug/")
    SET (RELEASE_PATH "../../Build/vc11-x86_64/Release/")
    include_directories( ../../source/)
    SET(CMAKE_CXX_STANDARD_LIBRARIES ${CMAKE_CXX_STANDARD_LIBRARIES} libhcblas.lib)
    IF(EXISTS "${DEBUG_PATH}/libhcrng.lib")
     LINK_DIRECTORIES (${DEBUG_PATH})
    ELSE(EXISTS "${DEBUG_PATH}/libhcrng.lib")
     LINK_DIRECTORIES (${RELEASE_PATH})
    ENDIF(EXISTS "${DEBUG_PATH}/libhcrng.lib")
    FOREACH(test_file ${TESTSRCS})
      STRING( REPLACE ".cpp" "" testname ${test_file} )
      ADD_EXECUTABLE( ${testname} ${test_file})
    ENDFOREACH()
  ELSE(MSVC)
    MESSAGE(FATAL_ERROR "TARGET IDE ISNT VISUAL STUDIO 11/12.... EXITING")
  ENDIF(MSVC)
ELSE(WIN32)

  # Choice to take compilation flags from source or package
  ADD_DEFINITIONS(-DLINUX)
  option(PROFILE "HCRNG profiling" OFF)
  IF(PROFILE)
   ADD_DEFINITIONS(-DPROFILE)
  ENDIF(PROFILE)

  if(EXISTS ${MCWHCCBUILD})
    execute_process(COMMAND ${HCC_CONFIG} --build --cxxflags
                            OUTPUT_VARIABLE HCC_CXXFLAGS)
    execute_process(COMMAND ${HCC_CONFIG} --build --ldflags 
                            OUTPUT_VARIABLE HCC_LDFLAGS)
  else(EXISTS ${MCWHCCBUILD})
    execute_process(COMMAND ${HCC_CONFIG} --cxxflags
                            OUTPUT_VARIABLE HCC_CXXFLAGS)
    execute_process(COMMAND ${HCC_CONFIG} --ldflags 
                            OUTPUT_VARIABLE HCC_LDFLAGS)
  endif(EXISTS ${MCWHCCBUILD})

  string(STRIP "${HCC_CXXFLAGS}" HCC_CXXFLAGS)
  set (HCC_CXXFLAGS "${HCC_CXXFLAGS}")
  string(STRIP "${HCC_LDFLAGS}" HCC_LDFLAGS)
  set (HCC_LDFLAGS "${HCC_LDFLAGS}")
  SET (LINK "-lhcrng")
  FOREACH(test_file ${TESTSRCS})
    SET_PROPERTY(SOURCE ${test_file} APPEND_STRING PROPERTY COMPILE_FLAGS " ${HCC_CXXFLAGS}")
    STRING( REPLACE "_test.cpp" "" testname ${test_file} )
    ADD_EXECUTABLE( bin/${testname} ${test_file} )
    SET_PROPERTY(TARGET bin/${testname} APPEND_STRING PROPERTY LINK_FLAGS " ${HCC_LDFLAGS} ${LINK}")
  ENDFOREACH()

ENDIF(WIN32)
