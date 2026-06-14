#pragma once
#include <string>
#include <vector>
using namespace std;

enum class TokenType {
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