#include <iostream>
#include <vector>
#include "base.h"

Bytecode* readBytecode(std::string fileName);


void interpretFunction(std::vector<BCFunction*> functionArray, int name) {
    long int *regArray = new long int[functionArray[name]->regsNumber];
    for(int i = 0; i < functionArray[name]->regsNumber; i++) regArray[i] = 0;

    for (int i = 0; i < functionArray[name]->commands.size(); i++) {
        BCCommand command = functionArray[name]->commands[i];
        int resReg = command.result;
        int arg0Reg = command.arg0;
        int arg1Reg = command.arg1;
        switch (command.type) {
            case IADD:
                regArray[resReg] = regArray[arg0Reg] + regArray[arg1Reg];
                break;
            case ISUB:
                regArray[resReg] = regArray[arg0Reg] - regArray[arg1Reg];
                break;
            case IMUL:
                regArray[resReg] = regArray[arg0Reg] * regArray[arg1Reg];
                break;
            case IDIV:
                regArray[resReg] = regArray[arg0Reg] / regArray[arg1Reg];
                break;
            case IMOD:
                regArray[resReg] = regArray[arg0Reg] % regArray[arg1Reg];
                break;
            case LAND:
                regArray[resReg] = (bool)regArray[arg0Reg] && (bool)regArray[arg1Reg];
                break;
            case LOR:
                regArray[resReg] = (bool)regArray[arg0Reg] || (bool)regArray[arg1Reg];
                break;
            case LNOT:
                regArray[resReg] = !(bool)regArray[arg0Reg];
                break;
            case IMOV:
                regArray[resReg] = regArray[arg0Reg];
                break;
            case ILOAD:
                regArray[resReg] = arg0Reg;
                break;
            case ICMPEQ:
                regArray[arg0Reg] == regArray[arg1Reg] ? regArray[resReg] = 1 : regArray[resReg] = 0;
                break;
            case ICMPLS:
                regArray[arg0Reg] < regArray[arg1Reg] ? regArray[resReg] = 1 : regArray[resReg] = 0;
                break;
            case GOTO:
                i = resReg - 1;
                continue;
            case IF:
                if(regArray[arg0Reg] != 0) i = resReg - 1;
                continue;
            case RET:
                delete [] regArray;
                return;
            case IWRITE:
                std::cout << regArray[arg0Reg] << std::endl;
                break;
            case IREAD:
                std::cin >> regArray[resReg];
                break;
            case CALL:
                interpretFunction(functionArray, arg0Reg);
                break;
        }
    }
    delete [] regArray;
}

void interpret(Bytecode* bytecode) {
    interpretFunction(bytecode->functions, 0);
}

int main() {
    //тестовая программа
    std::vector<BCCommand> testCom(15);
    testCom[0].type = IREAD;
    testCom[0].arg0 = 0;
    testCom[0].arg1 = 0;
    testCom[0].result = 0;

    testCom[1].type = ILOAD;
    testCom[1].arg0 = 1;
    testCom[1].arg1 = 0;
    testCom[1].result = 1;

    testCom[2].type = ILOAD;
    testCom[2].arg0 = 1;
    testCom[2].arg1 = 0;
    testCom[2].result = 2;

    testCom[3].type = ILOAD;
    testCom[3].arg0 = 1;
    testCom[3].arg1 = 0;
    testCom[3].result = 3;

    testCom[4].type = ICMPLS;
    testCom[4].arg0 = 1;
    testCom[4].arg1 = 0;
    testCom[4].result = 4;

    testCom[5].type = LNOT;
    testCom[5].arg0 = 4;
    testCom[5].arg1 = 0;
    testCom[5].result = 4;

    testCom[6].type = IF;
    testCom[6].arg0 = 4;
    testCom[6].arg1 = 0;
    testCom[6].result = 12;

    testCom[7].type = IADD;
    testCom[7].arg0 = 2;
    testCom[7].arg1 = 3;
    testCom[7].result = 3;

    testCom[8].type = ISUB;
    testCom[8].arg0 = 3;
    testCom[8].arg1 = 2;
    testCom[8].result = 2;

    testCom[9].type = ILOAD;
    testCom[9].arg0 = 1;
    testCom[9].arg1 = 0;
    testCom[9].result = 4;

    testCom[10].type = IADD;
    testCom[10].arg0 = 1;
    testCom[10].arg1 = 4;
    testCom[10].result = 1;

    testCom[11].type = GOTO;
    testCom[11].arg0 = 0;
    testCom[11].arg1 = 0;
    testCom[11].result = 4;

    testCom[12].type = IWRITE;
    testCom[12].arg0 = 3;
    testCom[12].arg1 = 0;
    testCom[12].result = 0;

    testCom[13].type = CALL;
    testCom[13].arg0 = 1;
    testCom[13].arg1 = 0;
    testCom[13].result = 0;

    testCom[14].type = RET;
    testCom[14].arg0 = 0;
    testCom[14].arg1 = 0;
    testCom[14].result = 0;


    std::vector<BCCommand> testCom2(3);
    testCom2[0].type = ILOAD;
    testCom2[0].arg0 = 404;
    testCom2[0].arg1 = 0;
    testCom2[0].result = 1;

    testCom2[1].type = IWRITE;
    testCom2[1].arg0 = 1;
    testCom2[1].arg1 = 0;
    testCom2[1].result = 0;

    testCom2[2].type = RET;
    testCom2[2].arg0 = 0;
    testCom2[2].arg1 = 0;
    testCom2[2].result = 0;


    BCFunction testFunction;
    testFunction.name = "main";
    testFunction.regsNumber = 15;
    testFunction.commands = testCom;

    BCFunction testFunction2;
    testFunction2.name = "CALL_test";
    testFunction2.regsNumber = 3;
    testFunction2.commands = testCom2;

    Bytecode testBC;
    testBC.functions.push_back(&testFunction);
    testBC.functions.push_back(&testFunction2);

    interpret(&testBC);

    return 0;
}