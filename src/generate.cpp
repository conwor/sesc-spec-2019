#include <iostream>
#include <map>
#include "base.h"

using namespace std;

int regsNumber = 0;

/*
 * addExpression casts expressions like:
 * (3 + 123) * (12 * (37 - 12)) or (1 == 2) && (3 != 4) or (a + 2) * 10
 * in BCCommands like IADD, ILOAD or LAND
 */
void addExpression(vector<BCCommand> *result, Expression *expr,
        int commandResult) {
    int left, right;
    TokenType leftT, rightT;

    if (expr->token.type == TT_IDENT) {
        BCCommand ass(expr->token.value, commandResult, BCCommandType::IMOV);
        result->push_back(ass);
        return;
    }

    leftT = expr->lArg->token.type;
    rightT = expr->rArg->token.type;

    //Using DFS to go through the tree

    switch (leftT) {
    case TT_LITERAL: {
        left = regsNumber;
        regsNumber++;
        BCCommand lLoad(expr->lArg->token.value, left, BCCommandType::ILOAD);
        result->push_back(lLoad);
        break;
    }
    case TT_IDENT:
        left = expr->lArg->token.value;
        break;
    case TT_OPERATION:
        left = regsNumber;
        regsNumber++;
        addExpression(result, expr->lArg, left);
        break;
    }

    if (!(expr->token.value == LNOT && rightT == TT_OPERATION)) {
        switch (rightT) {
        case TT_LITERAL: {
            right = regsNumber;
            regsNumber++;
            BCCommand rLoad(expr->rArg->token.value, right, BCCommandType::ILOAD);
            result->push_back(rLoad);
            break;
        }
        case TT_IDENT:
            right = expr->rArg->token.value;
            break;
        case TT_OPERATION:
            right = regsNumber;
            regsNumber++;
            addExpression(result, expr->rArg, right);
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

        int less = regsNumber;
        regsNumber++;

        int eq = regsNumber;
        regsNumber++;

        int notEq = regsNumber;
        regsNumber++;

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

        int less = regsNumber;
        regsNumber++;

        BCCommand lessCom(left, right, less, ICMPLS);
        result->push_back(lessCom);

        left = less;
        break;
    }
    case NE: {
        comType = BCCommandType::LNOT;

        int eq = regsNumber;
        regsNumber++;

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
        com.arg1 = commandResult;
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
        map<string, int> *name_to_index) {
    vector<BCCommand> result;
    for (auto i : *operators) {

//ASSIGN
        AssignOperator* asPtr = dynamic_cast<AssignOperator*>(i);
        if (asPtr != NULL) {
            BCCommand assign;

            assign.arg1 = (*name_to_index)[asPtr->variableName];
            assign.type = BCCommandType::IMOV;

            if (asPtr->value->token.type != TokenType::TT_LITERAL) {
                //For a := 17 + 21 * 3
                int commandResult = regsNumber;
                regsNumber++;
                addExpression(&result, asPtr->value, commandResult);
                assign.arg0 = commandResult;
            } else {
                //For a := 12
                BCCommand load(asPtr->value->token.value, regsNumber,
                        BCCommandType::ILOAD);
                result.push_back(load);

                assign.arg0 = regsNumber;
                regsNumber++;
            }

            result.push_back(assign);
        }

//VARDEF
        VarDefOperator* varPtr = dynamic_cast<VarDefOperator*>(i);
        if (varPtr != NULL) {
            (*name_to_index)[varPtr->name] = regsNumber;
            regsNumber++;
        }

        /*
         * IF
         *
         * If condition is true, it goes to the then part's start.
         * If condition is false, it goes to the else part, that after the if command.
         * There is a goto through the then part in the end of else part.
         */
        IfOperator *ifPtr = dynamic_cast<IfOperator*>(i);
        if (ifPtr != NULL) {
            int commandResult = regsNumber;
            regsNumber++;

            addExpression(&result, ifPtr->condition, commandResult);

            BCCommand ifCom;
            ifCom.arg0 = commandResult;

            vector<BCCommand> elsePart = generateCommands(&(ifPtr->elsePart),
                    name_to_index);

            ifCom.arg1 = result.size() + 2 + elsePart.size();
            ifCom.type = BCCommandType::IF;

            result.push_back(ifCom);

            for (auto& i : elsePart) {
                result.push_back(i);
            }

            vector<BCCommand> thenPart = generateCommands(&(ifPtr->thenPart),
                    name_to_index);

            BCCommand gotoEndOfThen;
            gotoEndOfThen.arg0 = result.size() + 1 + thenPart.size();
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
        WhileOperator *whilePtr = dynamic_cast<WhileOperator*>(i);
        if (whilePtr != NULL) {
            int commandResult = regsNumber;
            regsNumber++;

            BCCommand gotoIf(result.size(), GOTO);

            addExpression(&result, whilePtr->condition, commandResult);

            vector<BCCommand> cmds = generateCommands(&(whilePtr->body),
                    name_to_index);

            BCCommand ifCom(commandResult, result.size() + cmds.size() + 2, IF);

            result.push_back(ifCom);

            for (auto i : cmds) {
                result.push_back(i);
            }

            result.push_back(gotoIf);
        }

    }
    return result;
}

BCFunction *generateFunction(Function *func) {
    map<string, int> name_to_index; //for variables

    BCFunction *result = new BCFunction;

    result->name = func->name;
    result->commands = generateCommands(&(func->body), &name_to_index);
    result->regsNumber = regsNumber;

    regsNumber = 0; //for next function
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
