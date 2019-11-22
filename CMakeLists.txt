cmake_minimum_required(VERSION 3.10)
project(AzurePipelineTest VERSION 0.1.0)

enable_testing()

add_executable(cppfs-test cppfs-test.cpp)
target_compile_features(cppfs-test PUBLIC cxx_std_17)

add_test(NAME cppfs-test-runtime COMMAND cppfs-test)