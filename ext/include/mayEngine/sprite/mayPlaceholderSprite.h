#pragma once

#include "maySpriteBase.h"

NS_MAY_BEGIN

class MAY_DLL PlaceholderSprite : public SpriteBase
{
public:
    PlaceholderSprite() = default;
    ~PlaceholderSprite() override = default;
    void setSize(float width, float height);
};

NS_MAY_END
