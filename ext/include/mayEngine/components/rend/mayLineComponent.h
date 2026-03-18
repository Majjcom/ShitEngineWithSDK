/*****************************************************************//**
 * @file   mayLineComponent.h
 * @brief  对线条渲染的封装
 * 
 * @author Majjcom
 * @date   January 2023
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
 * 直线绘制组件
 */
class MAY_DLL RendLineComponent
    : public Component
    , public RendShaderBase
    , public RendPipelineConfig
    , public RendLineModeBase
    , public RendCameraBase
    , public RendComponentColorBase
    , public RendComponentMultiAlphaBase<2>
{
    M_GET_CLASS_NAME(RendLineComponent);

public:
    /**
     * 创建线渲染组件.
     * 
     * @param pointFrom_    线开始位置
     * @param pointTo_      线结束位置
     * @param wide_         线宽
     * @param node_         父节点指针
     * @return              线渲染组件指针
     */
    static RendLineComponent* create(Vec3 pointFrom_, Vec3 pointTo_, float wide_, Node* node_);

    /**
     * 组件初始化.
     * 
     * @return 初始化结果
     */
    bool init() override;

    /**
     * 组件主函数.
     * 
     */
    void main_update(float dt) override;

    /**
     * 获取点位置.
     * 
     * @param point_    0-线开始位置, 1-线结束位置
     * @return          点
     */
    Vec3 getPoint(unsigned int point_) const;

    /**
     * 设置点位置.
     * 
     * @param point_    0-线开始位置, 1-线结束位置
     * @param pos_      点
     */
    void setPoint(unsigned int point_, Vec3 pos_);

    /**
     * 获取线宽度
     *
     * @return 线宽度
     */
    float getWide() const;

    /**
     * 设置线宽度
     *
     * @param wide_ 线宽度
     */
    void setWide(float wide_);

private:
    Vec3 points[2];
    float wide{};

    static mvector<Graphic::Task*> _render_call(RendLineComponent* self);
};

NS_MAY_END
