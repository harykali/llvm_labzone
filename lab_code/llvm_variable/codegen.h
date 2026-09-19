#pragma once
#include "ast.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
class CodeGen: public Visitor {
public:   
    CodeGen(std::shared_ptr<Program> p){
        module= std::make_shared<llvm::Module>("expr", context);
        VisitorProgram(p.get());

    }
private:
    // llvm::Value* VisitorExpr(Expr *expr) override;
    llvm::Value* VisitorVariableDecl(VariableDecl *variableDecl) override;
    llvm::Value* VisitorAssignExpr(AssignExpr *assignExpr) override;
    llvm::Value* VisitorBinaryExpr(BinaryExpr *binaryExpr) override;
    // llvm::Value* VisitorFactorExpr(FactorExpr *factorExpr) override;
    llvm::Value* VisitorNumberExpr(NumberExpr *numberExpr) override;
    llvm::Value* VisitorProgram(Program *program) override;
    llvm::Value* VisitorVariableAccessExpr(VariableAccessExpr *variableAccessExpr) override;
private:
    llvm::LLVMContext context;
    llvm::IRBuilder<> IRbuilder{context};
    std::shared_ptr<llvm::Module> module;

    llvm::StringMap<llvm::Value*> varAddr;

};