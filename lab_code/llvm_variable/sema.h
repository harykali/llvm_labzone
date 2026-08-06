#pragma once
#include "scope.h"
#include "ast.h"
class Sema{
    public:
        std::shared_ptr<ASTNode> SemaVariableDecl(llvm::StringRef name, CType *ty);
    private:
        Scope scope;


};