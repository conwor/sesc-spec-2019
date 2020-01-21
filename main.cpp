//
// Created by EGor on 29.10.2019.
//

#include <iostream>
#include "src/lexicalAnalysis/lexic.h"

#include <bits/stdc++.h>

std::map<std::string, std::array<int,2>> StringToToken{

        //TokenType : Keyword
        {"void", {{TT_KEYWORD, KW_VOID}}},
        {"int", {{TT_KEYWORD, KW_INT}}},
        {"if", {{TT_KEYWORD, KW_IF}}},
        {"else", {{TT_KEYWORD, KW_ELSE}}},
        {"while", {{TT_KEYWORD, KW_WHILE}}},
        {"return", {{TT_KEYWORD, KW_RETURN}}}

};

int main() {
    try {
        std::cout << ReadFile("H:\\Programming\\test.lang");
    }
    catch( std::string& Text ){
        std::cout << Text;
    }
    return 0;
}
