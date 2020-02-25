#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "generate.h"
#include "lexic.h"
#include "print.h"
#include "Parser.h"

using namespace std;

int main(){
	string input, output;
	cin >> input >> output;

	string file = readFile(input);
	vector<Token> tokens = makeTokens(file);

	for(auto i: tokens){
		cout << i.type << ' ';
	}
	cout << endl;

	IR* ir = parseProgram(tokens);
	Bytecode *bc = generateBytecode(ir);

	ofstream out(output);
	writeBytecode(bc, out);
	return 0;
}
