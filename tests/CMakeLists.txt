set(TEST_SOURCES tests_table_hachage.cpp ${PROJECT_SOURCE_DIR}/tableDispersion.h ${PROJECT_SOURCE_DIR}/HacheurIntQuad.cpp)

add_executable(
        tests_table_hachage
        ${TEST_SOURCES}
)

target_include_directories(tests_table_hachage PRIVATE ${PROJECT_SOURCE_DIR} )

target_link_libraries(
        tests_table_hachage
        gtest_main
        gtest
        pthread
)

include(GoogleTest)
gtest_discover_tests(tests_table_hachage)