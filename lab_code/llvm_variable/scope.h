#pragma once
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringMap.h"
#include "type.h"
#include <memory>
#include <vector>
enum class SymbolKind{
    LovalVariable,
    //GlobalVariable,

};

class Symbol{
    private:
        SymbolKind kind;
        CType *ty;
        llvm::StringRef name;
    public:
        Symbol(SymbolKind kind, CType *ty, llvm::StringRef name):kind(kind),ty(ty),name(name){};
        CType *GetType() const { return ty; }
};



class Env{
public:
    //symbol info
    llvm::StringMap<std::shared_ptr<Symbol>> VariableSymbolTable;
};
class Scope {

    private:
        std::vector<std::shared_ptr<Env>> envs;
    public:
        Scope();
        void EnterScope();
        void ExitScope();
        std::shared_ptr<Symbol> FindSymbol(llvm::StringRef name);
        std::shared_ptr<Symbol> FindSymbolInCurrentScope(llvm::StringRef name);
        void AddSymbol(SymbolKind kind, CType *ty, llvm::StringRef name);
    };
