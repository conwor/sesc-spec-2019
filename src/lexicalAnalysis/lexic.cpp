//
// Created by EGor on 29.10.2019.
//
#include <iostream>
#include <vector>

#include <bits/stdc++.h>
#include <fstream>
#include <regex>

#include "../base.h"

using namespace std;

/*
 * How I want to do it
 * i need to have an associative array with keys as keywords and operators, like this
 * {
 *      "void" => [TokenType: TT_KEYWORD, value KW_VOID]
 *      "int" => [TokenType: TT_KEYWORD, KW_INT]
 *      ...
 * }
 *
 */

map<string, int, int > StringToToken{

    //TokenType : Keyword
        {"void", TT_KEYWORD, KW_VOID},
        {"int", TT_KEYWORD, KW_INT},
        {"if", TT_KEYWORD, KW_IF},
        {"else", TT_KEYWORD, KW_ELSE},
        {"while", TT_KEYWORD, KW_WHILE},
        {"return", TT_KEYWORD, KW_RETURN},

    // TokenType : Operation
        {"+", TT_OPERATION, ADD},
        {"-", TT_OPERATION, SUB},
        {"*", TT_OPERATION, MUL},
        {"/", TT_OPERATION, DIV},
        {"%", TT_OPERATION, MOD},

        {"=", TT_OPERATION, ASSIGN},
        {"==", TT_OPERATION, EQ},
        {"!=", TT_OPERATION, NE},
        {"<", TT_OPERATION, LS},
        {">", TT_OPERATION, BG},
        {"<=", TT_OPERATION, LS_EQ},
        {">=", TT_OPERATION, BG_EQ},
        {"&&", TT_OPERATION, AND},
        {"||", TT_OPERATION, OR},
        {"!", TT_OPERATION, NOT},

        {"{", TT_OPERATION, OPEN_BRACE},
        {"}", TT_OPERATION, CLOSE_BRACE},
        {"(", TT_OPERATION, OPEN_PARENTHESE},
        {")", TT_OPERATION, CLOSE_PARENTHESE},

        {";", TT_OPERATION, SEMICOLON}
};

string ReadFile(string fileName) {
    char chr;
    ifstream file;
    string output;

    file.open(fileName);

    if( !file.is_open() ) {
        string error;
        error = "No such file";
        throw error;
    }
    for(chr = file.get(); !file.eof(); chr = file.get() ){
        output += chr;
    }
    return output;
}

vector<Token> makeTokens(string& text) {
    vector<Token> output;

    int i = -1; // char counter;
    int line_counter = 0;

    int curtype = -1;
    string cursubstring; // current substring

    regex literal_regex ("[0..9]*");
    regex identkeyword_regex("a..zA..Z_[a..zA..Z_0..9]*");
    regex operation_regex ("[+-=*/%&|{}()!;]*");

    Token *token;

    while(i++ < text.length()) { // main cycle
        if (text[i] == '\n') line_counter++;
        if (isspace(text[i])) continue;

               // if it's literal
        if (regex_match(cursubstring + to_string(i), literal_regex)) {

            curtype = TT_LITERAL;
            cursubstring += to_string(i);

        } else // if it's identificator or a keyword
        if (regex_match(cursubstring + to_string(i), identkeyword_regex)) {

            curtype = TT_IDENT; // might be a keyword;
            cursubstring += to_string(i);

        } else // if it's operation
        if (regex_match(cursubstring + to_string(i), operation_regex)) {

            curtype = TT_OPERATION;
            cursubstring += to_string(i);

        } else {// then it's the end of lexeme
            switch (curtype){
                case TT_LITERAL:
                    token = new Token();
                    token->type = TT_LITERAL;
                    token->value = stoi(cursubstring);
                    break;
                case TT_OPERATION:
                    token = new Token();
                    token->type = TT_OPERATION;
                    token->value = StringToToken(cursubstring);
                    break;
                default:
                    break;
            }
        }

    }

    return output;
}


