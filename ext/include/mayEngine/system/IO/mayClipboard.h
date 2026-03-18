#pragma once

#include "../../mayStlDefine.h"

NS_MAY_BEGIN

namespace IO
{
    /**
     * 剪贴板工具类
     */
    class MAY_DLL Clipboard
    {
    public:
        /**
         * 向剪贴板内写入文本
         *
         * @param text UTF-8文本
         */
        static void setClipboardText(const mstring& text);

        /**
         * 从剪贴板读取文本
         *
         * @return 剪贴板UTF-8文本
         */
        static mstring getClipboardText();
    };
}

NS_MAY_END
