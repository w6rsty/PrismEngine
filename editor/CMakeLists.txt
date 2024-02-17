aux_source_directory(src/ EDITOR_SRC)
aux_source_directory(src/panel EDITOR_SRC)

add_executable(editor ${EDITOR_SRC})
target_link_libraries(editor PRIVATE Prism Prism::Core entt  imguizmo)
target_include_directories(editor PRIVATE src/)

target_compile_features(editor PRIVATE cxx_std_17)
target_compile_definitions(editor PRIVATE _CRT_SECURE_NO_WARNINGS)
