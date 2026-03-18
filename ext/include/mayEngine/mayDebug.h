#pragma once

#include "mayDefine.h"

NS_MAY_BEGIN

namespace DEBUG
{
    /**
     * 初始化logger
     */
    MAY_DLL void init();

    /**
     * 释放logger
     */
    MAY_DLL void uninit();

    /**
     * 日志等级
     */
    enum class LogLevel : int32_t
    {
        /// 追踪
        Trace = 0,

        /// 调试
        Debug,

        /// 信息
        Info,

        /// 警告
        Warn,

        /// 错误
        Error,

        /// 关键错误
        Critical,

        /// 关闭
        Off,
    };

    /**
     * 输出日志
     */
    MAY_DLL void log(const char* info, ...);

    /**
     * 输出日志
     *
     * @param info  格式化字符串
     * @param level 日志等级
     * @param ...   参数
     */
    MAY_DLL void logv2(LogLevel level, const char* info, ...);

    /**
     * 设置日志等级
     *
     * @param level 日志等级
     */
    MAY_DLL void setLogLevel(LogLevel level);
}

NS_MAY_END

// V1 Logger
#if (defined(_DEBUG) && defined(_WIN32)) || defined(MAY_LOGGING)
# ifdef __clang__
#  define mLogv1(arg, ...) MayEngine::DEBUG::log(arg, ##__VA_ARGS__)
# else
#  define mLogv1(arg, ...) MayEngine::DEBUG::log(arg, __VA_ARGS__)
# endif
#else
#define mLogv1(arg, ...)
#endif
#if defined(_DEBUG) && defined(__ANDROID__)
# ifdef mLogv1
#  undef mLogv1
# endif
# define mLogv1(arg, ...) MayEngine::DEBUG::log(arg, ##__VA_ARGS__)
#endif

// V2 Logger
#if (defined(_DEBUG) && defined(_WIN32)) || defined(MAY_LOGGING)
# ifdef __clang__
#  define mLogv2l(arg, l, ...) MayEngine::DEBUG::logv2(l, arg, ##__VA_ARGS__)
# else
#  ifdef _MAYENGINE_MLOG_FILE_INFO
#   define mLogv2l(arg, l, ...) MayEngine::DEBUG::logv2(l, "[%s:%d] " arg, __FILE__, __LINE__, __VA_ARGS__)
#  else
#   define mLogv2l(arg, l, ...) MayEngine::DEBUG::logv2(l, arg, __VA_ARGS__)
#  endif
# endif
#elif defined(_DEBUG) && defined(__ANDROID__)
# define mLogv2l(arg, l, ...) MayEngine::DEBUG::logv2(l, arg, ##__VA_ARGS__)
#else
#define mLogv2l(arg, l, ...)
#endif

// V2 Logger Info
#define MLOGV2LI MayEngine::DEBUG::LogLevel::Info
#ifdef __clang__
# define mLogv2(arg, ...) mLogv2l(arg, MLOGV2LI, ##__VA_ARGS__)
#else
# define mLogv2(arg, ...) mLogv2l(arg, MLOGV2LI, __VA_ARGS__)
#endif

// V2 Logger Trace
#define MLOGV2LT MayEngine::DEBUG::LogLevel::Trace
#ifdef __clang__
# define mLogv2t(arg, ...) mLogv2l(arg, MLOGV2LT, ##__VA_ARGS__)
#else
# define mLogv2t(arg, ...) mLogv2l(arg, MLOGV2LT, __VA_ARGS__)
#endif

// V2 Logger Debug
#define MLOGV2LD MayEngine::DEBUG::LogLevel::Debug
#ifdef __clang__
# define mLogv2d(arg, ...) mLogv2l(arg, MLOGV2LD, ##__VA_ARGS__)
#else
# define mLogv2d(arg, ...) mLogv2l(arg, MLOGV2LD, __VA_ARGS__)
#endif

// V2 Logger Warn
#define MLOGV2LW MayEngine::DEBUG::LogLevel::Warn
#ifdef __clang__
# define mLogv2w(arg, ...) mLogv2l(arg, MLOGV2LW, ##__VA_ARGS__)
#else
# define mLogv2w(arg, ...) mLogv2l(arg, MLOGV2LW, __VA_ARGS__)
#endif

// V2 Logger Error
#define MLOGV2LE MayEngine::DEBUG::LogLevel::Error
#ifdef __clang__
# define mLogv2e(arg, ...) mLogv2l(arg, MLOGV2LE, ##__VA_ARGS__)
#else
# define mLogv2e(arg, ...) mLogv2l(arg, MLOGV2LE, __VA_ARGS__)
#endif

// V1 logger with file info
#if (defined(_DEBUG) && defined(_WIN32)) || defined(MAY_LOGGING)
# ifdef __clang__
#  define mLogf(arg, ...) MayEngine::DEBUG::log("[%s:%d] ", __FILE__, __LINE__); \
                          MayEngine::DEBUG::log(arg, ##__VA_ARGS__)
# else
#  define mLogf(arg, ...) MayEngine::DEBUG::log("[%s:%d] ", __FILE__, __LINE__); \
                          MayEngine::DEBUG::log(arg, __VA_ARGS__)
# endif
#else
# define mLogf(arg, ...)
#endif

#if defined(_DEBUG) && defined(__ANDROID__)
# ifdef mLogf
#  undef mLogf
# endif
# define mLogf(arg, ...) MLOG_FUNCTION(arg, ##__VA_ARGS__)
#endif

#ifdef MAYENGINE_USE_MLOGV2
# ifdef __clang__
#  define mLog(arg, ...) mLogv2(arg, ##__VA_ARGS__)
#  define mLogt(arg, ...) mLogv2t(arg, ##__VA_ARGS__)
#  define mLogd(arg, ...) mLogv2d(arg, ##__VA_ARGS__)
#  define mLogw(arg, ...) mLogv2w(arg, ##__VA_ARGS__)
#  define mLoge(arg, ...) mLogv2e(arg, ##__VA_ARGS__)
# else
#  define mLog(arg, ...) mLogv2(arg, __VA_ARGS__)
#  define mLogt(arg, ...) mLogv2t(arg, __VA_ARGS__)
#  define mLogd(arg, ...) mLogv2d(arg, __VA_ARGS__)
#  define mLogw(arg, ...) mLogv2w(arg, __VA_ARGS__)
#  define mLoge(arg, ...) mLogv2e(arg, __VA_ARGS__)
# endif
#else
# ifdef __clang__
#  define mLog(arg, ...) mLogv1(arg, ##__VA_ARGS__)
# else
#  define mLog(arg, ...) mLogv1(arg, __VA_ARGS__)
# endif
#endif
