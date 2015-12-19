cmake_minimum_required (VERSION 2.8)

DETECT_ARCH()

default_source_scan("${SRCDIR}")

add_library(${target_name} STATIC ${sources} ${headers})

include_directories("${BOOST_ROOT}")
include_directories("${SRCDIR}/src")
include_directories("${SRCDIR}/include")
include_directories("${SRCDIR}/../include")

make_project(${target_name})
    
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
endif()
                      
