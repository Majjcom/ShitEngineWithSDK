/*****************************************************************//**
 * @file   mayResourseManager.h
 * @brief  资源管理器封装
 * 
 * @author Majjcom
 * @date   July 2022
 *********************************************************************/
#pragma once

#include "mayEngine/mayStlDefine.h"
#include "mayEngine/mayStdExport.h"
#include <mutex>
#include <atomic>
#include <future>

NS_MAY_BEGIN
    class MayResourceFuture;

/**
 * 资源管理器类.
 */
class MAY_DLL ResourceManager
{
public:

    /**
     * 加载资源，输入包路径和包密钥.
     * 
     * @param packag    包路径
     * @param key       包密钥
     */
    ResourceManager(const mstring& package, int key);

    /**
     * 加载资源，输入包路径和包密钥.
     *
     * @param packag    包路径
     * @param key       包密钥
     */
    ResourceManager(const mstring& package, const mstring& key);

    /**
     * 加载资源，输入包路径、包密钥和校验公钥
     *
     * @param package    包路径
     * @param key        包密钥
     * @param pubkey     校验公钥
     */
    ResourceManager(const mstring& package, const mstring& key, const mstring& pubkey);

    /**
     * 析构函数.
     * 
     */
    ~ResourceManager();

    /**
     * 检查是否有加载错误.
     * 
     * @return 是否有加载错误
     */
    bool getLoadError() const;

    /**
     * 获取资源数据.
     * 
     * @param path  资源路径(区分大小写，并统一使用正斜杠)
     * @param data_ 资源填充地址
     * @param size_ 填充地址大小验证
     * @return      0 -> 正常, -1 -> 提供空间不足, -2 -> 资源不存在, -3 -> 校验错误
     */
    int getResource(const mstring& path, char* data_, size_t size_) const;

    /**
     * 异步获取资源数据
     *
     * @param path  资源路径
     * @param data_ 资源填充地址
     * @param size_ 填充地址大小验证
     * @return      异步资源
     */
    MayResourceFuture* getResourceFuture(const mstring& path, char* data_, size_t size_);

    /**
     * 获取资源大小.
     * 
     * @param path 资源路径(区分大小写，并统一使用正斜杠)
     * @return 资源大小  0 -> 资源不存在
     */
    size_t getResourceSize(const mstring& path) const;

    /**
     * 获取此资源包的所有资源路径
     *
     * @return 资源路径列表
     */
    mvector<mstring> getAllResourcesLocation() const;

    /**
     * 获取资源路径.
     *
     * @return 资源路径
     */
    mstring getPath() const;

private:
    void* object = nullptr;
    bool closed = false;
    bool loaded = false;
    bool auth = false;
    bool loadFailed = false;
    void* thread_pool = nullptr;
    mstring path_load;
};

/**
 * 异步资源代理
 */
class MayResourceFuture
{
public:
    explicit MayResourceFuture(void* proxy_ptr);

    /**
     * 获取资源（等待资源加载完成）
     *
     * @return 资源地址
     */
    void* await();

    /**
     * 内部API<br/>
     * 完成资源加载
     */
    void loadFinish();

    /**
     * 获取是否完成加载
     *
     * @return 是否完成加载
     */
    bool isLoadFinish() const;

    /**
     * 获取是否存在错误
     *
     * @return 是否存在错误
     */
    bool getError() const;

    /**
     * 内部API<br/>
     * 设置错误
     *
     * @param err 是否错误
     */
    void setError(bool err);

private:
    std::promise<void*> promise;
    void* ptr = nullptr;
    std::atomic_bool load_finish = false;
    bool err = false;
};

NS_MAY_END
