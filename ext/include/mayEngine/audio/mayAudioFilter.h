/*****************************************************************//**
 * @file   mayAudioFilter.h
 * @brief  音频过滤器组件
 * 
 * @author Majjcom
 * @date   August 2023
 *********************************************************************/
#pragma once

#include "../mayDefine.h"

NS_MAY_BEGIN

namespace AudioEngine
{
    /**
     * 过滤器类.
     */
    class MAY_DLL Filter {
    public:
        /**
         * 过滤器类型.
         */
        enum FilterType
        {
            /** 不使用过滤器 */
            Null = 0x0000,

            /** 高频通过过滤器 */
            HighPass = 0x0001,

            /** 低频通过过滤器 */
            LowPass = 0x0002,

            /** 通带过滤器 */
            BandPass = 0x0003,
        };

        Filter();
        ~Filter();

        /**
         * 获取过滤器类型.
         * 
         * @return  过滤器类型
         */
        FilterType getType() const;

        /**
         * 设置过滤器类型.
         * 
         * @param t 过滤器类型
         */
        void setType(FilterType t) const;

        /**
         * 设置过滤器强度.
         * 
         * @param g 强度
         */
        void setGain(float g) const;

        /**
         * 设置过滤器低音通过强度.
         * 
         * @param g 强度
         */
        void setLF(float g) const;

        /**
         * 设置过滤器高音通过强度.
         * 
         * @param g 强度
         */
        void setHF(float g) const;

        /**
         * 获取过滤器实例.
         * 
         * @return 
         */
        void* getIns() const;

    private:
        void* ins;
    };
}

NS_MAY_END
