/*****************************************************************//**
 * \file   mayType.h
 * \brief  部分类型定义
 * 
 * \author Majjcom
 * \date   July 2022
 *********************************************************************/
#pragma once

#include "mayDefine.h"

NS_MAY_BEGIN

/**
 * 对象类型枚举.
 */
enum class MayType
{
    /// 场景
    Scene,

    /// 节点
    Node,

    /// 组件
    Component,

    /// 相机
    Camera,
};

enum class TextureWrapping
{
    Repeat = 0,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder,
};

NS_MAY_END
