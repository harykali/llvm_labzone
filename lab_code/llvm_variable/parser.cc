/**
prog : (expr? ";")*
expr : term (("+" | "-") term)* ;
term : factor (("*" | "/") factor)* ;
factor : number | "(" expr ")" ;
number: ([0-9])+ ;
 */

 /*
 
 
 */
#include "parser.h"
#include "ast.h"
std::shared_ptr<Program> Parser::ParseProgram()
{
    // while Ture .....EXIT
    // token->eof
    Advance();
    std::vector<std::shared_ptr<ASTNode>> exprVec;
    while (tok.tokenType != TokenType::eof)
    {
        // parse expr
        // expect ';'
        if (tok.tokenType == TokenType::semi)
        {
            Advance();
            continue;
        }
        if (tok.tokenType == TokenType::kw_int)
        {
            const auto &exprs = ParseDecl();
            for (const auto &expr : exprs)
            {
                exprVec.push_back(expr);
            }
        }
        else
        {
            auto exp = ParseExpr();
            exprVec.push_back(exp);
        }
    }
    auto program = std::make_shared<Program>();
    program->exprVec = std::move(exprVec);
    return program;
};

std::vector<std::shared_ptr<ASTNode>> Parser::ParseDecl()
{
    Consume(TokenType::kw_int);
    CType *baseTy = CType::GetIntType();
    std::vector<std::shared_ptr<ASTNode>> astArr;
    ///int a,b=3;
    ///a,b=3;
    int i=0;
    while(tok.tokenType!=TokenType::semi){
        //count ","
        if(i++>0)
        {
            assert(Consume(TokenType::comma));
        }
        //int a=3; -> int a; a=3;
        //variable declaration
        auto variableDecl = sema.SemaVariableDecl(tok.content, baseTy);
        astArr.push_back(variableDecl);
        Consume(TokenType::identifier);

        if(tok.tokenType == TokenType::equal){
            Advance();
            auto right=ParseExpr();
            auto assignExpr = std::make_shared<AssignExpr>();
            assignExpr->left = variableDecl;
            assignExpr->right = right;

            astArr.push_back(assignExpr);

        }

    }
    Consume(TokenType::semi);
    return astArr;

};

// left combining

// expr : term (("+" | "-") term)* ;
std::shared_ptr<ASTNode> Parser::ParseExpr()
{
    auto left = ParseTerm();

    while (tok.tokenType == TokenType::plus || tok.tokenType == TokenType::minus)
    {
        OPCode op;
        if (tok.tokenType == TokenType::plus)
        {
            op = OPCode::ADD;
        }
        else
        {
            op = OPCode::SUB;
        }
        Advance();
        auto binaryExpr = std::make_shared<BinaryExpr>();
        binaryExpr->op = op;
        binaryExpr->left = left;
        binaryExpr->right = ParseTerm();

        left = binaryExpr;
    }
    return left;
}

// left combining can be optimized by right combining, but it is more complex to implement,
// and the performance improvement is not significant, so we use left combining here.
// term : factor (("*" | "/") factor)* ;
std::shared_ptr<ASTNode> Parser::ParseTerm()
{

    auto left = ParseFactor();
    while (tok.tokenType == TokenType::star || tok.tokenType == TokenType::slash)
    {
        OPCode op;
        if (tok.tokenType == TokenType::star)
        {
            op = OPCode::MUL;
        }
        else
        {
            op = OPCode::DIV;
        }
        Advance();
        auto binaryExpr = std::make_shared<BinaryExpr>();
        binaryExpr->op = op;
        binaryExpr->left = left;
        binaryExpr->right = ParseFactor();

        left = binaryExpr;
    }
    return left;
};
// term : factor (("*" | "/") factor)* ;
std::shared_ptr<ASTNode> Parser::ParseFactor()
{
    if (tok.tokenType == TokenType::l_parent)
    {
        Advance();
        auto expr = ParseExpr();
        assert(tok.tokenType == TokenType::r_parent);
        Advance();
        return expr;
    }
    else if (tok.tokenType==TokenType::identifier){
        ///semc
        auto expr = std::make_shared<VariableAccessExpr>();
        expr->name = tok.content;
        expr->ty = tok.type;
        Advance();
        return expr;
    }

    else
    {
        auto factor = std::make_shared<NumberExpr>();
        factor->number = tok.value;
        Advance();
        return factor;
    }
};

bool Parser::Expect(TokenType tokenType)
{
    if (tok.tokenType == tokenType)
    {
        return true;
    }
    else
    {
        llvm::errs() << "Error: Expected token type " << static_cast<int>(tokenType) << " but got " << static_cast<int>(tok.tokenType) << " at row " << tok.row << ", col " << tok.col << "\n";
        return false;
    }
};

bool Parser::Consume(TokenType tokenType)
{
    if (Expect(tokenType))
    {
        Advance();
        return true;
    }
    else
    {
        return false;
    }
};

void Parser::Advance()
{
    lexer.NextToken(tok);
};