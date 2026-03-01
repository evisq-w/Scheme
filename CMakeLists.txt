set(BASIC_TESTS
    # from tokenizer
    tests/test_tokenizer.cpp

    # from parser
    tests/test_parser.cpp

    tests/test_boolean.cpp
    tests/test_eval.cpp
    tests/test_integer.cpp
    tests/test_list.cpp
    tests/test_fuzzing_2.cpp)

add_catch(test_scheme_basic
    ${BASIC_TESTS})

include(sources.cmake)

target_include_directories(scheme_basic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SCHEME_COMMON_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/basic/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/basic/src
    ${CMAKE_CURRENT_SOURCE_DIR}/basic/src/builtins
    ${CMAKE_CURRENT_SOURCE_DIR}/src/builtins
)

target_link_libraries(test_scheme_basic scheme_basic)

add_executable(scheme_basic_repl repl/main.cpp)
target_link_libraries(scheme_basic_repl scheme_basic)