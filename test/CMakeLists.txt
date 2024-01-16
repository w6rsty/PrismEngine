macro(AddConsoleTest name file)
    add_executable(${name} ${file}.cpp)    
    target_include_directories(${name} PRIVATE ./ ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${name} PRIVATE Prism::Core Prism)
    add_test(NAME ${name}
             COMMAND $<TARGET_FILE:${name}>)
    set_tests_properties(${name} PROPERTIES LABELS "console")
endmacro()

macro(AddConsoleFailTest name file)
    add_executable(${name} ${file}.cpp)    
    target_include_directories(${name} PRIVATE ./ ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${name} PRIVATE Prism::Core Prism)
    add_test(NAME ${name}
             COMMAND $<TARGET_FILE:${name}>)
    set_tests_properties(${name} PROPERTIES WILL_FAIL TRUE)
    set_tests_properties(${name} PROPERTIES LABELS "console")
endmacro()

macro(AddVisualableTest name)
    add_executable(${name} ${name}.cpp)    
    target_include_directories(${name} PRIVATE ./ ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${name} PRIVATE Prism::Core Prism)
    add_test(NAME ${name}
             COMMAND $<TARGET_FILE:${name}>)
    set_tests_properties(${name} PROPERTIES LABELS "visualable")
endmacro()

AddConsoleFailTest(assert assert/assert)
AddConsoleFailTest(todo assert/todo)

AddConsoleTest(logger log/logger)