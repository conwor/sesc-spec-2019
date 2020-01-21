//
// Created by EGor on 29.10.2019.
//
#include <iostream>
#include <vector>

#include <map>
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

map<string, pair<TokenType,int>> stringToToken{

        //TokenType : Keyword
        {"void", pair<TokenType, int>(TT_KEYWORD, KW_VOID)},
        {"int", pair<TokenType, int>(TT_KEYWORD, KW_INT)},
        {"if", pair<TokenType, int>(TT_KEYWORD, KW_IF)},
        {"else", pair<TokenType, int>(TT_KEYWORD, KW_ELSE)},
        {"while", pair<TokenType, int>(TT_KEYWORD, KW_WHILE)},
        {"return", pair<TokenType, int>(TT_KEYWORD, KW_RETURN)},

        {"+", pair<TokenType, int>(TT_OPERATION, ADD)},
        {"-", pair<TokenType, int>(TT_OPERATION, SUB)},
        {"*", pair<TokenType, int>(TT_OPERATION, MUL)},
        {"/", pair<TokenType, int>(TT_OPERATION, DIV)},
        {"%", pair<TokenType, int>(TT_OPERATION, MOD)},

        {"=", pair<TokenType, int>(TT_OPERATION, ASSIGN)},

        {"==", pair<TokenType, int>(TT_OPERATION, EQ)},
        {"!=", pair<TokenType, int>(TT_OPERATION, NE)},
        {"<", pair<TokenType, int>(TT_OPERATION, LS)},
        {">", pair<TokenType, int>(TT_OPERATION, BG)},
        {"<=", pair<TokenType, int>(TT_OPERATION, LS_EQ)},
        {">=", pair<TokenType, int>(TT_OPERATION, BG_EQ)},
        {"!", pair<TokenType, int>(TT_OPERATION, NOT)},
        {"&&", pair<TokenType, int>(TT_OPERATION, AND)},
        {"||", pair<TokenType, int>(TT_OPERATION, OR)},

        {";", pair<TokenType, int>(TT_OPERATION, SEMICOLON)},
        {"{", pair<TokenType, int>(TT_OPERATION, OPEN_BRACE)},
        {"}", pair<TokenType, int>(TT_OPERATION, CLOSE_BRACE)},
        {"(", pair<TokenType, int>(TT_OPERATION, OPEN_PARENTHESE)},
        {")", pair<TokenType, int>(TT_OPERATION, CLOSE_PARENTHESE)}

};

string readFile(string fileName) {
    char chr;
    ifstream file;
    string output;

    file.open(fileName);

    if( !file.is_open() )
        throw "No such file";

    for(chr = file.get(); !file.eof(); chr = file.get() ){
        output += chr;
    }
    return output;
}

vector<Token> makeTokens(string text) {
    vector<Token> output;

    int i = 0; // char counter;
    int line_counter = 0;
    int column_counter = 0;
    
    int identificator_index = 0;
    
    TokenType curtype = TT_INVALID;
    string cursubstring; // current substring

    regex space_regex("[ \f\n\r\t\v]*");
    regex literal_regex ("[0-9]*");
    regex identkeyword_regex("[a-zA-Z_][a-zA-Z_0-9]*");
    regex operation_regex (R"((\+|\*|\{|}|\(|\)|\|\||;|&&|-|/|%|!|=|==|<=|<|>|>=|!=))"); // [+=*/%&|{}()!;]*



    while(i < text.length()) { // main cycle
        column_counter++;
        if (text[i] == '\n') {
            line_counter++;
            column_counter = 0;
        }

        cout << endl << cursubstring;

            // if it's a sequence of spaces
        if (regex_match(cursubstring + text[i], space_regex)){
            i++;
            continue;
        } else // if it's literal
        if (regex_match(cursubstring + text[i], literal_regex)) {

            curtype = TT_LITERAL;
            cursubstring += text[i];

        } else // if it's identificator or a keyword
        if (regex_match(cursubstring + text[i], identkeyword_regex)) {

            curtype = TT_IDENT; // might be a keyword;
            cursubstring += text[i];

        } else // if it's operation
        if (regex_match(cursubstring + text[i], operation_regex)) {

            curtype = TT_OPERATION;
            cursubstring += text[i];

        } else
        {// then it's the end of lexeme
            Token token{};
            token.line = line_counter;
            token.column = column_counter;
            switch (curtype){
                case TT_LITERAL:

                    token.type = TT_LITERAL;
                    token.value = stoi(cursubstring);

                    break;
                case TT_OPERATION:

                    token.type = TT_OPERATION;

                    token.value = stringToToken[cursubstring].second;

                    break;
                case TT_IDENT: // or TT_KEYWORD

                    if( stringToToken.find(cursubstring) != stringToToken.end() ){
                        token.type = stringToToken[cursubstring].first;
                        token.value = stringToToken[cursubstring].second;
                    } else {
                        stringToToken[cursubstring] = make_pair(TT_IDENT, identificator_index);
                        token.type = TT_IDENT;
                        token.value = identificator_index;
                        identificator_index++;
                    }

                    break;
                case TT_INVALID:
                    throw "LEXICAL ANALYSER ERROR:\n Invalid TokenType on row "+to_string(line_counter)+", col "+to_string(column_counter)+" : "+cursubstring+", "+text[i];
                    break;
                default:
                    break;
            }
            cursubstring = "";
            curtype = TT_INVALID;
            output.push_back(token);
            continue; // remember the char after our curstring?
        }

        i++;

    }

    return output;
}


