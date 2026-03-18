#pragma once

#include <functional>

#include "../../mayValueMap.h"
#include "../../mayDefine.h"
#include "../../mayRef.h"

namespace MayEngine
{
    class ResourceManager;
}

NS_MAY_BEGIN

/**
 * Lua环境类
 */
class MAY_DLL LuaEnv final
{
    M_GET_CLASS_NAME_N(LuaEnv);

public:
    LuaEnv(LuaEnv&) = delete;
    LuaEnv(LuaEnv&&) = delete;
    virtual ~LuaEnv();

    /**
     * 初始化Lua环境
     * @param debugger 是否启用调试器
     */
    static void init(bool debugger = false);

    /**
     * 重启Lua环境（需要先释放所有Lua组件）
     *
     * @param debugger 是否启用调试器
     */
    static void restart(bool debugger = false);

    /**
     * 释放Lua环境
     */
    static void uninit();

    /**
     * 获取Lua环境单例
     *
     * @return Lua环境实例
     */
    static LuaEnv* getInstance();

    /**
     * 运行脚本
     *
     * @param mpurl 脚本mpurl
     */
    void runScript(const mstring& mpurl) const;

    /**
     * 执行lua gc
     */
    void gc() const;

    /**
     * 将某个包注册到Lua环境中，供其使用<br>
     * 需要注意，要确保注册的包在使用期间不被释放
     *
     * @param man 注册MayPack
     */
    void registerPackage(ResourceManager* man) const;

    /**
     * Lua注册API返回值类型
     */
    typedef std::tuple<MayValueMap::MappingValue, mvector<void*>> LuaReturnValue;

    /**
     * Lua注册API函数
     */
    typedef std::function<LuaReturnValue(const MayValueMap::MappingValue&, const mvector<void*>&)> LuaRegisterFunc;

    /**
     * 注册C函数<br>
     * 注册的函数可以通过 me.cfunctions.xxx() 进行调用
     *
     * @param name 函数名称
     * @param func 函数地址
     */
    void registerFunction(const mstring& name, LuaRegisterFunc func) const;

    /**
     * 内部API
     * @return
     */
    void* get() const;

private:
    explicit LuaEnv(bool debugger = false);
    void* p_data = nullptr;
};

NS_MAY_END
