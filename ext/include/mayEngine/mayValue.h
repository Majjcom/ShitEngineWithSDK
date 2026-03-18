#pragma once

#include "mayStlDefine.h"
#include <memory>

NS_MAY_BEGIN

/**
 * 值类型
 */
enum class MayValueType
{
    /// 空
    Null = 0,

    /// 字符串
    String,

    /// 数字
    Number,

    /// 布尔
    Bool,
};

/**
 * 值类
 */
class MAY_DLL MayValue
{
public:
    ~MayValue();

    /**
     * 类型
     */
    MayValueType type{ MayValueType::Null };

    /**
     * 字符串值
     */
    mstring value_string{};

    /**
     * 数字值
     */
    union
    {
        /// 双精度浮点数值
        double value_number_floating;

        /// 64位整数值
        int64_t value_number_integer;
    } value_number;

    /// 数字是否为整数
    bool value_number_is_integer{ true };

    /**
     * 布尔值
     */
    bool value_bool{ false };

    static MayValue null();
    static MayValue number(double num);
    static MayValue number(int num);
    static MayValue number(int64_t num);
    static MayValue boolean(bool boolean);
    static MayValue string(const mstring& str);
};

NS_MAY_END
