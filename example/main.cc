#include <iostream>
#include <string>
#include "zip_util/zip_wrapper.h"
int main() {
  std::string folder_path = "./test_data/source";
  std::string zip_file = "./test_data/dest.zip";
  std::string dest_path = "./test_data/dest";
  ZipWrapper wrap;
  wrap.AddFolderToZip(folder_path, zip_file);
  wrap.ListZipContents(zip_file);
  wrap.ExtractZipToFolder(zip_file, dest_path);
  std::cout << "Folder compressed successfully" << std::endl;
  return 0;
}
