#pragma once

#include "../mayStlDefine.h"

NS_MAY_BEGIN

class RigidBodyNode;

class MAY_DLL PhysicsShapeBase
{
public:
    PhysicsShapeBase();
    virtual ~PhysicsShapeBase();

    /**
     * 绑定刚体节点 (用户不需要调用，应使用刚体的绑定函数).
     * 
     * @param bodyNode  刚体节点
     */
    virtual void bindBody(RigidBodyNode* bodyNode);

    /**
     * 生成形状属性.
     */
    void genDef();

    /**
     * 设置摩擦.
     * 
     * @param friction  摩擦，0-1之间
     */
    void setFriction(float friction) const;

    /**
     * 设置补偿.
     * 
     * @param restitution   补偿，0-1之间
     */
    void setRestitution(float restitution) const;

    /**
     * 设置密度.
     * 
     * @param density   密度
     */
    void setDensity(float density) const;

    /**
     * 设置过滤器.
     * 
     * @param categoryBits  自身属性
     * @param maskBits      检测属性
     * @param groupIndex    组标志
     */
    void setFilter(uint64_t categoryBits, uint64_t maskBits, int32_t groupIndex) const;

    /**
     * 设置是否开启碰撞事件.
     *
     * @param enable  是否开启
     */
    void enableContactEvents(bool enable) const;

    /**
     * (内部API)获取形状.
     * 
     * @return  形状id
     */
    uint64_t getShape() const;

protected:
    virtual void* genPolygon();

    uint64_t shape = 0;
    uint64_t world = 0;
    void* def = nullptr;
};

NS_MAY_END
