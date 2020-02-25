#include "base.h"
#include <cassert>

Bytecode* readBytecode(std::string fileName);

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

    interpret(readBytecode(std::string("C:\\sesc-spec\\src\\input.txt")));

    return 0;
}