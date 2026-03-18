#pragma once

#include "mayStlDefine.h"
#include "mayVec3.h"

NS_MAY_BEGIN

class Scene;
class Node;

/**
 * 将全局坐标(窗口)转换为场景坐标(中心为[0, 0])
 *
 * @param pos 全局坐标
 * @param scene 场景
 * @return 转换后的场景坐标
 */
MAY_DLL Vec3 convertPos(const Vec3& pos, const Scene* scene);

/**
 * 将场景坐标转换为全局坐标
 *
 * @param pos   场景坐标
 * @param scene 场景
 * @return 全局坐标[窗口]
 */
MAY_DLL Vec3 convertSceneToGlobal(const Vec3& pos, const Scene* scene);

/**
 * 按照比例计算铺开场景的真实大小
 *
 * @param primary 场景大小
 * @param target 真实窗口大小
 * @return 扩展后的真实场景大小
 */
MAY_DLL Vec3 calculateFixedSize(const Vec3& primary, const Vec3& target);

/**
 * 获取真实本地文件路径
 *
 * @param path 文件路径
 * @return 本地路径
 */
MAY_DLL mstring getLocalFilePath(const mstring& path);

/**
 * 判断两个浮点数是否相等
 *
 * @param a
 * @param b
 * @param absEps 绝对误差
 * @param relEps 相对误差
 * @return
 */
inline bool nearlyEqual(const double a, const double b, const double absEps = 1e-12, const double relEps = 1e-8)
{
    const double diff = std::abs(a - b);
    if (diff <= absEps) return true;
    return diff <= (std::abs(a) + std::abs(b)) * relEps;
}

inline Vec3 moveWithAnchor(const Vec3& p, const Vec3& anc, const Vec3& size)
{
    return p - Vec3(anc.getx() * size.getx(), anc.gety() * size.gety(), 0.0f);
}

inline bool checkPosInRect(const Vec3& pos, const Vec3& lt, const Vec3& rb)
{
    const float cx = pos.getx();
    const float cy = pos.gety();
    const float ltx = lt.getx();
    const float lty = lt.gety();
    const float rbx = rb.getx();
    const float rby = rb.gety();
    if (cx >= ltx && cx <= rbx &&
        cy >= rby && cy <= lty)
    {
        return true;
    }
    return false;
}

inline Vec3 byteColor(const Vec3& color)
{
    return { color.getx() / 255.0f, color.gety() / 255.0f, color.getz() / 255.0f };
}

NS_MAY_END
