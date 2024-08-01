#include "zip_util/zip_wrapper.h"
#include <sys/stat.h>
#include <memory>
#include <algorithm>
#include <vector>

// namespace {
auto CloseZip = [](unzFile uzf) {
  if (unzClose(uzf) != UNZ_OK) {
    LOG(ERROR) << "Error closing ZIP file" << ENDL;
  }
};
retcode ZipWrapper::AddFileToZip() {
  return retcode::SUCCESS;
}

retcode ZipWrapper::AddFolderToZip(const std::string& folder_path,
                                    const std::string& zip_file) {
//
  zipFile zip = zipOpen(zip_file.c_str(), APPEND_STATUS_CREATE);
  if (zip == nullptr) {
    LOG(ERROR) << "failed to open ZIP file" << ENDL;
    return retcode::FAIL;
  }
  std::filesystem::path _path = folder_path;
  // add folder to zip file
  AddFolderToZip(_path, zip, "");

  // 关闭 ZIP 文件
  zipClose(zip, nullptr);
}

retcode ZipWrapper::AddFolderToZip(const std::filesystem::path& folder_path,
                                   zipFile zip,
                                   const std::string& folder_name) {
  std::string folderPathStr = PathToMiniZipFormat(folder_path);
  for (const auto& entry : std::filesystem::recursive_directory_iterator(folder_path)) {
    if (entry.is_directory()) {
      // 递归添加子文件夹
      std::string sub_folder_name = PathToMiniZipFormat(entry.path());
      AddFolderToZip(entry.path(), zip, folder_name + sub_folder_name + "/");
    } else {
      // 添加文件到 ZIP 文件
      std::string filename = PathToMiniZipFormat(entry.path().filename());
      std::string entryName = folder_name + filename;
      zip_fileinfo zinfo = {};
      std::cout << "file_name: " << filename << " "
          << "entryName: " << entryName << "\n";
      auto path_info = entry.path().c_str();
      struct stat file_info;
      if (stat(path_info, &file_info) == -1) {
        LOG(ERROR) << "stat" << ENDL;
        return retcode::FAIL;
      }
      struct tm* last_modified = std::gmtime(&file_info.st_mtime);
      // LOG(INFO) << "file info: " << path_info << " "
      //     << "year: " << last_modified->tm_year + 1900 << " "
      //     << "month: " << last_modified->tm_mon + 1 << " "
      //     << "day: " << last_modified->tm_mday;
      zinfo.tmz_date.tm_year = last_modified->tm_year + 1900;
      zinfo.tmz_date.tm_mon = last_modified->tm_mon + 1;
      zinfo.tmz_date.tm_mday = last_modified->tm_mday;
      zinfo.dosDate = 0;
      zinfo.internal_fa = 0;
      zinfo.external_fa = 0;

      FILE* file = fopen(entry.path().string().c_str(), "rb");
      if (file) {
        int err = zipOpenNewFileInZip(zip, entryName.c_str(), &zinfo,
                                      NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);
        if (err == ZIP_OK) {
          std::vector<char> buffer(1024);
          size_t size;
          while ((size = fread(buffer.data(), 1, buffer.size(), file)) > 0) {
            zipWriteInFileInZip(zip, buffer.data(), (uInt)size);
          }
        }
        fclose(file);
        zipCloseFileInZip(zip);
      }
    }
  }
  return retcode::SUCCESS;
}

retcode ZipWrapper::ExtractZipToFolder(const std::string& zip_file,
                            const std::string& dest_folder_path) {
  // open zip file
  unzFile uzip_handler_ = unzOpen(zip_file.c_str());
  if (uzip_handler_ == nullptr) {
    LOG(ERROR) << "Error opening ZIP file: " << zip_file << ENDL;
    return retcode::FAIL;
  }
  // 获取 ZIP 文件的全局信息（可选）
  unz_global_info global_info;
  if (unzGetGlobalInfo(uzip_handler_, &global_info) != UNZ_OK) {
    LOG(ERROR) << "Error getting global info from ZIP file: " << zip_file << ENDL;
    return retcode::FAIL;
  }

  // 遍历 ZIP 文件中的所有文件
  for (uLong i = 0; i < global_info.number_entry; ++i) {
    // 获取当前文件的信息
    unz_file_info file_info;
    char filename[256];
    if (unzGetCurrentFileInfo(uzip_handler_, &file_info, filename,
        sizeof(filename), NULL, 0, NULL, 0) != UNZ_OK) {
      LOG(ERROR) << "Error getting file info, skipping" << ENDL;
      continue;
    }

    // 构造输出文件的完整路径
    std::string fullFilePath = dest_folder_path + "/" + std::string(filename);

    // 打开 ZIP 中的当前文件
    //
    if (unzOpenCurrentFile(uzip_handler_) != UNZ_OK) {
      LOG(ERROR) << "failed to open file in ZIP: " << filename << ENDL;
      continue;
    }

    // create output file
    FILE* outFile = fopen(fullFilePath.c_str(), "wb");
    if (outFile == NULL) {
      LOG(ERROR) << "Error creating file: " << fullFilePath << ENDL;
      unzCloseCurrentFile(uzip_handler_);
      continue;
    }

    // read data from zip file and write to output
    std::vector<char> buffer(1024);
    int size;
    do {
      size = unzReadCurrentFile(uzip_handler_, buffer.data(), (unsigned)buffer.size());
      if (size < 0) {
        LOG(ERROR) << "Error reading file in ZIP: " << filename << ENDL;
        break;
      }
      if (fwrite(buffer.data(), 1, size, outFile) != size) {
        LOG(ERROR) << "Error writing to file: " << fullFilePath << ENDL;
        break;
      }
    } while (size > 0);

    // 关闭文件和 ZIP 中的当前文件
    fclose(outFile);
    if (unzCloseCurrentFile(uzip_handler_) != UNZ_OK) {
      LOG(ERROR) << "Error closing file in ZIP: " << filename << ENDL;
      return retcode::FAIL;
    }

    // 移动到下一个文件
    if (i < global_info.number_entry - 1) {
      if (unzGoToNextFile(uzip_handler_) != UNZ_OK) {
        LOG(ERROR) << "Error going to next file in ZIP file" << ENDL;
        break;
      }
    }
  }
  unzClose(uzip_handler_);
  return retcode::SUCCESS;
}

void ZipWrapper::ListZipContents(const std::string& zip_file) {
  // 打开 ZIP 文件进行读取
  unzFile uzip_handler_ = unzOpen(zip_file.c_str());
  if (uzip_handler_ == nullptr) {
    LOG(ERROR) << "Error opening ZIP file: " << zip_file << "\n";
    return;
  }
  // 获取 ZIP 文件的全局信息
  unz_global_info global_info;
  if (unzGetGlobalInfo(uzip_handler_, &global_info) != UNZ_OK) {
    LOG(ERROR) << "Error getting global info from ZIP file: "
        << zip_file << "\n";
    return;
  }

  // 遍历 ZIP 文件中的所有文件
  for (uLong i = 0; i < global_info.number_entry; ++i) {
    // 获取当前文件的信息
    unz_file_info file_info;
    char filename[256];
    if (unzGetCurrentFileInfo(uzip_handler_, &file_info, filename,
        sizeof(filename), NULL, 0, NULL, 0) != UNZ_OK) {
      LOG(ERROR) << "Error getting file info from ZIP file" << std::endl;
      continue;
    }

    // 打印文件信息
    LOG(INFO) << "File " << i + 1 << " in ZIP: " << filename << std::endl;

    // 移动到下一个文件
    if (i < global_info.number_entry - 1) {
      if (unzGoToNextFile(uzip_handler_) != UNZ_OK) {
        LOG(ERROR) << "Error going to next file in ZIP file" << "\n";
        break;
      }
    }
  }
  unzClose(uzip_handler_);
}

std::string ZipWrapper::PathToMiniZipFormat(const std::filesystem::path& path) {
  std::string result = path.string();
  std::replace(result.begin(), result.end(), '\\', '/');
  return result;
}
// }  // namespace
