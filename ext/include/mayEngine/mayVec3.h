/*****************************************************************//**
 * @file   mayVec3.h
 * @brief  Vec3 三维向量类型定义
 * 
 * @author Majjcom
 * @date   July 2022
 *********************************************************************/
#pragma once

#include "mayDefine.h"
#include <cmath>

NS_MAY_BEGIN

/**
 * Vec3 三维向量类.
 */
class MAY_DLL Vec3
{
public:
    Vec3();
    Vec3(const Vec3& other);
    Vec3(float x, float y, float z);
    Vec3(float x, float y);
    Vec3(int x, int y, int z);
    Vec3(int x, int y);
    explicit Vec3(float x);
    explicit Vec3(int x);
    float getx() const;
    float gety() const;
    float getz() const;
    void set(float x, float y, float z);
    void set(const Vec3& v);
    void setx(float n);
    void sety(float n);
    void setz(float n);

    /**
     * 根据三个坐标(左手坐标系)旋转.
     * 
     * @param angles x y z 旋转角
     * @return 旋转后的向量
     */
    Vec3 rotate_to(const Vec3& angles) const;

    /**
     * 根据旋转、移动、缩放对向量进行变换.
     * 
     * @param angles    旋转角
     * @param move      移动向量
     * @param scale     缩放向量
     * @return          变换后的向量
     */
    Vec3 transFrom(const Vec3& angles, const Vec3& move, const Vec3& scale) const;

    /**
     * 获取向量的模.
     * 
     * @return 向量的模
     */
    float mod() const;

    /**
     * 获取该向量的相反向量.
     * 
     * @return 相反向量
     */
    Vec3 minus() const;

    /**
     * 获取标准化向量.
     * 
     * @return  向量
     */
    Vec3 normalized() const;

    /**
     * 获取向量长度.
     * 
     * @return  长度
     */
    float length() const;

    /**
     * 获取两个向量的夹角大小.
     * 
     * @param from  起始向量
     * @param to    结束向量
     * @return      向量夹角(角度)
     */
    static float getAngle(Vec3 from, Vec3 to);

    /**
     * 获取两个点连成的线（2D）在原始方向上的夹角.
     * 
     * @param from  线开始
     * @param to    线结束
     * @return      角度
     */
    static float getLineAngle(Vec3 from, Vec3 to);

    /**
     * 将弧度转换为角度
     *
     * @param val 弧度值
     * @return 角度值
     */
    static float angleToDegree(float val);

    /**
     * 将角度值转换为弧度
     *
     * @param val 角度值
     * @return 弧度值
     */
    static float degreeToAngle(float val);

    Vec3 operator* (const float& n) const;
    float operator* (const Vec3& v) const;
    MAY_DLL friend Vec3 operator* (const float& n, const Vec3& v);
    MAY_DLL friend Vec3 operator- (const Vec3& v);
    Vec3 operator+ (const Vec3& v) const;
    Vec3 operator- (const Vec3& v) const;
    void operator+= (const Vec3& v);
    void operator-= (const Vec3& v);
    bool operator== (const Vec3& v) const;
    bool operator!= (const Vec3& v) const;
    void operator*= (const float& v);

private:
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
};

NS_MAY_END
