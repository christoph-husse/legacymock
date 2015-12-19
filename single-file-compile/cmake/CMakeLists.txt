cmake_minimum_required (VERSION 2.8)

set(ARTIFACTS_ROOT "${CMAKE_SOURCE_DIR}/../../_output/" CACHE PATH "Root directory for built libraries and executables.")

macro(make_project project_name)
    project(${project_name})

    include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

    # get directory difference between current dir and root dir
    STRING (REGEX REPLACE "^${CMAKE_SOURCE_DIR}" "" TARGET_PARTIAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/")

    set_target_properties(${project_name}
        PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${TARGET_PARTIAL_PATH}"
            ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib/${TARGET_PARTIAL_PATH}"
            ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/${TARGET_PARTIAL_PATH}"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${TARGET_PARTIAL_PATH}"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/${TARGET_PARTIAL_PATH}"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/${TARGET_PARTIAL_PATH}"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${TARGET_PARTIAL_PATH}"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/${TARGET_PARTIAL_PATH}"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/${TARGET_PARTIAL_PATH}"
    )
endmacro(make_project)

if("$ENV{BOOST_ROOT} " STREQUAL " ")
    set(BOOST_ROOT "${Boost_INCLUDE_DIRS}" CACHE PATH "Root directory to your boost installation.")
else()
    set(BOOST_ROOT "$ENV{BOOST_ROOT}" CACHE PATH "Root directory to your boost installation.")
endif()

if("$ENV{GTEST_ROOT} " STREQUAL " ")
    set(GTEST_ROOT "${CMAKE_SOURCE_DIR}/../../citrix.gmock/include/" CACHE PATH "Root directory to your GTest installation.")
else()
    set(GTEST_ROOT "$ENV{GTEST_ROOT}" CACHE PATH "Root directory to your GTest installation.")
endif()

if("$ENV{GMOCK_ROOT} " STREQUAL " ")
    set(GMOCK_ROOT "${CMAKE_SOURCE_DIR}/../../citrix.gmock/include/" CACHE PATH "Root directory to your GMock installation.")
else()
    set(GMOCK_ROOT "$ENV{GMOCK_ROOT}" CACHE PATH "Root directory to your GMock installation.")
endif()

macro(postbuild_copy_file target src dst)
    get_filename_component(src2 "${src}" REALPATH)
    get_filename_component(dst2 "${dst}" REALPATH)

    add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ARGS "${src2}" "${dst2}")
endmacro(postbuild_copy_file)

macro(postbuild_copy_runtime projectName targetDir)
    get_target_property(tmpPath ${projectName} RUNTIME_OUTPUT_DIRECTORY)
    file(MAKE_DIRECTORY "${targetDir}")
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        postbuild_copy_file(${projectName} "${tmpPath}/${projectName}.exe" "${targetDir}/${projectName}.exe")
    ELSE()
        postbuild_copy_file(${projectName} "${tmpPath}/${projectName}" "${targetDir}/${projectName}")
    ENDIF()
endmacro(postbuild_copy_runtime)

add_definitions(/bigobj)

message (INFO " Using Boost: '${BOOST_ROOT}'")
message (INFO " Using GTest: '${GTEST_ROOT}'")
message (INFO " Using GMock: '${GMOCK_ROOT}'")

add_executable(standalone WIN32 "${CMAKE_SOURCE_DIR}/../../include/citrix/srclink/citrix.mocking-all.cpp" "${CMAKE_SOURCE_DIR}/../main.cpp")
make_project(standalone)
include_directories("${CMAKE_SOURCE_DIR}/../../include")

add_executable(unittests WIN32 "${CMAKE_SOURCE_DIR}/../../include/citrix/srclink/citrix.unittests-all.cpp")
make_project(unittests)
include_directories("${CMAKE_SOURCE_DIR}/../../include")
include_directories("${CMAKE_SOURCE_DIR}/../../unittests")
include_directories("${BOOST_ROOT}")
include_directories("${GTEST_ROOT}")
include_directories("${GMOCK_ROOT}")

postbuild_copy_runtime(unittests "${ARTIFACTS_ROOT}/bin/")