add_executable(LayerManagerTest LayerManagerTest.cpp)

target_link_libraries(LayerManagerTest PRIVATE Runic doctest)

target_include_directories(LayerManagerTest PRIVATE ${doctest_SOURCE_DIR}/doctest)

add_test(NAME LayerManagerTest COMMAND LayerManagerTest)
