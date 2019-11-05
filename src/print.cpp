#include "base.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#define PRINTBCTYPE(type)   \
    os << to_string(type) << ' ';

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
        if(command.type == BCCommandType::IADD ||
           command.type == BCCommandType::IMUL ||
           command.type == BCCommandType::IDIV ||
           command.type == BCCommandType::ISUB ||
           command.type == BCCommandType::IMOD ||
           command.type == BCCommandType::LOR ||
           command.type == BCCommandType::LAND ||
           command.type == BCCommandType::LNOT ||
           command.type == BCCommandType::ICMPEQ||
           command.type == BCCommandType::ICMPLS){
            os << command.arg0 << ' ' << command.arg1 << ' ' << command.result << endl;
        } else if(command.type == BCCommandType::IMOV ||
                command.type == BCCommandType::ILOAD ||
                command.type == BCCommandType::IF){
            os << command.arg0 << ' ' << command.arg1 << endl;
        } else if(command.type == BCCommandType::RET){
            continue;
        } else{
            cout << command.arg0 << endl;
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
