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

/*
 * addExpression casts expressions like:
 * (3 + 123) * (12 * (37 - 12))
 * in BCCommands like IADD or ILOAD
 */
void addExpression(BCFunction *result, Expression *expr, int commandResult) {
    int left, right;
    TokenType leftT, rightT;
    leftT = expr->lArg->token.type;
    rightT = expr->rArg->token.type;

    //Using DFS to go through the tree
    left = regsNumber;
    regsNumber++;
    if (leftT != TokenType::TT_LITERAL) {
        addExpression(result, expr->lArg, left);
    }

    right = regsNumber;
    regsNumber++;
    if (rightT != TokenType::TT_LITERAL) {
        addExpression(result, expr->rArg, right);
    }

    if (leftT == TokenType::TT_LITERAL && rightT == TokenType::TT_LITERAL) {
        BCCommand lLoad(expr->lArg->token.value, left, BCCommandType::ILOAD);
        BCCommand rLoad(expr->rArg->token.value, right, BCCommandType::ILOAD);

        result->commands.push_back(lLoad);
        result->commands.push_back(rLoad);
    } else if (rightT == TokenType::TT_LITERAL) {
        BCCommand load(expr->rArg->token.value, right, BCCommandType::ILOAD);

        result->commands.push_back(load);
    } else if (leftT == TokenType::TT_LITERAL) {
        BCCommand load(expr->lArg->token.value, left, BCCommandType::ILOAD);

        result->commands.push_back(load);
    }

    //Casts OperatorType into the BCCommandType
    BCCommandType comType = commandTypeCast(
            static_cast<OperatorType>(expr->token.value));

    BCCommand com(left, right, commandResult, comType);

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

            assign.arg1 = name_to_index[asPtr->variableName];
            assign.type = BCCommandType::IMOV;

            if (asPtr->value->token.type != TokenType::TT_LITERAL) {
                //For a := 17 + 21 * 3
                int commandResult = regsNumber;
                regsNumber++;
                addExpression(result, asPtr->value, commandResult);
                assign.arg0 = commandResult;
            } else {
                //For a := 12
                BCCommand load(asPtr->value->token.value, regsNumber,
                        BCCommandType::ILOAD);
                result->commands.push_back(load);

                assign.arg0 = regsNumber;
                regsNumber++;
            }

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
