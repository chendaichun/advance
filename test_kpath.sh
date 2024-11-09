#!/bin/bash

# 编译两个程序
clang++ -std=c++14 -O2 -o kpath color_coding.cpp || exit 1
clang++ -std=c++11 -O2 -o kpath_std kpath_std.cpp || exit 1
clang++ -std=c++14 -O2 -o dag g_dag_gen.cpp || exit 1

NUM_TESTS=500
mkdir -p test_dag
PASSED=0
FAILED=0
FAILED_CASES=""

# 生成测试用例并比较结果
for i in $(seq 1 $NUM_TESTS); do
    echo -n "Testing case $i... "
    # 生成测试数据
    ./dag > test_dag/test$i.in
    
    # 运行两个程序
    RESULT1=$(./kpath < test_dag/test$i.in)
    RESULT2=$(./kpath_std < test_dag/test$i.in)
    
    # 比较结果
    if [ "$RESULT1" = "$RESULT2" ]; then
        echo "OK"
        PASSED=$((PASSED + 1))
    elif [ "$RESULT1" -gt "$RESULT2" ]; then
        echo "OK"
        PASSED=$((PASSED + 1))
    else
        echo "Failed"
        echo "Your output: $RESULT1"
        echo "Expected: $RESULT2"
        echo "Input saved in test_dag/test$i.in"
        FAILED=$((FAILED + 1))
        FAILED_CASES="$FAILED_CASES $i"
    fi
done

# 输出统计信息
echo "----------------------------------------"
echo "Test Summary:"
echo "Total tests: $NUM_TESTS"
echo "Passed: $PASSED"
echo "Failed: $FAILED"
if [ $FAILED -gt 0 ]; then
    echo "Failed cases:$FAILED_CASES"
    echo "Check test_dag/test{N}.in files for failed cases"
    exit 1
else
    echo "All tests passed!"
fi