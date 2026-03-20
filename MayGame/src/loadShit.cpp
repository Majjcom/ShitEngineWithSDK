#include "loadShit.h"
#include "lua.hpp"

LuaEnv::LuaReturnValue shit_engine_load_shit_code(const MayValueMap::MappingValue& value, const mvector<void*>&)
{
    if (value.type != MayValueMap::MappingValue::Value || value.val.value.type != MayValueType::String)
    {
        MayValueMap::MappingValue vl;
        vl.type = MayValueMap::MappingValue::Value;
        vl.val.value.type = MayValueType::String;
        vl.val.value.value_string = "Argument 1 is not a lua chunk.";

        MayValueMap::MappingValue v;
        v.type = MayValueMap::MappingValue::ValueList;
        v.val.value_list.emplace_back(vl);

        return { v, {} };
    }

    lua_State* L = luaL_newstate();
    if (luaL_loadstring(L, value.val.value.value_string.c_str()))
    {
        MayValueMap::MappingValue vl;
        vl.type = MayValueMap::MappingValue::Value;
        vl.val.value.type = MayValueType::String;
        vl.val.value.value_string = lua_tostring(L, -1);

        MayValueMap::MappingValue v;
        v.type = MayValueMap::MappingValue::ValueList;
        v.val.value_list.emplace_back(vl);

        lua_close(L);

        return { v, {} };
    }

    mvector<uint8_t> compiled;
    lua_dump(L, [](lua_State*, const void* p, const size_t sz, void* ud) -> int
    {
        if (sz == 0) return 0;
        mvector<uint8_t>* data = (mvector<uint8_t>*)ud;
        const size_t beg = data->size();
        data->resize(beg + sz);
        memcpy(data->data() + beg, p, sz);
        return 0;
    }, &compiled, false);

    MayValueMap::MappingValue v;
    v.type = MayValueMap::MappingValue::Value;
    v.val.value = MayValue::binary(compiled);

    lua_pop(L, -1);

    lua_close(L);

    return { v, {} };
}
