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
    class SceneRQTask;
    class Ref;
    namespace Graphic
    {
        class Pipeline;
    }
}

NS_MAY_BEGIN;

typedef mlist<Ref*>                    RefArray;
typedef mdeque<mlist<Ref*>::iterator>  RefReleaseArray;
typedef mqueue<SceneRQTask*>           RendTasksArray;
typedef mmap<int, Graphic::Pipeline*>  PipeMap;

NS_MAY_END;
