//
// Created by salah on 29.10.2019.
//
#pragma once
#include "../base.h"

IR parseProgram(std::vector<Token> tokens);
Function* parseFunction(std::vector<Token> tokens);
std::vector<Operator*> parseBody(std::vector<Token> tokens);
std::string readFunctionType(std::vector<Token> tokens);
int readFunctionName(std::vector<Token> tokens);
Expression* parseExpression(std::vector<Token> tokens);
void skipToken(TokenType tokenType, int ttType, std::vector<Token> tokens);
Operator* parseOperator(std::vector<Token> tokens);
Token getToken(std::vector<Token> tokens);
bool nextToken(TokenType tokenType, int ttType, std::vector<Token> tokens);
bool nextToken(TokenType tokenTyp, std::vector<Token> tokens);