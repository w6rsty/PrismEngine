file(GLOB_RECURSE SANDBOX_SRC
    "sandbox.cpp"
)

add_executable(sandbox ${SANDBOX_SRC})
target_link_libraries(sandbox PRIVATE Prism Prism::Core)