/*****************************************************************//**
 * @file    mayEngine.h
 * @brief   封装了引擎类
 * 
 * @version 1.3
 * @author  Majjcom
 * @date    July 2022
 *********************************************************************/
#pragma once

#include "mayResourseManager.h"
#include "mayStdExport.h"
#include "mayStlDefine.h"
#include "mayGraphic.h"
#include "mayArray.h"
#include "memory/mayMemory.h"

NS_MAY_BEGIN
    class Scene;
class Ref;

/**
 * 引擎类.
 */
class MAY_DLL Engine
{
public:
    /**
     * 获取引擎实例，一个进程只能拥有一个实例，在init后自动创建.
     * 
     * @return 
     */
    static Engine* getInstance();

    /**
     * 引擎初始化函数，只能调用一次，在此之前，请进行一些设置.
     * 
     * @param width     设置窗口宽度
     * @param height    设置窗口高速
     * @param isLimited 设置窗口是否可以改变大小
     * @param title     设置窗口标题
     */
    static void init(int width, int height, bool isLimited, const mstring& title);

    /**
     * 释放引擎资源，退出时请调用.
     * 
     */
    static void uninit();

    /**
     * 设置是否改变DPI设置.
     * 默认 true.
     * 
     * @param yesno 是否设置
     */
    static void changeAwareDPI(bool yesno);

    /**
     * 是否允许高DPI设置（须要设置changeAwareDPI为false）
     *
     * @param yesno 是否设置
     */
    static void setAllowHighDpi(bool yesno);

    /**
     * 设置加载OpenGLES驱动，默认true
     *
     * @param yesno 是否设置
     */
    static void setLoadOpenGLESDriver(bool yesno);

    /**
     * 检查是否已经初始化.
     * 
     * @return 初始化状态
     */
    static bool isInited();

    /**
     * 获取正在运行的场景.
     * 
     * @return 正在运行的场景指针
     */
    Scene* getRunningScene() const;

    /**
     * 将节点插入对象池，会进行一次完整排序.
     * 
     * @param object 待插入对象指针
     */
    void insertToPool(MayEngine::Ref* object);

    /**
     * 对对象进行快速插入，需要提前设置父节点和节点内序号.
     * 
     * @param object 待插入对象指针
     */
    void quickInsert(MayEngine::Ref* object);

    /**
     * 使用快速排序，对指定节点进行快速排序.
     * 
     * @param object 待排序对象指针
     */
    void quickReRank(MayEngine::Ref* object);

    /**
     * 设置是否忽略真实dt.
     *
     * @param yesno 设置忽略
     */
    void setIgnoreDt(bool yesno);

    /**
     * 获取是否忽略真实dt.
     *
     * @return 忽略状态
     */
    bool getIgnoreDt() const;

    /**
     * @brief 是否阻止完整排序.
     * 
     * @param yesno 设置阻塞
     */
    void setRankBlock(bool yesno);

    /**
     * @brief 设置垃圾清理周期.
     * 
     * @param rate 频率，每rate帧运行一次垃圾清理
     */
    void setGarbageCleanRate(unsigned int rate);

    /**
     * @brief 垃圾清理周期类型.
     */
    enum class GarbageCleanRateType
    {
        RateFrame = 0,
        RateTime,
    };

    /**
     * 设置垃圾清理周期类型.
     *
     * @param type 类型
     */
    void setGarbageCleanRateType(GarbageCleanRateType type);

    /**
     * 获取垃圾清理周期类型.
     * @return 类型
     */
    GarbageCleanRateType getGarbageCleanRateType() const;

    /**
     * @brief 在本循环结束立即执行垃圾回收.
     * 
     */
    void requireGC();

    /**
     * 设置是否在运行时检查无效对象.
     * 
     * @param yesno 是否检查
     */
    void setShouldCheckUselessNodes(bool);

    /**
     * 已某一场景为启动对象.
     * 
     * @param scene 场景指针
     */
    void startWithScene(Scene* scene);

    /**
     * 以某一场景进行运行.
     * 
     * @param scene 场景指针
     */
    void runWithScene(Scene* scene);

    /**
     * 节点序号改变时调用，调用后下一帧将进行一次完全排序.
     * 
     */
    void rankChanged();

    /**
     * 设置帧速度.
     * 
     * @param fps 帧速度，帧 / 秒
     */
    void setFrameRate(int fps);

    /**
     * 获取帧速度.
     * 
     * @return fps 帧速度，帧 / 秒
     */
    int getFrameRate() const;

    /**
     * 从配置文件加载渲染管线.
     * 
     * @param man           资源管理器实例
     * @param config_path   配置文件路径
     * @return              渲染管线id
     */
    int loadPipeline(ResourceManager* man, const mstring& config_path);

    /**
     * 切换到目标管线.
     * 
     * @param id    渲染管线id
     */
    void usePipeline(int id) const;

    /**
     * 获取渲染管线实例.
     * 
     * @param id    渲染管线id
     * @return      渲染管线实例
     */
    Graphic::Pipeline* getPipeline(int id) const;

    /**
     * 获取当前渲染管线实例.
     * 
     * @return      渲染管线实例
     */
    Graphic::Pipeline* getCurrentPipeline();

    /**
     * 释放渲染管线.
     * 
     * @param id    渲染管线id
     */
    void removePipeline(int id);

    /**
     * 关闭回调类型
     */
    typedef std::function<bool()> CloseRequiredCallback;

    /**
     * 设置关闭回调
     *
     * @param callback 关闭回调
     */
    void setCloseRequiredCallback(const CloseRequiredCallback& callback);

    /**
     * 加载ICU4C数据文件
     *
     * @param man  数据包
     * @param path 文件路径
     */
    void loadICUData(const ResourceManager* man, const mstring& path);

    /**
     * 获取当前对象池列表
     *
     * @return 类列表
     */
    mvector<mstring> getClassesInPool() const;

    /**
     * 退出引擎进程.
     * 
     */
    void exit() const;

    ~Engine();

private:
    RefArray uniteObjectPool;
    RefArray preObjectPool;
    RefArray quickPool;     // 快速插入队列
    RefArray quickReRankPool;   // 快速排序队列
    Scene* runningScene = nullptr;
    Scene* nextScene = nullptr;
    bool running = false;
    bool loopEnded = false;
    bool needReRank = false;
    bool shouldCheckUselessNodes = false;
    bool rankBlock = false;
    int frame = 60;
    uint64_t last_sys_time = 0;
    float dt = 0;
    unsigned int garbageCleanRate = 120;
    double lastClean = 0;
    GarbageCleanRateType garbageCleanRateType = GarbageCleanRateType::RateFrame;
    bool ignore_dt = false;
    bool required_gc = false;
    //Pipe
    int pipe_id_max = 0;
    PipeMap pipe_map;
    std::mutex preObjectPoolMutex;
    std::mutex quickPoolMutex;
    std::mutex quickReRankPoolMutex;
    me_unique_ptr<char> ICUData;
    CloseRequiredCallback closeRequiredCallback = nullptr;

    void engingLoop();
    void engineLoopContent();
    void garbageClean();
    void _exit();
};

NS_MAY_END
