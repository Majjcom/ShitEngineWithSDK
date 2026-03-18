#pragma once

#include "mayPhyiscsShapeBase.h"
#include "../mayVec3.h"

NS_MAY_BEGIN

class MAY_DLL PhysicsShapeBox : public PhysicsShapeBase
{
public:
    /**
     * 创建盒子形状.
     * 
     * @param size  盒子尺寸
     */
    explicit PhysicsShapeBox(Vec3 size);

    virtual ~PhysicsShapeBox();

protected:
    virtual void* genPolygon();

private:
    float width;
    float height;

};

NS_MAY_END
