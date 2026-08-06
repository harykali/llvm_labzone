#include "sema.h"
#include "llvm/Support/raw_ostream.h"
std::shared_ptr<ASTNode> Sema::SemaVariableDecl(llvm::StringRef name, CType *ty)
{       
        //1.Is there a repeant declaration in the current scope?
        std::shared_ptr<Symbol> symbol = scope.FindSymbolInCurrentScope(name);

        if (symbol){
            llvm::errs() << "Error: Variable " << name.str() << " is already declared in the current scope.\n";
            exit(1);
            return nullptr;
        }

        auto variableDecl = std::make_shared<VariableDecl>();
        variableDecl->name = name;
        variableDecl->ty = ty;
        return variableDecl;
}