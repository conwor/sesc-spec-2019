#include "base.h"
#include <fstream>
#include <cassert>

Bytecode* readBytecode(std::string fileName) {
    Bytecode* BC = new Bytecode;

    std::ifstream fin(fileName);
	if (!fin.is_open()) {
	    std::cout << "file error" << std::endl;
	    assert(false);
	}

    int fN = 0;
    fin >> fN;
    for (; fN > 0; fN--) {
        BCFunction* newFunc = new BCFunction;

        int name = 0;
        fin >> name;
        newFunc->name = name;

        int regs = 0;
        fin >> regs;
        newFunc->regsNumber = regs;

        int cN = 0;
        fin >> cN;
        for (; cN > 0; cN--) {
            BCCommand com;

            char type[10] = "";
            fin >> type;

            int A0 = 0;
            int A1 = 0;
            int R = 0;

            if (std::strncmp(type, "IADD", 4) == 0) {
                com.type = IADD;
                fin >> A0 >> A1 >> R;
                com.arg0 = A0;
                com.arg1 = A1;
                com.result = R;
            }
            else if (std::strncmp(type, "ISUB", 4) == 0) {
                com.type = ISUB;
                fin >> A0 >> A1 >> R;
                com.arg0 = A0;
                com.arg1 = A1;
                com.result = R;
            }
            else if (std::strncmp(type, "IMUL", 4) == 0) {
                com.type = IMUL;
                fin >> A0 >> A1 >> R;
                com.arg0 = A0;
                com.arg1 = A1;
                com.result = R;
            }
            else if (std::strncmp(type, "IDIV", 4) == 0) {
                com.type = IDIV;
                fin >> A0 >> A1 >> R;
                com.arg0 = A0;
                com.arg1 = A1;
                com.result = R;
            }
            else if (std::strncmp(type, "IMOD", 4) == 0) {
                com.type = IMOD;
                fin >> A0 >> A1 >> R;
                com.arg0 = A0;
                com.arg1 = A1;
                com.result = R;
            }
            else if (std::strncmp(type, "LAND", 4) == 0) {
                com.type = LAND;
                fin >> A0 >> A1 >> R;
                com.arg0 = A0;
                com.arg1 = A1;
                com.result = R;
            }
            else if (std::strncmp(type, "LOR", 3) == 0) {
                com.type = LOR;
                fin >> A0 >> A1 >> R;
                com.arg0 = A0;
                com.arg1 = A1;
                com.result = R;
            }
            else if (std::strncmp(type, "LNOT", 4) == 0) {
                com.type = LNOT;
                fin >> A0 >> R;
                com.arg0 = A0;
                com.result = R;
            }
            else if (std::strncmp(type, "IMOV", 4) == 0) {
                com.type = IMOV;
                fin >> A0 >> R;
                com.arg0 = A0;
                com.result = R;
            }
            else if (std::strncmp(type, "ILOAD", 5) == 0) {
                com.type = ILOAD;
                fin >> A0 >> R;
                com.arg0 = A0;
                com.result = R;
            }
            else if (std::strncmp(type, "ICMPEQ", 6) == 0) {
                com.type = ICMPEQ;
                fin >> A0 >> A1 >> R;
                com.arg0 = A0;
                com.arg1 = A1;
                com.result = R;
            }
            else if (std::strncmp(type, "ICMPLS", 6) == 0) {
                com.type = ICMPLS;
                fin >> A0 >> A1 >> R;
                com.arg0 = A0;
                com.arg1 = A1;
                com.result = R;
            }
            else if (std::strncmp(type, "GOTO", 4) == 0) {
                com.type = GOTO;
                fin >> A0;
                com.arg0 = A0;
            }
            else if (std::strncmp(type, "IF", 2) == 0) {
                com.type = IF;
                fin >> A0 >> A1;
                com.arg0 = A0;
                com.arg1 = A1;
            }
            else if (std::strncmp(type, "RET", 3) == 0) {
                com.type = RET;
            }
            else if (std::strncmp(type, "IWRITE", 6) == 0) {
                com.type = IWRITE;
                fin >> A0;
                com.arg0 = A0;
            }
            else if (std::strncmp(type, "IREAD", 5) == 0) {
                com.type = IREAD;
                fin >> R;
                com.result = R;
            }
            else if (std::strncmp(type, "CALL", 4) == 0) {
                com.type = CALL;
                fin >> A0;
                com.arg0 = A0;
            }
            else{
                std::cout << "unknown operator";
                assert(false);
            }

            newFunc->commands.push_back(com);
        }
        BC->functions.push_back(newFunc);
    }
    fin.close();
    return BC;
}