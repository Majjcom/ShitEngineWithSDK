#pragma once

#include "../../../mayComponent.h"
#include "../../../mayVec3.h"

NS_MAY_BEGIN

/**
 * 物理世界组件，用于管理物理模拟
 */
class MAY_DLL PhysicsWorldComponent : public Component
{
    M_GET_CLASS_NAME(PhysicsWorldComponent);

public:
    ~PhysicsWorldComponent() override;

    /**
     * 创建物理世界模拟组件.
     * 
     * @param node_ 父节点
     * @return      组件指针
     */
    static PhysicsWorldComponent* create(Node* node_);

    /**
     * 主要的调用，每帧运行。如有必要请重写.
     */
    void main_update(float dt) override;

    /**
     * 组件初始化.
     *
     * @return 初始化结果
     */
    bool init() override;

    /**
     * (内部API)获取世界.
     * 
     * @return 世界id
     */
    uint64_t getWorldId() const;

    /**
     * 设置物理帧模拟次数.
     * 
     * @param count 模拟次数
     */
    void setPhysicsStepCount(int count);

    /**
     * 获取世界重力
     *
     * @return 重力向量
     */
    Vec3 getGravity() const;

    /**
     * 设置世界重力.
     * 
     * @param gravity   重力向量
     */
    void setGravity(Vec3 gravity) const;

    /**
     * 获取是否启用接触回调
     *
     * @return 是否启用接触回调
     */
    bool getEnableContactCallback() const;

    /**
     * 设置是否启用接触回调
     *
     * @param flag 是否启用接触回调
     */
    void setEnableContactCallback(bool flag);

    /**
     * 触发爆炸.
     *
     * @param pos       爆炸位置
     * @param radius    爆炸半径
     * @param impulse   爆炸冲量
     * @param falloff   爆炸衰减
     * @param mask_bits 接触过滤
     */
    void worldExplode(const Vec3& pos, float radius, float impulse, float falloff = 0, uint64_t mask_bits = UINT64_MAX) const;

    /**
     * 绑定形状和节点，用于接触回调
     *
     * @param shapeId 形状
     * @param node 节点
     */
    void bind_shape_node(uint64_t shapeId, Node* node);

    /**
     * 释放形状绑定
     *
     * @param shapeId 形状
     */
    void unbind_shape_node(uint64_t shapeId);

private:
    PhysicsWorldComponent();
    void applyContactCallback() const;
    uint64_t world = 0;
    bool enable_contact_callback = false;
    int physics_step_count = 4;
    mmap<uint64_t, Node*> shape_node_map;

};

NS_MAY_END
