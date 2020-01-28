//
// Created by salah on 29.10.2019.
//

#include "../base.h"
#include "Parser.h"
#include <iostream>

using namespace std;
IR parseProgram(vector<Token> tokens) {
    parseFunction(tokens);
}

Function parseFunction(vector<Token> tokens) {
    Function* function = new Function();
    string name;

    readFunctionType(tokens);
    readFunctionName(tokens);

    skipToken(TT_OPERATION, OPEN_PARENTHESE, tokens);

    // readParameters(tokens);

    skipToken(TT_OPERATION, CLOSE_PARENTHESE, tokens);

    skipToken(TT_OPERATION, OPEN_BRACE, tokens);

    //parseBody(tokens);

    skipToken(TT_OPERATION, CLOSE_BRACE, tokens);

    return *function;
}

vector<Operator*> parseBody(vector<Token> tokens) {
    vector<Operator*> body;

    skipToken(TT_OPERATION, OPEN_BRACE, tokens);

    while (tokens.front().type != TT_OPERATION && tokens.front().value != CLOSE_BRACE) {
        body.push_back(parseOperator(tokens));
    }

    skipToken(TT_OPERATION, CLOSE_BRACE, tokens);
}

string readFunctionType(vector<Token> tokens) {
    string returnType;

    Token returnTypeToken = getToken(tokens);
    if (returnTypeToken.type != TT_KEYWORD) { throw; }
    switch (returnTypeToken.value) {
        case KW_VOID :
            returnType = "void";
            break;
        default:;
            throw;
    }

    return returnType;
}

string readFunctionName(vector<Token> tokens) {
    string name;

    Token functionNameToken = getToken(tokens);
    if (functionNameToken.type != TT_IDENT) {
        throw;
    }

    name = functionNameToken.value;

    return name;
}

Operator* parseOperator(vector<Token> tokens) {
    Token operatorToken = tokens.front();
    switch (operatorToken.type) {
        case TT_KEYWORD :
            switch(operatorToken.value) {
                case KW_IF : {
                    skipToken(TT_OPERATION, OPEN_PARENTHESE, tokens);

                    Expression ifCondition = parseExpression(tokens);

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
                    break;
                    //throw an exception
            }
            break;
        case TT_IDENT :
            break;
        case TT_LITERAL:
            break;
        case TT_OPERATION:
            break;
    }
}

Expression parseExpression(vector<Token> tokens) {

}

void skipToken(TokenType tokenType, int ttType, vector<Token> tokens) {
    auto index = tokens.cbegin();
    if (tokens.at(0).type == tokenType && tokens.at(0).value == ttType) {
        tokens.erase(index);
    } else {
        throw;
    }
}

Token getToken(vector<Token> tokens) {
    Token token = tokens.front();
    tokens.erase(tokens.cbegin());

    return token;
}