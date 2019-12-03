#include <iostream>
#include <map>
#include "base.h"

using namespace std;

vector<string> indexToString = {"a", "b", "c", "main", "foo", "baz"};

class regsInfo {
public:
    int allocateRegister() {
        regsNumber++;
        return regsNumber - 1;
    }

    void allocateVariable(int nameIndex) {
        nameToIndex[indexToString[nameIndex]] = regsNumber;
        regsNumber++;
    }

    int registerOf(int nameIndex) {
        return nameToIndex.at(indexToString[nameIndex]);
    }

    int getRegsNum() {
        return regsNumber;
    }
private:
    int regsNumber = 0;
    map<string, int> nameToIndex;
};

class funcInfo {
public:
    int allocateFunc(int nameIndex) {
        int size = nameToIndex.size();
        nameToIndex[indexToString[nameIndex]] = size;
        return size;
    }

    int numOf(int nameIndex){
        return nameToIndex.at(indexToString.at(nameIndex));
    }

    int getFuncsNum(){
        return nameToIndex.size();
    }

private:
    map<string, int> nameToIndex;
};

/*
 * addExpression casts expressions like:
 * (3 + 123) * (12 * (37 - 12)) or (1 == 2) && (3 != 4) or (a + 2) * 10
 * in BCCommands like IADD, ILOAD or LAND
 */
void addExpression(vector<BCCommand> *result, Expression *expr,
        int commandResult, regsInfo *regsInformation, funcInfo *funcInformation) {
    FunctionCall *funcPtr = dynamic_cast<FunctionCall*>(expr);
    if(funcPtr != NULL){
        BCCommand funcCall(funcInformation->numOf(funcPtr->token.value), BCCommandType::CALL);
        result->push_back(funcCall);
        return;
    }

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
        left = regsInformation->allocateRegister();
        BCCommand lLoad(expr->lArg->token.value, left, BCCommandType::ILOAD);
        result->push_back(lLoad);
        break;
    }
    case TT_IDENT:
        left = expr->lArg->token.value;
        break;
    case TT_OPERATION:
        left = regsInformation->allocateRegister();
        addExpression(result, expr->lArg, left, regsInformation, funcInformation);
        break;
    }

    if (!(expr->token.value == LNOT && rightT == TT_OPERATION)) {
        switch (rightT) {
        case TT_LITERAL: {
            right = regsInformation->allocateRegister();
            BCCommand rLoad(expr->rArg->token.value, right,
                    BCCommandType::ILOAD);
            result->push_back(rLoad);
            break;
        }
        case TT_IDENT:
            right = expr->rArg->token.value;
            break;
        case TT_OPERATION:
            right = regsInformation->allocateRegister();
            addExpression(result, expr->rArg, right, regsInformation, funcInformation);
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

        int less = regsInformation->allocateRegister();

        int eq = regsInformation->allocateRegister();

        int notEq = regsInformation->allocateRegister();

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

        int less = regsInformation->allocateRegister();

        BCCommand lessCom(left, right, less, ICMPLS);
        result->push_back(lessCom);

        left = less;
        break;
    }
    case NE: {
        comType = BCCommandType::LNOT;

        int eq = regsInformation->allocateRegister();

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
        regsInfo *regsInformation, funcInfo *funcInformation) {
    vector<BCCommand> result;
    for (auto _operator : *operators) {
        //ASSIGN
        AssignOperator* asPtr = dynamic_cast<AssignOperator*>(_operator);
        if (asPtr != NULL) {
            BCCommand assign;

            assign.result = regsInformation->registerOf(asPtr->variableNameIndex);
            assign.type = BCCommandType::IMOV;

            int commandResult = regsInformation->allocateRegister();
            addExpression(&result, asPtr->value, commandResult, regsInformation, funcInformation);
            assign.arg0 = commandResult;

            result.push_back(assign);
        }

        //VARDEF
        VarDefOperator* varPtr = dynamic_cast<VarDefOperator*>(_operator);
        if (varPtr != NULL) {
            regsInformation->allocateVariable(varPtr->nameIndex);
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
            int commandResult = regsInformation->allocateRegister();

            addExpression(&result, ifPtr->condition, commandResult, regsInformation, funcInformation);

            BCCommand ifCom;
            ifCom.arg0 = commandResult;

            vector<BCCommand> elsePart = generateCommands(&(ifPtr->elsePart),
                    regsInformation, funcInformation);

            ifCom.result = result.size() + 2 + elsePart.size();
            ifCom.type = BCCommandType::IF;

            result.push_back(ifCom);

            for (auto& i : elsePart) {
                result.push_back(i);
            }

            vector<BCCommand> thenPart = generateCommands(&(ifPtr->thenPart),
                    regsInformation, funcInformation);

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
            int commandResult = regsInformation->allocateRegister();

            BCCommand gotoIf(result.size(), GOTO);

            addExpression(&result, whilePtr->condition, commandResult, regsInformation, funcInformation);

            vector<BCCommand> cmds = generateCommands(&(whilePtr->body), regsInformation, funcInformation);

            BCCommand ifCom(commandResult, result.size() + cmds.size() + 2, IF);

            result.push_back(ifCom);

            for (auto cmd : cmds) {
                result.push_back(cmd);
            }

            result.push_back(gotoIf);
        }

        //Expressions
        ExpressionOperator *exprPtr = dynamic_cast<ExpressionOperator*>(_operator);
        if (exprPtr != NULL) {
            int commandResult = regsInformation->allocateRegister();

            addExpression(&result, exprPtr->expr, commandResult, regsInformation, funcInformation);
        }

    }
    return result;
}

BCFunction *generateFunction(Function *func, funcInfo *funcInformation) {
    regsInfo regsInformation;

    BCFunction *result = new BCFunction;
    result->name = indexToString[func->nameIndex];
    result->commands = generateCommands(&(func->body), &regsInformation, funcInformation);
    result->regsNumber = regsInformation.getRegsNum();
    return result;
}

Bytecode* generateBytecode(IR* ir) {
    Bytecode *result = new Bytecode;
    funcInfo funcInformation;
    for(auto *function : ir->functions){
        funcInformation.allocateFunc(function->nameIndex);
    }
    for (auto *function : ir->functions) {
        BCFunction *func = generateFunction(function, &funcInformation);
        result->functions.push_back(func);
    }
    return result;
}
