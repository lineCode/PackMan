cmake_minimum_required (VERSION 2.6)
project (PackMan)

# Include folder.
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/Include)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/Vendor/cppfs/source/cppfs/include)

# Libraries.
option (BUILD_SHARED_LIBS OFF)
option (OPTION_BUILD_TESTS OFF)

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/Vendor/cppfs)

# Command line executable.
add_executable (PackManCMD Source/Main.cpp Source/PackMan.cpp Include/PackMan.h)

target_link_libraries (PackManCMD cppfs)

# Static library.
add_library (PackMan Source/PackMan.cpp Include/PackMan.h)

# Doxygen.
find_package (Doxygen)
if (DOXYGEN_FOUND)
    set (DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Documentation/Doxyfile.in)
    set (DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file (${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target (Documentation ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM)
endif (DOXYGEN_FOUND)