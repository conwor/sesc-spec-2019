//
// Created by salah on 29.10.2019.
//

#include "../base.h"
#include "Parser.h"
#include <iostream>

using namespace std;

IR parseProgram(vector<Token> tokens) {
    while (!tokens.empty()) {
        parseFunction(tokens);
    }
}

Function* parseFunction(vector<Token> tokens) {
    Function* function = new Function();
    int name;

    readFunctionType(tokens);
    name = readFunctionName(tokens);

    skipToken(TT_OPERATION, OPEN_PARENTHESE, tokens);

    //readParameters(tokens);

    skipToken(TT_OPERATION, CLOSE_PARENTHESE, tokens);

    vector<Operator*> body = parseBody(tokens);

    function->name = name;
    function->body = body;
    if (name == -1) function->isMain = true; // main number

    return function;
}

vector<Operator*> parseBody(vector<Token> tokens) {
    vector<Operator*> body;

    skipToken(TT_OPERATION, OPEN_BRACE, tokens);

    while (nextToken(TT_OPERATION, CLOSE_BRACE, tokens)) {
        body.push_back(parseOperator(tokens));
    }

    skipToken(TT_OPERATION, CLOSE_BRACE, tokens);
}

string readFunctionType(vector<Token> tokens) {
    if (nextToken(TT_KEYWORD, KW_VOID, tokens)) {
        skipToken(TT_KEYWORD, KW_VOID, tokens);
        return "void";
    }

    throw;
}

int readFunctionName(vector<Token> tokens) {
    if (nextToken(TT_IDENT, tokens)) {
        return getToken(tokens).type;
    }

    throw;
}

Operator* parseOperator(vector<Token> tokens) {
    Token operatorToken = tokens.front();
    switch (operatorToken.type) {
        case TT_KEYWORD :
            switch(operatorToken.value) {
                case KW_IF : {
                    skipToken(TT_OPERATION, OPEN_PARENTHESE, tokens);

                    Expression* ifCondition = parseExpression(tokens);

                    skipToken(TT_OPERATION, CLOSE_PARENTHESE, tokens);

                    vector<Operator*> ifBody = parseBody(tokens);
                    }
                    break;
                case KW_WHILE :
                    break;
                case KW_INT: {
                    VarDefOperator* defOperator = new VarDefOperator();

                    Token nameToken = getToken(tokens);
                    if (nameToken.type != TT_IDENT) {
                        throw;
                    }

                    defOperator->name = nameToken.value;

                    skipToken(TT_OPERATION, SEMICOLON, tokens);

                    return defOperator;
                    }
                    break;
                case KW_RETURN: {
                    ExpressionOperator *returnOperator = new ExpressionOperator();

                    }
                    break;
                default:
                    throw;
            }
            break;
        case TT_IDENT :
        case TT_LITERAL:
        case TT_OPERATION:
            break;
    }
}

Expression* parseExpression(vector<Token> tokens) {

}

bool nextToken(TokenType tokenType, int ttType, vector<Token> tokens) {
    return tokens.at(0).type == tokenType && tokens.at(0).value == ttType;
}

bool nextToken(TokenType tokenType, vector<Token> tokens) {
    return tokens.at(0).type == tokenType;
}
    
void skipToken(TokenType tokenType, int ttType, vector<Token> tokens) {
    auto index = tokens.begin();
    if (tokens.at(0).type == tokenType && tokens.at(0).value == ttType) {
        tokens.erase(index);
    } else {
        throw;
    }
}

Token getToken(vector<Token> tokens) {
    Token token = tokens.at(0);
    tokens.erase(tokens.begin());

    return token;
}