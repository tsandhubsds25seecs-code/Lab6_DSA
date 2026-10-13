#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// ---------------- Character Stack ----------------
class CharStack {
    char arr[200];
    int top;
public:
    CharStack() { top = -1; }
    bool isEmpty() { return top == -1; }
    bool isFull()  { return top == 199; }
    void push(char c) {
        if (isFull()) { cout << "Stack Overflow\n"; return; }
        arr[++top] = c;
    }
    char pop() {
        if (isEmpty()) { cout << "Stack Underflow\n"; return '\0'; }
        return arr[top--];
    }
    char peek() {
        if (isEmpty()) return '\0';
        return arr[top];
    }
};
// ---------------- Number Stack ----------------
class NumStack {
    double arr[200];
    int top;
public:
    NumStack() { top = -1; }
    bool isEmpty() { return top == -1; }
    void push(double x) { arr[++top] = x; }
    double pop() { return arr[top--]; }
};
// ---------------- String Stack ----------------
class StrStack {
    string arr[200];
    int top;
public:
    StrStack() { top = -1; }
    bool isEmpty() { return top == -1; }
    void push(string s) { arr[++top] = s; }
    string pop() { return arr[top--]; }
};
// ---------------- Helpers ----------------
int priority(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}
bool isOperator(char c) {
    return c=='+' || c=='-' || c=='*' || c=='/' || c=='^';
}
bool isOperand(char c) {
    return (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9');
}
// ---------------- 1) Infix -> Postfix ----------------
string infix_to_postfix(string expr) {
    CharStack st;
    string output = "";
    for (int i = 0; i < (int)expr.size(); i++) {
        char c = expr[i];
        if (c == ' ') continue;
        if (isOperand(c)) {
            output += c;
        }
        else if (c == '(') {
            st.push(c);
        }
        else if (c == ')') {
            while (!st.isEmpty() && st.peek() != '(')
                output += st.pop();
            if (!st.isEmpty()) st.pop();   // discard '('
        }
        else if (isOperator(c)) {
            while (!st.isEmpty() && st.peek() != '(' &&
                   priority(st.peek()) >= priority(c))
                output += st.pop();
            st.push(c);
        }
    }
    while (!st.isEmpty()) output += st.pop();
    return output;
}// ---------------- 2) Infix -> Prefix ----------------
string reverse_and_swap(string s) {
    string r = "";
    for (int i = (int)s.size() - 1; i >= 0; i--) {
        char c = s[i];
        if (c == '(') c = ')';
        else if (c == ')') c = '(';
        r += c;
    }
    return r;
}
string infix_to_prefix(string expr) {
    string rev  = reverse_and_swap(expr);
    string post = infix_to_postfix(rev);
    string pre  = "";
    for (int i = (int)post.size() - 1; i >= 0; i--)
        pre += post[i];
    return pre;
}// ---------------- 3) Postfix -> Infix ----------------
string postfix_to_infix(string expr) {
    StrStack st;
    for (int i = 0; i < (int)expr.size(); i++) {
        char c = expr[i];
        if (c == ' ') continue;
        if (isOperand(c)) {
            st.push(string(1, c));
        } else if (isOperator(c)) {
            string b = st.pop();     // right
            string a = st.pop();     // left
            st.push("(" + a + c + b + ")");
        }
    }
    string r = st.pop();
    if (r.size() >= 2 && r.front() == '(' && r.back() == ')')
        r = r.substr(1, r.size() - 2);
    return r;
}
// ---------------- 4) Evaluate Postfix ----------------
double evaluate_postfix(string expr) {
    NumStack st;
    for (int i = 0; i < (int)expr.size(); i++) {
        char c = expr[i];
        if (c == ' ') continue;
        if (c >= '0' && c <= '9') {
            st.push(c - '0');
        } else if (isOperator(c)) {
            double b = st.pop();      // RIGHT (first pop)
            double a = st.pop();      // LEFT
            double r = 0;
            if (c == '+') r = a + b;
            if (c == '-') r = a - b;
            if (c == '*') r = a * b;
            if (c == '/') r = a / b;
            if (c == '^') r = pow(a, b);
            st.push(r);
        }
    }
    return st.pop();
}
// ---------------- 5) Evaluate Prefix ----------------
double evaluate_prefix(string expr) {
    NumStack st;
    for (int i = (int)expr.size() - 1; i >= 0; i--) {
        char c = expr[i];
        if (c == ' ') continue;
        if (c >= '0' && c <= '9') {
            st.push(c - '0');
        } else if (isOperator(c)) {
            double a = st.pop();      // LEFT (first pop in reverse scan)
            double b = st.pop();      // RIGHT
            double r = 0;
            if (c == '+') r = a + b;
            if (c == '-') r = a - b;
            if (c == '*') r = a * b;
            if (c == '/') r = a / b;
            if (c == '^') r = pow(a, b);
            st.push(r);
        }
    }
    return st.pop();
}
// ---------------- Main ----------------
int main() {
    cout << "===== TASK 1: Expression Conversion & Evaluation =====\n\n";

    string infix = "A+B*C/D-F+A^E";
    cout << "Infix   : " << infix << "\n";
    cout << "Postfix : " << infix_to_postfix(infix) << "\n";
    cout << "Prefix  : " << infix_to_prefix(infix) << "\n\n";

    cout << "Postfix 623+-382/+*2^3+ = "
         << evaluate_postfix("623+-382/+*2^3+") << "  (expected 52)\n";
    cout << "Postfix 45+72-*         = "
         << evaluate_postfix("45+72-*") << "  (expected 45)\n";
    cout << "Prefix  +*23/84         = "
         << evaluate_prefix("+*23/84") << "  (expected 8)\n";
    cout << "Postfix->Infix 342*+    = "
         << postfix_to_infix("342*+") << "\n";

    return 0;
}
