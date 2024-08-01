#ifndef ZIP_UTIL_ZIP_WRAPPER_H_
#define ZIP_UTIL_ZIP_WRAPPER_H_
#include <zip.h>
#include <unzip.h>
#include <string>
#include <filesystem>

#include "common/common.h"
// namespace {
class ZipWrapper {
 public:
  enum class Mode {
    ZIP,
    UnZip,
  };

 public:
  ZipWrapper() = default;
  ~ZipWrapper() = default;

 public:
  retcode AddFileToZip();
  retcode AddFolderToZip(const std::string& folder_path,
                         const std::string& zip_file);
  retcode AddFolderToZip(const std::filesystem::path& folder_path,
                         zipFile zip,
                         const std::string& folder_name);
  retcode ExtractZipToFolder(const std::string& zip_file,
                             const std::string& dest_path);
  void ListZipContents(const std::string& zip_file);

 protected:
  std::string PathToMiniZipFormat(const std::filesystem::path& path);

//  private:
//   Mode mode_;
//   zipFile zip_handler_{nullptr};
//   unzFile uzip_handler_{nullptr};
};
// }  // namespace
#endif  // ZIP_UTIL_ZIP_WRAPPER_H_
