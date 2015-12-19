cmake_minimum_required (VERSION 2.8)
project (LegacyDemo)

find_package(PythonInterp)
find_package(Boost)

execute_process(COMMAND "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/GenerateMocks.py")

add_executable (LegacyDemo 

# our little program
    main.cpp 
    MyClass.cpp
    MyClass.h

# this will compile all of LegacyMock right into our program
    ../../include/citrix/srclink/citrix.mocking-all.cpp 

# this will compile the generated mock
    GeneratedMocks/MyClassMock.cpp
    GeneratedMocks/MyClassMock.h)

target_include_directories (LegacyDemo PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}"

# LegacyMock requires you to add this to includes atm
    "${Boost_INCLUDE_DIRS}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include"

# This project also generates GMocks for the test-class so we need
# to include GMock. 
    "${CMAKE_CURRENT_SOURCE_DIR}/../../citrix.gmock/include"    
)

add_custom_command(
    TARGET LegacyDemo
    PRE_BUILD
    COMMAND "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/GenerateMocks.py"
)
