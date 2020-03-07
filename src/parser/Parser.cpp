//
// Created by salah on 29.10.2019.
//

#include "../base.h"
#include "Parser.h"
#include <iostream>
#include <stack>

using namespace std;

bool isSameToken(TokenType tokenType, int ttType, Token token) {
    return token.type == tokenType && token.value == ttType;
}

bool isSameToken(TokenType tokenType, Token token) {
    return token.type == tokenType;
}

bool nextToken(TokenType tokenType, int ttType, stack<Token>& stack) {
    return isSameToken(tokenType, ttType, stack.top());
}

bool nextToken(TokenType tokenType, int ttType, vector<Token>& tokens) {
	if(tokens.empty()){
		return false;
	}
    return isSameToken(tokenType, ttType, tokens.at(0));
}

bool nextToken(TokenType tokenType, vector<Token>& tokens) {
	if(tokens.empty()){
		return false;
	}
    return isSameToken(tokenType, tokens.at(0));
}

void skipToken(TokenType tokenType, int ttType, vector<Token>& tokens) {
    auto index = tokens.begin();
    if (isSameToken(tokenType, ttType, tokens.at(0))) {
        tokens.erase(index);
    } else {
        throw;
    }
}

void skipToken(vector<Token>& tokens) {
    tokens.erase(tokens.begin());
}

Token getToken(vector<Token>& tokens) {
    Token token = tokens.at(0);
    tokens.erase(tokens.begin());

    return token;
}

Token getToken(stack<Token>& stack) {
    Token token = stack.top();
    stack.pop();

    return token;
}

Expression* getExpression(stack<Expression*>& stack) {
    Expression* expression = stack.top();
    stack.pop();

    return expression;
}

KeyWordType readFunctionType(vector<Token>& tokens) {
    skipToken(tokens);

    return KW_VOID;
}

int readFunctionName(vector<Token>& tokens) {
    if (nextToken(TT_IDENT, tokens)) {
        return getToken(tokens).value;
    }

    throw;
}

int getPriority(Token token) {
    switch (token.value) {
        case EQ:
        case NE:
        case LS:
        case BG:
        case LS_EQ:
        case BG_EQ:
        case AND:
        case OR:
        case NOT:
            return 1;
        case ADD:
        case SUB:
        case MOD:
            return 2;
        case MUL:
        case DIV:
            return 3;
        default:
        	return 0;
    }
}

Expression* parseExpression(vector<Token>& tokens, TokenType finishingTokenType, int finishingTokenValue) {
    vector<Token> rpnExpression;
    stack<Token> tokenStack;

    bool doCycle = true;
    while (doCycle) {
        if (nextToken(TT_IDENT, tokens)) {
            rpnExpression.push_back(getToken(tokens));
        } else if (nextToken(TT_OPERATION, OPEN_PARENTHESE, tokens)) {
            tokenStack.push(getToken(tokens));
        } else if (nextToken(TT_OPERATION, CLOSE_PARENTHESE, tokens)) {
            while (!nextToken(TT_OPERATION, OPEN_PARENTHESE, tokenStack)) {
                rpnExpression.push_back(getToken(tokenStack));
                if (tokenStack.empty()) {
                    if (finishingTokenValue == CLOSE_PARENTHESE) {
                        doCycle = false;
                        break;
                    }
                    throw; // missing close parenthesis
                }
            }
            tokenStack.pop();
        } else if (nextToken(TT_OPERATION, tokens) && !nextToken(TT_OPERATION, ASSIGN, tokens)) {
            Token token = getToken(tokens);
            if (getPriority(token) > getPriority(tokenStack.top())) {
                tokenStack.push(token);
            } else if (getPriority(token) <= getPriority(tokenStack.top())) {
                rpnExpression.push_back(getToken(tokenStack));
                tokenStack.push(token);
            }
        } else if (nextToken(TT_OPERATION, SEMICOLON, tokens) && nextToken(finishingTokenType, finishingTokenValue, tokens)) {
            break;
        } else {
            throw;
        }
    }

    while (!tokenStack.empty()) {
        rpnExpression.push_back(getToken(tokenStack));
    }

    stack<Expression*> expressionStack;
    while (!rpnExpression.empty()) {
        Expression* expression = new Expression();
        if (nextToken(TT_IDENT, tokens)) {
            expression->token = getToken(tokens);
            expressionStack.push(expression);
        } else if (nextToken(TT_OPERATION, tokens)) {
            expression->token = getToken(tokens);
            Expression* rArg = getExpression(expressionStack);
            Expression* lArg = getExpression(expressionStack);
            expression->lArg = lArg;
            expression->rArg = rArg;
            expressionStack.push(expression);
        }
    }

    return expressionStack.top();
}

vector<Operator*> parseBody(vector<Token>& tokens);

Operator* parseOperator(vector<Token>& tokens) {
    Token operatorToken = tokens.front();
    switch (operatorToken.type) {
        case TT_KEYWORD :
            switch(operatorToken.value) {
                case KW_IF : {
                    skipToken(TT_KEYWORD, KW_IF, tokens);

                    IfOperator* ifOperator = new IfOperator();

                    skipToken(TT_OPERATION, OPEN_PARENTHESE, tokens);

                    Expression* ifCondition = parseExpression(tokens, TT_OPERATION, CLOSE_PARENTHESE);
                    ifOperator->condition = ifCondition;

                    skipToken(TT_OPERATION, CLOSE_PARENTHESE, tokens);

                    vector<Operator*> thenBody = parseBody(tokens);
                    ifOperator->thenPart = thenBody;

                    if (nextToken(TT_KEYWORD, KW_ELSE, tokens)) {
                        skipToken(TT_KEYWORD, KW_ELSE, tokens);
                        vector<Operator*> elseBody = parseBody(tokens);
                        ifOperator->elsePart = elseBody;
                    }

                    return ifOperator;
                }
                case KW_WHILE : {
                    skipToken(TT_KEYWORD, KW_WHILE, tokens);

                    WhileOperator* whileOperator = new WhileOperator();

                    skipToken(TT_OPERATION, OPEN_PARENTHESE, tokens);

                    Expression* whileCondition = parseExpression(tokens, TT_OPERATION, CLOSE_PARENTHESE);
                    whileOperator->condition = whileCondition;

                    skipToken(TT_OPERATION, CLOSE_PARENTHESE, tokens);

                    vector<Operator*> whileBody = parseBody(tokens);
                    whileOperator->body = whileBody;

                    return whileOperator;
                }
                case KW_INT: {
                    skipToken(TT_KEYWORD, KW_INT, tokens);

                    VarDefOperator* defOperator = new VarDefOperator();

                    Token nameToken = getToken(tokens);
                    if (nameToken.type != TT_IDENT) {
                        throw;
                    }

                    defOperator->name = nameToken.value;

                    skipToken(TT_OPERATION, SEMICOLON, tokens);

                    return defOperator;
                }
                case KW_RETURN: {
                    ExpressionOperator* returnOperator = new ExpressionOperator();
                }
                    break;
                default:
                    throw;
            }
            break;
        case TT_IDENT:
        case TT_LITERAL:
        case TT_OPERATION: {
            ExpressionOperator* expressionOperator = new ExpressionOperator();

            Expression* expression = parseExpression(tokens, TT_OPERATION, SEMICOLON);
            expressionOperator->expr = expression;

            return expressionOperator;
        }
    }
}

vector<Operator*> parseBody(vector<Token>& tokens) {
    vector<Operator*> body;

    skipToken(TT_OPERATION, OPEN_BRACE, tokens);

    while (!nextToken(TT_OPERATION, CLOSE_BRACE, tokens)) {
        body.push_back(parseOperator(tokens));
    }

    skipToken(TT_OPERATION, CLOSE_BRACE, tokens);
    return body;
}

Function* parseFunction(vector<Token>& tokens) {
    Function* function = new Function();
    int name;

    readFunctionType(tokens);
    name = readFunctionName(tokens);

    skipToken(TT_OPERATION, OPEN_PARENTHESE, tokens);

    //readParameters(tokens);

    skipToken(TT_OPERATION, CLOSE_PARENTHESE, tokens);

    vector<Operator*> body = parseBody(tokens);

    function->name = name;
    function->body = body;
    if (name == -1) function->isMain = true; // main number

    return function;
}

IR* parseProgram(vector<Token>& tokens) {
	vector<Function*> funcs;
    while (!tokens.empty()) {
        funcs.push_back(parseFunction(tokens));
    }
    IR* ir = new IR(funcs);
    return ir;
}
