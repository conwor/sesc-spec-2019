/**
 * Base structures for compiler & interpreter.
 */

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
    ADD, SUB, MUL, DIV, MOD,
    ASSIGN,
    EQ, NE, LS, BG, LS_EQ, BG_EQ, AND, OR, NOT,
    OPEN_BRACE, CLOSE_BRACE, OPEN_PARENTHESE, CLOSE_PARENTHESE,
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
    Token token;
    Expression* lArg;
    Expression* rArg;
};

struct Operator {
};

struct IfOperator : public Operator {
    Expression* condition;
    std::vector<Operator*> thenPart;
    std::vector<Operator*> elsePart;
};

struct WhileOperator : public Operator {
    Expression* condition;
    std::vector<Operator*> body;
};

struct VarDefOperator : public Operator {
     int name;
};

struct AssignOperator : public Operator {
    int name;
    Expression* value;
};

struct ExpressionOperator : public Operator {
    Expression* expr;
};

struct Function {
    int name;
    bool isMain = false;
    std::vector<Operator*> body;
};

struct IR {
    std::vector<Function*> functions;
};

///////////////////////////////////////////////////////////////////////////////
// Bytecode

enum BCCommandType {
    IADD, ISUB, IMUL, IDIV, IMOD,
    LAND, LOR, LNOT,
    IMOV,
    ILOAD,
    ICMPEQ, ICMPLS,
    GOTO, IF, RET,
    IWRITE, IREAD,
    CALL
};

struct BCCommand {
    BCCommandType type;

    // arguments and result
    // depends on command type, may be unused or reused for something else
    int arg0;
    int arg1;
    int result;
};

struct BCFunction {
    std::string name;
    int regsNumber;
    std::vector<BCCommand> commands;
};

struct Bytecode {
    std::vector<BCFunction*> functions;
};
