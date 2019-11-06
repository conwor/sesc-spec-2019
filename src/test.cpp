#include "base.h"
#include <fstream>
#include <sstream>
#include <string>
#include <exception>

using namespace std;

void TestAssign() {
    {
        ostringstream exp;
        exp << "1" << endl << "main" << endl;
        exp << "10" << endl <<  "10" << endl;
        exp << "ILOAD 3 4" << endl;
        exp << "ILOAD 1 5" << endl;
        exp << "IADD 4 5 3" << endl;
        exp << "ILOAD 10 6" << endl;
        exp << "IMUL 3 6 2" << endl;
        exp << "ILOAD 11 8" << endl;
        exp << "ILOAD 2 9" << endl;
        exp << "IADD 8 9 7" << endl;
        exp << "IMUL 2 7 1" << endl;
        exp << "IMOV 1 0" << endl;

        ostringstream out;

        BCCommand load34(3, 4, ILOAD);
        BCCommand load15(1, 5, ILOAD);
        BCCommand load106(10, 6, ILOAD);
        BCCommand load118(11, 8, ILOAD);
        BCCommand load29(2, 9, ILOAD);

        BCCommand add453(4, 5, 3, IADD);
        BCCommand add897(8, 9, 7, IADD);

        BCCommand mul362(3, 6, 2, IMUL);
        BCCommand mul271(2, 7, 1, IMUL);

        BCCommand move(1, 0, IMOV);

        BCFunction func;
        func.name = "main";
        func.regsNumber = 10;
        func.commands = {load34, load15, add453, load106, mul362, load118,
                load29, add897, mul271, move};

        Bytecode bc;
        bc.functions.push_back(&func);

        writeBytecode(&bc, out);

        if(out.str() != exp.str()){
            cerr << "Wrong TEST at 56 line" << endl;
            throw system_error();
        }

        Expression three(3);
        Expression ten(10);
        Expression one(1);
        Expression eleven(11);
        Expression two(2);

        Expression sum31(&three, &one, TT_OPERATION, 0);
        Expression sum112(&eleven, &two, TT_OPERATION, 0);
        Expression mul10(&sum31, &ten, TT_OPERATION, 2);
        Expression mul(&mul10, &sum112, TT_OPERATION, 2);

        VarDefOperator var;
        var.name = "a";

        AssignOperator assign;
        assign.value = &mul;
        assign.variableName = "a";


        Function irFunc;

        irFunc.body = {&var, &assign};
        irFunc.name = "main";

        IR ir;

        ir.functions = {&irFunc};

        Bytecode *b = generateBytecode(&ir);

        ostringstream ss;

        writeBytecode(b, ss);
        if(ss.str() != exp.str()){
            cerr << "ERROR ASSIGN EXPRESSION IS WRONG!" << endl;
            throw system_error();
        } else{
            cerr << "ASSIGN EXPRESSION IS RIGHT!" << endl;
        }
    }
    {
        ostringstream exp;
        exp << "1" << endl;
        exp << "main" << endl;
        exp << "2" << endl;
        exp << "2" << endl;
        exp << "ILOAD 10 1" << endl;
        exp << "IMOV 1 0" << endl;

        VarDefOperator var;
        var.name = "a";

        Expression ten(10);

        AssignOperator assign;
        assign.variableName = "a";
        assign.value = &ten;

        Function func;
        func.name = "main";
        func.body = {&var, &assign};

        IR ir;
        ir.functions = {&func};

        Bytecode *bc = generateBytecode(&ir);

        ostringstream out;
        writeBytecode(bc, out);

        if(out.str() != exp.str()){
            cerr << "ERROR ASSIGN NUM IS WRONG!" << endl;
            throw system_error();
        } else{
            cerr << "ASSIGN NUM IS RIGHT!" << endl;
        }
    }
}

