#pragma once
#include "scope.h"
#include "ast.h"
class Sema{
    public:
        std::shared_ptr<ASTNode> SemaVariableDecl(llvm::StringRef name, CType *ty);
        std::shared_ptr<ASTNode> SemaVariableAccess(llvm::StringRef name, CType *ty);
        std::shared_ptr<ASTNode> SemaAssignExpr(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right);
    private:
        Scope scope;


};