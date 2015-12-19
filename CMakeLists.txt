cmake_minimum_required (VERSION 2.8)
    
 # enables precompiled header for given target (currently supported for MSVC only, feel free to add more)
MACRO(USE_PCH target)
	IF(NOT EXISTS "${STDAFX_CPP}")
		message(WARN " No precompiled header found during source scan or source scan not used. Set STDAFX_CPP manually to the correct path.")
	else()
		# message(INFO " Enabling precompiled header for ${STDAFX_CPP}")
		if (MSVC)
			set_property(TARGET ${target} APPEND_STRING PROPERTY COMPILE_FLAGS "/FIstdafx.h /Yustdafx.h")
			set_source_files_properties(${STDAFX_CPP} APPEND_STRING PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
		endif(MSVC)
	endif()
ENDMACRO(USE_PCH)

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

MACRO(ADD_SUBDIRECTORIES curdir)
	SUBDIRLIST(subdirs "${curdir}")
	FOREACH(child ${subdirs})
		IF(EXISTS "${curdir}/${child}/CMakeLists.txt")
			# construct canonical path to source directory
			get_filename_component(SRCDIR "${curdir}/${child}" REALPATH)
			set(SRCDIR "${SRCDIR}/")
			get_filename_component(target_name "${child}" NAME)
			add_subdirectory("${curdir}/${child}")
		ENDIF()
	ENDFOREACH()
ENDMACRO()

# internal macro for default_source_scan() 
macro(scan_sources root_name dir_name_ out_list extenstions)

    get_filename_component(dir_name "${dir_name_}" REALPATH)
	
    FOREACH (ext IN ITEMS ${extenstions})
        FILE (GLOB_RECURSE new_list "${dir_name}/*.${ext}")
        FOREACH (src_ IN ITEMS ${new_list})
			get_filename_component(src "${src_}" REALPATH)
            get_filename_component(fileDir "${src}" PATH)

            # add IDE file group based on relative path to ${dir_name}
            STRING (REGEX REPLACE "^${dir_name}" "${root_name}" rel_dir "${fileDir}/")
            STRING (REGEX REPLACE "[/]+" "\\\\" rel_dir "${rel_dir}")
			
			SOURCE_GROUP ("${rel_dir}" FILES "${src}")
			LIST(APPEND ${out_list} "${src}")
			
			IF("${src}" MATCHES "/stdafx\\.cpp$")
				SET(STDAFX_CPP "${src}")
			Endif()
        ENDFOREACH (src_)
    ENDFOREACH(ext)
endmacro(scan_sources)

macro(postbuild_copy_file target src dst)
    get_filename_component(src2 "${src}" REALPATH)
    get_filename_component(dst2 "${dst}" REALPATH)

    add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ARGS "${src2}" "${dst2}")
endmacro(postbuild_copy_file)

# scans the given directory ${rootDir} recursively for C++ header/source files
# and adds them to the current compilation stage. The relative directory tree
# within the scan directory is kept for all IDE's supporting some kind of folders.
macro(default_source_scan rootDir)
    scan_sources("Header Files" "${rootDir}" headers "${INC_EXTENSIONS}")
    scan_sources("Source Files" "${rootDir}" sources "${SRC_EXTENSIONS}")
endmacro(default_source_scan)

macro(default_tests_scan rootDir moduleName)
    scan_sources("Header Files\\\\\\\\${moduleName}" "${rootDir}" headers "${INC_EXTENSIONS}")
    scan_sources("Source Files\\\\\\\\${moduleName}" "${rootDir}" sources "${SRC_EXTENSIONS}")
endmacro(default_tests_scan)

# create a new project with name ${project_name}
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

# tries to find a certain library in directory ${dir_name} while trying
# a semicolon separated list of aliases (you can also specify only one potential name).
# If it can find the library, a variable named ${var_prefix} is filled with a
# canonical path to it. Otherwise, an invalid path named "${var_prefix}-NOTFOUND" is stored.
# Further, if the library can not be found, CMake will report this issue and prevent
# generation until it is resolved.
macro(link_external_library var_prefix dir_name lib_name_list)
    find_library(${var_prefix} NAMES ${lib_name_list} PATHS ${dir_name} NO_DEFAULT_PATH)

    if("${var_prefix}-NOTFOUND" STREQUAL "${${var_prefix}}")
        message(WARNING "A library with alias(es) \"${lib_name_list}\" could not be found in directory \"${dir_name}\"!")
    elseif(NOT EXISTS "${${var_prefix}}")
        message(WARNING "A library with alias(es) \"${lib_name_list}\" could not be found at path \"${${var_prefix}}\"!")
    endif()

    set(LIBS ${LIBS} ${var_prefix})
endmacro(link_external_library)

# allows you to put a project into a solution-subfolder in visual studio.
# Seems to be not supported for VS2012 in CMake 2.8.9 though.
MACRO(SET_VISUAL_STUDIO_FOLDER project_name folder)
    IF(MSVC)
        set(USING_VISUAL_STUDIO_EXPRESS CACHE "Are you targeting an express version of visual studio?")

        IF(NOT ${USING_VISUAL_STUDIO_EXPRESS})
            SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
            SET_PROPERTY(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "")

            IF(NOT ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 2.8)
                SET_PROPERTY(TARGET "${project_name}" PROPERTY FOLDER "${folder}")
            ENDIF()
        ENDIF()
        ELSE()
            SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS OFF)
        ENDIF()
    ENDIF()
ENDMACRO(SET_VISUAL_STUDIO_FOLDER)

# adds a post-build action (if supported by the target build system) which copies the 
# generated archive of a project ${projectName} to a given target directory ${targetDir}.
macro(postbuild_copy_archive projectName targetDir)
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        get_target_property(tmpPath ${projectName} ARCHIVE_OUTPUT_DIRECTORY)
        file(MAKE_DIRECTORY "${targetDir}")
        postbuild_copy_file(${projectName} "${tmpPath}/${projectName}.lib" "${targetDir}/${projectName}.lib")
    ENDIF()
endmacro(postbuild_copy_archive)

# adds a post-build action (if supported by the target build system) which copies the 
# compiled library of a project ${projectName} to a given target directory ${targetDir}.
macro(postbuild_copy_library projectName targetDir)
    get_target_property(tmpPath ${projectName} LIBRARY_OUTPUT_DIRECTORY)
    file(MAKE_DIRECTORY "${targetDir}")
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        postbuild_copy_file(${projectName} "${tmpPath}/${projectName}.dll" "${targetDir}/${projectName}.dll")
    ELSE()
        postbuild_copy_file(${projectName} "${tmpPath}/${projectName}.so" "${targetDir}/${projectName}.so")
    ENDIF()
endmacro(postbuild_copy_library)
  
# adds a post-build action (if supported by the target build system) which copies the 
# compiled binary of a project ${projectName} to a given target directory ${targetDir}.
macro(postbuild_copy_runtime projectName targetDir)
    get_target_property(tmpPath ${projectName} RUNTIME_OUTPUT_DIRECTORY)
    file(MAKE_DIRECTORY "${targetDir}")
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        postbuild_copy_file(${projectName} "${tmpPath}/${projectName}.exe" "${targetDir}/${projectName}.exe")
    ELSE()
        postbuild_copy_file(${projectName} "${tmpPath}/${projectName}" "${targetDir}/${projectName}")
    ENDIF()
endmacro(postbuild_copy_runtime)

# allows user to choose between static and dynamic linkage to visual studio runtime.
macro(configure_msvc_runtime)
    if(MSVC)
        if("${MSVC_STATIC_RUNTIME}" STREQUAL "")
            set(MSVC_STATIC_RUNTIME CACHE BOOL "Statically link with Visual Studio runtime.")
        endif()
		
		set(USE_PRECOMPILED_HEADER CACHE BOOL "Does not work well on all MSVC versions, but can reduce build time considerably! VS1023 seems to be the only one that really works well with this option.")

        set(VARIABLES
                CMAKE_C_FLAGS_DEBUG
                CMAKE_C_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_DEBUG
                CMAKE_CXX_FLAGS_RELEASE
        )
    
        if(${MSVC_STATIC_RUNTIME})
            set(CMAKE_C_FLAGS_RELEASE "/MP /Zm256 /wd4512 /wd4373 /wd4505 /EHa /MT /W4 /MP /Ox /Ob2 /Oi /Ot /GL /D NDEBUG /GR " CACHE STRING "" FORCE)
            set(CMAKE_C_FLAGS_DEBUG "/MP /Zm256 /wd4512 /wd4373 /wd4505 /RTC1 /EHa /MTd /W4 /Od /Zi /D DEBUG /GR " CACHE STRING "" FORCE)

            set(CMAKE_CXX_FLAGS_RELEASE "/MP /Zm256 /wd4512 /wd4373 /wd4505 /EHa /MT /W4 /MP /MT /Ox /Ob2 /Oi /Ot /GL /D NDEBUG /GR " CACHE STRING "" FORCE)
            set(CMAKE_CXX_FLAGS_DEBUG "/MP /Zm256 /wd4512 /wd4373 /wd4505 /RTC1 /EHa /MTd /W4 /Od /Zi /D DEBUG /GR " CACHE STRING "" FORCE)
        else()      
            set(CMAKE_C_FLAGS_RELEASE "/MP /Zm256 /wd4512 /wd4373 /wd4505 /EHa /MD /W4 /MP /Ox /Ob2 /Oi /Ot /GL /D NDEBUG /GR " CACHE STRING "" FORCE)
            set(CMAKE_C_FLAGS_DEBUG "/MP /Zm256 /wd4512 /wd4373 /wd4505 /RTC1 /EHa /MDd /W4 /Od /Zi /D DEBUG /GR " CACHE STRING "" FORCE)

            set(CMAKE_CXX_FLAGS_RELEASE "/MP /Zm256 /wd4512 /wd4373 /wd4505 /EHa /MD /W4 /MP /MT /Ox /Ob2 /Oi /Ot /GL /D NDEBUG /GR " CACHE STRING "" FORCE)
            set(CMAKE_CXX_FLAGS_DEBUG "/MP /Zm256 /wd4512 /wd4373 /wd4505 /RTC1 /EHa /MDd /W4 /Od /Zi /D DEBUG /GR " CACHE STRING "" FORCE)
        endif()
    endif()
endmacro()

macro(DETECT_ARCH)
    IF (CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(ARCH "x86")
    ELSE()
        set(ARCH "x64")
    ENDIF()
endmacro(DETECT_ARCH)

# file extensions of files you want to be added to the "Header Files" group.
set(INC_EXTENSIONS h;hpp;enum;)

# file extensions of files you want to be added to the "Source Files" group
set(SRC_EXTENSIONS c;cpp;cxx;)


# just copy CMake's default compiler flags into our variables
set(DEFAULT_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(DEFAULT_CXX_RELEASE_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
set(DEFAULT_CXX_DEBUG_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
set(DEFAULT_CXX_RELWITHDEBINFO_FLAGS "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
set(DEFAULT_CXX_MINSIZEREL_FLAGS "${CMAKE_CXX_FLAGS_MINSIZEREL}")


# include project specific stuff, otherwise this file does nothing!

include(CMake_ProjectConfig.txt)

