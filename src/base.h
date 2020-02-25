#pragma once

#include <iostream>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
// Lexical analysis structures

enum TokenType {
    TT_LITERAL, TT_IDENT, TT_KEYWORD, TT_OPERATION
};

enum KeyWordType {
    KW_VOID, KW_INT, KW_IF, KW_ELSE, KW_WHILE, KW_RETURN
};

enum OperatorType {
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    ASSIGN,
    EQ,
    NE,
    LS,
    BG,
    LS_EQ,
    BG_EQ,
    AND,
    OR,
    NOT,
    OPEN_BRACE,
    CLOSE_BRACE,
    OPEN_PARENTHESE,
    CLOSE_PARENTHESE,
    SEMICOLON
};

struct Token {
    // position in text, required for error messages
    int line;
    int column;

    TokenType type;

    // based on token type, either value of literal or enumeration value or number from idents table
    int value;
};

///////////////////////////////////////////////////////////////////////////////
// IR

struct Expression {
    Expression(){
    };

    Expression(Expression *_lArg, Expression *_rArg, TokenType _type, int _val) {
        lArg = _lArg;
        rArg = _rArg;
        token.type = _type;
        token.value = _val;
    }

    virtual ~Expression(){
    }

    Token token;
    Expression* lArg;
    Expression* rArg;
};

struct FunctionCall: public Expression{
    FunctionCall() {};

    FunctionCall(int _name){
        token.value = _name;
        token.type = TT_IDENT;
    }
};

struct Operator {
    virtual ~Operator() {
    }
};

struct WriteOperator: public Operator{
	WriteOperator(){
		value = NULL;
	}

	WriteOperator(Expression* _value){
		value = _value;
	}

	Expression* value;
};

struct ReadOperator: public Operator{
	ReadOperator(){
		name = 0;
	}

	ReadOperator(int _name){
		name = _name;
	}

	int name;
};

struct IfOperator: public Operator {
    IfOperator(){
    	thenPart = {};
    	elsePart = {};
    	condition = NULL;
    }

    IfOperator(Expression *_condition, std::vector<Operator*> *_then,
            std::vector<Operator*> *_else) {
        condition = _condition;
        thenPart = *_then;
        elsePart = *_else;
    }
    Expression* condition;
    std::vector<Operator*> thenPart;
    std::vector<Operator*> elsePart;
};

struct WhileOperator: public Operator {
    WhileOperator(){
    	condition = NULL;
    	body = {};
    }

    WhileOperator(Expression *_condition, std::vector<Operator*> *_body){
        condition = _condition;
        body = *_body;
    }

    Expression* condition;
    std::vector<Operator*> body;
};

struct VarDefOperator: public Operator {
    VarDefOperator() {
    	name = 0;
    }

    VarDefOperator(int _name) {
        name = _name;
    }

    int name;
};

struct AssignOperator: public Operator {
    AssignOperator() {
    	name = 0;
    	value = NULL;
    }

    AssignOperator(Expression *_expr, int _name) {
        value = _expr;
        name = _name;
    }
    int name;
    Expression* value;
};

struct ExpressionOperator: public Operator {
    ExpressionOperator(Expression *_expr){
        expr = _expr;
    }
    Expression* expr;
};

struct Function {
    Function() {
    	name = 0;
    	body = {};
    }

    Function(int _name, Operator *_op){
        name = _name;
        body.push_back(_op);
    }

    Function(int _name, const std::vector<Operator*>& _body) {
        name = _name;
        body = _body;
    }
    int name;
    std::vector<Operator*> body;
};

struct IR {
    IR(){}
    IR(const std::vector<Function*> _functions){
        functions = _functions;
    }
    IR(Function *_func){
        functions.push_back(_func);
    }
    std::vector<Function*> functions;
};

///////////////////////////////////////////////////////////////////////////////
// Bytecode

enum BCCommandType { //Don't forget enumString at print.cpp
    IADD,
    ISUB,
    IMUL,
    IDIV,
    IMOD,
    LAND,
    LOR,
    LNOT,
    IMOV,
    ILOAD,
    ICMPEQ,
    ICMPLS,
    GOTO,
    IF,
    RET,
    IWRITE,
    IREAD,
    CALL
};

struct BCCommand {
    BCCommand() {
    	arg0 = -1;
    	arg1 = -1;
    	result = -1;
    }

    BCCommand(int _arg0, int _result, BCCommandType _type) {
        arg0 = _arg0;
        arg1 = -1;
        result = _result;
        type = _type;
    }

    BCCommand(int _result, BCCommandType _type) {
    	arg0 = -1;
    	arg1 = -1;
        result = _result;
        type = _type;
    }

    BCCommand(int _arg0, int _arg1, int _result, BCCommandType _type) {
        arg0 = _arg0;
        arg1 = _arg1;
        result = _result;
        type = _type;
    }

    BCCommandType type;

    // arguments and result
    // depends on command type, may be unused or reused for something else
    int arg0;
    int arg1;
    int result;
};

struct BCFunction {
    int regsNumber;
    std::vector<BCCommand> commands;
};

struct Bytecode {
    std::vector<BCFunction*> functions;
};
