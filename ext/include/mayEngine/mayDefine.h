/*****************************************************************//**
 * \file   mayDefine.h
 * \brief  定义部分重要的宏
 * 
 * \author Majjcom
 * \date   July 2022
 *********************************************************************/
#pragma once

#include "mayExport.h"
#include "mayStdExport.h"

#define NS_MAY_BEGIN		namespace MayEngine {
#define NS_MAY_END			}
#define NS_MAY_US			using namespace MayEngine;
#define NS_STD_US           using namespace std;

#ifdef __ANDROID__
#include <SDL_assert.h>
#define MAY_ASSERT(x) SDL_assert(x)
#else
#define MAY_ASSERT(x) do { if (!(x)) *(int*)0 = 0; } while (0) 
#endif
#define M_SC(c, v) static_cast<c>(v)
#define M_DC(c, v) dynamic_cast<c>(v)
