#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <cstring>

#define MAX_STACK_SIZE 100
#define MAX_EXPR_SIZE 100

// 运算符总数
#define N_OPTR 9

// 运算符枚举
enum Operator { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE };

// 运算符优先级表
const char pri[N_OPTR][N_OPTR] = { // [栈顶][弼前]
    /* +  -  *  /  ^  !  (  )  \0 */
    /* + */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /* - */ '>', '>', '<', '<', '<', '<', '<', '>', '>',
    /* * */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /* / */ '>', '>', '>', '>', '<', '<', '<', '>', '>',
    /* ^ */ '>', '>', '>', '>', '>', '<', '<', '>', '>',
    /* ! */ '>', '>', '>', '>', '>', '>', ' ', '>', '>',
    /* ( */ '<', '<', '<', '<', '<', '<', '<', '=', ' ',
    /* ) */ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    /* \0 */ '<', '<', '<', '<', '<', '<', '<', ' ', '='
};

// 运算符栈
struct OpStack {
    Operator data[MAX_STACK_SIZE];
    int top;
};

// 操作数栈
struct NumStack {
    double data[MAX_STACK_SIZE];
    int top;
};

// 栈初始化
void initOpStack(OpStack* s) {
    s->top = -1;
}

void initNumStack(NumStack* s) {
    s->top = -1;
}

// 判断运算符栈是否为空
bool isEmptyOp(OpStack* s) {
    return s->top == -1;
}

// 判断操作数栈是否为空
bool isEmptyNum(NumStack* s) {
    return s->top == -1;
}

// 进栈运算符
void pushOp(OpStack* s, Operator op) {
    if (s->top >= MAX_STACK_SIZE - 1) {
        std::cerr << "运算符栈溢出！" << std::endl;
        exit(EXIT_FAILURE);
    }
    s->data[++(s->top)] = op;
}

// 出栈运算符
Operator popOp(OpStack* s) {
    if (isEmptyOp(s)) {
        std::cerr << "运算符栈为空，无法出栈！" << std::endl;
        exit(EXIT_FAILURE);
    }
    return s->data[(s->top)--];
}

// 读取栈顶运算符
Operator peekOp(OpStack* s) {
    if (isEmptyOp(s)) {
        return EOE; // 结束符
    }
    return s->data[s->top];
}

// 进栈操作数
void pushNum(NumStack* s, double num) {
    if (s->top >= MAX_STACK_SIZE - 1) {
        std::cerr << "操作数栈溢出！" << std::endl;
        exit(EXIT_FAILURE);
    }
    s->data[++(s->top)] = num;
}

// 出栈操作数
double popNum(NumStack* s) {
    if (isEmptyNum(s)) {
        std::cerr << "操作数栈为空，无法出栈！" << std::endl;
        exit(EXIT_FAILURE);
    }
    return s->data[(s->top)--];
}

// 读取栈顶操作数
double peekNum(NumStack* s) {
    if (isEmptyNum(s)) {
        std::cerr << "操作数栈为空！" << std::endl;
        exit(EXIT_FAILURE);
    }
    return s->data[s->top];
}

// 判断字符是否是运算符
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' ||
           c == '^' || c == '!' || c == '(' || c == ')';
}

// 将字符运算符转换为枚举类型
Operator charToOp(char c) {
    switch (c) {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MUL;
        case '/': return DIV;
        case '^': return POW;
        case '!': return FAC;
        case '(': return L_P;
        case ')': return R_P;
        case '\0': return EOE;
        default:
            std::cerr << "未知运算符：" << c << std::endl;
            exit(EXIT_FAILURE);
    }
}

// 将枚举运算符转换为字符
char opToChar(Operator op) {
    switch (op) {
        case ADD: return '+';
        case SUB: return '-';
        case MUL: return '*';
        case DIV: return '/';
        case POW: return '^';
        case FAC: return '!';
        case L_P: return '(';
        case R_P: return ')';
        case EOE: return '\0';
        default: return '\0';
    }
}

// 执行二元运算
double applyOperator(Operator op, double a, double b) {
    switch (op) {
        case ADD: return a + b;
        case SUB: return a - b;
        case MUL: return a * b;
        case DIV:
            if (b == 0) {
                std::cerr << "错误：除以零！" << std::endl;
                exit(EXIT_FAILURE);
            }
            return a / b;
        case POW: return pow(a, b);
        default:
            std::cerr << "不支持的二元运算符！" << std::endl;
            exit(EXIT_FAILURE);
    }
}

// 执行单元运算
double applyUnaryOperator(Operator op, double a) {
    switch (op) {
        case FAC: {
            if (a < 0 || floor(a) != a) {
                std::cerr << "错误：阶乘仅定义于非负整数！" << std::endl;
                exit(EXIT_FAILURE);
            }
            double result = 1;
            for(int i = 1; i <= static_cast<int>(a); i++) {
                result *= i;
            }
            return result;
        }
        default:
            std::cerr << "不支持的单元运算符！" << std::endl;
            exit(EXIT_FAILURE);
    }
}

// 计算表达式
double evaluate(const char* expr) {
    OpStack opStack;
    NumStack numStack;
    initOpStack(&opStack);
    initNumStack(&numStack);

    // 初始化运算符栈，压入结束符
    pushOp(&opStack, EOE);

    int i = 0;
    char current;
    Operator topOp;
    Operator incomingOp;

    while (i <= strlen(expr)) { // 包含'\0'
        current = expr[i];
        incomingOp = isOperator(current) ? charToOp(current) : EOE;

        // 如果是数字，解析完整的数字并压入操作数栈
        if (isdigit(current) || current == '.') {
            double num = 0;
            int start = i;
            while (isdigit(expr[i]) || expr[i] == '.') {
                i++;
            }
            char numStr[20];
            if (i - start >= 20) {
                std::cerr << "数字过长！" << std::endl;
                exit(EXIT_FAILURE);
            }
            strncpy(numStr, expr + start, i - start);
            numStr[i - start] = '\0';
            num = atof(numStr);
            pushNum(&numStack, num);
            continue; // 已经更新了i，继续下一轮
        }

        // 如果不是运算符，忽略空格
        if (!isOperator(current)) {
            if (current == ' ') {
                i++;
                continue;
            }
            if (current == '\0') {
                // 结束符
            } else {
                std::cerr << "无效字符：" << current << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        // 获取栈顶运算符
        topOp = peekOp(&opStack);

        // 根据优先级表决定操作
        char precedence = pri[topOp][incomingOp];

        if (precedence == '<') {
            pushOp(&opStack, incomingOp);
            i++;
        }
        else if (precedence == '>') {
            Operator op = popOp(&opStack);
            if(op == ADD || op == SUB || op == MUL || op == DIV || op == POW) {
                if(numStack.top < 1){
                    std::cerr << "操作数不足，无法进行二元运算！" << std::endl;
                    exit(EXIT_FAILURE);
                }
                double b = popNum(&numStack);
                double a = popNum(&numStack);
                double res = applyOperator(op, a, b);
                pushNum(&numStack, res);
            }
            else if(op == FAC) {
                if(numStack.top < 0){
                    std::cerr << "操作数不足，无法进行单元运算！" << std::endl;
                    exit(EXIT_FAILURE);
                }
                double a = popNum(&numStack);
                double res = applyUnaryOperator(op, a);
                pushNum(&numStack, res);
            }
            else {
                std::cerr << "未知或不支持的运算符！" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if (precedence == '=') {
            popOp(&opStack); // 弹出匹配的括号
            i++;
        }
        else {
            std::cerr << "表达式无效或存在语法错误！" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    if(numStack.top != 0) {
        std::cerr << "表达式错误，操作数栈未正确归零！" << std::endl;
        exit(EXIT_FAILURE);
    }

    return popNum(&numStack);
}

int main() {
    char expr[MAX_EXPR_SIZE];
    std::cout << "请输入表达式（支持 +, -, *, /, ^, !, (, )）：" << std::endl;
    if(!std::cin.getline(expr, MAX_EXPR_SIZE)){
        std::cerr << "读取输入失败！" << std::endl;
        return EXIT_FAILURE;
    }

    // 计算并输出结果
    double result = evaluate(expr);
    std::cout << "结果：" << result << std::endl;

    return 0;
}

