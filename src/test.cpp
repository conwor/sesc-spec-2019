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
    	// x = (3 + 2) * 10 * (11 + 2)
        ostringstream exp;
        exp << "1" << endl << endl;
        exp << "10" << endl <<  "10" << endl;
        exp << "ILOAD 3 1" << endl;
        exp << "ILOAD 1 2" << endl;
        exp << "IADD 1 2 3" << endl;
        exp << "ILOAD 10 4" << endl;
        exp << "IMUL 3 4 5" << endl;
        exp << "ILOAD 11 6" << endl;
        exp << "ILOAD 2 7" << endl;
        exp << "IADD 6 7 8" << endl;
        exp << "IMUL 5 8 9" << endl;
        exp << "IMOV 9 0" << endl;
        exp << "RET" << endl;

        ostringstream out;

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
        var.name = 0;

        AssignOperator assign;
        assign.value = &mul;
        assign.name = 0;


        Function irFunc;

        irFunc.body = {&var, &assign};
        irFunc.name = 3;

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
    	// x = 1
        ostringstream exp;
        exp << "1" << endl;
        exp << endl;
        exp << "2" << endl;
        exp << "2" << endl;
        exp << "ILOAD 10 1" << endl;
        exp << "IMOV 1 0" << endl;
        exp << "RET" << endl;

        VarDefOperator var;
        var.name = 0;

        Expression ten = makeIntExpr(10);

        AssignOperator assign;
        assign.name = 0;
        assign.value = &ten;

        Function func;
        func.name = 3;
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
    	// x = a * 3
        ostringstream exp;
        exp << "1" << endl;
        exp <<  endl;
        exp << "4" << endl;
        exp << "3" << endl;
        exp << "ILOAD 3 2" << endl;
        exp << "IMUL 1 2 3" << endl;
        exp << "IMOV 3 0" << endl;
        exp << "RET" << endl;

        ostringstream out;

        VarDefOperator var(0);

        Expression three = makeIntExpr(3);

        VarDefOperator varADef(1);

        Expression varA = makeVarExpr(1);

        Expression mul3a(&varA, &three, TT_OPERATION, 2);

        AssignOperator assign(&mul3a, 0);

        Function func(3, {&var, &varADef, &assign});

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
    	// a = b
        ostringstream exp;
        ostringstream out;

        exp << "1" << endl;
        exp <<  endl;
        exp << "2" << endl;
        exp << "1" << endl;
        exp << "IMOV 0 1" << endl;
        exp << "RET" << endl;

        VarDefOperator varA(0);
        VarDefOperator varB(1);

        Expression var = makeVarExpr(0);

        AssignOperator assign(&var, 1);

        Function func(3, {&varA, &varB, &assign});

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
    	// b = (a + b + 3) * 3
        ostringstream exp;
        ostringstream out;

        exp << "1" << endl;
        exp <<  endl;
        exp << "7" << endl;
        exp << "6" << endl;
        exp << "ILOAD 3 2" << endl;
        exp << "IADD 0 1 3" << endl;
        exp << "ILOAD 3 4" << endl;
        exp << "IADD 3 4 5" << endl;
        exp << "IMUL 2 5 6" << endl;
        exp << "IMOV 6 1" << endl;
        exp << "RET" << endl;

        VarDefOperator varA(0);
        VarDefOperator varB(1);

        Expression var0 = makeVarExpr(0);
        Expression var1 = makeVarExpr(1);

        Expression three = makeIntExpr(3);

        Expression sumBA(&var0, &var1, TT_OPERATION, 0);
        Expression sumBA3(&sumBA, &three, TT_OPERATION, 0);
        Expression mul3BA3(&three, &sumBA3, TT_OPERATION, 2);

        AssignOperator assign(&mul3BA3, 1);

        Function func(3, {&varA, &varB, &assign});

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
        exp <<  endl;
        exp << "21" << endl;
        exp << "22" << endl;
        exp << "ILOAD 1 1" << endl;
        exp << "ILOAD 2 2" << endl;
        exp << "ICMPLS 1 2 3" << endl;
        exp << "ILOAD 2 4" << endl;
        exp << "ILOAD 1 5" << endl;
        exp << "ICMPLS 4 5 6" << endl;
        exp << "LNOT 6 6" << endl;
        exp << "ICMPEQ 4 5 7" << endl;
        exp << "LNOT 7 8" << endl;
        exp << "LAND 6 8 9" << endl;
        exp << "LAND 3 9 10" << endl;
        exp << "ILOAD 0 11" << endl;
        exp << "ILOAD 2 12" << endl;
        exp << "ICMPEQ 11 12 13" << endl;
        exp << "LNOT 13 14" << endl;
        exp << "ILOAD 1 15" << endl;
        exp << "ILOAD 0 16" << endl;
        exp << "ICMPLS 15 16 17" << endl;
        exp << "LNOT 17 18" << endl;
        exp << "LOR 14 18 19" << endl;
        exp << "LAND 10 19 20" << endl;
        exp << "IMOV 20 0" << endl;
        exp << "RET" << endl;

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

        VarDefOperator var(0);

        AssignOperator assign(&And, 0);

        Function func(3, {&var, &assign});

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
    	// 1 != 2
        ostringstream exp;
        ostringstream out;

        exp << "1" << endl;
        exp <<  endl;
        exp << "5" << endl;
        exp << "5" << endl;
        exp << "ILOAD 0 1" << endl;
        exp << "ILOAD 2 2" << endl;
        exp << "ICMPEQ 1 2 3" << endl;
        exp << "LNOT 3 4" << endl;
        exp << "IMOV 4 0" << endl;
        exp << "RET" << endl;

        Expression one = makeIntExpr(0);
        Expression two = makeIntExpr(2);

        Expression notEq(&one, &two, TT_OPERATION, 7);

        VarDefOperator var(0);

        AssignOperator assign(&notEq, 0);

        Function func(3, {&var, &assign});

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
    	// 0 == 0 && 0 == 1
        ostringstream exp;
        exp << "1" << endl;
        exp <<  endl;
        exp << "8" << endl;
        exp << "8" << endl;
        exp << "ILOAD 1 1" << endl;
        exp << "ILOAD 0 2" << endl;
        exp << "ICMPEQ 1 2 3" << endl;
        exp << "ILOAD 0 4" << endl;
        exp << "ILOAD 0 5" << endl;
        exp << "ICMPEQ 4 5 6" << endl;
        exp << "LAND 3 6 7" << endl;
        exp << "IMOV 7 0" << endl;
        exp << "RET" << endl;

        Expression one = makeIntExpr(1);
        Expression zero = makeIntExpr(0);

        Expression eq10(&one, &zero, TT_OPERATION, 6);
        Expression eq00(&zero, &zero, TT_OPERATION, 6);

        Expression andEq(&eq10, &eq00, TT_OPERATION, 12);

        VarDefOperator var(0);

        AssignOperator assign(&andEq, 0);

        Function func(3, {&var, &assign});

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
    	// if (1 + 2 == 3)
    	// then x = 1
    	// else x = 2
        ostringstream exp;

        exp << "1" << endl;
        exp <<  endl;
        exp << "8" << endl;
        exp << "11" << endl;
        exp << "ILOAD 1 1" << endl;
        exp << "ILOAD 2 2" << endl;
        exp << "IADD 1 2 3" << endl;
        exp << "ILOAD 3 4" << endl;
        exp << "ICMPEQ 3 4 5" << endl;
        exp << "IF 5 9" << endl;
        exp << "ILOAD 2 6" << endl;
        exp << "IMOV 6 0" << endl;
        exp << "GOTO 11" << endl;
        exp << "ILOAD 1 7" << endl;
        exp << "IMOV 7 0" << endl;
        exp << "RET" << endl;

        Expression one = makeIntExpr(1);
        Expression two = makeIntExpr(2);
        Expression three = makeIntExpr(3);

        VarDefOperator var(0);
        AssignOperator assignThen(&one, 0);
        AssignOperator assignElse(&two, 0);

        Expression sum12(&one, &two, TT_OPERATION, 0);
        Expression eq3(&sum12, &three, TT_OPERATION, 6);

        IfOperator ifOp;
        ifOp.condition = &eq3;
        ifOp.thenPart.push_back(&assignThen);
        ifOp.elsePart.push_back(&assignElse);
        Function func(3, {&var, &ifOp});

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
    	// x = 1
    	// while (1 + 9 == 10)
    	// then x = 1 + 9
        stringstream exp;
        exp << "1" << endl;
        exp <<  endl;
        exp << "10" << endl;
        exp << "13" << endl;
        exp << "ILOAD 1 1" << endl;
        exp << "IMOV 1 0" << endl;
        exp << "ILOAD 1 2" << endl;
        exp << "ILOAD 9 3" << endl;
        exp << "IADD 2 3 4" << endl;
        exp << "ILOAD 10 5" << endl;
        exp << "ICMPEQ 4 5 6" << endl;
        exp << "IF 6 13" << endl;
        exp << "ILOAD 1 7" << endl;
        exp << "ILOAD 9 8" << endl;
        exp << "IADD 7 8 9" << endl;
        exp << "IMOV 9 0" << endl;
        exp << "GOTO 2" << endl;
        exp << "RET" << endl;

        stringstream out;


        Expression one = makeIntExpr(1);
        Expression ten = makeIntExpr(10);
        Expression nine = makeIntExpr(9);

        Expression sum19(&one, &nine, TT_OPERATION, 0);
        Expression eq10(&sum19, &ten, TT_OPERATION, 6);

        VarDefOperator var(0);

        AssignOperator assign0(&one, 0);

        AssignOperator assign1(&sum19, 0);
        WhileOperator whileOp;
        whileOp.condition = &eq10;
        whileOp.body.push_back(&assign1);
        Function func(3, {&var, &assign0, &whileOp});

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
        exp << endl;
        exp << "4" << endl;
        exp << "4" << endl;
        exp << "ILOAD 1 1" << endl;
        exp << "ILOAD 2 2" << endl;
        exp << "IADD 1 2 3" << endl;
        exp << "IMOV 3 0" << endl;
        exp << "RET" << endl;
        exp << endl;
        exp << "4" << endl;
        exp << "4" << endl;
        exp << "ILOAD 2 1" << endl;
        exp << "ILOAD 1 2" << endl;
        exp << "ISUB 1 2 3" << endl;
        exp << "IMOV 3 0" << endl;
        exp << "RET" << endl;

        Expression one = makeIntExpr(1);
        Expression two = makeIntExpr(2);

        Expression sum12(&one, &two, TT_OPERATION, IADD);
        Expression sub21(&two, &one, TT_OPERATION, ISUB);

        VarDefOperator var1(0);
        VarDefOperator var2(0);

        AssignOperator assign1(&sum12, 0);
        AssignOperator assign2(&sub21, 0);

        Function func1(4, {&var1, &assign1});
        Function func2(5, {&var2, &assign2});

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
        exp <<  endl;
        exp << "3" << endl;
        exp << "3" << endl;
        exp << "ILOAD 1 0" << endl;
        exp << "ILOAD 2 1" << endl;
        exp << "IMUL 0 1 2" << endl;
        exp << "RET" << endl;

        Expression one = makeIntExpr(1);
        Expression two = makeIntExpr(2);

        Expression mul12(&one, &two, TT_OPERATION, IMUL);

        ExpressionOperator expr(&mul12);

        Function func(3, &expr);

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
        exp <<  endl;
        exp << "1" << endl;
        exp << "1" << endl;
        exp << "ILOAD 1 0" << endl;
        exp << "RET" << endl;

        Expression one = makeIntExpr(1);

        ExpressionOperator exprOp(&one);

        Function func(3, &exprOp);

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

void TestFunctionCall(){
    {
        ostringstream exp;
        ostringstream out;

        exp << "2" << endl;
        exp << endl;
        exp << "3" << endl;
        exp << "4" << endl;
        exp << "ILOAD 1 0" << endl;
        exp << "ILOAD 2 1" << endl;
        exp << "IADD 0 1 2" << endl;
        exp << "CALL 1" << endl;
        exp << "RET" << endl;
        exp << endl;
        exp << "3" << endl;
        exp << "4" << endl;
        exp << "ILOAD 2 0" << endl;
        exp << "ILOAD 1 1" << endl;
        exp << "ISUB 0 1 2" << endl;
        exp << "CALL 0" << endl;
        exp << "RET" << endl;

        Expression one = makeIntExpr(1);
        Expression two = makeIntExpr(2);

        Expression sum12(&one, &two, TT_OPERATION, IADD);
        Expression sub21(&two, &one, TT_OPERATION, ISUB);

        ExpressionOperator sumOp(&sum12);
        ExpressionOperator subOp(&sub21);

        FunctionCall funcOne(4);
        FunctionCall funcZero(5);

        ExpressionOperator funcCallOne(&funcOne);
        ExpressionOperator funcCallZero(&funcZero);

        Function foo(5, {&sumOp, &funcCallOne});
        Function baz(4, {&subOp, &funcCallZero});

        IR ir({&foo, &baz});

        Bytecode *bc = generateBytecode(&ir);

        writeBytecode(bc, out);

        if(out.str() != exp.str()){
            cerr << "ERROR FUNCTIONCALL IS WRONG!" << endl;
            throw system_error();
        } else{
            cerr << "FUNCTIONCALL IS RIGHT!" << endl;
        }
    }
}

