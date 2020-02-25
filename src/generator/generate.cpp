#include <iostream>
#include <map>
#include "base.h"

using namespace std;

vector<string> indexToString = {"a", "b", "c", "main", "foo", "baz"};

class RegsInfo {
public:
    int allocateRegister() {
        regsNumber++;
        return regsNumber - 1;
    }

    void allocateVariable(int name) {
        nameToIndex[name] = regsNumber;
        regsNumber++;
    }

    int registerOf(int name) {
        return nameToIndex.at(name);
    }

    int getRegsNum() {
        return regsNumber;
    }
private:
    int regsNumber = 0;
    map<int, int> nameToIndex;
};

class FuncInfo {
public:
    int allocateFunc(int name) {
        int size = nameToIndex.size();
        nameToIndex[name] = size;
        return size;
    }

    int numOf(int name){
        return nameToIndex.at(name);
    }

    int getFuncsNum(){
        return nameToIndex.size();
    }

private:
    map<int, int> nameToIndex;
};

/*
 * addExpression casts expressions like:
 * (3 + 123) * (12 * (37 - 12)) or (1 == 2) && (3 != 4) or (a + 2) * 10
 * in BCCommands like IADD, ILOAD or LAND
 */
int genExpression(vector<BCCommand>& result, Expression *expr,
		RegsInfo& regsInformation, FuncInfo& funcInformation) {
    FunctionCall *funcPtr = dynamic_cast<FunctionCall*>(expr);
    if(funcPtr != NULL){
        BCCommand funcCall(funcInformation.numOf(funcPtr->token.value), BCCommandType::CALL);
        result.push_back(funcCall);
        return -1;
    }

    int left, right;
    TokenType leftT, rightT;

    switch (expr->token.type) {
    case TT_IDENT: {
        return expr->token.value;
    }

    case TT_LITERAL: {
    	int commandResult = regsInformation.allocateRegister();
        BCCommand load(expr->token.value, commandResult, BCCommandType::ILOAD);
        result.push_back(load);
        return commandResult;
    }
    }

    leftT = expr->lArg->token.type;
    rightT = expr->rArg->token.type;

    //Using DFS to go through the tree

    switch (leftT) {
    case TT_LITERAL: {
        left = regsInformation.allocateRegister();
        BCCommand lLoad(expr->lArg->token.value, left, BCCommandType::ILOAD);
        result.push_back(lLoad);
        break;
    }
    case TT_IDENT:
        left = expr->lArg->token.value;
        break;
    case TT_OPERATION:
        left = genExpression(result, expr->lArg, regsInformation, funcInformation);
        break;
    }

    if (!(expr->token.value == LNOT && rightT == TT_OPERATION)) {
        switch (rightT) {
        case TT_LITERAL: {
            right = regsInformation.allocateRegister();
            BCCommand rLoad(expr->rArg->token.value, right,
                    BCCommandType::ILOAD);
            result.push_back(rLoad);
            break;
        }
        case TT_IDENT:
            right = expr->rArg->token.value;
            break;
        case TT_OPERATION:
            right = genExpression(result, expr->rArg, regsInformation, funcInformation);
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

        int less = regsInformation.allocateRegister();

        int eq = regsInformation.allocateRegister();

        int notEq = regsInformation.allocateRegister();

        BCCommand lessCom(left, right, less, ICMPLS);
        result.push_back(lessCom);
        BCCommand notLess(less, less, LNOT);
        result.push_back(notLess);

        BCCommand eqCom(left, right, eq, ICMPEQ);
        result.push_back(eqCom);
        BCCommand notEqual(eq, notEq, LNOT);
        result.push_back(notEqual);

        left = less;
        right = notEq;
        break;
    }
    case BG_EQ: {
        comType = BCCommandType::LNOT;

        int less = regsInformation.allocateRegister();

        BCCommand lessCom(left, right, less, ICMPLS);
        result.push_back(lessCom);

        left = less;
        break;
    }
    case NE: {
        comType = BCCommandType::LNOT;

        int eq = regsInformation.allocateRegister();

        BCCommand eqCom(left, right, eq, ICMPEQ);
        result.push_back(eqCom);
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
    int commandResult = regsInformation.allocateRegister();
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

    result.push_back(com);
    return commandResult;
}

vector<BCCommand> generateCommands(vector<Operator*>& operators,
        RegsInfo& regsInformation, FuncInfo& funcInformation) {
    vector<BCCommand> result;
    for (auto _operator : operators) {
        //ASSIGN
        AssignOperator* asPtr = dynamic_cast<AssignOperator*>(_operator);
        if (asPtr != NULL) {
            BCCommand assign;

            assign.result = regsInformation.registerOf(asPtr->name);
            assign.type = BCCommandType::IMOV;
            assign.arg0 = genExpression(result, asPtr->value, regsInformation, funcInformation);


            result.push_back(assign);
        }

        //VARDEF
        VarDefOperator* varPtr = dynamic_cast<VarDefOperator*>(_operator);
        if (varPtr != NULL) {
            regsInformation.allocateVariable(varPtr->name);
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
            int commandResult = genExpression(result, ifPtr->condition, regsInformation, funcInformation);

            BCCommand ifCom;
            ifCom.arg0 = commandResult;

            vector<BCCommand> elsePart = generateCommands(ifPtr->elsePart,
                    regsInformation, funcInformation);

            ifCom.result = result.size() + 2 + elsePart.size();
            ifCom.type = BCCommandType::IF;

            result.push_back(ifCom);

            result.insert(result.end(), elsePart.begin(), elsePart.end());

            vector<BCCommand> thenPart = generateCommands(ifPtr->thenPart,
                    regsInformation, funcInformation);

            BCCommand gotoEndOfThen;
            gotoEndOfThen.result = result.size() + 1 + thenPart.size();
            gotoEndOfThen.type = BCCommandType::GOTO;

            result.push_back(gotoEndOfThen);

            result.insert(result.end(), thenPart.begin(), thenPart.end());

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
            BCCommand gotoIf(result.size(), GOTO);

            int commandResult = genExpression(result, whilePtr->condition, regsInformation, funcInformation);

            vector<BCCommand> cmds = generateCommands(whilePtr->body, regsInformation, funcInformation);

            BCCommand ifCom(commandResult, result.size() + cmds.size() + 2, IF);

            result.push_back(ifCom);

            result.insert(result.end(), cmds.begin(), cmds.end());

            result.push_back(gotoIf);
        }

        //ReadOperator
        ReadOperator *readPtr = dynamic_cast<ReadOperator*>(_operator);
        if(readPtr != NULL){
        	BCCommand read(readPtr->name, BCCommandType::IREAD);
        	result.push_back(read);
        }

        //WriteOperator
        WriteOperator *writePtr = dynamic_cast<WriteOperator*>(_operator);
        if(writePtr != NULL){
        	int expr_res = genExpression(result, writePtr->value, regsInformation, funcInformation);
        	BCCommand write(expr_res, BCCommandType::IWRITE);
        	result.push_back(write);
        }

        //Expressions
        ExpressionOperator *exprPtr = dynamic_cast<ExpressionOperator*>(_operator);
        if (exprPtr != NULL) {
            int commandResult = genExpression(result, exprPtr->expr, regsInformation, funcInformation);
        }

    }
    return result;
}

BCFunction *generateFunction(Function *func, FuncInfo& funcInformation) {
    RegsInfo regsInformation;

    BCFunction *result = new BCFunction;
    result->commands = generateCommands(func->body, regsInformation, funcInformation);
    result->regsNumber = regsInformation.getRegsNum();
    return result;
}

Bytecode* generateBytecode(IR* ir) {
    Bytecode *result = new Bytecode;
    FuncInfo funcInformation;
    for(auto *function : ir->functions){
        funcInformation.allocateFunc(function->name);
    }
    for (auto *function : ir->functions) {
        BCFunction *func = generateFunction(function, funcInformation);
        result->functions.push_back(func);
    }
    return result;
}
