/*****************************************************************//**
 * @file   mayGraphic.h
 * @brief  MayEngine渲染器
 * 
 * @author Majjcom
 * @date   August 2023
 *********************************************************************/
#pragma once

#include "mayResourseManager.h"
#include "mayStlDefine.h"
#include "mayVec3.h"

#define NS_GRAPHIC_BEG namespace MayEngine::Graphic {
#define NS_GRAPHIC_END }
#define NS_GRAPHIC_US  using namespace MayEngine::Graphic;

NS_MAY_BEGIN

class BunchSprite;
class SpriteBase;

NS_MAY_END

NS_GRAPHIC_BEG

using MayEngine::mstring;
using MayEngine::munordered_map;
using MayEngine::mmap;
using MayEngine::mvector;
using MayEngine::mstack;

class Shader;
class ComputeShader;

/**
 * 渲染器类型.
 */
enum class ShaderType
{
    /// 空
    None,

    /// 形状渲染器
    Shape,

    /// 贴图渲染器
    Texture,

    /// 自定义绘制调用
    CustomCall,

    /// 复制
    Copy,

    /// 计算着色器
    Compute,
};

class PipelinePrivate;
class Task;

// TODO: MSAA

/**
 * 渲染管线类.
 */
class MAY_DLL Pipeline
{
public:
    Pipeline() = delete;
    Pipeline(Pipeline&) = delete;

    /**
     * 加载渲染管线.
     * 
     * @param man           资源管理器实例
     * @param config_path   管线配置文件路径
     */
    Pipeline(MayEngine::ResourceManager* man, const mstring& config_path);
    ~Pipeline();

    /**
     * 推送渲染任务.
     * 
     * @param task      任务实例
     * @param bunchId   分支ID
     */
    void pushTask(Task* task, int side, int bunchId) const;

    /**
     * 设置渲染屏幕锁定.
     * 
     * @param width     设置宽度锁定
     * @param hight     设置高度锁定
     */
    void setLock(bool width, bool hight) const;

    /**
     * 获取渲染屏幕锁定.
     * 
     * @param width     宽锁定
     * @param hight     高锁定
     */
    void getLock(bool* width, bool* hight) const;

    /**
     * 设置渲染视角.
     * 
     * @param width     宽度
     * @param height    高度
     */
    void setView(int width, int height) const;

    /**
     * 获取渲染视角.
     * 
     * @param width     宽度
     * @param height    高度
     */
    void getView(int* width, int* height) const;

    /**
     * 设置渲染簇渲染尺寸.
     * 
     * @param mpurl     渲染簇url
     * @param size      尺寸
     */
    void setBunchSize(const mstring& mpurl, const Vec3& size) const;

    /**
     * 设置当前管线输出尺寸.
     * 
     * @param size      尺寸
     */
    void setSize(const Vec3& size) const;

    /**
     * 设置边缘簇尺寸
     *
     * @param mpurl 边缘簇mpurl
     * @param size  尺寸
     */
    void setSideBunchSize(const mstring& mpurl, const Vec3& size) const;

    /**
     * 设置边缘管线尺寸
     *
     * @param side_id 管线id
     * @param size    尺寸
     */
    void setSideSize(unsigned int side_id, const Vec3& size) const;
    
    /**
     * 设置指定 Shader Uniform 变量.
     * 
     * @param id        Shader 注册id
     * @param argument  Uniform 变量名
     * @param data      传入数据
     */
    template<typename T>
    void setUniform(int id, const mstring& argument, T data);

    /**
     * 设置纹理绑定
     *
     * @param id     绑定位置id
     * @param sprite 纹理
     */
    static void setTextureBinding(int id, SpriteBase* sprite);

    /**
     * 设置着色器纹理绑定
     *
     * @param shader_id 着色器ID
     * @param bind_id   绑定位置ID
     * @param sprite    纹理
     */
    void setTextureBinding(int shader_id, int bind_id, SpriteBase* sprite) const;

    /**
     * 获取着色器代理
     *
     * @param shader_id 着色器ID
     * @return 着色器指针
     */
    Shader* getShaderById(int shader_id) const;

    /**
     * 获取计算着色器代理<br/>
     * 需要开发者管理其生命周期
     *
     * @param shader_id 着色器id
     * @return 着色器指针
     */
    ComputeShader* getComputeShaderById(int shader_id) const;

    /**
     * 获取边缘管线渲染结果.<br>
     * 用户需要自行释放此对象
     * 
     * @param id 边缘管线id
     * @return 
     */
    BunchSprite* getSprite(unsigned int id) const;

    /**
     * 边缘管线更新方式
     */
    enum class SidePipelineUpdateMethod
    {
        /// 总是更新
        Always = 0,

        /// 停止更新
        Sleep,

        /// 手动更新
        Manual,
    };

    /**
     * 设置边缘管线更新方式
     *
     * @param id     边缘管线ID
     * @param method 更新方式
     */
    void setSidePipelineUpdateMethod(unsigned int id, SidePipelineUpdateMethod method) const;

    /**
     * 获取边缘管线更新方式
     *
     * @param id 边缘管线ID
     * @return   更新方式
     */
    SidePipelineUpdateMethod getSidePipelineUpdateMethod(unsigned int id) const;

    /**
     * 更新边缘管线
     *
     * @param id 边缘管线ID
     */
    void updateSidePipeline(unsigned int id) const;

    /**
     * 清除边缘管线视图
     *
     * @param id 边缘管线id
     */
    void clearSidePipeline(unsigned int id) const;

    /**
     * 使用指定颜色清除边缘管线视图
     *
     * @param id 边缘管线id
     * @param color 清除颜色
     * @param alpha 清除alpha
     */
    void clearSidePipeline(unsigned int id, const Vec3& color, float alpha) const;

    /**
     * 获取默认簇ID
     *
     * @return 默认BunchId
     */
    int getDefaultBunchId() const;

    /**
     * 获取默认旁路ID
     *
     * @return 默认SideId
     */
    int getDefaultSideId() const;

    /**
     * 设置管线MSAA
     *
     * @param value MSAAxX
     */
    void setMSAA(unsigned int value) const;

    /**
     * 设置旁路管线MSAA
     *
     * @param id    旁路管线id
     * @param value MSAAxX
     */
    void setSideMSAA(unsigned int id, unsigned int value) const;

    /**
     * 渲染指令.
     */
    void rend() const;

    /**
     * 丢弃所有渲染任务.
     */
    void drop_tasks() const;

private:
    PipelinePrivate* private_data;
};

NS_GRAPHIC_END
