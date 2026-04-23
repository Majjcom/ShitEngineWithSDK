SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# 定义路径变量
LOCAL_PATH="$SCRIPT_DIR"

# 输出路径信息 (对应 echo %LOCAL_PATH%)
echo "$LOCAL_PATH"

# 进入 Package 目录 (对应 cd %LOCAL_PATH%Package)
cd "$LOCAL_PATH/Package" || exit

# 执行编译工具 (对应 tools\ShitCompilerTool ...)
# Linux 下路径分隔符使用正斜杠 /
./tools/ShitCompilerTool tools/ShitCompiler shit res

echo "--------------------------"
echo "ShitLang 文件编译完成！！！"
echo "--------------------------"

# 执行打包工具 (对应 tools\MayPack ...)
# 注意：.. 表示上一级目录，Linux 同样支持
./tools/MayPack --pack -i res -o ../data.mp --key=ShitPak --keycache=kcache.json -v

echo "--------------------------"
echo "打包完成！！！"
echo "--------------------------"