#include "base.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

static vector<string> enumString = { "IADD", "ISUB", "IMUL", "IDIV", "IMOD",
        "LAND", "LOR", "LNOT", "IMOV", "ILOAD", "ICMPEQ", "ICMPLS", "GOTO",
        "IF", "RET", "IWRITE", "IREAD", "CALL" };

int numOfArgs(BCCommand *command) {
    switch (command->type) {
    case IADD:
    case IMUL:
    case IDIV:
    case ISUB:
    case IMOD:
    case LOR:
    case LAND:
    case ICMPEQ:
    case ICMPLS:
        return 3;
    case IMOV:
    case LNOT:
    case ILOAD:
    case IF:
        return 2;
    case RET:
        return 0;
    default:
        return 1;
    }
}

void printFunc(BCFunction *func, ostream& os) {
    for (auto command : func->commands) {
        os << enumString[command.type] << ' ';
        switch(numOfArgs(&command)){
        case 1:
            os << command.result << endl;
            break;
        case 2:
            os << command.arg0 << ' ' << command.result << endl;
            break;
        case 3:
            os << command.arg0 << ' ' << command.arg1 << ' ' << command.result << endl;
            break;
        }
    }
    os << "RET" << endl;
}

void writeBytecode(Bytecode* bc, ostream& os) {
    os << bc->functions.size() << endl;
    for (auto function : bc->functions) {
        os << endl;
        os << function->regsNumber << endl;
        os << function->commands.size() << endl;
        printFunc(function, os);
    }
}

void writeBytecode(Bytecode* bc, string filename) {
    filename += ".txt";
    ofstream os(filename);
    writeBytecode(bc, os);
}

