#pragma once

#include "../mayGraphicTask.h"

NS_GRAPHIC_BEG

class MAY_DLL TaskShape final : public Task
{
public:
    TaskShape(me_unique_ptr<RenderMesh> mesh, me_unique_ptr<RenderMeshDataLayout> layout, Shader* shader);
    virtual ~TaskShape();

};

NS_GRAPHIC_END
