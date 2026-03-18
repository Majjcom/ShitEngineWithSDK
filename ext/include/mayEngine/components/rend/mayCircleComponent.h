#pragma once

#include "../../mayComponent.h"

#include "base/mayRendComponentBase.h"

/// 默认分片数量
#define M_COMPONENT_REND_CIRCLE_DEFAULT_PARTITION 15

/// 最大分片数量
#define M_COMPONENT_REND_CIRCLE_MAX_PARTITION     400

NS_MAY_BEGIN

namespace Graphic
{
    class Task;
}

/**
 * 圆形渲染组件.
 */
class MAY_DLL RendCircleComponent
    : public Component
    , public RendShaderBase
    , public RendPipelineConfig
    , public RendComponentAlphaBase
    , public RendComponentColorBase
    , public RendLineModeBase
    , public RendCameraBase
{
    M_GET_CLASS_NAME(RendCircleComponent);

public:
    /**
     * 创建圆形渲染组件.
     * 
     * @param radius_   圆半径
     * @param node_     父节点
     * @return          组件实例
     */
    static RendCircleComponent* create(float radius_, Node* node_);

    /**
     * 组件初始化函数.
     * 
     * @return  初始化是否成功
     */
    bool init() override;

    /**
     * 主要更新函数.
     * 
     * @param dt    时间间隔
     */
    void main_update(float dt) override;

    /**
     * 获取组件半径.
     * 
     * @return  组件半径
     */
    float getRadius() const;

    /**
     * 设置组件半径.
     * 
     * @param radius_   半径
     */
    void setRadius(float radius_);

    /**
     * 获取分片数量.
     * 
     * @return  圆分片数量
     */
    int getPartition() const;

    /**
     * 设置圆分片数量.
     * 
     * @param partition_
     */
    void setPartition(int partition_);

    /**
     * 获取是否自动分片.
     * 
     * @return  自动分片
     */
    bool getAutoPartition() const;

    /**
     * 设置是否自动分片.
     * 
     * @param val_  自动分片
     */
    void setAutoPartition(bool val_);

    /**
     * 更新自动分片.
     */
    void updatePartition();

private:
    float radius = 1;
    int partition = M_COMPONENT_REND_CIRCLE_DEFAULT_PARTITION;
    mvector<Vec3> point_cache;
    bool need_update_cache = true;
    bool auto_partition = true;

    void updatePoints();
    static mvector<Graphic::Task*> _render_call(RendCircleComponent* self);
};

NS_MAY_END
