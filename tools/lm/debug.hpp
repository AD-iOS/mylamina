//
// Created by CGrakeski on 2026/4/2.
//

#pragma once

#define DEBUGGING

#undef DEBUGGING

#ifdef DEBUGGING

constexpr const char* get_filename(const char* path) {
    const char* last_slash = path;
    for (const char* p = path; *p; ++p) {
        if (*p == '/' || *p == '\\') last_slash = p + 1;
    }
    return last_slash;
}

#define LOG(msg) std::cerr << "\033[33m[DEBUG] " \
    << get_filename(__FILE__) << ":" << __LINE__ << " | " \
    << msg << "\033[0m" << std::endl

#define priv_VA_SELECT(_1, _2, NAME, ...) NAME
#define priv_ITIS_IMPL_1(expr)            (std::format("{}: {}", #expr, (expr)))
#define priv_ITIS_IMPL_2(expr, func)      (std::format("{}: {}", #expr, func(expr)))
#define ITIS(...)                    priv_VA_SELECT(__VA_ARGS__, priv_ITIS_IMPL_2, priv_ITIS_IMPL_1)(__VA_ARGS__)

#else

#define LOG(msg)
#define ITIS(...)

#endif