/*****************************************************************/ /**
 * \file   maySpriteComponent.h
 * \brief  对贴图渲染组件的封装
 *
 * \author Majjcom
 * \date   July 2022
 *********************************************************************/
#pragma once

#include "base/mayRendComponentBase.h"
#include "../../mayResourseManager.h"
#include "../../mayComponent.h"
#include "../../maySprite.h"

NS_MAY_BEGIN

class Component;
class Sprite;

namespace Graphic
{
    class Task;
}

/**
 * 贴图渲染组件.
 */
class MAY_DLL RendSpriteComponent
    : public Component
    , public RendShaderBase
    , public RendPipelineConfig
    , public RendCameraBase
    , public RendComponentColorBase
    , public RendComponentAlphaBase
{
    M_GET_CLASS_NAME(RendSpriteComponent);

public:
    ~RendSpriteComponent() override;

    /**
     * 根据指定资源包和资源路径加载资源，并创建贴图渲染组件.
     *
     * @param man_      资源包对象
     * @param path      资源路径
     * @param key       资源包密钥
     * @param node_     父节点指针
     * @return          贴图渲染组件指针
     */
    static RendSpriteComponent* create(ResourceManager* man_, const mstring& path, Node* node_);

    /**
     * 根据指定资源包和路径加载资源，并创建贴图渲染组件.
     *
     * @param man_ 资源包对象
     * @param path 资源路径
     * @param node_ 父节点指针
     * @param wraps 横向纹理包裹
     * @param wrapt 纵向纹理包裹
     * @return 贴图渲染组件指针
     */
    static RendSpriteComponent* create(ResourceManager* man_, const mstring& path, Node* node_,
                                       TextureWrapping wraps,
                                       TextureWrapping wrapt);

    /**
     * 根据传入的贴图创建渲染组件<br/>
     * 支持使用SharedSprite, BunchSprite, PlaceholderSprite创建组件
     *
     * @param sprite_ 贴图对象
     * @param node_   父节点
     * @return        渲染组件对象
     */
    static RendSpriteComponent* create(SpriteBase* sprite_, Node* node_);

    /**
     * 贴图渲染组件初始化函数.
     *
     * \return 是否初始化成功
     */
    bool init() override;

    /**
     * 主任务函数，由父节点调用.
     *
     * \param dt 上一帧运行时间
     */
    void main_update(float dt) override;

    /**
     * 更换贴图
     *
     * @param sprite_ 贴图
     */
    void changeSprite(SpriteBase* sprite_);

    /**
     * 获取贴图宽度.
     *
     * \return 贴图宽度
     */
    float getSpriteWidth() const;

    /**
     * 获取贴图高度.
     *
     * \return 贴图高度
     */
    float getSpriteHeight() const;

protected:
    mstring imgPath;
    bool shared = false;
    bool wrap_changed = false;
    ResourceManager* resman = nullptr;
    SpriteBase* texture = nullptr;
    RendSpriteComponent();
    TextureWrapping wrap_s = TextureWrapping::Repeat;
    TextureWrapping wrap_t = TextureWrapping::Repeat;
    void releaseCurrentTexture();
    static mvector<Graphic::Task*> _rander_call(RendSpriteComponent* self);
};

NS_MAY_END
