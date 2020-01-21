//
// Created by salah on 29.10.2019.
//
#include "../base.h"
#ifndef LANG_2019_PARSER_H
#define LANG_2019_PARSER_H

IR parseProgram(std::vector<Token> tokens);
Function parseFunction(std::vector<Token> tokens);
std::string readFunctionType(std::vector<Token> tokens);
std::string readFunctionName(std::vector<Token> tokens);
#endif //LANG_2019_PARSER_H
