#pragma once

#include "../mayGraphicTask.h"

NS_GRAPHIC_BEG

class Shader;

class MAY_DLL TaskCopy final : public Task
{
public:
    TaskCopy(int copy_method_id, int i_len, Shader* shader, unsigned int texture);
    ~TaskCopy() override;
    unsigned int getTexture() const;
    int getCopyMethod() const;
    size_t getISize() const override;

private:
    unsigned int texture;
    int copy_id;
    size_t i_size;
};

NS_GRAPHIC_END
