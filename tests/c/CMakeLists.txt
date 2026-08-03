add_executable(test_core test_core.c)
target_link_libraries(test_core PRIVATE hexis)
add_test(NAME CoreTest COMMAND test_core)
