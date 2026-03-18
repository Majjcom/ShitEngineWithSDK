#pragma once

#include "../../mayStlDefine.h"
#include "../../mayComponent.h"
#include "../../mayVec3.h"
#include "../../system/IO/mayInput.h"

#include <functional>
#include <optional>

NS_MAY_BEGIN

class MAY_DLL TouchButtonComponent : public Component
{
    M_GET_CLASS_NAME(TouchButtonComponent);

public:
    typedef std::function<void(int64_t, IO::KeyState)> BtnCallbackFunc;
    typedef std::function<void(bool)> BtnHoverChangeCallbackFunc;

    TouchButtonComponent(TouchButtonComponent&) = delete;
    TouchButtonComponent(TouchButtonComponent&&) = delete;

    static TouchButtonComponent* create(const Vec3& size, Node* node, const BtnCallbackFunc& callback);

    bool init() override;

    void main_update(float dt) override;

    void setSize(const Vec3& size);

    const Vec3& getSize() const;

    void setHoverChangeCallback(const BtnHoverChangeCallbackFunc& callback);

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
    TouchButtonComponent();
    Vec3 btnSize{};
    std::optional<int64_t> current_touch_id;
    std::optional<Vec3> current_touch_pos;
    mset<int64_t> pressing_ids;
    bool hovered = false;
    BtnCallbackFunc callBackFunc = nullptr;
    BtnHoverChangeCallbackFunc hoverChangeCallbackFunc = nullptr;
    Vec3 btnPosRecord{};
    BlockFlag blockFlag = BlockFlag::None;

};

NS_MAY_END
