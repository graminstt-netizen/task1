# CMake generated Testfile for 
# Source directory: C:/Users/Dima/gitproject/lib/tests
# Build directory: C:/Users/Dima/gitproject/build/lib/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(test_test_feature_a_test_lib "C:/Users/Dima/gitproject/build/lib/tests/Debug/test_test_feature_a_test_lib.exe")
  set_tests_properties(test_test_feature_a_test_lib PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Dima/gitproject/cmake/MyFunctions.cmake;41;add_test;C:/Users/Dima/gitproject/lib/tests/CMakeLists.txt;3;register_all_tests_in_dir;C:/Users/Dima/gitproject/lib/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(test_test_feature_a_test_lib "C:/Users/Dima/gitproject/build/lib/tests/Release/test_test_feature_a_test_lib.exe")
  set_tests_properties(test_test_feature_a_test_lib PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Dima/gitproject/cmake/MyFunctions.cmake;41;add_test;C:/Users/Dima/gitproject/lib/tests/CMakeLists.txt;3;register_all_tests_in_dir;C:/Users/Dima/gitproject/lib/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(test_test_feature_a_test_lib "C:/Users/Dima/gitproject/build/lib/tests/MinSizeRel/test_test_feature_a_test_lib.exe")
  set_tests_properties(test_test_feature_a_test_lib PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Dima/gitproject/cmake/MyFunctions.cmake;41;add_test;C:/Users/Dima/gitproject/lib/tests/CMakeLists.txt;3;register_all_tests_in_dir;C:/Users/Dima/gitproject/lib/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(test_test_feature_a_test_lib "C:/Users/Dima/gitproject/build/lib/tests/RelWithDebInfo/test_test_feature_a_test_lib.exe")
  set_tests_properties(test_test_feature_a_test_lib PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Dima/gitproject/cmake/MyFunctions.cmake;41;add_test;C:/Users/Dima/gitproject/lib/tests/CMakeLists.txt;3;register_all_tests_in_dir;C:/Users/Dima/gitproject/lib/tests/CMakeLists.txt;0;")
else()
  add_test(test_test_feature_a_test_lib NOT_AVAILABLE)
endif()
