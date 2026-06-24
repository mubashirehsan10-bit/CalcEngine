#pragma once
#include <string>
#include <vector>
#include<cctype>
using namespace std;

enum class TokenType {
    NONE,
    NUMBER,
    VARIABLE,
    OPERATOR,
    FUNCTION,
    LPAREN,
    RPAREN
};

class Token {
public:
    TokenType type;
    string value;

    Token(TokenType type, string value);
};

class Tokenizer {
private:
    string input;
    int pos;

public:
    Tokenizer(string input);
    vector<Token> tokenize();
};