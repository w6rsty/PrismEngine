cmake_minimum_required(VERSION 3.20)

project(PrismEngine
    VERSION 0.1.0
    LANGUAGES CXX C
)

add_subdirectory(3rdlibs)
add_subdirectory(prism)
add_subdirectory(sandbox)
add_subdirectory(editor)

option(PRISM_BUILD_UNIT_TEST "Build unit test" ON)
set(PRISM_BUILD_UNIT_TEST ON)
if (PRISM_BUILD_UNIT_TEST)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()

option(ENABLE_LOG "Enable log" ON)
option(ENABLE_ASSERTION "Enable assert" ON)
option(ENABLE_PROFILING "Enable profiling" OFF)
option(ENABLE_IMGUI "Enable imgui" ON)