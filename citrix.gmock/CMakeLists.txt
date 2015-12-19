cmake_minimum_required (VERSION 2.8)

DETECT_ARCH()

default_source_scan("${SRCDIR}")

add_library(${target_name} STATIC ${sources} ${headers})

include_directories("${SRCDIR}/include")

make_project(${target_name})
    
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_package (Threads)
    target_link_libraries(${target_name} ${CMAKE_THREAD_LIBS_INIT})
endif()
                      
