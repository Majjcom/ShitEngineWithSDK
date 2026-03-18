#pragma once

#include "../mayStlDefine.h"
#include "../physics/mayPhysicsDefine.h"
#include "../mayNode.h"

#define M_NODE_PHYSICS_DECL_CREATE(C) \
public: static C* create(PhysicsWorldComponent*);

#define M_NODE_PHYSICS_IMPL_CREATE(C)         \
C* C::create(PhysicsWorldComponent* world)    \
{                                             \
    C* ret = maymnew(C);                      \
    if (!ret)                                 \
        return nullptr;                       \
    ret->world = world->getWorldId();         \
    ret->world_comp = world;                  \
    Engine::getInstance()->insertToPool(ret); \
    if (!ret->init())                         \
    {                                         \
        ret->destroy();                       \
        return nullptr;                       \
    }                                         \
    return ret;                               \
}

#define M_NODE_PHYSICS_DECL_QCREATE(C) \
public: static C* quickCreate(PhysicsWorldComponent*, Node*, unsigned int);

#define M_NODE_PHYSICS_IMPL_QCREATE(C)                                           \
C* C::quickCreate(PhysicsWorldComponent* world, Node* parent, unsigned int rank) \
{                                                                                \
    C* ret = maymnew(C);                                                         \
    if (!M_DC(Scene*, parent))                                                   \
    {                                                                            \
        maymdel(ret);                                                            \
        return nullptr;                                                          \
    }                                                                            \
    if (!ret)                                                                    \
        return nullptr;                                                          \
    ret->rank = rank;                                                            \
    ret->parent = parent;                                                        \
    ret->world = world->getWorldId();                                            \
    ret->world_comp = world;                                                     \
    Engine::getInstance()->quickInsert(ret);                                     \
    if (!ret->init())                                                            \
    {                                                                            \
        ret->destroy();                                                          \
        return nullptr;                                                          \
    }                                                                            \
    return ret;                                                                  \
}

NS_MAY_BEGIN

class PhysicsWorldComponent;
class PhysicsShapeBase;

/**
 * 物理刚体节点，可以再物理世界中模拟的节点，只能放在根节点下
 */
class MAY_DLL RigidBodyNode : public Node
{
    M_NODE_PHYSICS_DECL_CREATE(RigidBodyNode);
    M_NODE_PHYSICS_DECL_QCREATE(RigidBodyNode);
    M_GET_CLASS_NAME(RigidBodyNode);

public:
    ~RigidBodyNode() override;

    /**
     * 节点初始化函数，如有需要请重写.
     * 
     * @return 是否初始化成功
     */
    bool init() override;

    /**
     * 刚体初始化函数.
     * 
     * @return 是否初始化成功
     */
    bool rigid_init();

    /**
     * 绑定形状.<br>
     * 由Node接管内存管理.
     * 
     * @param shape 形状对象指针
     */
    void bindShape(PhysicsShapeBase* shape);

    /**
     * 生命周期函数，用于释放部分资源<br>
     * 如果重写，请再次调用此函数
     */
    void onDestroy() override;

    /**
     * 供给引擎调用的生命周期函数.
     * 
     * @param dt 上一帧运行的真实时间
     */
    void step(float dt) override;

    /**
     * 设置该节点的父节点(父节点只能为Scene).
     * 
     * @param parent_ 父节点指针
     * @param rank_   节点层级顺序
     */
    void setParent(Node* parent_, unsigned int rank_) override;

    /**
     * 设置节点位置.
     * 
     * @param pos    位置信息
     */
    void setPosition(const Vec3& pos) override;
    void setPositionX(float n) override;
    void setPositionY(float n) override;
    void setPositionZ(float n) override;

    /**
     * 设置节点旋转信息.
     * 
     * @param rot    旋转信息
     */
    void setRotation(const Vec3& rot) override;
    void setRotationX(float n) override;
    void setRotationY(float n) override;
    void setRotationZ(float n) override;

    /**
     * (内部API)获取刚体指针.
     * 
     * @return 刚体id
     */
    uint64_t getBody() const;

    /**
     * (内部API)获取世界指针.
     * 
     * @return 世界id
     */
    uint64_t getWorld() const;

    /**
     * 刚体类型.
     */
    enum RigidBodyType
    {
        /// 静态刚体
        StaticBody = 0,

        /// 运动刚体
        KinematicBody = 1,

        /// 动态刚体
        DynamicBody = 2,
    };

    /**
     * 获取刚体类型类型.
     * 
     * @return  刚体类型
     */
    RigidBodyType getRigidBodyType() const;

    /**
     * 设置刚体类型.
     * 
     * @param type  刚体类型
     */
    void setRigidBodyType(RigidBodyType type) const;

    /**
     * 获取线性速度.
     * 
     * @return  速度向量
     */
    Vec3 getLinearVelocity() const;

    /**
     * 设置线性速度.
     * 
     * @param velocity  速度向量
     */
    void setLinearVelocity(const Vec3& velocity) const;

    /**
     * 获取角速度.
     * 
     * @return  角速度(弧度)
     */
    float getAngularVelocity() const;

    /**
     * 设置角速度.
     * 
     * @param velocity  角速度(弧度)
     */
    void setAngularVelocity(float velocity) const;

    /**
     * 获取线性阻尼.
     * 
     * @return  阻尼
     */
    float getLinearDamping() const;

    /**
     * 设置线性阻尼.
     * 
     * @param damping   阻尼
     */
    void setLinearDamping(float damping) const;

    /**
     * 获取角阻尼.
     * 
     * @return  阻尼
     */
    float getAngularDamping() const;

    /**
     * 设置角阻尼.
     * 
     * @param damping   阻尼
     */
    void setAngularDamping(float damping) const;

    /**
     * 获取当前激活状态
     *
     * @return 是否激活
     */
    bool getAwake() const;

    /**
     * 设置激活状态.
     * 
     * @param awake     是否激活
     */
    void setAwake(bool awake) const;

    /**
     * 施加一个力.
     * 
     * @param force     力向量
     * @param point     施加位置
     * @param wake      是否激活物理模拟
     */
    void applyForce(const Vec3& force, const Vec3& point, bool wake) const;

    /**
     * 施加一个扭矩.
     * 
     * @param torque    扭矩
     * @param wake      是否激活物理模拟
     */
    void applyTorque(float torque, bool wake) const;

    /**
     * 施加一个线性冲量.
     * 
     * @param impulse   冲量向量
     * @param point     施加位置
     * @param wake      是否激活物理模拟
     */
    void applyLinearImpulse(const Vec3& impulse, const Vec3& point, bool wake) const;

    /**
     * 施加一个角动惯量.
     * 
     * @param impulse   冲量
     * @param wake      是否激活物理模拟
     */
    void applyAngularImpulse(float impulse, bool wake) const;

    /**
     * 获取刚体是否作为子弹刚体
     *
     * @return 是否作为子弹刚体
     */
    bool getBullet() const;

    /**
     * 设置刚体是否作为子弹刚体
     *
     * @param flag 是否作为子弹刚体
     */
    void setBullet(bool flag) const;

    /**
     * 刚体物理接触事件.<br>
     * 如果需要在Lua中处理事件，请在重写函数后调用此函数.
     *
     * @param contactType   碰撞类型
     * @param other         碰撞对象
     */
    virtual void physicsContactEvent(PhysicsContactType contactType, RigidBodyNode* other);

protected:
    uint64_t world = 0;
    bool transfrom_force_change = false;
    PhysicsWorldComponent* world_comp = nullptr;
    
private:
    void applyPhysicalTransform();
    void forceChangePhysicalTransform() const;
    uint64_t body = 0;
    mvector<PhysicsShapeBase*> shapes;

};

NS_MAY_END
