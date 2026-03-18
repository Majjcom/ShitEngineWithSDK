/*****************************************************************//**
 * \file   mayRendComponentBase.h
 * \brief  对渲染组件的基本封装
 *
 * \author Majjcom
 * \date   July 2022
 *********************************************************************/
#pragma once

#include "../../../mayComponent.h"
#include "../../../mayDebug.h"
#include "../../../mayVec3.h"

NS_MAY_BEGIN;

class Camera;

class MAY_DLL RendPipelineConfig
{
public:
    /**
     * 设置指定Bunch id.
     *
     * @param id    bunch id
     */
    void setBunchId(int id);

    /**
     * 获取当前Bunch id.
     *
     * @return  bunch id
     */
    int getBunchId() const;

    /**
     * 设置Side id.
     *
     * @param id
     */
    void setSideId(int id);

    /**
     * 获取Side id.
     *
     * @return
     */
    int getSideId() const;

    /**
     * 管线配置类型
     */
    enum ConfigType
    {
        /// 继承配置
        CT_Inherit = 0,

        /// 自定义配置
        CT_Custom,
    };

    /**
     * 设置配置类型<br/>
     * 当配置被修改时，会自动切换至Custom配置
     *
     * @param config 配置类型
     */
    void setConfigType(ConfigType config);

    /**
     * 获取配置类型
     *
     * @return 配置类型
     */
    ConfigType getConfigType() const;

protected:
    int bunch_id = 0;
    int side_id = -1;
    ConfigType config_type = CT_Inherit;

};

class MAY_DLL RendComponentColorBase
{
public:
    /**
     * 获取颜色.
     *
     * \return
     */
    const Vec3& getColor() const;
    float getColorR() const;
    float getColorG() const;
    float getColorB() const;

    /**
     * 设置颜色.
     *
     * \param color
     */
    void setColor(const Vec3& color);
    void setColorR(float r);
    void setColorG(float g);
    void setColorB(float b);

protected:
    Vec3 color = Vec3(1, 1, 1);
};

class MAY_DLL RendComponentAlphaBase
{
public:
    /**
     * 获取透明度.
     *
     * \return 透明度 (0.0f ~ 1.0f)
     */
    float getAlpha() const;

    /**
     * 设置透明度.
     *
     * \param alpha_ 透明度 (0.0f ~ 1.0f)
     */
    void setAlpha(float alpha_);

protected:
    float alpha = 1.0f;
};

#pragma warning(disable : 6385)
#pragma warning(disable : 6386)
template<int C>
class MAY_DLL RendComponentMultiAlphaBase
{
public:
    /**
     * 获取透明度.
     *
     * @param point_    透明度点
     * @return          透明度 (0.0f ~ 1.0f)
     */
    float getAlpha(unsigned int point_) const
    {
        if (point_ >= C)
        {
            mLoge("argument point_ get invalid value %d.\n", point_);
            MAY_ASSERT(0);
        }
        return alpha[point_];
    }

    /**
     * 设置透明度.
     *
     * @param point_    透明度点
     * @param alp_      透明度 (0.0f ~ 1.0f)
     */
    void setAlpha(unsigned int point_, float alp_)
    {
        if (point_ >= C)
        {
            mLoge("argument point_ get invalid value %d.\n", point_);
            MAY_ASSERT(0);
        }
        this->alpha[point_] = alp_;
    }

    /**
     * 设置所有透明度
     *
     * @param alp_ 透明度 (0.0f ~ 1.0f)
     */
    void setAlphaAll(float alp_)
    {
        for (int i = 0; i < C; i++)
        {
            this->alpha[i] = alp_;
        }
    }

protected:
    float alpha[C] = { 1.0f };
};

template<int C>
class MAY_DLL RendComponentMultiColorBase
{
public:
    /**
     * 获取透明度.
     *
     * @param point_    颜色点
     * @return          颜色 (0.0f ~ 1.0f)
     */
    const Vec3& getColor(unsigned int point_) const
    {
        if (point_ >= C)
        {
            mLoge("argument point_ get invalid value %d.\n", point_);
            MAY_ASSERT(0);
        }
        return color[point_];
    }

    /**
     * 设置透明度.
     *
     * @param point_    颜色点
     * @param color_    颜色 (0.0f ~ 1.0f)
     */
    void setColor(unsigned int point_, const Vec3& color_)
    {
        if (point_ >= C)
        {
            mLoge("argument point_ get invalid value %d.\n", point_);
            MAY_ASSERT(0);
        }
        this->color[point_] = color_;
    }

protected:
    Vec3 color[C]{};
};

class MAY_DLL RendLineModeBase
{
public:
    /**
     * 设置是否启用线框绘制
     *
     * @param yesno 是否启用
     */
    void setLineMode(bool yesno);

    /**
     * 获取是否启用线框模式
     *
     * @return 是否启用
     */
    bool getLineMode() const;

protected:
    bool line_mode = false;

};

class MAY_DLL RendCameraBase
{
public:
    /**
     * 设置主相机
     *
     * @param cam 相机对象
     */
    void setCamera(Camera* cam);

    /**
     * 获取主相机
     *
     * @return 当前主相机
     */
    Camera* getCamera() const;

private:
    Camera* main_camera = nullptr;
};

namespace Graphic
{
    class Shader;
}

class MAY_DLL RendShaderBase
{
public:
    void setShader(Graphic::Shader* shader);
    Graphic::Shader* getShader() const;

private:
    Graphic::Shader* shader = nullptr;
};

NS_MAY_END