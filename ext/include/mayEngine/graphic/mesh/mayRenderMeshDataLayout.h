#pragma once

#include "../../mayStlDefine.h"

NS_MAY_BEGIN

class MAY_DLL RenderMeshDataLayout
{
public:
    enum class LayoutType
    {
        Byte = 0,
        UnsignedByte,
        Short,
        UnsignedShort,
        Int,
        UnsignedInt,
        Float,
    };

    inline static size_t getLayoutTypeSize(LayoutType layout);

    struct LayoutCell
    {
        LayoutType type;
        size_t count;
    };

    RenderMeshDataLayout();
    // RenderMeshDataLayout(RenderMeshDataLayout& other);
    explicit RenderMeshDataLayout(const mvector<LayoutCell>& layout);

    mvector<LayoutCell>& getLayout();

    size_t getLayoutTotalByteSize() const;

    size_t getLayoutTotalSize() const;

    static RenderMeshDataLayout getDefaultTextureLayout();
    static RenderMeshDataLayout getDefaulShapeLayout();

private:
    mvector<LayoutCell> _layout;

};

inline size_t RenderMeshDataLayout::getLayoutTypeSize(const LayoutType layout)
{
    switch (layout)
    {
    case LayoutType::Byte:
    case LayoutType::UnsignedByte:
        return 1;
    case LayoutType::Short:
    case LayoutType::UnsignedShort:
        return 2;
    case LayoutType::Int:
    case LayoutType::UnsignedInt:
    case LayoutType::Float:
        return 4;
    default:
        return 0;
    }
}

NS_MAY_END
