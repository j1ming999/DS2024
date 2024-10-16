#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LENGTH 105  // �������󳤶�
#define MAX_HEIGHT 104  // ���ӵ����߶�

// ջ�ṹ�壬���ڴ�����ӵ�����
typedef struct {
    int *data;
    int top;
    int capacity;
} Stack;

// ��ʼ��ջ
Stack* createStack(int capacity) {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->data = (int *)malloc(sizeof(int) * capacity);
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

// �ж�ջ�Ƿ�Ϊ��
int isEmpty(Stack* stack) {
    return stack->top == -1;
}

// ��ȡջ��Ԫ��
int top(Stack* stack) {
    return stack->data[stack->top];
}

// ����ջ��Ԫ��
void pop(Stack* stack) {
    if (!isEmpty(stack)) {
        stack->top--;
    }
}

// ��Ԫ��ѹ��ջ
void push(Stack* stack, int value) {
    stack->data[++stack->top] = value;
}

// �ͷ�ջ���ڴ�
void freeStack(Stack* stack) {
    free(stack->data);
    free(stack);
}

// ��������������ĺ���
int largestRectangleArea(int* heights, int heightsSize) {
    Stack* stack = createStack(heightsSize + 1);
    int maxArea = 0;
    int i;
    int* newHeights = (int*)malloc(sizeof(int) * (heightsSize + 1)); // ����һ�������飬׷��0
    for (i = 0; i < heightsSize; i++) {
        newHeights[i] = heights[i];
    }
    newHeights[heightsSize] = 0; // ��������һ���߶�Ϊ0������

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

// ������ɲ�������
void generateRandomTestData(int* heights, int length) {
    for (int i = 0; i < length; i++) {
        heights[i] = rand() % (MAX_HEIGHT + 1); // �߶���0��MAX_HEIGHT֮��
    }
}

// ���Ժ���
void runTests(int numTests) {
    srand(time(NULL)); // �趨�������
    for (int i = 0; i < numTests; i++) {
        int length = rand() % MAX_LENGTH + 1; // ����������ӵĸ���(1 <= length <= MAX_LENGTH)
        int* heights = (int*)malloc(sizeof(int) * length);
        
        generateRandomTestData(heights, length); // ��������߶�����
        
        int maxArea = largestRectangleArea(heights, length); // �������������
        
        printf("Test %d: Max area = %d (Array size: %d)\n", i + 1, maxArea, length);
        
        free(heights); // �ͷ��ڴ�
    }
}

int main() {
    int numTests = 10;  // ����10���������
    runTests(numTests); // ִ�в���
    return 0;
}

