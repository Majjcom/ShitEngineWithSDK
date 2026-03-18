#pragma once

#include "../mayGraphicTask.h"

NS_GRAPHIC_BEG

class Shader;

class MAY_DLL TaskTexture final : public Task
{
public:
    TaskTexture(me_unique_ptr<RenderMesh> mesh, me_unique_ptr<RenderMeshDataLayout> layout, Shader* shader, unsigned int texture);
    ~TaskTexture() override;
    unsigned int getTexture() const;

private:
    unsigned int texture;

};

NS_GRAPHIC_END
