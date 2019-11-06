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
    Expression(int num) {
        token.type = TokenType::TT_LITERAL;
        token.value = num;
    }

    Expression(Expression *LArg, Expression *RArg, TokenType type, int val) {
        lArg = LArg;
        rArg = RArg;
        token.type = type;
        token.value = val;
    }

    Token token;
    Expression* lArg;
    Expression* rArg;
};

struct Operator {
    virtual ~Operator() {
    }
};

struct IfOperator: public Operator {
    IfOperator() {
    }
    IfOperator(Expression *cond, std::vector<Operator*> *then,
            std::vector<Operator*> *Else) {
        condition = cond;
        thenPart = *then;
        elsePart = *Else;
    }
    Expression* condition;
    std::vector<Operator*> thenPart;
    std::vector<Operator*> elsePart;
};

struct WhileOperator: public Operator {
    Expression* condition;
    std::vector<Operator*> body;
};

struct VarDefOperator: public Operator {
    VarDefOperator() {
    }
    VarDefOperator(const std::string& Name) {
        name = Name;
    }

    std::string name;
};

struct AssignOperator: public Operator {
    AssignOperator() {
    }

    AssignOperator(Expression *expr, const std::string& name) {
        value = expr;
        variableName = name;
    }
    std::string variableName;
    Expression* value;
};

struct ExpressionOperator: public Operator {
    Expression* expr;
};

struct Function {
    Function() {
    }

    Function(const std::string& Name, const std::vector<Operator*>& Body) {
        name = Name;
        body = Body;
    }
    std::string name;
    std::vector<Operator*> body;
};

struct IR {
    std::vector<Function*> functions;
};

///////////////////////////////////////////////////////////////////////////////
// Bytecode

enum BCCommandType {
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
    }

    BCCommand(int Arg0, int Arg1, BCCommandType Type) {
        arg0 = Arg0;
        arg1 = Arg1;
        type = Type;
    }

    BCCommand(int Arg0, BCCommandType Type) {
        arg0 = Arg0;
        type = Type;
    }

    BCCommand(int Arg0, int Arg1, int Result, BCCommandType Type) {
        arg0 = Arg0;
        arg1 = Arg1;
        result = Result;
        type = Type;
    }

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

// Bytecode Generating

Bytecode* generateBytecode(IR* ir);

// Bytecode print

void writeBytecode(Bytecode* bc, std::string filename);

void writeBytecode(Bytecode* bc, std::ostream& os);

// Bytecode tests

void TestAssign();

void TestIf();

