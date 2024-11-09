#!/bin/bash

# 显示将要删除的文件
echo "以下文件将被删除:"
find . -maxdepth 1 -type f -regextype posix-extended -regex "./[0-9]+" -ls

# 询问用户确认
read -p "确认删除这些文件? (y/n) " answer

if [ "$answer" = "y" ] || [ "$answer" = "Y" ]; then
    # 删除匹配的文件
    find . -maxdepth 1 -type f -regextype posix-extended -regex "./[0-9]+" -delete
    echo "文件已删除"
else
    echo "操作已取消"
fi