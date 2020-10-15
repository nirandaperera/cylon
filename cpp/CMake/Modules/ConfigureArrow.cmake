set(ARROW_HOME ${CMAKE_BINARY_DIR}/arrow/install)
set(ARROW_ROOT ${CMAKE_BINARY_DIR}/arrow)

message("Python Executable Path ${PYTHON_EXEC_PATH}")

set(ARROW_CMAKE_ARGS " -DARROW_WITH_LZ4=OFF"
        " -DARROW_WITH_ZSTD=OFF"
        " -DARROW_WITH_BROTLI=OFF"
        " -DARROW_WITH_SNAPPY=OFF"
        " -DARROW_WITH_ZLIB=OFF"
        " -DARROW_BUILD_STATIC=ON"
        " -DARROW_BUILD_SHARED=ON"
        " -DARROW_BUILD_TESTS=OFF"
        " -DARROW_TEST_LINKAGE=static"
        " -DARROW_TEST_MEMCHECK=OFF"
        " -DARROW_BUILD_BENCHMARKS=OFF"
        " -DARROW_IPC=ON"
        " -DARROW_FLIGHT=OFF"
        " -DARROW_COMPUTE=ON"
        " -DARROW_CUDA=OFF"
        " -DARROW_JEMALLOC=OFF"
        " -DARROW_USE_GLOG=OFF"
        " -DARROW_DATASET=ON"
        " -DARROW_BUILD_UTILITIES=OFF"
        " -DARROW_HDFS=OFF"
        " -DCMAKE_VERBOSE_MAKEFILE=ON"
        " -DARROW_TENSORFLOW=OFF"
        " -DARROW_DATASET=OFF"
        " -DARROW_CSV=ON"
        " -DARROW_JSON=ON"
	    " -DARROW_BOOST_USE_SHARED=OFF"
        )

if (PYCYLON_BUILD)
    list(APPEND ARROW_CMAKE_ARGS " -DARROW_PYTHON=${PYCYLON_BUILD}"
        " -DPYTHON_EXECUTABLE=${PYTHON_EXEC_PATH}/bin/python3"
        )
endif (PYCYLON_BUILD)

message("CMake Source Dir :")
message(${CMAKE_SOURCE_DIR})
configure_file("${CMAKE_SOURCE_DIR}/CMake/Templates/Arrow.CMakeLists.txt.cmake"
        "${ARROW_ROOT}/CMakeLists.txt")

file(MAKE_DIRECTORY "${ARROW_ROOT}/build")
file(MAKE_DIRECTORY "${ARROW_ROOT}/install")

execute_process(
        COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE ARROW_CONFIG
        WORKING_DIRECTORY ${ARROW_ROOT})

if(ARROW_CONFIG)
    message(FATAL_ERROR "Configuring Arrow failed: " ${ARROW_CONFIG})
endif(ARROW_CONFIG)

set(PARALLEL_BUILD -j)
if($ENV{PARALLEL_LEVEL})
    set(NUM_JOBS $ENV{PARALLEL_LEVEL})
    set(PARALLEL_BUILD "${PARALLEL_BUILD}${NUM_JOBS}")
endif($ENV{PARALLEL_LEVEL})

execute_process(
        COMMAND ${CMAKE_COMMAND} --build .. -- -j 2
        RESULT_VARIABLE ARROW_BUILD
        WORKING_DIRECTORY ${ARROW_ROOT}/build)

if(ARROW_BUILD)
    message(FATAL_ERROR "Building Arrow failed: " ${ARROW_BUILD})
endif(ARROW_BUILD)

message(STATUS "Arrow installed here: " ${ARROW_ROOT}/install)
set(ARROW_LIBRARY_DIR "${ARROW_ROOT}/install/lib")
set(ARROW_INCLUDE_DIR "${ARROW_ROOT}/install/include")

find_library(ARROW_LIB arrow
        NO_DEFAULT_PATH
        HINTS "${ARROW_LIBRARY_DIR}")

find_library(ARROW_PYTHON arrow_python
        NO_DEFAULT_PATH
        HINTS "${ARROW_LIBRARY_DIR}")

if(ARROW_LIB)
    message(STATUS "Arrow library: " ${ARROW_LIB})
    set(ARROW_FOUND TRUE)
endif(ARROW_LIB)

set(FLATBUFFERS_ROOT "${ARROW_ROOT}/build/flatbuffers_ep-prefix/src/flatbuffers_ep-install")

message(STATUS "FlatBuffers installed here: " ${FLATBUFFERS_ROOT})
set(FLATBUFFERS_INCLUDE_DIR "${FLATBUFFERS_ROOT}/include")
set(FLATBUFFERS_LIBRARY_DIR "${FLATBUFFERS_ROOT}/lib")

add_definitions(-DARROW_METADATA_V4)
