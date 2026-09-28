#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>

inline void custom_assert_fail(const char* expr, const char* file, int line, std::string message) {
    printf("Assertion failed: (%s)\nFile: %s\nLine: %d\nMessage: %s\n", expr, file, line, message.c_str());
    std::abort();
}

inline void custom_assert_fail_soft(const char* expr, const char* file, int line, std::string message) {
    printf("Assertion failed: (%s)\nFile: %s\nLine: %d\nMessage: %s\n", expr, file, line, message.c_str());
}

#define K_ASSERT_CORE(condition, format_str, ...)                                                   \
    do {                                                                                            \
        if (!condition) {                                                                           \
            char assert_buf[512];                                                                   \
            std::snprintf(assert_buf, sizeof(assert_buf), format_str, ##__VA_ARGS__);               \
            custom_assert_fail(#condition, __FILE__, __LINE__, std::string(assert_buf));            \
        }                                                                                           \
    } while (0)

#define K_ASSERT_CORE_SOFT(condition, action, format_str, ...)                                      \
    do {                                                                                            \
        if (!condition) {                                                                           \
            char assert_buf[512];                                                                   \
            std::snprintf(assert_buf, sizeof(assert_buf), format_str, ##__VA_ARGS__);               \
            custom_assert_fail_soft(#condition, __FILE__, __LINE__, std::string(assert_buf));       \
            action;                                                                                 \
        }                                                                                           \
    } while (0)
