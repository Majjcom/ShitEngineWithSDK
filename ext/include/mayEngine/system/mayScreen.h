/*****************************************************************//**
 * \file   mayScreen.h
 * \brief  对显示的部分封装
 * 
 * \author Majjcom
 * \date   July 2022
 *********************************************************************/
#pragma once

#include "../mayStlDefine.h"
#include "mayEngine/mayVec3.h"

#include <functional>

#ifdef _WIN32
#include <Windows.h>
#endif

NS_MAY_BEGIN

/**
 * 显示管理器类.
 */
class MAY_DLL ScreenManager
{
public:

    /**
     * 初始化函数，该初始化会在引擎初始化时自动完成.
     */
    static void init();

    /**
     * 使用结束，释放资源，逆初始化函数.
     */
    static void uninit();

    /**
     * 获取显示管理器实例.
     * 
     * @return 显示管理器实例
     */
    static ScreenManager* getInstance();

    /**
     * 检查引擎主窗口是否被聚焦.
     * 
     * @return 引擎主窗口是否被聚焦
     */
    bool isWindowFocused() const;

    /**
     * 检查光标是否在显示器内.
     * 
     * @return 光标是否在显示器内
     */
    bool isCursorInWindow() const;

    /**
     * 窗口改变大小回调函数定义
     */
    typedef std::function<void(void* window, int wigth, int height)> ResizeCallbackFunc;

    /**
     * 设置窗口大小改变回调.
     * 
     * @param func 窗口大小改变回调函数
     */
    void setResizeCallback(const ResizeCallbackFunc& func) const;

    /**
     * 光标出入窗口回调函数定义
     */
    typedef std::function<void(void* window, int entered)> CursorEnterCallbackFunc;

    /**
     * 设置光标出入主窗口回调.
     * 
     * @param func 光标出入主窗口回调函数
     */
    void setCursorEnterCallback(const CursorEnterCallbackFunc& func) const;

    /**
     * 主窗口聚焦回调函数定义
     */
    typedef std::function<void(void* window, int focused)> WindowFocusCallbackFunc;

    /**
     * 设置主窗口聚焦回调.
     * 
     * @param func 主窗口聚焦回调函数
     */
    void setWindowFocusCallback(const WindowFocusCallbackFunc& func) const;


    /**
     * 设置窗口是否可透明.
     * 
     * @param yesno 窗口是否可透明
     */
    void setTransparency(bool yesno) const;

    /**
     * 设置窗口透明度.
     * 
     * @param to 窗口透明度
     */
    void setWindowAlpha(float to) const;

    /**
     * 设置视口尺寸.
     * 
     * @param width     视口宽度
     * @param height    视口高度
     */
    void setViewSize(int width, int height) const;

    /**
     * 设置窗口背景颜色 (0.0f ~ 1.0f).
     * 
     * @param r 红
     * @param g 绿
     * @param b 蓝
     * @param a 透明
     */
    void setBackColor(float r, float g, float b, float a) const;

    /**
     * 获取窗口宽度.
     * 
     * @return 窗口宽度
     */
    int getScreenWidth() const;

    /**
     * 获取窗口高度.
     *
     * @return 窗口高度
     */
    int getScreenHeight() const;

    /**
     * 获取渲染区域宽度
     *
     * @return 宽度
     */
    int getRenderWidth() const;

    /**
     * 获取渲染区域高度
     *
     * @return 高度
     */
    int getRenderHeight() const;

    /**
     * 获取主显示器信息.
     * 
     * @param width         宽度
     * @param height        高度
     * @param refreshRate   刷新率
     */
    void getMonitorInfo(int* width, int* height, int* refreshRate) const;
    
    /**
     * 设置窗口边框显示.
     * 
     * @param yesno 是否显示边框
     */
    void setDecorated(bool yesno) const;

    /**
     * 设置窗口最大化.
     */
    void setMaximized() const;

    /**
     * 设置全屏.
     */
    void setFullScreen() const;

    /**
     * 设置窗口窗口化.
     */
    void setWindowed() const;

    /**
     * 设置窗口顶置.
     * 
     * @param yesno 是否顶置
     */
    void setFloating(bool yesno) const;

    /**
     * 获取窗口位置.
     * 
     * @return Vec3(x, y) 窗口位置
     */
    Vec3 getWindowPos() const;

    /**
     * 设置窗口位置.
     * 
     * @param pos_ 窗口位置
     */
    void setWindowPos(const Vec3& pos_) const;

    /**
     * 设置窗口尺寸.
     * 
     * @param size_ 尺寸
     */
    void setWindowSize(const Vec3& size_) const;

    /**
     * 设置贴图过滤方式 (已弃用).
     * 
     * @param yesno 是否使用线性过滤
     */
    void setTextureFilterNearest(bool yesno) const;

    /**
     * 设置窗口标题.
     * 
     * @param title_
     */
    void setWindowTitle(const mstring& title_) const;

    /**
     * 设置窗口大小是否被限制.
     * 
     * @param yesno
     */
    void setWindowLimited(bool yesno) const;

    /**
     * 窗口状态
     */
    enum class WindowState : int8_t
    {
        /// 最小化
        Minimized = 0,

        /// 最大化
        Maximized,

        /// 普通状态
        Normal,
    };

    /**
     * 获取窗口状态.
     *
     * @return 窗口状态
     */
    WindowState getWindowState() const;

    /**
     * 窗口状态改变回调函数定义.
     */
    typedef std::function<void(void*, WindowState)> WindowStateChangeCallbackFunc;

    /**
     * 设置窗口状态改变回调函数.
     *
     * @param func 回调函数
     */
    void setWindowStateChangeCallback(const WindowStateChangeCallbackFunc& func);

    /**
     * 获取显示器尺寸
     *
     * @param display 显示器ID
     * @param width   宽度
     * @param height  高度
     */
    void getDesktopRect(int display, int* width, int* height) const;

    /**
     * 设置是否隐藏任务栏图标.
     * 
     * @param yesno
     */
    static void setSkipTaskbar(bool yesno);

    /**
     * 这是是否为工具窗口.
     *
     * @param yesno 是否为工具窗口
     */
    static void setUtilityWindow(bool yesno);

    /**
     * 屏幕方向.
     */
    enum Orientations
    {
        LandscapeLeft = (1 << 0),
        LandscapeRight = (1 << 1),
        Portrait = (1 << 2),
        PortraitUpsideDown = (1 << 3),
    };

    /**
     * 设置屏幕方向.
     * 
     * @param oritataions 屏幕方向
     */
    static void setOrientations(int oritataions);

    /**
     * 获取显示DPI
     *
     * @param display 显示ID
     * @return        显示DPI
     */
    static float getDisplayDpi(int display);

    /**
     * 消息弹窗类型
     */
    enum MessageBoxType
    {
        /// 错误
        MBT_ERROR = 0,

        /// 警告
        MBT_WARNING,

        /// 信息
        MBT_INFO,
    };

    /**
     * 显示消息弹窗
     *
     * @param type    弹窗类型
     * @param title   弹窗标题
     * @param message 弹窗显示信息
     */
    void showMessageBox(MessageBoxType type, const mstring& title, const mstring& message) const;

    /**
     * 显示询问弹窗
     *
     * @param title            弹窗标题
     * @param message          弹窗显示信息
     * @param acceptButtonText 接受按钮文字
     * @param rejectButtonText 拒绝按钮文字
     * @return                 是否接受
     */
    bool showAskMessageBox(
        const char* title, const char* message,
        const char* acceptButtonText = "Accept", const char* rejectButtonText = "Reject") const;

#ifdef _WIN32
    /**
     * 获取windows窗口句柄.
     * 
     * @return HWND
     */
    HWND getWin32Window() const;

    /**
     * 设置是否启用分层窗口.
     * 
     * @param yesno
     */
    void setWindowLayered(bool yesno) const;

    /**
     * 设置窗口是否为浮动工具栏.
     * 
     * @param yesno
     */
    void setToolWindow(bool yesno) const;

    /**
     * 设置窗口透明颜色.
     * 
     * @param color
     */
    void setLayeredWindowColorkey(const Vec3& color) const;

    /**
     * 设置窗口透明度及颜色
     *
     * @param color 颜色
     * @param alpha 透明度
     */
    void setLayeredWindowColorkey(const Vec3& color, float alpha) const;
#endif
private:
    WindowStateChangeCallbackFunc windowStateChangeCallbackFunc = nullptr;
    ScreenManager();
};

NS_MAY_END
