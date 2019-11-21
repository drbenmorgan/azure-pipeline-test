#include <iostream>

#if __has_include(<filesystem>)
  #include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
  #include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

#if defined(__cpp_lib_filesystem) || defined(__cpp_lib_experimental_filesystem)
void test_fs(const char* argv0) {
  fs::path p{argv0};
  std::clog << "raw path:       " << p << std::endl;
  std::clog << "canonical path: " << fs::canonical(p) << std::endl;
}
#endif

int main(int argc, const char *argv[])
{
#if defined(__cpp_lib_filesystem)
  std::cout << "__cpp_lib_filesystem!" << std::endl;
  test_fs(argv[0]);
#elif defined(__cpp_lib_experimental_filesystem)
  std::cout << "__cpp_lib_experimental_filesystem!" << std::endl;
  test_fs(argv[0]);
#else
  std::cerr << "no filesystem" << std::endl;
#endif
  return 0;
}


