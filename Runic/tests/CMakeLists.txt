add_executable(RunicTests RunicTests.cpp)

target_link_libraries(RunicTests PRIVATE Runic doctest)

target_include_directories(RunicTests PRIVATE ${doctest_SOURCE_DIR}/doctest)

add_test(NAME RunicTests COMMAND RunicTests)
