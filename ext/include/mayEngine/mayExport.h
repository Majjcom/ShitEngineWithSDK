/*****************************************************************//**
 * \file   mayExport.h
 * \brief  MSVC 需要的导出 DLL API 的宏定义
 * 
 * \author Majjcom
 * \date   July 2022
 *********************************************************************/
#pragma once

#ifdef _WIN32
# ifndef __clang__
#  ifdef EXPORT_MAY_ENGINE_API
#   define MAY_DLL		__declspec(dllexport)
#  else
#   define MAY_DLL		__declspec(dllimport)
#  endif
# else
#  ifdef EXPORT_MAY_ENGINE_API
#   define MAY_DLL      __attribute__((dllexport))
#  else
#   define MAY_DLL      __attribute__((dllimport))
#  endif
# endif
#else
# define MAY_DLL        __attribute__((visibility("default")))
#endif
