#pragma once

#include "mayValue.h"

NS_MAY_BEGIN

template <typename T>
struct MayValueMapValue {
    enum MayValueMapValueType {
        Value,
        ValueMap,
        ValueList,
        None,
    } type { None };

    struct {
        MayValue value{ MayValueType::Null };
        T value_map{};
        mvector<MayValueMapValue> value_list;
    } val;
};

/**
 * 值映射
 */
class MAY_DLL MayValueMap
{
public:
    typedef MayValueMapValue<MayValueMap> MappingValue;

    MayValueMap();
    ~MayValueMap();
    void set_value(const mstring& key, const mstring& value);
    void set_value(const mstring& key, const char* value);
    void set_value(const mstring& key, double value);
    void set_value(const mstring& key, int value);
    void set_value(const mstring& key, int64_t value);
    void set_value(const mstring& key, bool value);
    void set_value(const mstring& key, const mvector<uint8_t>& data);
    void set_value(const mstring& key, const MayValue& value);
    void set_value(const mstring& key, const MayValueMap& value_map);
    void set_value(const mstring& key, const mvector<MappingValue>& value_list);
    void set_value(const mstring& key, const MappingValue& value);
    void set_value(const mstring& key);
    void erase_value(const mstring& key);

    const mstring& get_value_string(const mstring& key) const;
    double get_value_number_floting(const mstring& key) const;
    int64_t get_value_number_integer(const mstring& key) const;
    bool get_value_bool(const mstring& key) const;
    MayValueMap& get_value_mapping(const mstring& key);
    mvector<MappingValue>& get_value_listing(const mstring& key);
    MayValueType get_value_type(const mstring& key) const;
    bool get_value_number_is_integer(const mstring& key) const;
    const MappingValue& get_value(const mstring& key) const;
    MappingValue get_full() const;

    bool has_value(const mstring& key);
    bool is_value(const mstring& key);
    bool is_mapping(const mstring& key);
    bool is_listing(const mstring& key);

    mstring to_json_string() const;
    mstring to_pretty_json_string() const;
    static MayValueMap from_json_string(const mstring& data);

    typedef munordered_map<mstring, MappingValue> Mapping;
    Mapping v_map;

};

NS_MAY_END
