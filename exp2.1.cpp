#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LENGTH 105  // 数组的最大长度
#define MAX_HEIGHT 104  // 柱子的最大高度

// 栈结构体，用于存放柱子的索引
typedef struct {
    int *data;
    int top;
    int capacity;
} Stack;

// 初始化栈
Stack* createStack(int capacity) {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->data = (int *)malloc(sizeof(int) * capacity);
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

// 判断栈是否为空
int isEmpty(Stack* stack) {
    return stack->top == -1;
}

// 获取栈顶元素
int top(Stack* stack) {
    return stack->data[stack->top];
}

// 弹出栈顶元素
void pop(Stack* stack) {
    if (!isEmpty(stack)) {
        stack->top--;
    }
}

// 将元素压入栈
void push(Stack* stack, int value) {
    stack->data[++stack->top] = value;
}

// 释放栈的内存
void freeStack(Stack* stack) {
    free(stack->data);
    free(stack);
}

// 计算最大矩形面积的函数
int largestRectangleArea(int* heights, int heightsSize) {
    Stack* stack = createStack(heightsSize + 1);
    int maxArea = 0;
    int i;
    int* newHeights = (int*)malloc(sizeof(int) * (heightsSize + 1)); // 创建一个新数组，追加0
    for (i = 0; i < heightsSize; i++) {
        newHeights[i] = heights[i];
    }
    newHeights[heightsSize] = 0; // 在最后添加一个高度为0的柱子

    for (i = 0; i <= heightsSize; i++) {
        while (!isEmpty(stack) && newHeights[i] < newHeights[top(stack)]) {
            int height = newHeights[top(stack)];
            pop(stack);
            int width = isEmpty(stack) ? i : i - top(stack) - 1;
            int area = height * width;
            if (area > maxArea) {
                maxArea = area;
            }
        }
        push(stack, i);
    }

    free(newHeights);
    freeStack(stack);
    return maxArea;
}

// 随机生成测试数据
void generateRandomTestData(int* heights, int length) {
    for (int i = 0; i < length; i++) {
        heights[i] = rand() % (MAX_HEIGHT + 1); // 高度在0到MAX_HEIGHT之间
    }
}

// 测试函数
void runTests(int numTests) {
    srand(time(NULL)); // 设定随机种子
    for (int i = 0; i < numTests; i++) {
        int length = rand() % MAX_LENGTH + 1; // 随机生成柱子的个数(1 <= length <= MAX_LENGTH)
        int* heights = (int*)malloc(sizeof(int) * length);
        
        generateRandomTestData(heights, length); // 生成随机高度数组
        
        int maxArea = largestRectangleArea(heights, length); // 计算最大矩形面积
        
        printf("Test %d: Max area = %d (Array size: %d)\n", i + 1, maxArea, length);
        
        free(heights); // 释放内存
    }
}

int main() {
    int numTests = 10;  // 进行10次随机测试
    runTests(numTests); // 执行测试
    return 0;
}

