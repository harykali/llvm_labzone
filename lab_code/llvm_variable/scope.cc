#include "scope.h"

Scope::Scope()
{
    envs.push_back(std::make_shared<Env>()); // global env
}
void Scope::EnterScope()
{
    envs.push_back(std::make_shared<Env>());
}
void Scope::ExitScope()
{
    if (envs.size() > 1)
        envs.pop_back();
}
std::shared_ptr<Symbol> Scope::FindSymbol(llvm::StringRef name)
{
    for (auto it = envs.rbegin(); it != envs.rend(); ++it)
    {
        auto &table = (*it)->VariableSymbolTable;
        if (table.count(name) > 0)
        {
            return table[name];
        }
    }
    return nullptr;
}

std::shared_ptr<Symbol> Scope::FindSymbolInCurrentScope(llvm::StringRef name)
{
    auto &table = envs.back()->VariableSymbolTable;
    if (table.count(name) > 0)
    {
        return table[name];
    }
    return nullptr;
}
void Scope::AddSymbol(SymbolKind kind, CType *ty, llvm::StringRef name)
{
    auto symbol = std::make_shared<Symbol>(kind, ty, name);
    envs.back()->VariableSymbolTable.insert({name, symbol});
}
