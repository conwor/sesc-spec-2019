//
// Created by EGor on 29.10.2019.
//

#include <iostream>
#include "lexic.h"

using namespace std;

ostream& operator<< (ostream& out, const Token& tk){
    out << "Token(" << tk.line << ":" << tk.column << " tt:" << tk.type << " value:" << tk.value << ")";
    return out;
}

int main() {
    string text;
    vector<Token> tokens;
    try {
        text = readFile("H:\\Programming\\test.lang");
        cout << text;
        tokens = makeTokens(text);
        for(auto token : tokens){
            cout << token << endl;
        }
    }
    catch( string& Text ){
        cout << Text;
    }

    return 0;
}
