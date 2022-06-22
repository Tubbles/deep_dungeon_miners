#include <cstdint>
#include <cstdio>
#include <fmt/format.h>
#include <string>

#define __FILENAME__ (__FILE__ + SOURCE_PATH_SIZE)

// Macros here so we can grab function, filename and line

// dfmt - debug format output to stdout, only when DEBUG is defined
#ifndef NDEBUG
#define dfmt(str) fmt::print("[DEBUG {}@{}:{}] {}\n", __FUNCTION__, __FILENAME__, __LINE__, str)
#else
#define dfmt(str)
#endif

// wfmt - warning format output to stdout
#define wfmt(str) fmt::print("[WARN. {}@{}:{}] {}\n", __FUNCTION__, __FILENAME__, __LINE__, str)

// efmt - error format output to stdout, and then exit
#define efmt(str, ecode)                                                                \
    do {                                                                                \
        fmt::print("[ERROR {}@{}:{}] {}\n", __FUNCTION__, __FILENAME__, __LINE__, str); \
        exit(ecode);                                                                    \
    } while (0)

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))
