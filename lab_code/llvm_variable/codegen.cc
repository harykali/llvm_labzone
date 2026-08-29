#include "codegen.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Value.h"
using namespace llvm;
llvm::Value *CodeGen::VisitorBinaryExpr(BinaryExpr *binaryExpr)
{
    llvm::Value *left = binaryExpr->left->Accept(this);
    llvm::Value *right = binaryExpr->right->Accept(this);
    switch (binaryExpr->op)
    {
    case OPCode::ADD:
    {
        return IRbuilder.CreateNSWAdd(left, right, "addtmp");
    }
    case OPCode::SUB:
    {
        return IRbuilder.CreateNSWSub(left, right, "subtmp");
    }
    case OPCode::MUL:
    {
        return IRbuilder.CreateNSWMul(left, right, "multmp");
    }
    case OPCode::DIV:
    {
        return IRbuilder.CreateSDiv(left, right, "divtmp");
    }
    default:
        break;
    }
    return nullptr;
}
llvm::Value *CodeGen::VisitorNumberExpr(NumberExpr *numberExpr)
{
    return IRbuilder.getInt32(numberExpr->number);
}

llvm::Value *VisitorVariableDecl(VariableDecl *variableDecl)
{
    return nullptr;
}
llvm::Value *VisitorAssignExpr(AssignExpr *assignExpr)
{
    return nullptr;
}
llvm::Value *VisitorVariableAccessExpr(VariableAccessExpr *variableAccessExpr)
{
    return nullptr;
}

llvm::Value *CodeGen::VisitorProgram(Program *program)
{

    auto printfFuncType = FunctionType::get(IRbuilder.getInt32Ty(), {IRbuilder.getPtrTy()}, true);
    auto printfFunc = Function::Create(printfFuncType, GlobalValue::ExternalLinkage, "printf", module.get());

    auto mFuncType = FunctionType::get(IRbuilder.getInt32Ty(), false);
    auto mFunc = Function::Create(mFuncType, GlobalValue::ExternalLinkage, "main", module.get());

    BasicBlock *entryBB = BasicBlock::Create(context, "entry", mFunc);

    IRbuilder.SetInsertPoint(entryBB);

    for (auto &expr : program->exprVec)
    {
        llvm::Value *p = expr->Accept(this);
        if (!p)
            continue;

        if (p->getType()->isIntegerTy() && p->getType()->getIntegerBitWidth() != 32)
        {
            p = IRbuilder.CreateIntCast(p, IRbuilder.getInt32Ty(), true, "tmp_cast");
        }

        IRbuilder.CreateCall(printfFunc, {IRbuilder.CreateGlobalString("expr val: %d\n"), p});
    }

    IRbuilder.CreateRet(IRbuilder.getInt32(0));

    verifyFunction(*mFunc);
    module->print(outs(), nullptr);

    return nullptr;
}
