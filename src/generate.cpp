#include <iostream>
#include <map>
#include "base.h"

using namespace std;

int regsNumber = 0;

BCCommandType commandTypeCast(OperatorType type) {
    switch (type) {
    case (ADD):
        return BCCommandType::IADD;
    case (SUB):
        return BCCommandType::ISUB;
    case (DIV):
        return BCCommandType::IDIV;
    case (MUL):
        return BCCommandType::IMUL;
    case (MOD):
        return BCCommandType::IMOD;
    default:
        cerr << "ERROR";
        exit(1);
    }
}

//EXPRESSION
void addExpression(BCFunction *result, Expression *expr,
        const map<string, int>& name_to_index, int commandResult) {
    int left, right;
    TokenType leftT, rightT;
    leftT = expr->lArg->token.type;
    rightT = expr->rArg->token.type;

    left = regsNumber;
    regsNumber++;
    if (leftT != TokenType::TT_LITERAL) {
        addExpression(result, expr->lArg, name_to_index, left);
    }

    right = regsNumber;
    regsNumber++;
    if (rightT != TokenType::TT_LITERAL) {
        addExpression(result, expr->rArg, name_to_index, right);
    }

    if (leftT == TokenType::TT_LITERAL && rightT == TokenType::TT_LITERAL) {
        BCCommand load0, load1;

        load0.arg0 = expr->lArg->token.value;
        load0.arg1 = left;
        load0.type = BCCommandType::ILOAD;

        load1.arg0 = expr->rArg->token.value;
        load1.arg1 = right;
        load1.type = BCCommandType::ILOAD;

        result->commands.push_back(load0);
        result->commands.push_back(load1);
    } else if(leftT != TokenType::TT_LITERAL){
        BCCommand load;

        load.arg0 = expr->rArg->token.value;
        load.arg1 = right;
        load.type = BCCommandType::ILOAD;


        result->commands.push_back(load);
    } else if(rightT != TokenType::TT_LITERAL){
        BCCommand load;

        load.arg0 = expr->lArg->token.value;
        load.arg1 = left;
        load.type = BCCommandType::ILOAD;

        result->commands.push_back(load);
    }

    BCCommand com;
    com.arg0 = left;
    com.arg1 = right;
    com.result = commandResult;
    com.type = commandTypeCast(static_cast<OperatorType>(expr->token.value));

    result->commands.push_back(com);
}

BCFunction *generateFunction(Function *func) {
    BCFunction *result = new BCFunction;
    result->name = func->name;
    map<string, int> name_to_index;
    for (const auto& i : func->body) {

        //ASSIGN
        AssignOperator* asPtr = dynamic_cast<AssignOperator*>(i);
        if (asPtr != NULL) {
            BCCommand assign;

            int commandResult = regsNumber;
            regsNumber++;
            addExpression(result, asPtr->value, name_to_index, commandResult);

            assign.arg0 = name_to_index[asPtr->variableName];
            assign.arg1 = commandResult;
            assign.type = BCCommandType::IMOV;

            result->commands.push_back(assign);
        }

        //VARDEF
        VarDefOperator* varPtr = dynamic_cast<VarDefOperator*>(i);
        if (varPtr != NULL) {
            name_to_index[varPtr->name] = regsNumber;
            regsNumber++;
        }

    }
    result->regsNumber = regsNumber;
    regsNumber = 0;
    return result;
}

Bytecode* generateBytecode(IR* ir) {
    Bytecode *result = new Bytecode;
    for (auto *i : ir->functions) {
        BCFunction *func = generateFunction(i);
        result->functions.push_back(func);
    }
    return result;
}

//int main(){
//    VarDefOperator var;
//    var.name = "a";
//
//
//    Token token;
//    token.type = TokenType::TT_LITERAL;
//    token.value = 42;
//
//
//    Expression expr;
//    expr.token = token;
//    AssignOperator assign;
//    assign.value = &expr;
//    assign.variableName = "a";
//
//
//    Function func;
//    func.name = "name";
//    func.body.push_back(&var);
//    func.body.push_back(&assign);
//    IR ir;
//    ir.functions.push_back(&func);
//
//
//    BCCommand Assign;
//    Assign.type = BCCommandType::ILOAD;
//    Assign.arg0 = 1;
//    Assign.arg1 = 42;
//    BCFunction BFunc;
//    BFunc.commands = {Assign};
//    BFunc.regsNumber = 1;
//    BFunc.name = "name";
//
//
//    return 0;
//}
