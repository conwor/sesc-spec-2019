#include "base.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;


void TestExpressionAssign() {
    {
        ostringstream exp;
        exp << "1" << endl << "main\n10\n9" << endl;
        exp << "ILOAD 3 4" << endl;
        exp << "ILOAD 1 5" << endl;
        exp << "IADD 4 5 3" << endl;
        exp << "ILOAD 10 6" << endl;
        exp << "IMUL 3 6 2" << endl;
        exp << "ILOAD 11 8" << endl;
        exp << "ILOAD 2 9" << endl;
        exp << "IADD 8 9 7" << endl;
        exp << "IMUL 2 7 1" << endl;
        exp << "IMOV 1 10" << endl;

        ostringstream out;

        BCFunction func;
        func.name = "main";
        func.regsNumber = 10;
        func.commands =

        Bytecode bc;
    }
}
