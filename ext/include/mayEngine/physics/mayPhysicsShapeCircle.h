#pragma once

#include "mayPhyiscsShapeBase.h"
#include "../mayVec3.h"

NS_MAY_BEGIN

class MAY_DLL PhysicsShapeCircle : public PhysicsShapeBase
{
public:
    /**
     * 创建圆形形状
     *
     * @param radius 半径
     */
    explicit PhysicsShapeCircle(float radius);

    ~PhysicsShapeCircle() override;

    void bindBody(RigidBodyNode* bodyNode) override;

private:
    float radius;
};

NS_MAY_END
