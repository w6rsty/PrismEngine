file(GLOB_RECURSE SANDBOX_SRC
    "sandbox.cpp"
    "sandbox2d.cpp"
    "particle_system.cpp"
)

add_executable(sandbox ${SANDBOX_SRC})
target_link_libraries(sandbox PRIVATE Prism glm)

set(PRISM_IMGUI OFF)