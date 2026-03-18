#pragma once

#include "../mayDefine.h"
#include "maySpriteBase.h"

NS_MAY_BEGIN

/**
 * Bunch导出.
 */
class MAY_DLL BunchSprite : public SpriteBase
{
public:
    explicit BunchSprite(unsigned int id);

    /**
     * 设置 BunchSprite 尺寸.
     *
     * @param width     宽
     * @param height    高
     */
    void setSize(float width_, float height_);
    ~BunchSprite() override;

private:
};

NS_MAY_END
