#pragma once

#include "../../mayStlDefine.h"
#include "mayEngine/memory/mayMemory.h"

NS_MAY_BEGIN

class MAY_DLL RenderMesh
{
public:
    RenderMesh();

    template <size_t NV, size_t NI>
    static RenderMesh* create(const float (&vertices)[NV], const unsigned int (&indoces)[NI]);
    RenderMesh(const mvector<float>& vertices_in, const mvector<unsigned int>& indices_in);
    RenderMesh(const float* pvertices, size_t nvertices, const unsigned int* pindices, size_t nindices);

    mvector<float>& getVertices();
    mvector<unsigned int>& getIndices();

    size_t getVSize() const;
    size_t getISize() const;

private:
    mvector<float> _vertices;
    mvector<unsigned int> _indices;
};

template<size_t NV, size_t NI>
RenderMesh* RenderMesh::create(const float (&vertices)[NV], const unsigned int (&indoces)[NI])
{
    RenderMesh* ret = maymnew(RenderMesh);
    ret->_vertices.resize(NV);
    ret->_indices.resize(NI);
    memcpy(ret->_vertices.data(), vertices, NV * sizeof(float));
    memcpy(ret->_indices.data(), indoces, NI * sizeof(unsigned int));
    return ret;
}

NS_MAY_END
