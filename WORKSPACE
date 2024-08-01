load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

http_archive(
  name = "zlib",
  build_file = "//bazel:zlib.BUILD",
  sha256 = "c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1",
  strip_prefix = "zlib-1.2.11",
  urls = [
    "https://primihub.oss-cn-beijing.aliyuncs.com/zlib-1.2.11.tar.gz"
  ],
)
