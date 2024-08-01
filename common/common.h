#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_
#include <iostream>
// namespace {
enum class retcode {
  SUCCESS = 0,
  FAIL,
};
#define LOG(INFO) std::cout
#define LOG(ERROR) std::cerr
#define ENDL "\n"
// }  // namespace
#endif  // COMMON_COMMON_H_
