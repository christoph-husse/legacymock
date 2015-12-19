cmake_minimum_required (VERSION 2.8)

DETECT_ARCH()

default_source_scan("${SRCDIR}include/")
default_source_scan("${SRCDIR}src/")

add_library(${target_name} STATIC ${sources} ${headers})

include_directories("${SRCDIR}../include")
include_directories("${BOOST_ROOT}")

make_project(${target_name})
    
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

    postbuild_copy_archive(${target_name} "${CMAKE_SOURCE_DIR}/lib/${ARCH}/")
    
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    
endif()
                      