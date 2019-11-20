#include <iostream>
#include <map>
#include "base.h"

using namespace std;

class regsInfo {
public:
    int allocateRegister() {
        regsNumber++;
        return regsNumber - 1;
    }

    void allocateVariable(string varName) {
        nameToIndex[varName] = regsNumber;
        regsNumber++;
    }

    int registerOf(string varName) {
        return nameToIndex.at(varName);
    }

    int getRegsNum() {
        return regsNumber;
    }
private:
    int regsNumber = 0;
    map<string, int> nameToIndex;
};

/*
 * addExpression casts expressions like:
 * (3 + 123) * (12 * (37 - 12)) or (1 == 2) && (3 != 4) or (a + 2) * 10
 * in BCCommands like IADD, ILOAD or LAND
 */
void addExpression(vector<BCCommand> *result, Expression *expr,
        int commandResult, regsInfo *info) {
    int left, right;
    TokenType leftT, rightT;

    switch (expr->token.type) {
    case TT_IDENT: {
        BCCommand ass(expr->token.value, commandResult, BCCommandType::IMOV);
        result->push_back(ass);
        return;
    }
    case TT_LITERAL: {
        BCCommand load(expr->token.value, commandResult, BCCommandType::ILOAD);
        result->push_back(load);
        return;
    }
    }

    leftT = expr->lArg->token.type;
    rightT = expr->rArg->token.type;

    //Using DFS to go through the tree

    switch (leftT) {
    case TT_LITERAL: {
        left = info->allocateRegister();
        BCCommand lLoad(expr->lArg->token.value, left, BCCommandType::ILOAD);
        result->push_back(lLoad);
        break;
    }
    case TT_IDENT:
        left = expr->lArg->token.value;
        break;
    case TT_OPERATION:
        left = info->allocateRegister();
        addExpression(result, expr->lArg, left, info);
        break;
    }

    if (!(expr->token.value == LNOT && rightT == TT_OPERATION)) {
        switch (rightT) {
        case TT_LITERAL: {
            right = info->allocateRegister();
            BCCommand rLoad(expr->rArg->token.value, right,
                    BCCommandType::ILOAD);
            result->push_back(rLoad);
            break;
        }
        case TT_IDENT:
            right = expr->rArg->token.value;
            break;
        case TT_OPERATION:
            right = info->allocateRegister();
            addExpression(result, expr->rArg, right, info);
            break;
        }
    }

//Casts OperatorType into the BCCommandType
    BCCommandType comType;
    switch (expr->token.value) {
    case ADD:
        comType = BCCommandType::IADD;
        break;
    case SUB:
        comType = BCCommandType::ISUB;
        break;
    case DIV:
        comType = BCCommandType::IDIV;
        break;
    case MUL:
        comType = BCCommandType::IMUL;
        break;
    case MOD:
        comType = BCCommandType::IMOD;
        break;
    case AND:
        comType = BCCommandType::LAND;
        break;
    case OR:
        comType = BCCommandType::LOR;
        break;
    case EQ:
        comType = BCCommandType::ICMPEQ;
        break;
    case LS:
        comType = BCCommandType::ICMPLS;
        break;
    case NOT:
        comType = BCCommandType::LNOT;
        break;
    case BG: {
        comType = BCCommandType::LAND;

        int less = info->allocateRegister();

        int eq = info->allocateRegister();

        int notEq = info->allocateRegister();

        BCCommand lessCom(left, right, less, ICMPLS);
        result->push_back(lessCom);
        BCCommand notLess(less, less, LNOT);
        result->push_back(notLess);

        BCCommand eqCom(left, right, eq, ICMPEQ);
        result->push_back(eqCom);
        BCCommand notEqual(eq, notEq, LNOT);
        result->push_back(notEqual);

        left = less;
        right = notEq;
        break;
    }
    case BG_EQ: {
        comType = BCCommandType::LNOT;

        int less = info->allocateRegister();

        BCCommand lessCom(left, right, less, ICMPLS);
        result->push_back(lessCom);

        left = less;
        break;
    }
    case NE: {
        comType = BCCommandType::LNOT;

        int eq = info->allocateRegister();

        BCCommand eqCom(left, right, eq, ICMPEQ);
        result->push_back(eqCom);
        left = eq;
        break;
    }
    default:
        cerr << "Error unknown OperatorType "
                << static_cast<int>(expr->token.value) << "at commandTypeCast"
                << endl;
        exit(1);
    }

    BCCommand com;
    if (comType == LNOT) {
        com.arg0 = left;
        com.result = commandResult;
        com.type = comType;
    } else {
        com.arg0 = left;
        com.arg1 = right;
        com.result = commandResult;
        com.type = comType;
    }

    result->push_back(com);
}

vector<BCCommand> generateCommands(vector<Operator*> *operators,
        regsInfo *info) {
    vector<BCCommand> result;
    for (auto _operator : *operators) {

        //ASSIGN
        AssignOperator* asPtr = dynamic_cast<AssignOperator*>(_operator);
        if (asPtr != NULL) {
            BCCommand assign;

            assign.result = info->registerOf(asPtr->variableName);
            assign.type = BCCommandType::IMOV;

            int commandResult = info->allocateRegister();
            addExpression(&result, asPtr->value, commandResult, info);
            assign.arg0 = commandResult;

            result.push_back(assign);
        }

        //VARDEF
        VarDefOperator* varPtr = dynamic_cast<VarDefOperator*>(_operator);
        if (varPtr != NULL) {
            info->allocateVariable(varPtr->name);
        }

        /*
         * IF
         *
         * If condition is true, it goes to the then part's start.
         * If condition is false, it goes to the else part, that after the if command.
         * There is a goto through the then part in the end of else part.
         */
        IfOperator *ifPtr = dynamic_cast<IfOperator*>(_operator);
        if (ifPtr != NULL) {
            int commandResult = info->allocateRegister();

            addExpression(&result, ifPtr->condition, commandResult, info);

            BCCommand ifCom;
            ifCom.arg0 = commandResult;

            vector<BCCommand> elsePart = generateCommands(&(ifPtr->elsePart),
                    info);

            ifCom.result = result.size() + 2 + elsePart.size();
            ifCom.type = BCCommandType::IF;

            result.push_back(ifCom);

            for (auto& i : elsePart) {
                result.push_back(i);
            }

            vector<BCCommand> thenPart = generateCommands(&(ifPtr->thenPart),
                    info);

            BCCommand gotoEndOfThen;
            gotoEndOfThen.result = result.size() + 1 + thenPart.size();
            gotoEndOfThen.type = BCCommandType::GOTO;

            result.push_back(gotoEndOfThen);

            for (auto& i : thenPart) {
                result.push_back(i);
            }

        }

        /*
         * While
         *
         * If condition is true, it goes to the main part's start
         * If condition is false, it goes through the main part
         * There is goto to condition in the end of main part
         */
        WhileOperator *whilePtr = dynamic_cast<WhileOperator*>(_operator);
        if (whilePtr != NULL) {
            int commandResult = info->allocateRegister();

            BCCommand gotoIf(result.size(), GOTO);

            addExpression(&result, whilePtr->condition, commandResult, info);

            vector<BCCommand> cmds = generateCommands(&(whilePtr->body), info);

            BCCommand ifCom(commandResult, result.size() + cmds.size() + 2, IF);

            result.push_back(ifCom);

            for (auto cmd : cmds) {
                result.push_back(cmd);
            }

            result.push_back(gotoIf);
        }

        ExpressionOperator *exprPtr =
                dynamic_cast<ExpressionOperator*>(_operator);
        if (exprPtr != NULL) {
            int commandResult = info->allocateRegister();

            addExpression(&result, exprPtr->expr, commandResult, info);
        }

    }
    return result;
}

BCFunction *generateFunction(Function *func) {
    regsInfo info;

    BCFunction *result = new BCFunction;

    result->name = func->name;
    result->commands = generateCommands(&(func->body), &info);
    result->regsNumber = info.getRegsNum();

    return result;
}

Bytecode* generateBytecode(IR* ir) {
    Bytecode *result = new Bytecode;
    for (auto *function : ir->functions) {
        BCFunction *func = generateFunction(function);
        result->functions.push_back(func);
    }
    return result;
}
