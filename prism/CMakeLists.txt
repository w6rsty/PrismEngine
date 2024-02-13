aux_source_directory(src/core prism_core_src)
aux_source_directory(src prism_src)
aux_source_directory(src/imgui prism_src)
aux_source_directory(src/renderer prism_src)
aux_source_directory(src/platform/Windows prism_src)
aux_source_directory(src/platform/OpenGL prism_src)
aux_source_directory(src/scene prism_src)

file(GLOB_RECURSE prism_headers ./include/*.hpp)
file(GLOB prism_shaders ./shaders/*.shader)

source_group(${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "include" FILES ${prism_headers})
source_group(shaders FILES ${prism_shaders})
source_group(${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${prism_src})

add_library(prism_core STATIC)
target_sources(prism_core PRIVATE ${prism_core_src})
target_include_directories(prism_core PUBLIC ./include)
target_compile_features(prism_core PUBLIC cxx_std_17)
target_link_libraries(prism_core PUBLIC glfw glad debugbreak imgui glm stb_image entt toml++)
target_compile_definitions(prism_core PUBLIC _CRT_SECURE_NO_WARNINGS)
if (CMAKE_CXX_FLAGS_DEBUG) 
    target_compile_definitions(prism_core PUBLIC ENABLE_LOG)
    target_compile_definitions(prism_core PUBLIC ENABLE_ASSERTION)
endif()

# target_compile_definitions(prism_core PUBLIC PRISM_PROFILE)

target_compile_definitions(prism_core PUBLIC PRISM_IMGUI)


add_library(prism STATIC)
target_sources(prism PRIVATE ${prism_src})
target_link_libraries(prism PUBLIC prism_core)
add_library(Prism::Core ALIAS prism_core)
add_library(Prism ALIAS prism)