#include <iostream>
#include <sstream>
#include <unordered_set>
using namespace std;

enum TokenType {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LEFTPARENT,
    RIGHTPARENT,
    END,
    FUNCTION,
    COMMA
};

struct Token {
    TokenType type;
    double value;
    string function;

    Token(TokenType type) : type(type), value(0) {}
    Token(TokenType type, double value) : type(type), value(value) {}
    Token(TokenType type, string function) : type(type), function(function) {}
};

class Tokenization {
public:
    Tokenization(string& input) :expression(input), pos(0) {}

    Token getNextToken() {
        while (pos < expression.size() && isspace(expression[pos])) {
            pos++;
        }

        if (pos >= expression.size()) {
            return Token(END);
        }

        char currentChar = expression[pos];

        if (isdigit(currentChar)) {
            return Token(NUMBER, getNumber());
        }
        else if (isalpha(currentChar)) {
            string function = getFunction();
            if (isFunction(function)) {
                return Token(FUNCTION, function);
            }
            else {
                throw runtime_error("Unknown function:  " + function);
            }

        }
        else if (currentChar == '+') {
            pos++;
            return Token(PLUS);
        }
        else if (currentChar == '-') {
            pos++;
            return Token(MINUS);
        }
        else if (currentChar == '*') {
            pos++;
            return Token(MULTIPLY);
        }
        else if (currentChar == '/') {
            pos++;
            return Token(DIVIDE);
        }
        else if (currentChar == '(') {
            pos++;
            return Token(LEFTPARENT);
        }
        else if (currentChar == ')') {
            pos++;
            return Token(RIGHTPARENT);
        }
        else if (currentChar == ',') {
            pos++;
            return Token(COMMA);
        }
        else {
            throw runtime_error("Unknown character!");
        }
    }
private:
    string expression;
    size_t pos;
    unordered_set<string> functions = { "pow", "abs", "min", "max" };

    bool isFunction(string& function) {
        return functions.find(function) != functions.end();
    }

    double getNumber() {
        stringstream temp;
        while (pos < expression.size() && isdigit(expression[pos])) {
            temp << expression[pos++];
        }
        double number;
        temp >> number;
        return number;
    }
    string getFunction() {
        string function;
        while (pos < expression.size() && isalpha(expression[pos])) {
            function += expression[pos++];
        }
        return function;
    }
};

class Interpreter {
public:
    Interpreter(string& expression) :tokenization(expression), currentToken(tokenization.getNextToken()) {}
    double evaluate() {
        return plusMinus();
    }
private:
    Tokenization tokenization;
    Token currentToken;

    void goToNext(TokenType tokenType) {
        if (currentToken.type == tokenType) {
            currentToken = tokenization.getNextToken();
        }
        else {
            cerr << "Error parsing input.\n";
        }
    }

    double calculateFunction(string function, double args1, double args2) {
        if (function == "pow") {
            return pow(args1, args2);
        }
        else if (function == "abs") {
            return abs(args1);
        }
        else if (function == "min") {
            return min(args1, args2);
        }
        else if (function == "max") {
            return max(args1, args2);
        }
    }

    double symbol() {
        if (currentToken.type == NUMBER) {
            double value = currentToken.value;
            goToNext(NUMBER);
            return value;
        }
        else if (currentToken.type == LEFTPARENT) {
            goToNext(LEFTPARENT);
            double result = plusMinus();
            goToNext(RIGHTPARENT);
            return result;
        }
        else if (currentToken.type == MINUS) {
            goToNext(MINUS);
            return -symbol();
        }
        else if (currentToken.type == FUNCTION) {
            string function = currentToken.function;
            goToNext(FUNCTION);
            goToNext(LEFTPARENT);
            double args1 = plusMinus();
            double args2 = 0;
            if (currentToken.type == COMMA) {
                goToNext(COMMA);
                args2 = plusMinus();
            }
            goToNext(RIGHTPARENT);
            return calculateFunction(function, args1, args2);
        }
        cerr << "Something went wrong:(\n";
        return 0;
    }

    double multDiv() {
        double result = symbol();
        while (currentToken.type == TokenType::DIVIDE || currentToken.type == TokenType::MULTIPLY) {
            Token token = currentToken;
            if (token.type == TokenType::MULTIPLY) {
                goToNext(TokenType::MULTIPLY);
                result *= symbol();
            }
            else if (token.type == TokenType::DIVIDE) {
                goToNext(TokenType::DIVIDE);
                double divisor = symbol();
                if (divisor != 0) {
                    result /= divisor;
                }
                else { cerr << "Division by zero is impossible.\n"; }
            }
        }
        return result;
    }

    double plusMinus() {
        double result = multDiv();
        while (currentToken.type == TokenType::PLUS || currentToken.type == TokenType::MINUS) {
            Token token = currentToken;
            if (token.type == TokenType::PLUS) {
                goToNext(TokenType::PLUS);
                result += multDiv();
            }
            else if (token.type == TokenType::MINUS)
            {
                goToNext(TokenType::MINUS);
                result -= multDiv();
            }
        }
        return result;
    }
};


int main()
{
    string input;
    cout << "Enter an arithmetic expression:" << endl;
    getline(cin, input);

    Interpreter interpreter(input);
    double result = interpreter.evaluate();

    cout << "Answer: " << result << endl;
}

