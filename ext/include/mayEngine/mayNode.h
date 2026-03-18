/*****************************************************************//**
 * @file   mayNode.h
 * @brief  Node 节点类封装
 * 
 * @author Majjcom
 * @date   July 2022
 *********************************************************************/
#pragma once

#include "mayComponent.h"
#include "mayStlDefine.h"
#include "mayArray.h"
#include "mayVec3.h"
#include "mayRef.h"
#include <chrono>

#include "mayEngine/memory/mimalloc.h"

NS_MAY_BEGIN

class Component;
class Scene;
class Vec3;
class Ref;

typedef mvector<Component*> ComponentArray;

#define M_NODE_MAX_TRANSFORM_RECURVE_COUNT 5000

/// Node creation declaration
#define M_NODE_DECL_CREATE(c, ...) \
public:                            \
    static c* create(__VA_ARGS__);

/// Node creation implementation
#define M_NODE_IMPL_CREATE(c)                   \
c* c::create()                                  \
{                                               \
    c* ret = maymnew(c);                        \
    if (!ret)                                   \
        return nullptr;                         \
    Engine::getInstance()->insertToPool(ret);   \
    if (!ret->init())                           \
    {                                           \
        ret->destroy();                         \
        return nullptr;                         \
    }                                           \
    return ret;                                 \
}

/// Node quick creation declaration
#define M_NODE_DECL_QCREATE(c) \
public:                        \
    static c* quickCreate(Node* parent, unsigned int rank);

/// Node quick creation implementation
#define M_NODE_IMPL_QCREATE(c)                      \
c* c::quickCreate(Node* parent, unsigned int rank)  \
{                                                   \
    c* ret = maymnew(c);                            \
    if (!ret)                                       \
        return nullptr;                             \
    ret->rank = rank;                               \
    ret->parent = parent;                           \
    Engine::getInstance()->quickInsert(ret);        \
    if (!ret->init())                               \
    {                                               \
        ret->destroy();                             \
        return nullptr;                             \
    }                                               \
    return ret;                                     \
}



typedef unsigned int ComponentID;

/**
 * 节点类.
 */
class MAY_DLL Node : public Ref
{
    M_GET_CLASS_NAME(Node);

public:

    /**
     * 节点创建函数，调用后将进行一次完整排序。如有继承此类请重写.
     * 
     * @return 生成节点的指针
     */
    static Node* create();

    /**
     * 快速创建函数。如有继承次类请重写.
     * 
     * @param parent    父节点指针
     * @param rank      排序
     * @return          新节点指针
     */
    static Node* quickCreate(Node* parent, unsigned int rank);

    /**
     * 节点析构函数，如有需要请带 virtual 重写.
     * 
     */
    ~Node() override;

    /**
     * 节点启用函数
     */
    void enable() override;

    /**
     * 节点生命周期函数，如有需要请重写.
     */
    virtual void update(float dt) {};

    /**
     * 节点生命周期函数，如有需要请重写.
     */
    virtual void onEnable() {};

    /**
     * 节点生命周期函数，如有需要请重写.
     */
    virtual void onEnter() {};

    /**
     * 节点生命周期函数，如有需要请重写.
     */
    virtual void onDestroy() {};

    /**
     * 节点初始化函数，如有需要请重写.
     * 
     * @return 是否初始化成功
     */
    virtual bool init();

    /**
     * 销毁对象函数.
     * 调用后，节点不再可用，并会在垃圾清理中销毁.
     * 
     */
    void destroy() override;

    /**
     * 获取根节点指针，请强制转换成 Scene* 后使用.
     * 
     * @return 根节点指针
     */
    virtual Scene* getRoot();    // Please convert to Scene*

    /**
     * 检查节点是否被销毁.
     * 
     * @return 是否被销毁
     */
    bool isDestroyed() override;

    /**
     * 检查节点是否已启用.
     * 
     * @return
     */
    bool isEnabled() override;

    /**
     * 供给引擎调用的生命周期函数.
     * 
     * @param dt 上一帧运行的真实时间
     */
    virtual void step(float dt);

    /**
     * 设置该节点的父节点.
     * 
     * @param parent_ 父节点指针
     * @param rank   节点层级顺序
     */
    virtual void setParent(Node* parent_, unsigned int rank);

    /**
     * 设置节点层级顺序.
     * 
     * @param rank 层级顺序
     */
    void setRank(unsigned int rank);

    /**
     * 获取节点层级顺序.
     * 
     * @return 节点层级顺序
     */
    unsigned int getRank() const;

    /**
     * 设置节点快速排序.
     *
     * @param rank_
     */
    void setQuickRank(unsigned int rank_);

    /**
     * 获取该节点的父节点.
     * 
     * @return
     */
    Node* getParent() const;

    /**
     * 获取节点位置.
     * 
     * @return 节点位置
     */
    virtual const Vec3& getPosition();
    virtual float getPositionX();
    virtual float getPositionY();
    virtual float getPositionZ();

    /**
     * 设置节点位置.
     * 
     * @param target 节点位置
     */
    virtual void setPosition(const Vec3& target);
    virtual void setPositionX(float n);
    virtual void setPositionY(float n);
    virtual void setPositionZ(float n);

    /**
     * 获取节点旋转信息.
     * 
     * @return 节点旋转信息
     */
    virtual const Vec3& getRotation();
    virtual float getRotationX();
    virtual float getRotationY();
    virtual float getRotationZ();

    /**
     * 设置节点旋转信息.
     * 
     * @param target 节点旋转信息
     */
    virtual void setRotation(const Vec3& target);
    virtual void setRotationX(float n);
    virtual void setRotationY(float n);
    virtual void setRotationZ(float n);

   /**
    * 获取节点缩放信息.
    * 
    * @return 节点缩放信息
    */
    virtual const Vec3& getScale();
    virtual float getScaleX();
    virtual float getScaleY();
    virtual float getScaleZ();

    /**
     * 设置节点缩放信息.
     * 
     * @param target 节点缩放信息
     */
    virtual void setScale(const Vec3& target);
    virtual void setScaleX(float n);
    virtual void setScaleY(float n);
    virtual void setScaleZ(float n);

    /**
     * 获取节点锚点信息.
     * 
     * @return 节点锚点信息
     */
    virtual const Vec3& getAnchor();
    virtual float getAnchorX();
    virtual float getAnchorY();
    virtual float getAnchorZ();

    /**
     * 设置节点锚点信息.
     * 
     * @param target 节点锚点信息
     */
    virtual void setAnchor(const Vec3& target);
    virtual void setAnchorX(float n);
    virtual void setAnchorY(float n);
    virtual void setAnchorZ(float n);

    /**
     * 添加节点组件.
     * 
     * @param component 组件指针
     * @return 组件ID
     */
    ComponentID addComponent(Component* component);

    /**
     * 添加节点组件.
     *
     * @param component 组件指针
     * @return 组件ID
     */
    ComponentID addComponentQueued(Component* component);

    /**
     * 根据组件ID获取组件.
     * 
     * @param   id_ 组件ID
     * @return  组件指针
     */
    Component* getComponent(ComponentID id_) const;

    /**
     * 根据组件类型获取组件
     *
     * @tparam T 组件类型
     * @return 第一个满足类型的组件
     */
    template <typename T>
    T* getComponent() const
    {
        for (Component* comp : components)
        {
            if (T* tc = M_DC(T*, comp))
            {
                return tc;
            }
        }
        return nullptr;
    }

    /**
     * 获取组件列表.
     * 
     * @return 组件列表
     */
    const ComponentArray& getComponents();

    /**
     * 获取指定类型的所有组件
     *
     * @tparam T 组件类型
     * @return 返回所有该类型的组件
     */
    template <typename T>
    mvector<T*> getComponents() const
    {
        mvector<T*> ret;
        for (Component* comp : components)
        {
            if (T* tc = M_DC(T*, comp))
            {
                ret.push_back(tc);
            }
        }
        return std::move(ret);
    }

    /**
     * 发送事件.
     *
     * @param event 事件名
     * @param value 事件值
     * @param refs  参数列表
     */
    void sendEvents(const mstring& event, const MayValueMap::MappingValue& value, const mvector<void*>& refs) const;

    /**
     * 获取节点类型.
     * 
     * @return 节点类型
     */
    MayType getType() override;

    /**
     * 将子坐标装换为世界坐标.
     * 
     * @param p0 子坐标
     * @param depth 递归深度 (填0即可)
     * @return   世界坐标
     */
    virtual Vec3 transformToWorldPoint(const Vec3& p0, uint32_t depth);

    /**
     * 将本地旋转转换成世界旋转.
     * 
     * @param r0 旋转量
     * @param depth 递归深度 (填0即可)
     * @return   世界旋转量
     */
    virtual Vec3 transformToWorldRotation(const Vec3& r0, uint32_t depth);

protected:

    /**
     * 父节点指针.
     */
    Node* parent = nullptr;

    /**
     * 该节点的层级顺序.
     */
    unsigned int rank = 0;

    /**
     * 是否已经调用onEnter.
     */
    bool entered = false;

    /**
     * 节点坐标信息.
     */
    Vec3 position = Vec3();

    /**
     * 节点旋转信息.
     */
    Vec3 rotation = Vec3();

    /**
     * 节点缩放信息.
     */
    Vec3 scale = Vec3(1.0f, 1.0f, 1.0f);

    /**
     * 节点锚点位置.
     */
    Vec3 anchor = Vec3(0.5f, 0.5f, 0.5f);

    /**
     * 节点组件池.
     */
    ComponentArray components;

    /**
     * 组件添加队列.
     */
    mqueue<Component*> component_add_queue;

    /**
     * 节点构建函数.
     *
     */
    Node();

    /**
     * 激活调用所有节点组件
     * 
     * @param 帧间隔
     */
    void callComponents(float dt);

private:
    bool component_calling = false;
    void releaseAllComponents();
};

NS_MAY_END
