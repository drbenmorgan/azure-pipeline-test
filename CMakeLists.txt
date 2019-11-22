cmake_minimum_required(VERSION 3.10)
project(AzurePipelineTest VERSION 0.1.0)

# Checks on C++17 fs support level
# 1. Full Native
try_run(CPPFS_FULL_RUNTIME_SUPPORT CPPFS_COMPILE_SUPPORT 
  ${CMAKE_CURRENT_BINARY_DIR}/_check_cppfs/full
  ${PROJECT_SOURCE_DIR}/cmake/cppfs-try-run.cpp
  CMAKE_FLAGS -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_STANDARD_REQUIRED=ON
  )
message(STATUS "CPPFS_FULL: x${CPPFS_FULL_RUNTIME_SUPPORT}x, x${CPPFS_COMPILE_SUPPORT}x")

# 2. Native plus support lib (GNU/Clang)
#    - Can be stdc++fs OR c++fs/c++experimental on Linux Clang
if(NOT CPPFS_COMPILE_SUPPORT OR CPPFS_FULL_RUNTIME_SUPPORT STREQUAL "FAILED_TO_RUN")
  try_run(CPPFS_RUNTIME_STDCPPFS CPPFS_COMPILE_STDCPPFS 
    ${CMAKE_CURRENT_BINARY_DIR}/_check_cppfs/stdc++fs
    ${PROJECT_SOURCE_DIR}/cmake/cppfs-try-run.cpp
    CMAKE_FLAGS -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_STANDARD_REQUIRED=ON
    LINK_LIBRARIES stdc++fs
    )
  message(STATUS "CPPFS_LIB: x${CPPFS_RUNTIME_STDCPPFS}x, x${CPPFS_COMPILE_STDCPPFS}x")

  # If we're running Clang/libc++, then if the above fails, check if c++fs works
  # That's from libc++ 7 and higher (before was c++experimental)

endif()



# 3. None (so use Boost workaround)

# Build
add_executable(cppfs-test cppfs-test.cpp)
target_compile_features(cppfs-test PUBLIC cxx_std_17)

# Test
enable_testing()
add_test(NAME cppfs-test-runtime COMMAND cppfs-test)