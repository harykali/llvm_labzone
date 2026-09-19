#pragma once
#include <memory>
#include <vector>
#include "llvm/IR/Value.h"
#include "type.h"
class Program;
class Expr;
class BinaryExpr;
class FactorExpr;

class NumberExpr;
class VariableDecl;
class VariableAccessExpr;
class AssignExpr;

class Visitor
{
public:
    virtual ~Visitor() {};
    virtual llvm::Value *VisitorProgram(Program *p) = 0;
    virtual llvm::Value *VisitorVariableDecl(VariableDecl *decl) { return nullptr; }
    virtual llvm::Value *VisitorAssignExpr(AssignExpr *expr) { return nullptr; }
    virtual llvm::Value *VisitorBinaryExpr(BinaryExpr *expr) = 0;
    virtual llvm::Value *VisitorNumberExpr(NumberExpr *expr) = 0;
    virtual llvm::Value *VisitorVariableAccessExpr(VariableAccessExpr *factorExpr) = 0;
};

// llvm rtti: Runtime Type Information
class ASTNode
{
public:
    enum class Kind
    {
        ND_VariableDecl,
        ND_BinaryExpr,
        ND_NumberExpr,
        ND_VariableAccessExpr,
        ND_AssignExpr,
    };

private:
    const Kind kind;

public:
    virtual ~ASTNode() {};
    CType *ty;
    ASTNode(Kind kind) : kind(kind), ty(nullptr) {};
    const Kind getKind() const { return kind; }
    virtual llvm::Value *Accept(Visitor *v) { return nullptr; };
};

class VariableDecl : public ASTNode
{
public:
    llvm::StringRef name;
    VariableDecl() : ASTNode(Kind::ND_VariableDecl) {};
    static bool classof(const ASTNode *node)
    {
        return node->getKind() == Kind::ND_VariableDecl;
    }
    llvm::Value *Accept(Visitor *visitor) override
    {
        return visitor->VisitorVariableDecl(this);
    }
};

// class Expr{
//     public:
//         virtual ~Expr(){};
//         virtual llvm::Value* Accept(Visitor* visitor){return nullptr;};

// };

enum class OPCode
{
    ADD,
    SUB,
    MUL,
    DIV

};

class BinaryExpr : public ASTNode
{
public:
    BinaryExpr() : ASTNode(Kind::ND_BinaryExpr) {};
    ~BinaryExpr() {};
    OPCode op;
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    llvm::Value *Accept(Visitor *visitor) override
    {
        return visitor->VisitorBinaryExpr(this);
    }
    static bool classof(const ASTNode *node)
    {
        return node->getKind() == Kind::ND_BinaryExpr;
    }
};

class NumberExpr : public ASTNode
{
public:
    NumberExpr() : ASTNode(Kind::ND_NumberExpr) {};
    ~NumberExpr() {};
    int number;
    llvm::Value *Accept(Visitor *visitor) override
    {
        return visitor->VisitorNumberExpr(this);
    }
    static bool classof(const ASTNode *node)
    {
        return node->getKind() == Kind::ND_NumberExpr;
    }
};

class VariableAccessExpr : public ASTNode
{
public:
    llvm::StringRef name;
    VariableAccessExpr() : ASTNode(Kind::ND_VariableAccessExpr) {};
    static bool classof(const ASTNode *node)
    {
        return node->getKind() == Kind::ND_VariableAccessExpr;
    }
    llvm::Value *Accept(Visitor *visitor) override
    {
        return visitor->VisitorVariableAccessExpr(this);
    }
};

class AssignExpr : public ASTNode
{
public:
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    AssignExpr() : ASTNode(Kind::ND_AssignExpr) {};
    static bool classof(const ASTNode *node)
    {
        return node->getKind() == Kind::ND_AssignExpr;
    }
    llvm::Value *Accept(Visitor *visitor) override
    {
        return visitor->VisitorAssignExpr(this);
    }
};

class Program
{
public:
    ~Program() {};
    std::vector<std::shared_ptr<ASTNode>> exprVec;
    llvm::Value *Accept(Visitor *visitor)
    {
        return visitor->VisitorProgram(this);
    }
};
