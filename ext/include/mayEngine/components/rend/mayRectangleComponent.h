/*****************************************************************//**
 * @file   mayRectAngleComponent.h
 * @brief  对形状渲染组件的封装
 * 
 * @author Majjcom
 * @date   July 2022
 *********************************************************************/
#pragma once

#include "base/mayRendComponentBase.h"
#include "../../mayComponent.h"
#include "../../mayVec3.h"

NS_MAY_BEGIN

class Component;

namespace Graphic
{
    class Task;
}

/**
 * 形状渲染组件类.
 */
class MAY_DLL RendRectangleComponent
    : public Component
    , public RendShaderBase
    , public RendPipelineConfig
    , public RendComponentColorBase
    , public RendComponentAlphaBase
    , public RendLineModeBase
    , public RendCameraBase
{
    M_GET_CLASS_NAME(RendRectangleComponent);

public:
    /**
     * 根据尺寸创建形状渲染组件.
     * 
     * @param size_ 形状大小
     * @param node_ 父节点指针
     * @return      形状渲染组件指针
     */
    static RendRectangleComponent* create(const Vec3& size_, Node* node_);

    /**
     * 贴图渲染组件初始化函数.
     *
     * @return 是否初始化成功
     */
    bool init() override;

    /**
     * 主任务函数，由父节点调用.
     *
     * @param dt 上一帧运行时间
     */
    void main_update(float dt) override;

    /**
     * 获取形状尺寸.
     * 
     * @return 形状尺寸
     */
    Vec3 getSize();

    /**
     * 设置形状尺寸.
     * 
     * @param size_ 形状尺寸
     */
    void setSize(Vec3 size_);

private:
    Vec3 rectSize = Vec3(0, 0, 0);

    static mvector<Graphic::Task*> _render_call(RendRectangleComponent* self);
};

NS_MAY_END
