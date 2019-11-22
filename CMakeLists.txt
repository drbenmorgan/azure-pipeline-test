cmake_minimum_required(VERSION 3.10)
project(AzurePipelineTest VERSION 0.1.0)

# Checks on C++17 fs support level
# 1. Full Native
try_compile(CPPFS_IS_NATIVE
  ${CMAKE_CURRENT_BINARY_DIR}/_check_cppfs/native
  ${PROJECT_SOURCE_DIR}/cmake/cppfs-try-run.cpp
  CMAKE_FLAGS -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_STANDARD_REQUIRED=ON
  )
message(STATUS "CPPFS_IS_NATIVE: x${CPPFS_IS_NATIVE}x")

# 2. Native plus support lib (GNU/Clang)
#    - Can be stdc++fs OR c++fs/c++experimental (Clang), OR Boost::filesystem as a fallback
if(NOT CPPFS_IS_NATIVE)
  try_compile(CPPFS_IS_STDCPPFS
    ${CMAKE_CURRENT_BINARY_DIR}/_check_cppfs/stdc++fs
    ${PROJECT_SOURCE_DIR}/cmake/cppfs-try-run.cpp
    CMAKE_FLAGS -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_STANDARD_REQUIRED=ON
    LINK_LIBRARIES stdc++fs
    )
  message(STATUS "CPPFS_IS_STDCPPFS: x${CPPFS_IS_STDCPPFS}x")

  # Not clear yet how reliable this is on Windows/Clang or Linux/GCC (when libc++ is installed)
  try_compile(CPPFS_IS_CPPFS
    ${CMAKE_CURRENT_BINARY_DIR}/_check_cppfs/c++fs
    ${PROJECT_SOURCE_DIR}/cmake/cppfs-try-run.cpp
    CMAKE_FLAGS -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_STANDARD_REQUIRED=ON
    LINK_LIBRARIES c++fs
    )
  message(STATUS "CPPFS_IS_CPPFS: x${CPPFS_IS_CPPFS}x")

  if(NOT CPPFS_IS_STDCPPFS AND NOT CPPFS_IS_CPPFS)
    set(CPPFS_IS_BOOSTFS TRUE)
    message(STATUS "CPPFS_IS_BOOSTFS: x${CPPFS_IS_BOOSTFS}x")
    find_package(Boost 1.63 REQUIRED filesystem)
  endif()
endif()

# Build
add_executable(cppfs-test cppfs-test.cpp)
target_compile_features(cppfs-test PUBLIC cxx_std_17)
# Use genexes, but not one call per genex which allows them to be used unquoted
# Can put the link genexs in one call but then have to quote them
target_link_libraries(cppfs-test PRIVATE $<$<BOOL:${CPPFS_IS_STDCPPFS}>:stdc++fs>)
target_link_libraries(cppfs-test PRIVATE $<$<BOOL:${CPPFS_IS_CPPFS}>:c++fs>)
target_compile_definitions(cppfs-test PRIVATE $<$<BOOL:${CPPFS_IS_BOOSTFS}>:CPPFS_IS_BOOSTFS>)
target_link_libraries(cppfs-test PRIVATE $<$<BOOL:${CPPFS_IS_BOOSTFS}>:Boost::filesystem>)

# Test
enable_testing()
add_test(NAME cppfs-test-runtime COMMAND cppfs-test)