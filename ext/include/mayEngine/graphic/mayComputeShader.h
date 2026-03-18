#pragma once

#include "../mayGraphic.h"

NS_GRAPHIC_BEG

class MAY_DLL ComputeShader
{
public:
    explicit ComputeShader(Shader* shader);
    ~ComputeShader();

    enum class InnerDataType
    {
        // R
        R8I = 0,
        R16F,
        R16I,
        R32F,
        R32I,

        // RG
        RG8I,
        RG16F,
        RG16I,
        RG32F,
        RG32I,

        // RGB
        RGB8I,
        RGB16F,
        RGB16I,
        RGB32F,
        RGB32I,

        // RGBA
        RGBA8I,
        RGBA16F,
        RGBA16I,
        RGBA32F,
        RGBA32I,
    };

    enum class ArrayDataType
    {
        Float = 0,
        Byte,
        UnsignedByte,
        Int,
        UnsignedInt,
        Short,
        UnsignedShort,
    };

    enum class BufferAccess
    {
        ReadWrite = 0,
        ReadOnly,
        WriteOnly,
    };

    enum class MemoryBarrierBits : std::uint32_t
    {
        VERTEX_ATTRIB_ARRAY_BARRIER_BIT = 0x00000001,
        ELEMENT_ARRAY_BARRIER_BIT       = 0x00000002,
        UNIFORM_BARRIER_BIT             = 0x00000004,
        TEXTURE_FETCH_BARRIER_BIT       = 0x00000008,
        SHADER_IMAGE_ACCESS_BARRIER_BIT = 0x00000020,
        COMMAND_BARRIER_BIT             = 0x00000040,
        PIXEL_BUFFER_BARRIER_BIT        = 0x00000080,
        TEXTURE_UPDATE_BARRIER_BIT      = 0x00000100,
        BUFFER_UPDATE_BARRIER_BIT       = 0x00000200,
        FRAMEBUFFER_BARRIER_BIT         = 0x00000400,
        TRANSFORM_FEEDBACK_BARRIER_BIT  = 0x00000800,
        ATOMIC_COUNTER_BARRIER_BIT      = 0x00001000,
        ALL_BARRIER_BITS                = 0xFFFFFFFF,
    };

    typedef uint32_t BufferDataId;

    BufferDataId createBuffer(uint32_t width, uint32_t height, InnerDataType data_type) const;
    void bindBuffer(BufferDataId buffer, uint32_t binding, InnerDataType data_type, BufferAccess access) const;
    void writeBuffer(BufferDataId buffer, uint32_t width, uint32_t height, void* data,
                     ArrayDataType array_data_type, InnerDataType data_type) const;
    void readBuffer(BufferDataId buffer, uint32_t width, uint32_t height, void* data,
                    ArrayDataType array_data_type, InnerDataType data_type) const;
    void copyBuffer(BufferDataId from, BufferDataId to, int x, int y, int w, int h) const;
    void dispatchCompute(uint32_t x, uint32_t y, uint32_t z) const;
    void memoryBarrier(MemoryBarrierBits memory_barrier_bits) const;

private:
    Shader* shader_ = nullptr;
    void* private_data_ = nullptr;
};

inline ComputeShader::MemoryBarrierBits operator|(const ComputeShader::MemoryBarrierBits lhs, const ComputeShader::MemoryBarrierBits rhs)
{
    return M_SC(ComputeShader::MemoryBarrierBits, (uint32_t)lhs | (uint32_t)rhs);
}

NS_GRAPHIC_END
