/*****************************************************************//**
 * @file   mayRef.h
 * @brief  Ref 对象基类封装
 * 
 * @author Majjcom
 * @date   July 2022
 *********************************************************************/
#pragma once

#include "mayStlDefine.h"
#include "mayType.h"
#include "memory/mayMemory.h"

/// Get class name
#define M_GET_CLASS_NAME(C)                         \
public:                                             \
    virtual const char* getClassName() const        \
    {                                               \
        if (this->class_name.empty())               \
        {                                           \
            return #C;                              \
        }                                           \
        return this->class_name.c_str();            \
    }

/// Get class name n
#define M_GET_CLASS_NAME_N(C)                       \
public:                                             \
    virtual const MayEngine::mstring getClassName() \
    {                                               \
        return #C;                                  \
    }

NS_MAY_BEGIN

class MAY_DLL Ref
{
    M_GET_CLASS_NAME(Ref);
public:

    /**
     * 激活对象函数.
     * 
     */
    virtual void enable();

    /**
     * 析构函数.
     * 
     */
    virtual ~Ref();

    /**
     * 失活函数.
     * 
     */
    virtual void disable();

    /**
     * 销毁函数.
     * 
     */
    virtual void destroy();

    /**
     * 检查是否激活.
     * 
     * @return 是否激活
     */
    virtual bool isEnabled();

    /**
     * 检查是否销毁.
     * 
     * @return 是否销毁
     */
    virtual bool isDestroyed();

    /**
     * 设置对象名称函数.
     *
     * @param name_ 对象名称
     */
    virtual void setClassName(const mstring& name_);

    /**
     * 获取对象类型函数，请重写.
     * 
     * @return 对象类型
     */
    virtual MayType getType() = 0;

protected:
    bool enabled = true;
    bool destroyed = false;
    mstring class_name;
};

NS_MAY_END
