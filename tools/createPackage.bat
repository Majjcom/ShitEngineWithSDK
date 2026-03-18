@echo off
set LOCAL_PATH=%~dp0
echo %LOCAL_PATH%
cd %LOCAL_PATH%Package
ShitCompilerTool ShitCompiler shit res
echo --------------------------
echo ShitLang 文件编译完成！！！
echo --------------------------
MayPack --pack -i res -o ..\data.mp --key=ShitPak --keycache=kcache.json -v
echo --------------------------
echo 打包完成！！！
echo --------------------------