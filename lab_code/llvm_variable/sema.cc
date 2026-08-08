#include "sema.h"
#include "llvm/Support/raw_ostream.h"
std::shared_ptr<ASTNode> Sema::SemaVariableDecl(llvm::StringRef name, CType *ty)
{
    // 1.Is there a repeant declaration in the current scope?
    std::shared_ptr<Symbol> symbol = scope.FindSymbolInCurrentScope(name);

    if (symbol)
    {
        llvm::errs() << "Error: Variable " << name.str() << " is already declared in the current scope.\n";
        exit(1);
        return nullptr;
    }

    // add symbol to symbol table
    scope.AddSymbol(SymbolKind::LovalVariable, ty, name);

    auto variableDecl = std::make_shared<VariableDecl>();
    variableDecl->name = name;
    variableDecl->ty = ty;

    return variableDecl;
}

std::shared_ptr<ASTNode> Sema::SemaVariableAccess(llvm::StringRef name, CType *ty)
{
    // Implementation for variable access semantic analysis
    std::shared_ptr<Symbol> symbol = scope.FindSymbol(name);

    if (symbol == nullptr)
    {
        llvm::errs() << "Error: Variable " << name.str() << " is not declared in the current scope.\n";
        exit(1);
        return nullptr;
    }

    auto expr = std::make_shared<VariableAccessExpr>();

    return expr;
}
std::shared_ptr<ASTNode> Sema::SemaAssignExpr(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right)
{
    if (left == nullptr || right == nullptr)
    {
        llvm::errs() << "Error: Left or right expression is null in assignment.\n";
        exit(1);
        return nullptr;
    }

    if (!llvm::isa<VariableAccessExpr>(left.get()))
    {   
        //must be a left value

        llvm::errs() << "Error: Left expression in assignment must be a variable access.\n";
        exit(1);
        return nullptr;
    }

    auto assignExpr = std::make_shared<AssignExpr>();
    assignExpr->left = left;
    assignExpr->right = right;
    return assignExpr;
}