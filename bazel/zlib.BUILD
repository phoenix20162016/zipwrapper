package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # BSD/MIT-like license (for zlib)

cc_library(
  name = "zlib",
  srcs = glob([
    "*.c",
    "*.h",
  ]) + [
    "contrib/minizip/ioapi.c",
    "contrib/minizip/ioapi.h",
    "contrib/minizip/unzip.c",
    "contrib/minizip/unzip.h",
    "contrib/minizip/zip.c",
    "contrib/minizip/zip.h",
    "contrib/minizip/crypt.h",
  ],
  hdrs = [
    "zlib.h",
  ],
  copts = select({
    "@bazel_tools//src/conditions:windows": [],
    "//conditions:default": [
        "-Wno-shift-negative-value",
        "-DZ_HAVE_UNISTD_H",
    ],
  }),
  includes = [
    ".",
    "contrib/minizip",
  ],
)
