#include <string>
#include <stack>
using namespace std;

int precedence(char oprt) {
    if (oprt == '|')
        return 1;
    else if (oprt == '&')
        return 2;
    else if (oprt == '!')
        return 3;
    else
        return 0;
}

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
    // remove whitespace in newInfix
    string newInfix = "";
    for (int i = 0; i < infix.length(); i++) {
        if (infix[i] != ' ')
            newInfix += infix[i];
    }
    if (newInfix.length() == 0)
        return 1;

    postfix = "";
    stack<char> operators;
    int parenthesisCount = 0;

    for (int i = 0; i < newInfix.length(); i++) {
        switch (newInfix[i]) {
        case '(':
            if (i != 0 && isdigit(newInfix[i - 1]))
                return 1;
            operators.push('(');
            parenthesisCount++;
            break;
        case ')':
            if (operators.empty() || postfix.length() == 0) // no operater or no operand exist
                return 1;
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
            }
            if (operators.top() == '(')
                operators.pop();
            parenthesisCount--;
            break;

        case '!':
            if (i == newInfix.length() - 1 || (!isdigit(newInfix[i + 1]) && newInfix[i + 1] != '!' && newInfix[i + 1] != '('))  // followed by operators besides ! and (
                return 1;
            operators.push(newInfix[i]);
            break;
        case '&':
        case '|':
            if (i == 0 || (!isdigit(newInfix[i - 1]) && newInfix[i - 1] != ')') ||
                i == newInfix.length() - 1 || (!isdigit(newInfix[i + 1]) && newInfix[i + 1] != '(' && newInfix[i + 1] != '!'))  // preceded by operators besides ) and !
                return 1;
            while (!operators.empty() && operators.top() != '(' && precedence(newInfix[i]) <= precedence(operators.top())) {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(newInfix[i]);
            break;

        default:
            if ((i != newInfix.length() - 1 && (isdigit(newInfix[i + 1]) || newInfix[i + 1] == '(' || newInfix[i + 1] == '!')) ||
                (i != 0 && (isdigit(newInfix[i - 1]) || newInfix[i - 1] == ')')))
                return 1;
            postfix += newInfix[i];
        }
    }
    if (parenthesisCount != 0)
        return 1;
    while (!operators.empty()) {
        postfix += operators.top();
        operators.pop();
    }

    stack<bool> operands;
    bool sum = false;
    try {
        for (char cur : postfix) {
            if (isdigit(cur))
                operands.push(values[cur - '0']);
            else if (cur == '!') {
                bool operand = operands.top();
                operands.pop();
                sum = !operand;
                operands.push(sum);
            }
            else if (cur == '&') {
                bool operand2 = operands.top();
                operands.pop();
                bool operand1 = operands.top();
                operands.pop();
                sum = operand1 && operand2;
                operands.push(sum);
            }
            else if (cur == '|') {
                bool operand2 = operands.top();
                operands.pop();
                bool operand1 = operands.top();
                operands.pop();
                sum = operand1 || operand2;
                operands.push(sum);
            }
        }
    }
    catch (const exception& e) {
        return 1;
    }
    result = operands.top();
    return 0;
}