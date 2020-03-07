#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "generator/generate.h"
#include "lexer/lexic.h"
#include "generator/print.h"
#include "parser/Parser.h"

using namespace std;

int main(){
	istringstream in("input.txt output.txt");
	string input, output;
	in >> input >> output;

	string file = readFile(input);
	cout << *(file.rbegin()) << endl;
	vector<Token> tokens = makeTokens(file);

	for(auto i: tokens){
		cout << i.type << ' ' << i.value << endl;
	}
	cout << endl;

	IR* ir = parseProgram(tokens);
	Bytecode *bc = generateBytecode(ir);

	ofstream out(output);
	writeBytecode(bc, out);
	return 0;
}
