#include <iostream>
#include <vector>
#include <cassert>
#include "base.h"

void interpretFunction(std::vector<BCFunction*> functionArray, int name) {
    long int *regs = new long int[functionArray[name]->regsNumber];
    int i = 0;
    while (true) {
        if(i >= functionArray[name]->commands.size()){
            std::cout << "Command vector: out of bounds error" << std::endl;
            assert(false);
        }
        BCCommand command = functionArray[name]->commands[i];
        int res = command.result;
        int arg0 = command.arg0;
        int arg1 = command.arg1;
        switch (command.type) {
            case IADD:
                regs[res] = regs[arg0] + regs[arg1];
                break;
            case ISUB:
                regs[res] = regs[arg0] - regs[arg1];
                break;
            case IMUL:
                regs[res] = regs[arg0] * regs[arg1];
                break;
            case IDIV:
                regs[res] = regs[arg0] / regs[arg1];
                break;
            case IMOD:
                regs[res] = regs[arg0] % regs[arg1];
                break;
            case LAND:
                regs[res] = (bool)regs[arg0] && (bool)regs[arg1];
                break;
            case LOR:
                regs[res] = (bool)regs[arg0] || (bool)regs[arg1];
                break;
            case LNOT:
                regs[res] = !(bool)regs[arg0];
                break;
            case IMOV:
                regs[res] = regs[arg0];
                break;
            case ILOAD:
                regs[res] = arg0;
                break;
            case ICMPEQ:
                regs[res] = (regs[arg0] == regs[arg1]);
                break;
            case ICMPLS:
                regs[res] = (regs[arg0] < regs[arg1]);
                break;
            case GOTO:
                i = arg0 - 1;
                break;
            case IF:
                if(regs[arg0] != 0) i = arg1 - 1;
                break;
            case RET:
                delete [] regs;
                return;
            case IWRITE:
                std::cout << regs[arg0] << std::endl;
                break;
            case IREAD:
                std::cin >> regs[res];
                break;
            case CALL:
                interpretFunction(functionArray, arg0);
                break;
            default :
                std::cout << "Unknown operator: " << command.type << std::endl;
                assert(false);
                break;
        }
        i++;
    }
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
    testCom[6].arg1 = 12;
    testCom[6].result = 0;

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
    testCom[11].arg0 = 4;
    testCom[11].arg1 = 0;
    testCom[11].result = 0;

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


    std::vector<BCCommand> testCom2(10);
    testCom2[0].type = ILOAD;
    testCom2[0].arg0 = 200;
    testCom2[0].arg1 = 0;
    testCom2[0].result = 1;

    testCom2[1].type = ILOAD;
    testCom2[1].arg0 = 100;
    testCom2[1].arg1 = 0;
    testCom2[1].result = 2;

    testCom2[2].type = IMUL;
    testCom2[2].arg0 = 1;
    testCom2[2].arg1 = 2;
    testCom2[2].result = 3;

    testCom2[3].type = IDIV;
    testCom2[3].arg0 = 3;
    testCom2[3].arg1 = 1;
    testCom2[3].result = 4;

    testCom2[4].type = IMOD;
    testCom2[4].arg0 = 4;
    testCom2[4].arg1 = 1;
    testCom2[4].result = 5;

    testCom2[5].type = ICMPEQ;
    testCom2[5].arg0 = 5;
    testCom2[5].arg1 = 2;
    testCom2[5].result = 6;

    testCom2[6].type = LOR;
    testCom2[6].arg0 = 6;
    testCom2[6].arg1 = 7;
    testCom2[6].result = 8;

    testCom2[7].type = LAND;
    testCom2[7].arg0 = 8;
    testCom2[7].arg1 = 6;
    testCom2[7].result = 9;

    testCom2[8].type = IWRITE;
    testCom2[8].arg0 = 9;
    testCom2[8].arg1 = 0;
    testCom2[8].result = 0;

    testCom2[9].type = RET;
    testCom2[9].arg0 = 0;
    testCom2[9].arg1 = 0;
    testCom2[9].result = 0;



    BCFunction testFunction;
    testFunction.name = "main";
    testFunction.regsNumber = 15;
    testFunction.commands = testCom;

    BCFunction testFunction2;
    testFunction2.name = "CALL_test";
    testFunction2.regsNumber = 10;
    testFunction2.commands = testCom2;

    Bytecode testBC;
    testBC.functions.push_back(&testFunction);
    testBC.functions.push_back(&testFunction2);

    interpret(&testBC);

    return 0;
}