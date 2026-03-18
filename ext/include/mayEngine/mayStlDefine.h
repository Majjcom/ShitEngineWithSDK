#pragma once

#include "mayDefine.h"
#include "memory/mimalloc.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <sstream>

// #include <robin_hood.h>

NS_MAY_BEGIN

using mstring = std::basic_string<char, std::char_traits<char>, mi_stl_allocator<char>>;
using mu16string = std::basic_string<char16_t, std::char_traits<char16_t>, mi_stl_allocator<char16_t>>;
using mu32string = std::basic_string<char32_t, std::char_traits<char32_t>, mi_stl_allocator<char32_t>>;

template<typename T>
using mvector = std::vector<T, mi_stl_allocator<T>>;

template<typename Kty, typename Vty>
using mmap = std::map<Kty, Vty, std::less<Kty>, mi_stl_allocator<std::pair<const Kty, Vty>>>;

template<typename Kty, typename Vty>
using munordered_map = std::unordered_map<Kty, Vty, std::hash<Kty>, std::equal_to<Kty>, mi_stl_allocator<std::pair<const Kty, Vty>>>;

// Can't Use RobinHood mapping
//template<typename Kty, typename Vty>
//using munordered_map = robin_hood::unordered_map<Kty, Vty, robin_hood::hash<Kty>, std::equal_to<Kty>, 80>;

template<typename Kty>
using mset = std::set<Kty, std::less<Kty>, mi_stl_allocator<Kty>>;

template<typename Kty>
using munordered_set = std::unordered_set<Kty, std::hash<Kty>, std::equal_to<Kty>, mi_stl_allocator<Kty>>;

template<typename Ty>
using mlist = std::list<Ty, mi_stl_allocator<Ty>>;

template<typename T>
using mdeque = std::deque<T, mi_stl_allocator<T>>;

template<typename T>
using mqueue = std::queue<T, mdeque<T>>;

template<typename T>
using mstack = std::stack<T, mdeque<T>>;

using mstringstream = std::basic_stringstream<char, std::char_traits<char>, mi_stl_allocator<char>>;

using mstringbuf = std::basic_stringbuf<char, std::char_traits<char>, mi_stl_allocator<char>>;

using mstrview = std::basic_string_view<char>;

NS_MAY_END

#if !defined(_WIN32) && defined(__clang__) && !defined(__ANDROID__)
namespace std {
    template<>
    struct hash<MayEngine::mstring> {
        size_t operator()(const MayEngine::mstring& s) const {
            return hash<string_view>{}(string_view(s.data(), s.size()));
        }
    };
}
#endif
