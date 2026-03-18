/*****************************************************************//**
 * @file   mayScene.h
 * @brief  Scene 场景类封装
 * 
 * @author Majjcom
 * @date   July 2022
 *********************************************************************/
#pragma once

#include "mayGraphic.h"
#include "mayCamera.h"
#include "mayArray.h"
#include "mayNode.h"
#include "mayType.h"
#include "mayResourseManager.h"

NS_MAY_BEGIN

typedef unsigned int ShaderTypeEnum;

/**
 * 渲染吸附结构体.
 */
struct ScreenLock
{
    bool width;
    bool height;
};

class Camera;
class Node;
class SceneRQTask;


/**
 * 场景类.
 */
class MAY_DLL Scene : public Node
{
    M_GET_CLASS_NAME(Scene);

public:
    ~Scene() override;

    /**
     * 创建场景，继承此类时请重写.
     * 
     * @param size_ 场景尺寸
     * @return 场景实例指针
     */
    static Scene* create(Vec3 size_);

    /**
     * 场景初始化函数，如有需要请重写.
     * 
     * @return 是否初始化成功
     */
    bool init() override;

    /**
     * 检查是否已销毁.
     * 
     * @return 是否已销毁
     */
    bool isDestroyed() final;

    /**
     * 检查是否已激活.
     * 
     * @return 是否已激活
     */
    bool isEnabled() final;

    /**
     * 获取根节点.
     * 
     * @return 根节点指针
     */
    Scene* getRoot() override;

    /**
     * 设置主相机.
     * 
     * @param cam 主相机指针
     */
    void setMainCamera(Camera* cam);

    /**
     * 获取主相机.
     * 
     * @return 主相机指针
     */
    Camera* getMainCamera() const;

    /**
     * 获取对象类型.
     * 
     * @return 对象类型
     */
    MayType getType() override;

    /**
     * 添加场景渲染任务.
     * 
     * @param task 场景渲染队列任务
     */
    void addRendTask(SceneRQTask* task);

    /**
     * 推送场景渲染任务.
     * 
     */
    void pushTasks();

    /**
     * 设置场景尺寸.
     * 
     * @param size_
     */
    void setSize(const Vec3& size_);

    /**
     * 获取场景尺寸.
     * 
     * @return 场景尺寸
     */
    const Vec3& getSize() const;
    float getSizeY() const;
    float getSizeZ() const;
    float getSizeX() const;

    /**
     * 设置渲染吸附.
     * 
     * @param width_    设置宽吸附
     * @param height_   设置高吸附
     */
    void setLock(bool width_, bool height_);

    /**
     * 获取渲染吸附.
     * 
     * @return 渲染吸附
     */
    ScreenLock getLock() const;

    /**
     * 将子坐标装换为世界坐标.
     *
     * @param p0 子坐标
     * @param depth 递归深度 (填0即可)
     * @return   世界坐标
     */
    Vec3 transformToWorldPoint(const Vec3& p0, uint32_t depth) final;

    /**
     * 将本地旋转转换成世界旋转.
     *
     * @param r0 旋转量
     * @param depth 递归深度 (填0即可)
     * @return   世界旋转量
     */
    Vec3 transformToWorldRotation(const Vec3& r0, uint32_t depth) final;

protected:
    /**
     * 主相机.
     */
    Camera* mainCamera = nullptr;

    /**
     * 场景尺寸.
     */
    Vec3 size{};

    /**
     * 渲染任务池.
     */
    RendTasksArray rendTasks;

    /**
     * 是否吸附高度.
     */
    bool lockHeight = true;

    /**
     * 是否吸附宽度.
     */
    bool lockWidth = true;

    /**
     * 计算线程池.
     */
    void* threadPool = nullptr;

    Scene();
    void pushTasksNormal();
    void pushTasksMultiThreads();
};

NS_MAY_END
