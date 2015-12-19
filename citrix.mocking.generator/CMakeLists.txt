cmake_minimum_required (VERSION 2.8)

DETECT_ARCH()

default_source_scan("${SRCDIR}/src")

add_executable(${target_name} WIN32 ${sources} ${headers})

include_directories("${SRCDIR}../include")
include_directories("${SRCDIR}")
include_directories("${BOOST_ROOT}")

make_project(${target_name})

target_link_libraries(${target_name} citrix.mocking)
    

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	if(MSVC_VERSION EQUAL 1700)
		# why did the chicken cross the road? VS2012 was in it's way...
		add_definitions(-D_VARIADIC_MAX=10)
	else()
		USE_PCH(${target_name})
	endif()

    postbuild_copy_runtime(${target_name} "${ARTIFACTS_ROOT}/bin/")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
endif()
                      
