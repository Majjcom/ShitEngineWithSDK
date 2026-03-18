#pragma once

#include "base/mayRendComponentBase.h"

#include "../../mayVec3.h"
#include "../../mayText.h"

NS_MAY_BEGIN

class SharedSprite;

namespace Graphic
{
    class Task;
}

/**
 * 文字渲染组件
 */
class MAY_DLL RendTextComponent
    : public Component
    , public RendShaderBase
    , public RendPipelineConfig
    , public RendCameraBase
    , public RendComponentAlphaBase
    , public RendComponentColorBase
{
    M_GET_CLASS_NAME(RendTextComponent);

public:
    RendTextComponent(RendTextComponent&) = delete;
    ~RendTextComponent() override;

    /**
     * 创建文字渲染组件
     *
     * @param textRenderer 字体渲染器
     * @param node_ 节点
     */
    static RendTextComponent* create(MayText* textRenderer, Node* node_);

    /**
     * 初始化函数
     *
     * @return 是否初始化成功
     */
    bool init() override;

    /**
     * 生命周期函数，主要逻辑，重写后请重写调用此函数
     *
     * @param dt 帧时间
     */
    void main_update(float dt) override;

    /**
     * 设置文本
     *
     * @param text_ UTF-8文本
     */
    void setText(const mstring& text_);

    /**
     * 获取文本
     *
     * @return 文本
     */
    const mstring& getText();

    /**
     * 设置文字渲染区域
     *
     * @param area_ 渲染区域
     */
    void setArea(const Vec3& area_);

    /**
     * 获取文字渲染区域
     *
     * @return 渲染区域
     */
    const Vec3& getArea() const;

    /**
     * 设置字体大小
     *
     * @param size_ 字体大小
     */
    void setFontSize(unsigned int size_);

    /**
     * 获取字体大小
     *
     * @return 字体大小
     */
    unsigned int getFontSize() const;

    /**
     * 设置行间距
     *
     * @param spacing_ 行间距
     */
    void setLineSpacing(float spacing_);

    /**
     * 获取行间距
     *
     * @return 行间距
     */
    float getLineSpacing() const;

    /**
     * 区域边缘间隔
     */
    enum class TextAreaMargin
    {
        /// 顶部间隔
        Top = 0,

        /// 底部间隔
        Bottom,

        /// 左侧间隔
        Left,

        /// 右侧间隔
        Right,
    };

    /**
     * 设置区域边缘间隔
     *
     * @param side 边
     * @param val_ 间隔大小
     */
    void setAreaMargin(TextAreaMargin side, float val_);

    /**
     * 获取区域边缘间隔
     *
     * @param side 便
     * @return 间隔大小
     */
    float getAreaMargin(TextAreaMargin side) const;

    /**
     * 水平对齐方式
     */
    enum class TextAlignHorizontal
    {
        /// 左对齐
        Left = 0,

        /// 中心对齐
        Center,

        /// 右对齐
        Right
    };

    /**
     * 设置水平对齐方式
     *
     * @param align 水平对齐方式
     */
    void setTextAlignHorizontal(TextAlignHorizontal align);

    /**
     * 获取水平对齐方式
     *
     * @return 水平对齐方式
     */
    TextAlignHorizontal getTextAlignHorizontal() const;

private:
    RendTextComponent();
    void updateTextCache();
    void updateTextCacheAlignHorLeft();
    void updateTextCacheAlignHorCenter();
    void updateTextCacheAlignHorRight();

    MayText* text_renderer = nullptr;
    mstring text;
    Vec3 area;
    unsigned int font_size = 12;
    float areaTopMargin = 2.0f;
    float areaBottomMargin = 2.0f;
    float areaLeftMargin = 2.0f;
    float areaRightMargin = 2.0f;
    float lineSpacing = 4.0f;
    TextAlignHorizontal alignHorizontal = TextAlignHorizontal::Left;

    bool need_update = false;
    mvector<MayTextData> text_sprites;
    mvector<Vec3> vertices_cache;

    static mvector<Graphic::Task*> _render_call(const RendTextComponent* self);

};

NS_MAY_END
