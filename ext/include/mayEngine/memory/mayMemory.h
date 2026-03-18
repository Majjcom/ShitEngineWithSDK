#pragma once

#include "../mayExport.h"
#include "../mayDebug.h"

#include <type_traits>

#define MAY_ATTACH_ALLOCATOR_HEADER <mayEngine/memory/mimalloc-new-delete.h>

MAY_DLL void* operator new(size_t size, const char* adetail, const char* bdetail) noexcept;
MAY_DLL void operator delete(void* ptr, const char* adetail, const char* bdetail) noexcept;

#define maymnew(C, ...) new ("", "") C(__VA_ARGS__)
#define maymdelete(p) operator delete (p, "", "")

#define maymcdelete(p, C) [](C* ptr) \
{ \
    if (!ptr) \
    { \
        mLog("Fail to cast object to `%s`.\n", #C); \
        return; \
    } \
    ptr->~C(); \
    maymdelete(ptr); \
}(dynamic_cast<C*>(p))

#define maymscdelete(p, C) [](C* ptr) \
{ \
    ptr->~C(); \
    maymdelete(ptr); \
}(static_cast<C*>(p))

template<typename T>
void maymdel(T* ptr)
{
    if (std::is_destructible_v<T>)
    {
        ptr->~T();
    }
    maymdelete(ptr);
}

NS_MAY_BEGIN

template <class T>
struct me_default_delete { // default deleter for unique_ptr
    constexpr me_default_delete() noexcept = default;

    template <class _Ty2, std::enable_if_t<std::is_convertible_v<_Ty2*, T*>, int> = 0>
    me_default_delete(const me_default_delete<_Ty2>&) noexcept {}

    void operator()(T* _Ptr) const noexcept /* strengthened */ { // delete a pointer
        static_assert(0 < sizeof(T), "can't delete an incomplete type");
        maymdel(_Ptr);
    }
};

template <typename T>
using me_unique_ptr = std::unique_ptr<T, me_default_delete<T>>;

template <typename T>
using me_shared_ptr = std::shared_ptr<T>;

template <typename T, typename... Args>
me_unique_ptr<T> me_make_unique(Args... args)
{
    return me_unique_ptr<T>(new ("", "") T(args...));
}

NS_MAY_END
