#!/bin/bash

# 编译所有程序
g++ -std=c++11 -O2 -o kargen kargen.cpp || exit 1
g++ -std=c++11 -O2 -o maxflow maxflow.cpp || exit 1
g++ -std=c++11 -O2 -o gen gen.cpp || exit 1

# 测试次数和设置
NUM_TESTS=50
CORRECT=0

# 创建测试目录
mkdir -p tests

# 清理函数
cleanup() {
    kill $(jobs -p) 2>/dev/null
    exit 1
}

# 设置清理陷阱
trap cleanup SIGINT SIGTERM

echo "Generating test cases..."
# 生成所有测试用例（一次性完成）
for i in $(seq 1 $NUM_TESTS); do
    n=$((RANDOM % 50 + 5))
    m=$((RANDOM % 1000 + 10))
    echo "Generating test $i: n=$n, m=$m"
    ./gen $n $m $i > tests/test$i.in
done

echo "Running tests..."
# 运行测试
for i in $(seq 1 $NUM_TESTS); do
    echo -n "Testing $i/$NUM_TESTS... "
    
    # 直接运行算法并捕获输出
    karger_result=$(./kargen < tests/test$i.in 2>&1)
    if [ $? -ne 0 ]; then
        echo "Karger failed"
        continue
    fi
    
    maxflow_result=$(./maxflow < tests/test$i.in 2>&1)
    if [ $? -ne 0 ]; then
        echo "Maxflow failed"
        continue
    fi
    
    # 提取最后一行作为结果
    karger_result=$(echo "$karger_result" | tail -n 1)
    
    # 比较结果
    if [ "$karger_result" == "$maxflow_result" ]; then
        echo "Correct!"
        ((CORRECT++))
    else
        echo "Wrong!"
        echo "Karger: $karger_result"
        echo "Maxflow: $maxflow_result"
    fi
done

# 计算正确率
if [ $CORRECT -eq 0 ]; then
    ACCURACY=0
else
    ACCURACY=$(echo "scale=2; $CORRECT * 100 / $NUM_TESTS" | bc)
fi

echo "---------------------"
echo "Final Results:"
echo "Total tests: $NUM_TESTS"
echo "Correct: $CORRECT"
echo "Accuracy: $ACCURACY%"
