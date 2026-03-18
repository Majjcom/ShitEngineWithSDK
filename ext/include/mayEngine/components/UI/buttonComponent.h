#pragma once

#include "../../mayComponent.h"
#include "../../mayVec3.h"
#include "../../system/IO/mayInput.h"

#include <functional>

NS_MAY_BEGIN

/**
 * 按钮组件.
 */
class MAY_DLL ButtonComponent : public Component
{
    M_GET_CLASS_NAME(ButtonComponent);

public:
    typedef std::function<void(IO::MouseButton, IO::KeyState)> BtnCallbackFunc;
    typedef std::function<void(bool)> BtnHoverChangeCallbackFunc;

    ButtonComponent(ButtonComponent&) = delete;
    ButtonComponent(ButtonComponent&&) = delete;

    /**
     * 创建按钮组件.
     * 
     * @param size_     按钮大小
     * @param node_     组件父节点
     * @param callback  按钮回调函数
     * @return          组件指针
     */
    static ButtonComponent* create(const Vec3& size_, Node* node_, const BtnCallbackFunc& callback);

    /**
     * 初始化函数.
     * 
     * @return 初始化是否成功
     */
    bool init() override;

    /**
     * 主生命周期函数.
     * 
     * @param dt
     */
    void main_update(float dt) override;

    /**
     * 设置按钮尺寸.
     * 
     * @param size_ 按钮尺寸
     */
    void setSize(const Vec3& size_);

    /**
     * 获取按钮尺寸.
     *
     * @return 按钮尺寸
     */
    const Vec3& getSize() const;

    /**
     * 设置鼠标悬浮回调
     *
     * @param func 回调函数
     */
    void setHoverChangeCallback(const BtnHoverChangeCallbackFunc& func);

    enum class BlockFlag : uint8_t
    {
        None = 0,
        EnableBlock = 1,
        IgnoreBlock = 1 << 1,
    };

    void setBlockFlag(BlockFlag flag);

    BlockFlag getBlockFlag() const;

    static void resetBlockData();

private:
    ButtonComponent();
    Vec3 btnSize = Vec3(0);
    BtnCallbackFunc callbackFunc = nullptr;
    BtnHoverChangeCallbackFunc hoverChangeCallbackFunc = nullptr;
    bool inRectRecord = false;
    IO::KeyState buttonState[2] = { IO::RELEASE, IO::RELEASE };
    bool buttonReleaseEnable[2] = { false, false };
    Vec3 buttonPosRecord[2]{};
    BlockFlag blockFlag = BlockFlag::None;

    void makeCallBack(IO::MouseButton btn, IO::KeyState status) const;
};

NS_MAY_END
