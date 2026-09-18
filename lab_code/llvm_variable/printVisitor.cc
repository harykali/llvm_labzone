#include "printVisitor.h"

PrintVisitor::PrintVisitor(std::shared_ptr<Program> program) {
    VisitorProgram(program.get());
}

// llvm::Value* PrintVisitor::VisitorExpr(Expr *expr) {
//     return expr->Accept(this);
// }

llvm::Value* PrintVisitor::VisitorProgram(Program *program) {
    for (auto& expr : program->exprVec) {
        expr->Accept(this);
        llvm::outs() << ";\n";
    }
    return nullptr;
}

llvm::Value* PrintVisitor::VisitorBinaryExpr(BinaryExpr *binaryExpr) {
    binaryExpr->left->Accept(this);

    switch (binaryExpr->op) {
        case OPCode::ADD:
            llvm::outs() << " + ";
            break;
        case OPCode::SUB:
            llvm::outs() << " - ";
             break;
        case OPCode::MUL:
            llvm::outs() << " * ";
            break;
        case OPCode::DIV:
            llvm::outs() << " / ";
            break;
    }
    binaryExpr->right->Accept(this);
    return nullptr;
}


llvm::Value* PrintVisitor::VisitorVariableDecl(VariableDecl *variableDecl){
    if(variableDecl->ty==CType::GetIntType()){
        llvm::outs()<<"int " << variableDecl->name;
    }
    return nullptr;
}
llvm::Value* PrintVisitor::VisitorAssignExpr(AssignExpr *assignExpr) {
    if (VariableDecl::classof(assignExpr->left.get())) {
        auto *decl = static_cast<VariableDecl *>(assignExpr->left.get());
        llvm::outs() << decl->name;
    } else {
        assignExpr->left->Accept(this);
    }
    llvm::outs() << " = ";
    assignExpr->right->Accept(this);
    return nullptr;
}



llvm::Value* PrintVisitor::VisitorNumberExpr(NumberExpr *numberExpr) {
    llvm::outs() << numberExpr->number;
    return nullptr;
}


// llvm::Value* PrintVisitor::VisitorFactorExpr(FactorExpr *factorExpr) {
//     llvm::outs() << factorExpr->number;
//     return nullptr;
// }

llvm::Value* PrintVisitor::VisitorVariableAccessExpr(VariableAccessExpr *variableAccessExpr) {
    llvm::outs() << variableAccessExpr->name;
    return nullptr;
}