cmake_minimum_required (VERSION 2.6)
PROJECT(mmdd C CXX)

include_directories( ./source/include/ )
include_directories( ${CMAKE_INSTALL_PREFIX}/include/fedlibrary/include )

IF(NOT CMAKE_BUILD_TYPE )
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
ENDIF()

IF( CMAKE_BUILD_TYPE STREQUAL "Debug" )
  set(  MMDD_VERSION
        -debug
     )
  set(  MMDC_VERSION
        -debug
     )
  set(  MMDE_VERSION
        -debug
     )
  set(  FEDLIB_VER
        -3.2.0-static-debug
     )
ELSE()
  set(  MMDD_VERSION
     )
  set(  MMDC_VERSION
     )
  set(  MMDE_VERSION
     )
  set(  FEDLIB_VER
        -3.2.0-static
     )
ENDIF()


option (MMDD_EXEC_SUPPORT          "Support Exec commands"             OFF) 

if (MMDD_EXEC_SUPPORT)
  add_definitions( -D_MMDD_EXEC_ENABLED )
endif (MMDD_EXEC_SUPPORT)


set(  MMDD_INCLUDE 
        ./source/include/Mmdd.h
        ./source/include/MmddConfig.h
        ./source/include/MmddConfigException.h
        ./source/include/MmddEvents.h
        ./source/include/crc/crc-ccitt.h
        ./source/include/crc/lib_crc.h
        ./source/include/proto/mmd_proto.h
)

set(  MMDD_SRC   
        ./source/src/main.cpp
        ./source/src/Mmdd.cpp
        ./source/src/MmddConfig.cpp
        ./source/src/MmddConfigException.cpp
        ./source/src/MmddEvents.cpp
        ./source/src/crc/crc-ccitt.c
        ./source/src/crc/lib_crc.c
)

set(  MMDC_INCLUDE 
        ./source/include/crc/crc-ccitt.h
        ./source/include/crc/lib_crc.h
        ./source/include/proto/mmd_proto.h
)

set(  MMDE_INCLUDE 
        ./source/include/crc/crc-ccitt.h
        ./source/include/crc/lib_crc.h
        ./source/include/proto/mmd_proto.h
)

set(  MMDC_SRC   
        ./source/src/mmdc.cpp
        ./source/src/crc/crc-ccitt.c
        ./source/src/crc/lib_crc.c
)

set(  MMDE_SRC   
        ./source/src/mmde.cpp
        ./source/src/crc/crc-ccitt.c
        ./source/src/crc/lib_crc.c
)

set(  MMDD_LIBRARIES
      fedlibrary${FEDLIB_VER}
      pthread
)

set(  MMDC_LIBRARIES
      fedlibrary${FEDLIB_VER}
      pthread
)

set(  MMDE_LIBRARIES
      fedlibrary${FEDLIB_VER}
      pthread
)

IF( CMAKE_BUILD_TYPE STREQUAL "Debug" )
add_definitions(
       -ggdb3
)
ENDIF()

add_definitions( 
                  -Wall
                  -std=c++11
                  -D__STDC_CONSTANT_MACROS
                  -D_NO_FED_NAMESPACE
                  -D_TRACING_EXCEPTION_RAISE
                  -D_TRACING_EXCEPTION_CATCH
                  -D_ENABLE_ERROR_INFO
                  -D_ENABLE_LOG_INFO
                  -D_ENABLE_RAW_INFO
                  -D_ENABLE_METHOD_TRACING
                  -D_ENABLE_VERBOSE_INFO 
                  -D_ENABLE_ASSERTION_TRACING
                )

add_executable( mmdd${MMDD_VERSION}          ${MMDD_SRC}       )
add_executable( mmdc${MMDC_VERSION}          ${MMDC_SRC}       )
add_executable( mmde${MMDE_VERSION}          ${MMDE_SRC}       )

target_link_libraries( mmdd${MMDD_VERSION}   ${MMDD_LIBRARIES} )
target_link_libraries( mmdc${MMDC_VERSION}   ${MMDC_LIBRARIES} )
target_link_libraries( mmde${MMDE_VERSION}   ${MMDE_LIBRARIES} )

