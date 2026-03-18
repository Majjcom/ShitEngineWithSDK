#pragma once

#include "../mayStlDefine.h"
#include "../mayType.h"
#include "maySpriteBase.h"

#include <optional>
#include <future>

NS_MAY_BEGIN

/**
 * 共享贴图类.
 */
class MAY_DLL SharedSprite : public SpriteBase
{
public:
    /**
     * 从指定资源包路径加载图像.
     *
     * @param man_      资源包对象
     * @param res_path  资源路径
     * @param key_      资源包密钥
     */
    SharedSprite(ResourceManager* man_, const mstring& path);

    /**
     * 从指定资源包加载图像.
     *
     * @param man_ 资源包对象
     * @param path 资源路径
     * @param wraps 横向纹理包裹
     * @param wrapt 纵向纹理包裹
     */
    SharedSprite(ResourceManager* man_, const mstring& path, TextureWrapping wraps, TextureWrapping wrapt);

    /**
     * 从内存加载图像.
     *
     * @param data      图片二进制(rgba)
     * @param len       数据大小
     * @param width     图片宽度
     * @param height    图片高度
     */
    SharedSprite(const unsigned char* data, size_t len, unsigned int width, unsigned int height);

    /**
     * 从内存加载图像.
     *
     * @param data   图片二进制(rgba)
     * @param len    数据大小
     * @param width  图片宽度
     * @param height 图片高度
     * @param wraps  横向纹理包裹
     * @param wrapt  纵向纹理包裹
     */
    SharedSprite(const unsigned char* data, size_t len, unsigned int width, unsigned int height,
                 TextureWrapping wraps, TextureWrapping wrapt);

    ~SharedSprite() override;

    /**
     * 内部API<br/>
     * 获取贴图ID
     *
     * @return
     */
    unsigned int getId() override;

    /**
     * 资源共享统计.
     */
    void addChild(); // 用于统计共享数量

    /**
     * 资源共享统计.
     */
    void releaseChild(); // 同上

private:
    unsigned int sharedCount = 0;
    std::optional<std::future<unsigned int>> futureTextureId{};
};

NS_MAY_END
