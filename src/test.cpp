#include "base.h"
#include "generate.h"
#include "print.h"
#include <fstream>
#include <sstream>
#include <string>
#include <exception>

using namespace std;

Expression makeIntExpr(int i){
    Expression result;
    result.token.type = TT_LITERAL;
    result.token.value = i;
    return result;
}

Expression makeVarExpr(int reg){
    Expression result;
    result.token.type = TT_IDENT;
    result.token.value = reg;
    return result;
}

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

        Expression three = makeIntExpr(3);
        Expression ten = makeIntExpr(10);
        Expression one = makeIntExpr(1);
        Expression eleven = makeIntExpr(11);
        Expression two = makeIntExpr(2);

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

        Expression ten = makeIntExpr(10);

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
    {
        ostringstream exp;
        exp << "1" << endl;
        exp << "main" << endl;
        exp << "3" << endl;
        exp << "3" << endl;
        exp << "ILOAD 3 2" << endl;
        exp << "IMUL 1 2 1" << endl;
        exp << "IMOV 1 0" << endl;
        ostringstream out;

        VarDefOperator var("a");

        Expression three = makeIntExpr(3);

        Expression varA = makeVarExpr(1);

        Expression mul3a(&varA, &three, TT_OPERATION, 2);

        AssignOperator assign(&mul3a, "a");

        Function func("main", {&var, &assign});

        IR ir(&func);

        Bytecode *bc = generateBytecode(&ir);

        writeBytecode(bc, out);

        if(exp.str() != out.str()){
            cerr << "ERROR ASSIGN VAR SIMPLE EXPRESSION IS WRONG" << endl;
            throw system_error();
        } else{
            cerr << "ASSIGN VAR SIMPLE EXPRESSION IS RIGHT" << endl;
        }
    }
    {
        ostringstream exp;
        ostringstream out;

        exp << "1" << endl;
        exp << "main" << endl;
        exp << "3" << endl;
        exp << "2" << endl;
        exp << "IMOV 0 2" << endl;
        exp << "IMOV 2 1" << endl;

        VarDefOperator varA("a");
        VarDefOperator varB("b");

        Expression var = makeVarExpr(0);

        AssignOperator assign(&var, "b");

        Function func("main", {&varA, &varB, &assign});

        IR ir(&func);

        Bytecode *bc = generateBytecode(&ir);

        writeBytecode(bc, out);

        if(exp.str() != out.str()){
            cerr << "ERROR ASSIGN VAR IS WRONG!" << endl;
            throw system_error();
        } else{
            cerr << "STUPID ASSIGN VAR IS RIGHT!" << endl;
        }
    }
    {
        ostringstream exp;
        ostringstream out;

        exp << "1" << endl;
        exp << "main" << endl;
        exp << "7" << endl;
        exp << "6" << endl;
        exp << "ILOAD 3 3" << endl;
        exp << "IADD 0 1 5" << endl;
        exp << "ILOAD 3 6" << endl;
        exp << "IADD 5 6 4" << endl;
        exp << "IMUL 3 4 2" << endl;
        exp << "IMOV 2 1" << endl;

        VarDefOperator varA("a");
        VarDefOperator varB("b");

        Expression var0 = makeVarExpr(0);
        Expression var1 = makeVarExpr(1);

        Expression three = makeIntExpr(3);

        Expression sumBA(&var0, &var1, TT_OPERATION, 0);
        Expression sumBA3(&sumBA, &three, TT_OPERATION, 0);
        Expression mul3BA3(&three, &sumBA3, TT_OPERATION, 2);

        AssignOperator assign(&mul3BA3, "b");

        Function func("main", {&varA, &varB, &assign});

        IR ir(&func);

        Bytecode *bc = generateBytecode(&ir);

        writeBytecode(bc, out);

        if(out.str() != exp.str()){
            cerr << "ERROR ASSIGN VAR EXPRESSION IS WRONG!" << endl;
            throw system_error();
        } else{
            cerr << "ASSIGN VAR EXPRESSION IS RIGHT!" << endl;
        }
    }
}

void TestIf(){
    {
        ostringstream exp;
        ostringstream out;

        //(1 < 2 && 2 > 1) && (1 >= 0 || 2 != 0)
        exp << "1" << endl;
        exp << "main" << endl;
        exp << "21" << endl;
        exp << "22" << endl;
        exp << "ILOAD 1 4" << endl;
        exp << "ILOAD 2 5" << endl;
        exp << "ICMPLS 4 5 3" << endl;
        exp << "ILOAD 2 7" << endl;
        exp << "ILOAD 1 8" << endl;
        exp << "ICMPLS 7 8 9" << endl;
        exp << "LNOT 9 9" << endl;
        exp << "ICMPEQ 7 8 10" << endl;
        exp << "LNOT 10 11" << endl;
        exp << "LAND 9 11 6" << endl;
        exp << "LAND 3 6 2" << endl;
        exp << "ILOAD 0 14" << endl;
        exp << "ILOAD 2 15" << endl;
        exp << "ICMPEQ 14 15 16" << endl;
        exp << "LNOT 16 13" << endl;
        exp << "ILOAD 1 18" << endl;
        exp << "ILOAD 0 19" << endl;
        exp << "ICMPLS 18 19 20" << endl;
        exp << "LNOT 20 17" << endl;
        exp << "LOR 13 17 12" << endl;
        exp << "LAND 2 12 1" << endl;
        exp << "IMOV 1 0" << endl;

        Expression one = makeIntExpr(1);
        Expression two = makeIntExpr(2);
        Expression zero = makeIntExpr(0);

        Expression Less(&one, &two, TT_OPERATION, 8);
        Expression Big(&two, &one, TT_OPERATION, 9);

        Expression BigEq(&one, &zero, TT_OPERATION, 11);
        Expression NotEq(&zero, &two, TT_OPERATION, 7);

        Expression firstAnd(&Less, &Big, TT_OPERATION, 12);
        Expression Or(&NotEq, &BigEq, TT_OPERATION, 13);

        Expression And(&firstAnd, &Or, TT_OPERATION, 12);

        VarDefOperator var("a");

        AssignOperator assign(&And, "a");

        Function func("main", {&var, &assign});

        IR ir;
        ir.functions = {&func};

        Bytecode *bc = generateBytecode(&ir);

        writeBytecode(bc, out);

        if(out.str() != exp.str()){
            cerr << "ERROR HARD LOGICAL EXPRESSIONS ARE WRONG!" << endl;
            throw system_error();
        } else{
            cerr << "HARD LOGICAL EXPRESSIONS ARE RIGHT!" << endl;
        }
    }
    {
        ostringstream exp;
        ostringstream out;

        exp << "1" << endl;
        exp << "main" << endl;
        exp << "5" << endl;
        exp << "5" << endl;
        exp << "ILOAD 0 2" << endl;
        exp << "ILOAD 2 3" << endl;
        exp << "ICMPEQ 2 3 4" << endl;
        exp << "LNOT 4 1" << endl;
        exp << "IMOV 1 0" << endl;


        Expression one = makeIntExpr(0);
        Expression two = makeIntExpr(2);

        Expression notEq(&one, &two, TT_OPERATION, 7);

        VarDefOperator var("a");

        AssignOperator assign(&notEq, "a");

        Function func("main", {&var, &assign});

        IR ir(&func);

        Bytecode *bc = generateBytecode(&ir);

        writeBytecode(bc, out);

        if(exp.str() != out.str()){
            cerr << "ERROR NOT EQUAL IS WRONG!" << endl;
            throw system_error();
        } else {
            cerr << "NOT EQUAL IS RIGHT!" << endl;
        }
    }
    {
        ostringstream exp;
        exp << "1" << endl;
        exp << "main" << endl;
        exp << "8" << endl;
        exp << "8" << endl;
        exp << "ILOAD 1 3" << endl;
        exp << "ILOAD 0 4" << endl;
        exp << "ICMPEQ 3 4 2" << endl;
        exp << "ILOAD 0 6" << endl;
        exp << "ILOAD 0 7" << endl;
        exp << "ICMPEQ 6 7 5" << endl;
        exp << "LAND 2 5 1" << endl;
        exp << "IMOV 1 0" << endl;

        Expression one = makeIntExpr(1);
        Expression zero = makeIntExpr(0);

        Expression eq10(&one, &zero, TT_OPERATION, 6);
        Expression eq00(&zero, &zero, TT_OPERATION, 6);

        Expression andEq(&eq10, &eq00, TT_OPERATION, 12);

        VarDefOperator var("a");

        AssignOperator assign(&andEq, "a");

        Function func("main", {&var, &assign});

        IR ir;
        ir.functions = {&func};

        Bytecode *bc = generateBytecode(&ir);

        ostringstream out;
        writeBytecode(bc, out);

        if(out.str() != exp.str()){
            cerr << "LOGICAL EXPRESSIONS ARE WRONG!" << endl;
            throw system_error();
        } else{
            cerr << "LOGICAL EXPRESSIONS ARE RIGHT!" << endl;
        }
    }
    {
        ostringstream exp;
        exp << "1" << endl;
        exp << "main" << endl;
        exp << "8" << endl;
        exp << "11" << endl;
        exp << "ILOAD 1 3" << endl;
        exp << "ILOAD 2 4" << endl;
        exp << "IADD 3 4 2" << endl;
        exp << "ILOAD 3 5" << endl;
        exp << "ICMPEQ 2 5 1" << endl;
        exp << "IF 1 9" << endl;
        exp << "ILOAD 2 6" << endl;
        exp << "IMOV 6 0" << endl;
        exp << "GOTO 11" << endl;
        exp << "ILOAD 1 7" << endl;
        exp << "IMOV 7 0" << endl;


        Expression one = makeIntExpr(1);
        Expression two = makeIntExpr(2);
        Expression three = makeIntExpr(3);

        VarDefOperator var("a");
        AssignOperator assignThen(&one, "a");
        AssignOperator assignElse(&two, "a");

        Expression sum12(&one, &two, TT_OPERATION, 0);
        Expression eq3(&sum12, &three, TT_OPERATION, 6);

        IfOperator ifOp;
        ifOp.condition = &eq3;
        ifOp.thenPart.push_back(&assignThen);
        ifOp.elsePart.push_back(&assignElse);
        Function func("main", {&var, &ifOp});

        IR ir;
        ir.functions = {&func};
        Bytecode *bc = generateBytecode(&ir);

        ostringstream out;
        writeBytecode(bc, out);
        if(out.str() != exp.str()){
            cerr << "ERROR IF IS WRONG!" << endl;
            throw system_error();
        } else{
            cerr << "IF IS RIGHT!" << endl;
        }
    }
}

void TestWhile(){
    {
        stringstream exp;
        exp << "1" << endl;
        exp << "main" << endl;
        exp << "10" << endl;
        exp << "13" << endl;
        exp << "ILOAD 1 1" << endl;
        exp << "IMOV 1 0" << endl;
        exp << "ILOAD 1 4" << endl;
        exp << "ILOAD 9 5" << endl;
        exp << "IADD 4 5 3" << endl;
        exp << "ILOAD 10 6" << endl;
        exp << "ICMPEQ 3 6 2" << endl;
        exp << "IF 2 13" << endl;
        exp << "ILOAD 1 8" << endl;
        exp << "ILOAD 9 9" << endl;
        exp << "IADD 8 9 7" << endl;
        exp << "IMOV 7 0" << endl;
        exp << "GOTO 2" << endl;
        stringstream out;


        Expression one = makeIntExpr(1);
        Expression ten = makeIntExpr(10);
        Expression nine = makeIntExpr(9);

        Expression sum19(&one, &nine, TT_OPERATION, 0);
        Expression eq10(&sum19, &ten, TT_OPERATION, 6);

        VarDefOperator var("a");

        AssignOperator assign0(&one, "a");

        AssignOperator assign1(&sum19, "a");
        WhileOperator whileOp;
        whileOp.condition = &eq10;
        whileOp.body.push_back(&assign1);
        Function func("main", {&var, &assign0, &whileOp});

        IR ir(&func);

        Bytecode *bc = generateBytecode(&ir);

        writeBytecode(bc, out);

        if(out.str() != exp.str()){
            cerr << "ERROR WHILE IS WRONG!" << endl;
            throw system_error();
        } else{
            cerr << "WHILE IS RIGHT!" << endl;
        }
    }
}

void TestMultiFunc(){
    {
        ostringstream exp;
        ostringstream out;

        exp << "2" << endl;
        exp << "foo" << endl;
        exp << "4" << endl;
        exp << "4" << endl;
        exp << "ILOAD 1 2" << endl;
        exp << "ILOAD 2 3" << endl;
        exp << "IADD 2 3 1" << endl;
        exp << "IMOV 1 0" << endl;
        exp << "baz" << endl;
        exp << "4" << endl;
        exp << "4" << endl;
        exp << "ILOAD 2 2" << endl;
        exp << "ILOAD 1 3" << endl;
        exp << "ISUB 2 3 1" << endl;
        exp << "IMOV 1 0" << endl;

        Expression one = makeIntExpr(1);
        Expression two = makeIntExpr(2);

        Expression sum12(&one, &two, TT_OPERATION, IADD);
        Expression sub21(&two, &one, TT_OPERATION, ISUB);

        VarDefOperator var1("a");
        VarDefOperator var2("a");

        AssignOperator assign1(&sum12, "a");
        AssignOperator assign2(&sub21, "a");

        Function func1("foo", {&var1, &assign1});
        Function func2("baz", {&var2, &assign2});

        IR ir({&func1, &func2});

        Bytecode *bc = generateBytecode(&ir);

        writeBytecode(bc, out);

        if(out.str() != exp.str()){
            cerr << "ERROR MULTI-FUNC IS WRONG!" << endl;
            throw system_error();
        } else{
            cerr << "MULTI-FUNC IS RIGHT!" << endl;
        }
    }
}

void TestExpressionOp(){
    {
        ostringstream exp;
        ostringstream out;

        exp << "1" << endl;
        exp << "main" << endl;
        exp << "3" << endl;
        exp << "3" << endl;
        exp << "ILOAD 1 1" << endl;
        exp << "ILOAD 2 2" << endl;
        exp << "IMUL 1 2 0" << endl;

        Expression one = makeIntExpr(1);
        Expression two = makeIntExpr(2);

        Expression mul12(&one, &two, TT_OPERATION, IMUL);

        ExpressionOperator expr(&mul12);

        Function func("main", &expr);

        IR ir(&func);

        Bytecode *bc = generateBytecode(&ir);

        writeBytecode(bc, out);

        if(out.str() != exp.str()){
            cerr << "ERROR EXPRESSION OPERATOR IS WRONG!" << endl;
            throw system_error();
        } else{
            cerr << "EXPRESSION OPERATOR IS RIGHT!" << endl;
        }
    }
    {
        ostringstream exp;
        ostringstream out;

        exp << "1" << endl;
        exp << "main" << endl;
        exp << "1" << endl;
        exp << "1" << endl;
        exp << "ILOAD 1 0" << endl;


        Expression one = makeIntExpr(1);

        ExpressionOperator exprOp(&one);

        Function func("main", &exprOp);

        IR ir(&func);

        Bytecode *bc = generateBytecode(&ir);

        writeBytecode(bc, out);

        if(out.str() != exp.str()){
            cerr << "ERROR SIMPLE EXPRESSIONS ARE WRONG!" << endl;
            throw system_error();
        } else{
            cerr << "SIMPLE EXPRESSIONS ARE RIGHT!" << endl;
        }
    }
}

