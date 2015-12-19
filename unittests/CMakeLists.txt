cmake_minimum_required (VERSION 2.8)

DETECT_ARCH()

default_source_scan("${SRCDIR}")

SUBDIRLIST(subdirs "${SRCDIR}../")
FOREACH(child ${subdirs})
	IF(EXISTS "${SRCDIR}../${child}/CMakeLists.txt")
		# construct canonical path to source directory
		get_filename_component(childPath "${SRCDIR}../${child}" REALPATH)
		message("Scanning for tests in: '${child}'")
		
		default_tests_scan("${childPath}/tests" "${child}")
	ENDIF()
ENDFOREACH()

add_executable(${target_name} WIN32 ${sources} ${headers})

include_directories("${SRCDIR}../include")
include_directories("${SRCDIR}")
include_directories("${BOOST_ROOT}")
include_directories("${GTEST_ROOT}")

make_project(${target_name})

target_link_libraries(${target_name} citrix.hooking citrix.tracing citrix.gmock citrix.mocking citrix.binutils.udis86)
add_dependencies(${target_name} citrix.mocking.generator)
    
if(Python_FOUND)
	add_custom_command(
		TARGET ${target_name}
		PRE_BUILD
		COMMAND "${PYTHON_BINARY}" "\"${SRCDIR}/TestGenerators/GenerateTests.py\""
		)

	add_custom_command(
		TARGET ${target_name}
		PRE_BUILD
		COMMAND "${PYTHON_BINARY}" "\"${SRCDIR}/../citrix.tracing/src/BoostJsonParserHotfix.py\" -i \"${BOOST_ROOT}\" -o \"${SRCDIR}/boost_json_parser_hotfix.hpp\""
		)
	
	if(USE_GENERATOR)
		set(MockingGeneratorExe "${USE_GENERATOR}")
	else()
		get_property(MockingGeneratorExe TARGET citrix.mocking.generator PROPERTY LOCATION_${CMAKE_BUILD_TYPE})	
	endif()

	message(INFO "Use-Generator: '${MockingGeneratorExe}'")

	add_custom_command(
		TARGET ${target_name}
		PRE_BUILD
		COMMAND "${PYTHON_BINARY}" "\"${SRCDIR}/../unittests/ThingsToMock/GenerateTestMocks.py\" -gen \"${MockingGeneratorExe}\""
		)
endif()

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
                      
