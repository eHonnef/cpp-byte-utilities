cmake_minimum_required(VERSION 3.2)
project(ByteUtilities
        LANGUAGES CXX
        VERSION 0.0.1
)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CMAKE_GENERATOR "Unix Makefiles" CACHE INTERNAL "" FORCE)
    set(CMAKE_CXX_STANDARD 20)

    add_compile_options(-Wall -Wpedantic -Werror)
    set(${CMAKE_BINARY_DIR} "build")

    set(3rdParty_dir ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty)
    set(doctest_dir ${3rdParty_dir}/doctest)
    set(nanobench_dir ${3rdParty_dir}/nanobench)

    set(headers_dir ${CMAKE_CURRENT_LIST_DIR}/code/include)
    set(src_dir ${CMAKE_CURRENT_LIST_DIR}/code/src)

    include_directories(${headers_dir} ${3rdParty_dir})

    # Unit testing executable
    set(unit_test_bin "${PROJECT_NAME}_unit_test")
    add_executable(${unit_test_bin} ${src_dir}/Tests.cpp)

    # Benchmarks executable
    set(benchmarks_bin "${PROJECT_NAME}_benchmark")
    add_executable(${benchmarks_bin} ${src_dir}/Benchmarks.cpp)
else ()
    add_library(${PROJECT_NAME} INTERFACE)
    target_compile_options(${PROJECT_NAME} INTERFACE -w)
    target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/code/include)
endif ()
