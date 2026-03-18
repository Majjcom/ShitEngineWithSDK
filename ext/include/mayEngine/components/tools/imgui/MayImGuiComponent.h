#pragma once

#include "../../../mayStlDefine.h"
#include "../../../mayComponent.h"
#include "../../rend/base/mayRendComponentBase.h"

#include <functional>

NS_MAY_BEGIN

/**
 * 使用imgui进行ui绘制。建议重写进行操作<br>
 * 建议在管线配置文件中创建一个子管线，将imgui渲染在单独的管线内，再将子管线作为贴图渲染在场景中
 */
class MAY_DLL ImGuiComponent
    : public Component, public RendPipelineConfig
{
    M_GET_CLASS_NAME(ImGuiComponent);

public:
    /**
     * 创建imgui组件
     *
     * @param node_ 父节点
     * @return 组件对象
     */
    static ImGuiComponent* create(Node* node_);

    ~ImGuiComponent() override;

    /**
     * 初始化函数<br>
     * 用户请调用此init函数以确保正确初始化
     *
     * @return 是否初始化成功
     */
    bool init() override;

    /**
     * 预备更新函数，用于创建imgui帧，由引擎自动调用
     *
     * @param dt 帧时间
     */
    void pre_update(float dt) final;

    /**
     * 主更新函数，可重写
     *
     * @param dt 帧时间
     */
    void main_update(float dt) override;

    /**
     * 更新后函数，用于对imgui帧进行结束操作
     *
     * @param dt 帧时间
     */
    void after_update(float dt) final;

protected:
    void applyCtx() const;

private:
    void* ctx = nullptr;
    std::function<void(void*)> kfunc = nullptr;

    static mvector<void*> _render_call(ImGuiComponent* self);

};

NS_MAY_END
