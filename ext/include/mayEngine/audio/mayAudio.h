/*****************************************************************//**
 * @file   mayAudio.h
 * @brief  MayEngine音频组件
 * 
 * @author Majjcom
 * @date   July 2022
 *********************************************************************/
#pragma once

#include "../mayStlDefine.h"
#include "../mayResourseManager.h"

NS_MAY_BEGIN

namespace AudioEngine
{
    typedef unsigned int SoundId;
    typedef unsigned int PlayId;

    class Filter;

    MAY_DLL void release();

    class MAY_DLL AudioClip
    {
    public:
        /**
         * 传入音频路径将加载对应音频.
         *
         * @param snd_path  ogg音频文件路径
         */
        explicit AudioClip(const mstring& snd_path);

        /**
         * 从内存加载音频，在完成加载后请释放原数据以节约内存.
         *
         * @param mem       音频二进制数据
         * @param length    音频二进制数据长度
         */
        AudioClip(const char* mem, size_t length);

        /**
         * 从包加载音频.
         * 
         * @param pack_path 包路径
         * @param res_path  资源路径
         * @param key_      包密钥
         */
        AudioClip(const mstring& pack_path, const mstring& res_path, const mstring& key_);

        /**
         * 从包加载音频.
         * 
         * @param man_      包管理对象
         * @param res_path  资源路径
         * @param key_      包密钥
         */
        AudioClip(ResourceManager* man_, const mstring& res_path);

        /**
         * 析构函数.
         * 
         */
        ~AudioClip();

        /**
         * 登记使用.
         * 
         */
        void use();

        /**
         * 取消登记.
         * 
         */
        void unuse();

        /**
         * 获取音频资源id.
         * 
         * @return 音频资源id
         */
        SoundId getSoundId() const;

        /**
         * 释放资源.
         * 
         */
        void release();

    private:
        SoundId sid;
        unsigned int useCount = 0;
        bool released = false;
    };

    /**
     * 音频播放器类，暂时支持ogg格式解析.
     */
    class MAY_DLL AudioPlayer
    {
    public:
        /**
         * 创建播放对象.
         * 
         * @param aud_clip  音频片段
         */
        explicit AudioPlayer(const AudioClip& aud_clip);

        /**
         * 析构函数.
         * 
         */
        ~AudioPlayer();

        /**
         * 更改音频.
         * 
         * @param aud_clip  音频片段
         */
        void changeClip(const AudioClip& aud_clip);

        /**
         * 释放当前播放器对象.
         * 
         */
        void freePlayer();

        /**
         * 从头开始播放音频.
         * 
         */
        void play() const;

        /**
         * 停止音频播放.
         * 
         */
        void stop() const;

        /**
         * 暂停音频播放.
         * 
         */
        void pause() const;

        /**
         * 恢复播放暂停的音乐.
         * 
         */
        void resume() const;

        /**
         * 获取是否正在播放音频.
         *
         * @return 是否正在播放
         */
        bool isPlaying() const;

        /**
         * 设置音频音量.
         * 
         * @param to_   音频音量，从 0.0f ~ 1.0f
         */
        void setVolume(float to_) const;

        /**
         * 获取当前音量.
         * 
         * @return 当前音量 0.0f ~ 1.0f
         */
        float getVolume() const;

        /**
         * 获取当前音频的播放位置.
         * 
         * @return 音频位置
         */
        size_t getPos() const;

        /**
         * 获取音频的结束位置.
         * 
         * @return 音频位置
         */
        size_t getFinPos();

        /**
         * 设置音频的播放位置.
         * 
         * @param toPos 音频位置
         */
        void setPos(size_t toPos) const;

        /**
         * 设置循环播放.<br/>
         * loop_ == 0 -> 不循环<br/>
         * loop_ != 0 -> 无限循环
         * 
         * @param loop_ 循环次数
         */
        void setLoop(int loop_) const;

        /**
         * 设置停止后是否可以继续播放.<br/>
         * (已弃用)
         * 
         * @param keep_ bool
         */
        void setKeepAlive(bool keep_) const;

        /**
         * 获取音频的波特率.
         * 小贴士：音频位置 / 波特率 = 时间
         * 
         * @return 
         */
        size_t getSampleRate() const;

        /**
         * 加载音频响度数据.
         * 
         * @param mem       音频文件数据
         * @param length    音频文件长度
         * @param frame     帧大小
         */
        void loadAmpData(const char* mem, size_t length, int frame);

        /**
         * 获取采样点响度.
         * 
         * @param sample    采样点
         * @return          音频响度 Range(0.0f ~ 1.0f)
         */
        float getAmp(size_t sample) const;

        /**
         * 设置音频过滤器，同一时间只有一个filter是生效的.
         * 
         * @param filter    过滤器
         */
        void setFilter(const Filter* filter) const;

    private:
        PlayId pid;
        SoundId sid;
        int samples = -1;
        size_t ampSize = 0;
        int ampFrame = 0;
        bool released = false;
        float* ampData = nullptr;
    };
}

NS_MAY_END
