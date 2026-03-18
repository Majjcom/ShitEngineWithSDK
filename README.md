# ShitEngine

这是一个将`ShitLang`作为主要语言的简易游戏引擎，由C++开发



## 构建

目前仅支持在Windows中构建。



#### 准备工作

下载[boost-with-cmake-1.90.0](https://github.com/boostorg/boost/releases/tag/boost-1.90.0)

将内容解压到目录`ext/boost`下即可

Windows需要安装Visual Studio 2022，以及`10.0.19041.0`及以上版本的Windows SDK

如需开发测试，建议使用CLion+CMake+Ninja进行开发



#### 构建

进入Visual Studio命令行环境，直接运行`BUILD_RELEASE.BAT`即可进行发布构建

如需构建其他版本，可以使用参考此脚本中的配置



## 使用

运行ShitEditor，可以打开编辑器，此编辑器基本可以实现全套的ShitLang开发编辑过程

编辑器包含了ShitLang的编辑、调试功能，易于使用。

ShitEngine基于MayEngine（个人项目）开发，通过ShitCompiler使得ShitLang能够运行在MayEngine的Lua环境中

doc目录下提供了MayEngine相关的API参考，开发者可以参考这些文档使用ShitEditor



#### 资源

用到的资源文件需要放置到Package/res中，此目录下的所有文件会被打包到data.mp中

如需修改打包密钥，开发者可以从源码构建，并修改CMake文件和代码中的密钥。



#### 发布

使用ShitEditor内置的打包功能，或者运行`createPackage.bat`脚本即可编译Shit代码，并将res中的资源打包到data.mp

完整可运行的项目需要以下文件：

```text
d3dcompiler_47.dll
data.mp
libEGL.dll
libGLESv2.dll
MayEngine.dll
ShitGame.exe
lua（可选）
```

如果需要用到网络功能（如调试器），可以将lua目录也一起带上

