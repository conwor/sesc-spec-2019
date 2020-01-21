//
// Created by salah on 29.10.2019.
//

#include "../base.h"
#include "Parser.h"
#include <iostream>

using namespace std;
int index = 0;
IR parseProgram(vector<Token> tokens) {
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].type == TT_KEYWORD) {
            if (tokens[i].value == 1) {
                parseFunction(tokens);
            }
        }
    }
}

Function parseFunction(vector<Token> tokens) {
    Function *function = new Function();
    string name;

    while (index < tokens.size()) {
        readFunctionType(tokens);
        readFunctionName(tokens);

        Token openParenthesis = tokens.at(index);
        if (openParenthesis.type != TT_OPERATION || openParenthesis.value != OPEN_PARENTHESE) { /* throw an exception */ }
        index++;

        // readParameters(tokens);

        Token closeParenthesis = tokens.at(index);
        if (closeParenthesis.type != TT_OPERATION || closeParenthesis.value != CLOSE_PARENTHESE) { /* throw an exception */ }
        index++;

        Token openBrace = tokens.at(index);
        if (openBrace.type != TT_OPERATION || openBrace.value != OPEN_BRACE) { /* throw an exception */ }
        index++;

        //parseBody(tokens);

        Token closeBrace = tokens.at(index);
        if (closeBrace.type != TT_OPERATION || closeBrace.value != CLOSE_BRACE) { /* throw an exception */ }
        index++;
    }

    return *function;
}

vector<Operator*> parseBody(vector<Token> tokens) {
    vector<Operator*> body;

    while (index < tokens.size()) {

    }
}

string readFunctionType(vector<Token> tokens) {
    string returnType;

    Token returnTypeToken = tokens.at(index);
    if (returnTypeToken.type != TT_KEYWORD) { /* throw an exception */ }
    switch (returnTypeToken.value) {
        case KW_VOID :
            returnType = "void";
            break;
        default:;
            // throw an exception
    }

    index++;

    return returnType;
}

string readFunctionName(vector<Token> tokens) {
    string name;

    Token functionNameToken = tokens.at(index);
    if (functionNameToken.type != TT_IDENT) { /* throw an exception */ }
    name = functionNameToken.value;

    index++;

    return name;
}

Operator readOperator(vector<Token> tokens) {
    Token operatorToken = tokens.at(index);
    switch (operatorToken.type) {
        case TT_KEYWORD :
            switch(operatorToken.value) {
                case KW_IF :
                    
                    break;
                case KW_WHILE :
                    break;
                case KW_INT:
                    break;
                case KW_RETURN:
                    break;
                default:;
                    //throw an exception
            }
            break;
    }
}