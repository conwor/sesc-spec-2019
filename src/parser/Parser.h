//
// Created by salah on 29.10.2019.
//
#pragma once
#include "../base.h"
#ifndef LANG_2019_PARSER_H
#define LANG_2019_PARSER_H

IR parseProgram(std::vector<Token> tokens);
Function parseFunction(std::vector<Token> tokens);
std::string readFunctionType(std::vector<Token> tokens);
std::string readFunctionName(std::vector<Token> tokens);
Expression parseExpression(std::vector<Token> tokens);
void skipToken(TokenType tokenType, int ttType, std::vector<Token> tokens);
Operator* parseOperator(std::vector<Token> tokens);
Token getToken(std::vector<Token> tokens);
#endif //LANG_2019_PARSER_H
