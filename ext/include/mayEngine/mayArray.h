/*****************************************************************//**
 * @file   mayArray.h
 * @brief  对部分数组的封装
 * 
 * @author Majjcom
 * @date   July 2022
 *********************************************************************/
#pragma once

#include "mayDefine.h"
#include "mayStlDefine.h"

namespace MayEngine
{
    class Ref;
}

NS_MAY_BEGIN;

typedef mlist<Ref*>                    RefArray;
typedef mdeque<mlist<Ref*>::iterator>  RefReleaseArray;

NS_MAY_END;
