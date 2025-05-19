
add_executable(tests main.cxx)
target_link_libraries(tests
  PRIVATE
    Boost::boost
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main  )

target_include_directories(tests
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src  )

add_test(AllTests tests)