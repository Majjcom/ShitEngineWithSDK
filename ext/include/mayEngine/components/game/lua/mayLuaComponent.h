#pragma once

#include <tuple>
#include <functional>

#include "mayEngine/mayComponent.h"

#define M_LUA_COMP_REG_FUNC(N) \
    [this](const MayEngine::MayValueMap::MappingValue& v, const MayEngine::mvector<void*>& r){ return this->N(v, r); }

NS_MAY_BEGIN

class MAY_DLL LuaComponent : public Component
{
    M_GET_CLASS_NAME(LuaComponent);

public:
    /**
     * 从脚本创建Lua代理组件
     *
     * @param mpurl lua脚本mpurl
     * @param node  父节点
     * @return      组件
     */
    static LuaComponent* create(const mstring& mpurl, Node* node);

    /**
     * 内部API<br/>
     * 从Inner Proxy Id创建代理组件
     *
     * @param id   Lua Inner Proxy Id
     * @param node 父节点
     * @return     组件
     */
    static LuaComponent* create(int64_t id, Node* node);
    ~LuaComponent() override;

    /**
     * 初始化
     *
     * @return 是否初始化成功
     */
    bool init() override;

    /**
     * 内部API<br/>
     * 代理调用onEnter
     */
    void onEnter() override;

    /**
     * 内部API<br/>
     * 代理调用update(dt)
     *
     * @param dt delta
     */
    void main_update(float dt) final;

    /**
     * 内部API<br/>
     * 代理调用onDestroy
     */
    void onDestroy() override;

    /**
     * 内部API<br/>
     * 代理调用onEvent
     *
     * @param event
     * @param value
     * @param refs
     */
    void onEvent(const mstring& event, const MayValueMap::MappingValue& value, const mvector<void*>& refs) override;

    /**
     * Lua函数返回值类型
     */
    typedef std::tuple<MayValueMap::MappingValue, mvector<void*>> LuaReturnValue;

    /**
     * 调用Lua table class中的函数
     *
     * @param func_name 函数名
     * @param value     传入参数
     * @param refs      传入对象
     * @return          Lua返回值
     */
    LuaReturnValue call(
        const mstring& func_name, const MayValueMap:: MappingValue& value, const mvector<void*>& refs) const;

    /**
     * 调用无返回值的Lua table class中的函数
     *
     * @param func_name 函数名
     * @param value     传入参数
     * @param refs      传入对象
     */
    void callNoRet(const mstring& func_name, const MayValueMap:: MappingValue& value, const mvector<void*>& refs) const;

    /**
     * Lua注册C++函数类型
     */
    typedef std::function<LuaReturnValue(const MayValueMap::MappingValue&, const mvector<void*>&)> RegisterMethodDef;

    /**
     * 注册C++对象方法
     *
     * @param name 注册函数名称
     * @param method 函数地址
     */
    void registerMethod(const mstring& name, RegisterMethodDef method);

    /**
     * 内部API<br/>
     * Lua调用C++对象方法
     *
     * @param name 函数名
     * @param value 传入参数
     * @param refs 传入对象
     * @return Lua返回值
     */
    LuaReturnValue callRegistered(
        const mstring& name, const MayValueMap::MappingValue& value, const mvector<void*>& refs);

    /**
     * 内部API<br/>
     * 获取Inner Proxy Id
     *
     * @return Inner Proxy Id
     */
    int64_t getTableIndex() const;

private:
    LuaComponent();
    void* init_data = nullptr;
    void* p_data;

};

NS_MAY_END
