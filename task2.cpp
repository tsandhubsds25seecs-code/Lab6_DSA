#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ---------------- Character Stack ----------------
class CharStack {
    char arr[200];
    int top;
public:
    CharStack() { top = -1; }
    bool isEmpty() { return top == -1; }
    void push(char c) { arr[++top] = c; }
    char pop() { return arr[top--]; }
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

// ---------------- Part A: Bracket Validator ----------------
bool isValidBrackets(string s) {
    CharStack st;
    for (int i = 0; i < (int)s.size(); i++) {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        }
        else if (c == ')' || c == ']' || c == '}') {
            if (st.isEmpty()) return false;
            char open = st.pop();
            if (c == ')' && open != '(') return false;
            if (c == ']' && open != '[') return false;
            if (c == '}' && open != '{') return false;
        }
    }
    return st.isEmpty();
}

// ---------------- Part B: HTML/XML Validator ----------------
bool validate_markup(string s, vector<string>& errors) {
    StrStack st;
    int i = 0;
    while (i < (int)s.size()) {
        if (s[i] == '<') {
            int j = i + 1;
            while (j < (int)s.size() && s[j] != '>') j++;
            if (j == (int)s.size()) {
                errors.push_back("Missing '>' near position " + to_string(i));
                return false;
            }
            string tag = s.substr(i + 1, j - i - 1);

            if (!tag.empty() && tag[0] == '/') {
                string name = tag.substr(1);
                if (st.isEmpty()) {
                    errors.push_back("No opener for </" + name + ">");
                } else {
                    string open = st.pop();
                    if (open != name)
                        errors.push_back("<" + open + "> closed by </" + name + ">");
                }
            }
            else if (!tag.empty() && tag[tag.size()-1] == '/') {
                // self-closing <br/> — ignore
            }
            else {
                string name = tag;
                int sp = tag.find(' ');
                if (sp != -1) name = tag.substr(0, sp);
                st.push(name);
            }
            i = j + 1;
        } else {
            i++;
        }
    }
    while (!st.isEmpty())
        errors.push_back("Unclosed <" + st.pop() + ">");
    return errors.empty();
}

// ---------------- Main ----------------
int main() {
    cout << "===== TASK 2: Expression Validators =====\n\n";

    cout << "-- Bracket Validator --\n";
    cout << "{[()]}  -> " << (isValidBrackets("{[()]}") ? "valid" : "invalid") << "\n";
    cout << "{[(])}  -> " << (isValidBrackets("{[(])}") ? "valid" : "invalid") << "\n";
    cout << "(A+B)*C -> " << (isValidBrackets("(A+B)*C") ? "valid" : "invalid") << "\n";
    cout << "A+B)    -> " << (isValidBrackets("A+B)") ? "valid" : "invalid") << "\n\n";

    cout << "-- HTML/XML Validator --\n";
    vector<string> errs;
    bool ok = validate_markup("<html><body>Hi</body></html>", errs);
    cout << "Test 1 valid? " << ok << "\n";

    errs.clear();
    ok = validate_markup("<a><b></a></b>", errs);
    cout << "Test 2 valid? " << ok << "\n";
    for (int i = 0; i < (int)errs.size(); i++)
        cout << "   error: " << errs[i] << "\n";

    return 0;
}