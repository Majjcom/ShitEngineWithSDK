#pragma once

#include "../mayDefine.h"

NS_MAY_BEGIN

/**
* 贴图基类.
*/
class MAY_DLL SpriteBase
{
public:
    virtual ~SpriteBase();

    /**
     * 内部API<br/>
     * 获取贴图ID
     *
     * @return 贴图ID
     */
    virtual unsigned int getId();

    /**
     * 获取图像宽度.
     *
     * @return 图像宽度
     */
    float getWidth() const;

    /**
     * 获取图像高度.
     *
     * @return 图像高度
     */
    float getHeight() const;

    /**
     * 检查是否有加载错误.
     *
     * @return 是否有加载错误
     */
    bool getError() const;

protected:
    unsigned int textureId = 0;
    float width = 0;
    float height = 0;
    bool is_error = false;
};

NS_MAY_END
