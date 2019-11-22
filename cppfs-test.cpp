#include <iostream>

#if defined(CPPFS_IS_BOOSTFS)
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#else
#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif
#endif

void test_fs(const char* argv0) {
  fs::path p{argv0};
  std::clog << "raw path:       " << p << std::endl;
  std::clog << "canonical path: " << fs::canonical(p) << std::endl;
}

int main(int argc, const char *argv[])
{
#if defined(CPPFS_IS_BOOSTFS)
  std::cout << "boost::filesystem" << std::endl;
#elif defined(__cpp_lib_filesystem)
  std::cout << "__cpp_lib_filesystem!" << std::endl;
#elif defined(__cpp_lib_experimental_filesystem)
  std::cout << "__cpp_lib_experimental_filesystem!" << std::endl;
#else
  std::cerr << "no filesystem" << std::endl;
#endif
  test_fs(argv[0]);
  return 0;
}


