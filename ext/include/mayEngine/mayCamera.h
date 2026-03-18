/*****************************************************************//**
 * @file   mayCamera.h
 * @brief  Camera 类，封装了相机的大量功能
 * 
 * @author Majjcom
 * @date   July 2022
 *********************************************************************/
#pragma once

#include "mayDefine.h"
#include "mayNode.h"

NS_MAY_BEGIN

class Node;

/**
 * Camera 相机类.
 */
class MAY_DLL Camera : public Node
{
    M_GET_CLASS_NAME(Camera);

public:
    /**
     * 创建通用相机类，如果你对相机进行了功能调整，请在子类重写Create.
     * 
     * @return
     */
    static Camera* create();
    Vec3 transform(const Vec3& origin);
    MayType getType() override;

private:
    Camera() {};
};

NS_MAY_END
