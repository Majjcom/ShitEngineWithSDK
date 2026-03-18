/*****************************************************************//**
 * \file   mayInput.h
 * \brief  对输入的封装
 * 
 * \author Majjcom
 * \date   July 2022
 *********************************************************************/
#pragma once

#include "mayEngine/mayVec3.h"
#include "mayEngine/mayStlDefine.h"

#include <functional>

NS_MAY_BEGIN

namespace IO
{
    /**
     * 触摸手指信息.
     */
    struct TouchFinger
    {
        int64_t id;
        float x;
        float y;
        float pressure;
    };

    typedef mvector<TouchFinger> TouchFingersList;

    /**
     * 键盘输入状态枚举.
     */
    enum KeyState : int16_t
    {
        RELEASE = 0,
        PRESS = 1,
        REPEAT = 2,
        CANCLE = 3,
    };

    /**
     * 键盘扫描码枚举.
     */
    enum KeyCode : int16_t
    {
        KEY_UNKNOWN = -1,

        /* Printable keys */
        KEY_SPACE = 44,
        KEY_APOSTROPHE = 52,  /* ' */
        KEY_COMMA = 54,  /* , */
        KEY_MINUS = 45,  /* - */
        KEY_PERIOD = 55,  /* . */
        KEY_SLASH = 56,  /* / */
        KEY_0 = 39,
        KEY_1 = 30,
        KEY_2 = 31,
        KEY_3 = 32,
        KEY_4 = 33,
        KEY_5 = 34,
        KEY_6 = 35,
        KEY_7 = 36,
        KEY_8 = 37,
        KEY_9 = 38,
        KEY_SEMICOLON = 51,  /* ; */
        KEY_EQUAL = 46,  /* = */
        KEY_A = 4,
        KEY_B = 5,
        KEY_C = 6,
        KEY_D = 7,
        KEY_E = 8,
        KEY_F = 9,
        KEY_G = 10,
        KEY_H = 11,
        KEY_I = 12,
        KEY_J = 13,
        KEY_K = 14,
        KEY_L = 15,
        KEY_M = 16,
        KEY_N = 17,
        KEY_O = 18,
        KEY_P = 19,
        KEY_Q = 20,
        KEY_R = 21,
        KEY_S = 22,
        KEY_T = 23,
        KEY_U = 24,
        KEY_V = 25,
        KEY_W = 26,
        KEY_X = 27,
        KEY_Y = 28,
        KEY_Z = 29,
        KEY_LEFT_BRACKET = 47,  /* [ */
        KEY_BACKSLASH = 49,  /* \ */
        KEY_RIGHT_BRACKET = 48,  /* ] */
        KEY_GRAVE_ACCENT = 96,  /* ` */
        //KEY_WORLD_1 = 161, /* non-US #1 */
        //KEY_WORLD_2 = 161, /* non-US #2 */

        /* Function keys */
        KEY_ESCAPE = 41,
        KEY_ENTER = 40,
        KEY_TAB = 43,
        KEY_BACKSPACE = 42,
        KEY_INSERT = 73,
        KEY_DELETE = 76,
        KEY_RIGHT = 79,
        KEY_LEFT = 80,
        KEY_DOWN = 81,
        KEY_UP = 82,
        KEY_PAGE_UP = 75,
        KEY_PAGE_DOWN = 78,
        KEY_HOME = 74,
        KEY_END = 77,
        KEY_CAPS_LOCK = 57,
        KEY_SCROLL_LOCK = 71,
        KEY_NUM_LOCK = 83,
        KEY_PRINT_SCREEN = 70,
        KEY_PAUSE = 72,
        KEY_F1 = 58,
        KEY_F2 = 59,
        KEY_F3 = 60,
        KEY_F4 = 61,
        KEY_F5 = 62,
        KEY_F6 = 63,
        KEY_F7 = 64,
        KEY_F8 = 65,
        KEY_F9 = 66,
        KEY_F10 = 67,
        KEY_F11 = 68,
        KEY_F12 = 69,
        /*
        KEY_F13 = 302,
        KEY_F14 = 303,
        KEY_F15 = 304,
        KEY_F16 = 305,
        KEY_F17 = 306,
        KEY_F18 = 307,
        KEY_F19 = 308,
        KEY_F20 = 309,
        KEY_F21 = 310,
        KEY_F22 = 311,
        KEY_F23 = 312,
        KEY_F24 = 313,
        KEY_F25 = 314,
        */
        KEY_KP_0 = 98,
        KEY_KP_1 = 89,
        KEY_KP_2 = 90,
        KEY_KP_3 = 91,
        KEY_KP_4 = 92,
        KEY_KP_5 = 93,
        KEY_KP_6 = 94,
        KEY_KP_7 = 95,
        KEY_KP_8 = 96,
        KEY_KP_9 = 97,
        KEY_KP_DECIMAL = 220,
        KEY_KP_DIVIDE = 84,
        KEY_KP_MULTIPLY = 85,
        KEY_KP_SUBTRACT = 212,
        KEY_KP_ADD = 211,
        KEY_KP_ENTER = 88,
        KEY_KP_EQUAL = 103,
        KEY_LEFT_SHIFT = 225,
        KEY_LEFT_CONTROL = 224,
        KEY_LEFT_ALT = 226,
        KEY_LEFT_SUPER = 227,
        KEY_RIGHT_SHIFT = 229,
        KEY_RIGHT_CONTROL = 228,
        KEY_RIGHT_ALT = 230,
        KEY_RIGHT_SUPER = 231,
        KEY_MENU = 118,
    };

    /**
     * 鼠标按键枚举.
     */
    enum MouseButton : int16_t
    {
        BUTTON_1      = 0,
        BUTTON_2      = 1,
        BUTTON_3      = 2,
        BUTTON_4      = 3,
        BUTTON_5      = 4,
        BUTTON_6      = 5,
        BUTTON_7      = 6,
        BUTTON_8      = 7,
        BUTTON_LAST   = BUTTON_8,
        BUTTON_LEFT   = BUTTON_1,
        BUTTON_RIGHT  = BUTTON_2,
        BUTTON_MIDDLE = BUTTON_3,
    };

    /**
     * 鼠标按钮回调.
     */
    typedef std::function<void(int button, int action, int mods)> Mousebuttonfun;

    /**
     * 指针移动回调.
     */
    typedef std::function<void(double xpos, double ypos)> Cursorposfun;

    /**
     * 鼠标滚轮回调函数.
     */
    typedef std::function<void(int direction, float x, float y)> MouseWhellCallbackFunc;


    /**
     * 输入管理器类.
     */
    class MAY_DLL InputManager
    {
    public:
        /**
          * 初始化函数，该初始化会在引擎初始化时自动完成.
          *
          */
        static void init();

        /**
         * 结束使用，释放内存，逆初始化函数.
         * 
         */
        static void uninit();

        /**
         * 获取输入管理器实例.
         * 
         * \return 输入管理器实例
         */
        static InputManager* getInstance();

        /**
         * 检查指定按键的状态.
         * 
         * \param key   指定按键的扫描码
         * \return      指定按键的状态
         */
        KeyState checkKeyState(KeyCode key) const;

        /**
         * 获取指针位置.
         * 
         * @return 指针位置
         */
        Vec3 getCursorPos() const;

        /**
         * 检查指定鼠标按键状态.
         * 
         * @param button_   按钮码
         * @return          按键状态
         */
        KeyState checkMouseButton(MouseButton button_) const;

        /**
         * 设置鼠标按钮回调.
         * 
         * @param callback_ 回调函数
         */
        void setMouseButtonCallBack(const Mousebuttonfun& callback_);

        /**
         * 获取鼠标按钮回调.
         * 
         * @return 
         */
        const Mousebuttonfun& getMouseButtonCallBack() const;

        /**
         * 设置指针位置回调.
         * 
         * @param callback_ 回调函数
         */
        void setCursorPosCallBack(const Cursorposfun& callback_);

        /**
         * 获取指针位置回调.
         * 
         * @return 
         */
        const Cursorposfun& getCursorPosCallBack() const;

        /**
         * 设置鼠标滚轮回调.
         *
         * @param func 回调函数
         */
        void setMouseWhellCallback(const MouseWhellCallbackFunc& func);

        /**
         * 获取鼠标滚轮回调.
         *
         * @return
         */
        const MouseWhellCallbackFunc& getMouseWhellCakkback() const;

        /**
         * 获取当前触摸状态.
         * 
         * @return 所有手指信息
         */
        TouchFingersList getTouchFingers() const;

    private:
        Mousebuttonfun mouseButtonCallback = nullptr;
        Cursorposfun curdorPosCallback = nullptr;
        MouseWhellCallbackFunc mouseWhellCallback = nullptr;
        int64_t touch_device = -1;
        InputManager();
    };
}

NS_MAY_END
