project(sample)
cmake_minimum_required(VERSION 2.8)

set(TARGET sample_prog)

# Set the source dir
set(SAMPLE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

# Set the include dir
set(SAMPLE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# nsengine include dir (copied engine's files in to here)
set(NSENGINE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/nsengine/include/nsengine)


# Set project includes dir - this is where the compiler will search when saying #include <blabla.h>
include_directories(
    "${SAMPLE_INCLUDE_DIR}"
    "${NSENGINE_INCLUDE_DIR}"
    )

# Set the src files for the project - all files in source dir
file(GLOB SRC_FILES
"${SAMPLE_SOURCE_DIR}/*.cpp"
"${SAMPLE_SOURCE_DIR}/*.c"
)

# Set the header files - this is only used in the "add_custom_target" call below to correctly show the header files in Qt creator
file(GLOB HEADER_FILES
"${SAMPLE_INCLUDE_DIR}/*.h"
)

set(PLATFORM "x64" CACHE STRING "Platform for which to run cmake (run for each build type)")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${PLATFORM})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${PLATFORM})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${PLATFORM})
set(CMAKE_DEBUG_POSTFIX d)

# Preprocessor definitions - these are required since some of the included header files use them (try without after get working)
add_definitions(-DGLEW_MX -DGLEW_STATIC -DSTB_IMAGE_WRITE_IMPLEMENTATION -DSTB_IMAGE_IMPLEMENTATION)
if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
  add_definitions(-DNSDEBUG)
endif()

# Setup custom compiler and linker arguments depending on if unix or windows
if (UNIX)

    # only needed for unix compilers
    add_definitions(-std=c++11)

    # Set compiler flags for c++
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fms-extensions")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

    # Make it so the executable is created in bin
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

elseif (WIN32)

    # Set MSVC compiler flags for c++
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} /WX /NODEFAULTLIB:MSVCRT /NODEFAULTLIB:LIBCMT" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

    # Windows can build for 32 bit and 64 bit - unix doesn't support this very well
    set(PLATFORM "x64" CACHE STRING "Platform for which to run cmake (run for each build type)")

    # Make it so the executable is created in bin/platform (windows builds either x64 or x86)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${PLATFORM})

endif()

link_directories(${PROJECT_SOURCE_DIR}/deps/nsengine/lib/${PLATFORM})
message("Adding link directory: ${PROJECT_SOURCE_DIR}/deps/nsengine/lib/${PLATFORM}")

# This is here simply so Qt shows the files correctly in the project window
add_custom_target(headers SOURCES ${HEADER_FILES})
add_custom_target(sources SOURCES ${SRC_FILES})

add_executable(${TARGET} ${SRC_FILES})
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
if (WIN32)
    set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} )
    set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} )
    set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
    set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
    set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
    set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
endif (WIN32)

# Link all libraries
# The order here kind of matters - nsmath and nsengine need to go first cause they are missing symbols
# defined by later libs

# Find opengl
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(ERROR "Error: OpenGL not found on your system - please make sure you have it downloaded.")
endif(NOT OPENGL_FOUND)

# Set nsengine's libs path
target_link_libraries(${TARGET}
debug nsengined
optimized nsengine
debug nsmathd
optimized nsmath
debug freetyped
optimized freetype
general ${OPENGL_LIBRARIES}
)

if (UNIX)
    target_link_libraries(${TARGET}
    debug assimpd
    optimized assimp
    debug glfwd
    optimized glfw
    general sndfile
    general openal)
elseif (WIN32)
    target_link_libraries(${TARGET}
    general assimp-vc140-mt
    debug glfw3ddll
    optimized glfw3dll
    general libsndfile-1
    general OpenAL32
    DbgHelp)
endif()
