#include "base.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

static vector<string> enumString= {"IADD", "ISUB", "IMUL", "IDIV", "IMOD",
        "LAND", "LOR", "LNOT",
        "IMOV",
        "ILOAD",
        "ICMPEQ", "ICMPLS",
        "GOTO", "IF", "RET",
        "IWRITE", "IREAD",
        "CALL"};

void printFunc(BCFunction *func, ostream& os){
    for(auto command: func->commands){
        os << enumString[static_cast<int>(command.type)] << ' ';
        switch(command.type){
        case IADD:
        case IMUL:
        case IDIV:
        case ISUB:
        case IMOD:
        case LOR:
        case LAND:
        case LNOT:
        case ICMPEQ:
        case ICMPLS:
            os << command.arg0 << ' ' << command.arg1 <<
            ' ' << command.result << endl;
            break;
        case IMOV:
        case ILOAD:
        case IF:
            os << command.arg0 << ' ' << command.arg1 << endl;
            break;
        case RET:
            continue;
            break;
        default:
            os << command.arg0 << endl;
        }
    }
}

void writeBytecode(Bytecode* bc, string filename){
    filename += ".txt";
    ofstream os(filename);
    os << bc->functions.size() << endl;
    for(auto function: bc->functions){
        os << function->name << endl;
        os << function->regsNumber << endl;
        os << function->commands.size() << endl;
        printFunc(function, os);
    }
}

void writeBytecode(Bytecode* bc, ostream& os){
    os << bc->functions.size() << endl;
    for(auto function: bc->functions){
        os << function->name << endl;
        os << function->regsNumber << endl;
        os << function->commands.size() << endl;
        printFunc(function, os);
    }
}
