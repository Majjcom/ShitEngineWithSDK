/*****************************************************************//**
 * @file   mayText.h
 * @brief  字体渲染引擎
 * 
 * @author Majjcom
 * @date   June 2024
 *********************************************************************/
#pragma once

#include "mayResourseManager.h"
#include "mayDefine.h"
#include "mayStlDefine.h"

NS_MAY_BEGIN

class MayTextPrivate;
class SharedSprite;

/**
 * 字符信息
 */
struct MayTextData
{
    long width;
    long height;

    long size_height;
    long size_ascender;
    long size_descender;

    long horiBearingX;
    long horiBearingY;
    long horiAdvance;

    bool new_line = false;

    SharedSprite* sprite = nullptr;
};

/**
 * 字体渲染引擎
 */
class MAY_DLL MayText
{
public:
    MayText() = delete;
    MayText(MayText&) = delete;

    /**
     * 从资源包加载字体.
     * 
     * @param man       资源包
     * @param path      资源路径
     */
    MayText(ResourceManager* man, const mstring& path);
    ~MayText();

    /**
     * 加载单个字符的图片.
     * 
     * @param text      单个UTF-8字符
     * @param len       字符长度
     * @param size      字符大小
     * @return          字符图片
     */
    MayTextData load_char(const char* text, size_t len, unsigned int size) const;

    /**
     * 加载单个字符图片.
     * 
     * @param utext     单个Unicode字符
     * @param size      字符大小
     * @return          字符图片
     */
    MayTextData load_char(char32_t utext, unsigned int size) const;

    /**
     * 加载字符串的字符图片.
     * 
     * @param text      UTF-8字符串
     * @param len       字符串大小
     * @param size      字符大小
     * @return          字符图片vcetor
     */
    mvector<MayTextData> load_text(const char* text, size_t len, unsigned int size) const;

private:
    MayTextPrivate* p_data = nullptr;

};

NS_MAY_END
