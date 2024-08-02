#ifndef UTIL_FILE_UTIL_H_
#define UTIL_FILE_UTIL_H_

#include <vector>
#include <sstream>

#include "common/common.h"

// namespace  {
retcode ReadFileContents(const std::string& fpath, std::string* contents);
std::vector<std::string> GetFiles(const std::string& path);
retcode ValidateDir(const std::string& file_path);
bool FileExists(const std::string& file_path);
bool RemoveFile(const std::string& file_path);
int64_t FileSize(const std::string& file_path);
/**
 * complete path using provided path
 * if file_path is relative path, concat default_storage_path to file path
 * or using file_path
*/
std::string CompletePath(const std::string& default_storage_path,
                         const std::string& file_path);
// }  // namespace
#endif  // UTIL_FILE_UTIL_H_
