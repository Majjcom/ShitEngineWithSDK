/*****************************************************************//**
 * @file   mayComponent.h
 * @brief  Component 类，封装了供给 Node 类调用的接口
 * 
 * @author Majjcom
 * @date   July 2022
 *********************************************************************/
#pragma once

#include "mayRef.h"
#include "mayStlDefine.h"
#include "mayStdExport.h"
#include "mayValueMap.h"

NS_MAY_BEGIN

class Node;

/**
 * 节点类.
 */
class MAY_DLL Component : public Ref
{
    M_GET_CLASS_NAME(Component);

public:
    /**
     * 析构函数.
     * 
     */
    virtual ~Component();

    /**
     * 创建函数，如有必要请重写，该函数对于自动垃圾清理机制十分重要.
     * 
     * @return
     */
    [[nodiscard]] static Component* create();

    /**
     * 设置父节点.
     * 
     * @param node  父节点指针
     */
    void setNode(Node* node);

    /**
     * 获取父节点.
     * 
     * @return 父节点指针
     */
    Node* getNode();

    /**
     * 销毁函数，如有必要请重写，该函数对于自动垃圾清理机制十分重要.
     */
    void destroy() override;

    /**
     * 初始化函数，如有必要请重写.
     * 
     * @return 是否初始化成功
     */
    virtual bool init();

    /**
     * 步函数，为父节点提供接口.
     * 
     * @param dt 上一帧的真实持续时间
     */
    virtual void step(float dt);

    /**
     * 该节点首次进行生命周期调用时或在enable后首次调用时调用的函数，如有必要请重写.
     */
    virtual void onEnter() {};

    /**
     * 该节点被销毁时调用的函数，如有必要请重写.
     */
    virtual void onDestroy() {};

    /**
     * 先前的调用，每帧运行。如有必要请重写.
     */
    virtual void pre_update(float dt) {};

    /**
     * 主要的调用，每帧运行。如有必要请重写.
     */
    virtual void main_update(float dt) {};

    /**
     * 最后的调用，每帧运行。如有必要请重写.
     */
    virtual void after_update(float dt) {};

    /**
     * 事件接收函数，如有必要请重写.
     *
     * @param event 事件名
     * @param value 事件值
     * @param refs  参数列表
     */
    virtual void onEvent(const mstring& event, const MayValueMap::MappingValue& value, const mvector<void*>& refs) {};

    /**
     * 获取对象的类型.
     * 
     * @return 对象类型
     */
    MayType getType() override;

protected:
    Node* node = nullptr;
    bool entered = false;
    Component() {};
};

NS_MAY_END
