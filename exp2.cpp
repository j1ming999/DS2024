#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <cstring>

#define MAX_STACK_SIZE 100
#define MAX_EXPR_SIZE 100

// ���������
#define N_OPTR 9

// �����ö��
enum Operator { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE };

// ��������ȼ���
const char pri[N_OPTR][N_OPTR] = { // [ջ��][��ǰ]
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

// �����ջ
struct OpStack {
    Operator data[MAX_STACK_SIZE];
    int top;
};

// ������ջ
struct NumStack {
    double data[MAX_STACK_SIZE];
    int top;
};

// ջ��ʼ��
void initOpStack(OpStack* s) {
    s->top = -1;
}

void initNumStack(NumStack* s) {
    s->top = -1;
}

// �ж������ջ�Ƿ�Ϊ��
bool isEmptyOp(OpStack* s) {
    return s->top == -1;
}

// �жϲ�����ջ�Ƿ�Ϊ��
bool isEmptyNum(NumStack* s) {
    return s->top == -1;
}

// ��ջ�����
void pushOp(OpStack* s, Operator op) {
    if (s->top >= MAX_STACK_SIZE - 1) {
        std::cerr << "�����ջ�����" << std::endl;
        exit(EXIT_FAILURE);
    }
    s->data[++(s->top)] = op;
}

// ��ջ�����
Operator popOp(OpStack* s) {
    if (isEmptyOp(s)) {
        std::cerr << "�����ջΪ�գ��޷���ջ��" << std::endl;
        exit(EXIT_FAILURE);
    }
    return s->data[(s->top)--];
}

// ��ȡջ�������
Operator peekOp(OpStack* s) {
    if (isEmptyOp(s)) {
        return EOE; // ������
    }
    return s->data[s->top];
}

// ��ջ������
void pushNum(NumStack* s, double num) {
    if (s->top >= MAX_STACK_SIZE - 1) {
        std::cerr << "������ջ�����" << std::endl;
        exit(EXIT_FAILURE);
    }
    s->data[++(s->top)] = num;
}

// ��ջ������
double popNum(NumStack* s) {
    if (isEmptyNum(s)) {
        std::cerr << "������ջΪ�գ��޷���ջ��" << std::endl;
        exit(EXIT_FAILURE);
    }
    return s->data[(s->top)--];
}

// ��ȡջ��������
double peekNum(NumStack* s) {
    if (isEmptyNum(s)) {
        std::cerr << "������ջΪ�գ�" << std::endl;
        exit(EXIT_FAILURE);
    }
    return s->data[s->top];
}

// �ж��ַ��Ƿ��������
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' ||
           c == '^' || c == '!' || c == '(' || c == ')';
}

// ���ַ������ת��Ϊö������
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
            std::cerr << "δ֪�������" << c << std::endl;
            exit(EXIT_FAILURE);
    }
}

// ��ö�������ת��Ϊ�ַ�
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

// ִ�ж�Ԫ����
double applyOperator(Operator op, double a, double b) {
    switch (op) {
        case ADD: return a + b;
        case SUB: return a - b;
        case MUL: return a * b;
        case DIV:
            if (b == 0) {
                std::cerr << "���󣺳����㣡" << std::endl;
                exit(EXIT_FAILURE);
            }
            return a / b;
        case POW: return pow(a, b);
        default:
            std::cerr << "��֧�ֵĶ�Ԫ�������" << std::endl;
            exit(EXIT_FAILURE);
    }
}

// ִ�е�Ԫ����
double applyUnaryOperator(Operator op, double a) {
    switch (op) {
        case FAC: {
            if (a < 0 || floor(a) != a) {
                std::cerr << "���󣺽׳˽������ڷǸ�������" << std::endl;
                exit(EXIT_FAILURE);
            }
            double result = 1;
            for(int i = 1; i <= static_cast<int>(a); i++) {
                result *= i;
            }
            return result;
        }
        default:
            std::cerr << "��֧�ֵĵ�Ԫ�������" << std::endl;
            exit(EXIT_FAILURE);
    }
}

// ������ʽ
double evaluate(const char* expr) {
    OpStack opStack;
    NumStack numStack;
    initOpStack(&opStack);
    initNumStack(&numStack);

    // ��ʼ�������ջ��ѹ�������
    pushOp(&opStack, EOE);

    int i = 0;
    char current;
    Operator topOp;
    Operator incomingOp;

    while (i <= strlen(expr)) { // ����'\0'
        current = expr[i];
        incomingOp = isOperator(current) ? charToOp(current) : EOE;

        // ��������֣��������������ֲ�ѹ�������ջ
        if (isdigit(current) || current == '.') {
            double num = 0;
            int start = i;
            while (isdigit(expr[i]) || expr[i] == '.') {
                i++;
            }
            char numStr[20];
            if (i - start >= 20) {
                std::cerr << "���ֹ�����" << std::endl;
                exit(EXIT_FAILURE);
            }
            strncpy(numStr, expr + start, i - start);
            numStr[i - start] = '\0';
            num = atof(numStr);
            pushNum(&numStack, num);
            continue; // �Ѿ�������i��������һ��
        }

        // �����������������Կո�
        if (!isOperator(current)) {
            if (current == ' ') {
                i++;
                continue;
            }
            if (current == '\0') {
                // ������
            } else {
                std::cerr << "��Ч�ַ���" << current << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        // ��ȡջ�������
        topOp = peekOp(&opStack);

        // �������ȼ����������
        char precedence = pri[topOp][incomingOp];

        if (precedence == '<') {
            pushOp(&opStack, incomingOp);
            i++;
        }
        else if (precedence == '>') {
            Operator op = popOp(&opStack);
            if(op == ADD || op == SUB || op == MUL || op == DIV || op == POW) {
                if(numStack.top < 1){
                    std::cerr << "���������㣬�޷����ж�Ԫ���㣡" << std::endl;
                    exit(EXIT_FAILURE);
                }
                double b = popNum(&numStack);
                double a = popNum(&numStack);
                double res = applyOperator(op, a, b);
                pushNum(&numStack, res);
            }
            else if(op == FAC) {
                if(numStack.top < 0){
                    std::cerr << "���������㣬�޷����е�Ԫ���㣡" << std::endl;
                    exit(EXIT_FAILURE);
                }
                double a = popNum(&numStack);
                double res = applyUnaryOperator(op, a);
                pushNum(&numStack, res);
            }
            else {
                std::cerr << "δ֪��֧�ֵ��������" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if (precedence == '=') {
            popOp(&opStack); // ����ƥ�������
            i++;
        }
        else {
            std::cerr << "���ʽ��Ч������﷨����" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    if(numStack.top != 0) {
        std::cerr << "���ʽ���󣬲�����ջδ��ȷ���㣡" << std::endl;
        exit(EXIT_FAILURE);
    }

    return popNum(&numStack);
}

int main() {
    char expr[MAX_EXPR_SIZE];
    std::cout << "��������ʽ��֧�� +, -, *, /, ^, !, (, )����" << std::endl;
    if(!std::cin.getline(expr, MAX_EXPR_SIZE)){
        std::cerr << "��ȡ����ʧ�ܣ�" << std::endl;
        return EXIT_FAILURE;
    }

    // ���㲢������
    double result = evaluate(expr);
    std::cout << "�����" << result << std::endl;

    return 0;
}

