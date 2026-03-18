#pragma once

#include "../mayGraphic.h"
#include "mesh/mayRenderMesh.h"
#include "mesh/mayRenderMeshDataLayout.h"
#include "../memory/mayMemory.h"

NS_GRAPHIC_BEG

enum class RenderType
{
    /// 普通渲染
    RDNormal = 0,

    /// 线框渲染
    RDLine,
};

class Shader;

class MAY_DLL Task
{
public:
    Task() = delete;
    Task(Task&) = delete;
    Task(Task&&) = delete;
    Task(me_unique_ptr<RenderMesh> mesh, me_unique_ptr<RenderMeshDataLayout> layout, Shader* shader);
    explicit Task(Shader* shader);
    virtual ~Task();

    RenderMesh& getMesh() const;
    RenderMeshDataLayout& getLayout() const;

    me_unique_ptr<RenderMesh> moveMesh();
    me_unique_ptr<RenderMeshDataLayout> moveLayout();

    virtual size_t getVSize() const;
    virtual size_t getISize() const;

    ShaderType getType() const;
    Shader* getShader() const;
    void setBunchId(int id);
    int getBunchId() const;
    void setSideId(int side);
    int getSideId() const;
    void setBlend(bool b);
    bool getBlend() const;
    void setRenderType(RenderType type);
    RenderType getRenderType() const;

protected:
    ShaderType task_type;
    me_unique_ptr<RenderMesh> _mesh;
    me_unique_ptr<RenderMeshDataLayout> _layout;

private:

    int bunch_id = -1;
    int side_id = -1;
    RenderType render_type = RenderType::RDNormal;
    Shader* shader = nullptr;
    bool blend = false;
};

NS_GRAPHIC_END
