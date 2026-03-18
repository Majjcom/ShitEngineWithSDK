#pragma once

#include "../mayStlDefine.h"
#include "../mayType.h"
#include "maySpriteBase.h"

#include <optional>
#include <future>

NS_MAY_BEGIN

class ResourceManager;
class SharedSprite;

/**
 * 贴图类.
 */
class MAY_DLL Sprite : public SpriteBase
{
public:
    ~Sprite() override;

    /**
     * 从指定资源包的指定路径加载图像.
     *
     * @param man_  资源包对象
     * @param path  资源路径
     * @param key_  资源包密钥
     */
    Sprite(ResourceManager* man_, const mstring& path);

    /**
     * 从指定资源包加载图像.
     *
     * @param man_ 数据包对象
     * @param path 资源路径
     * @param wraps 横向纹理包裹
     * @param wrapt 纵向纹理包裹
     */
    Sprite(ResourceManager* man_, const mstring& path, TextureWrapping wraps, TextureWrapping wrapt);

    /**
     * 内部API<br/>
     * 获取资源ID
     *
     * @return
     */
    unsigned int getId() override;

protected:
    std::optional<std::future<unsigned int>> futureTextureId{};
};

NS_MAY_END
