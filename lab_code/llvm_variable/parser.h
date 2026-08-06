#pragma once
#include "lexer.h"
#include "ast.h"
#include "sema.h"
class Parser {
    private:
        Lexer &lexer;
        Sema &sema;
            //Check if the next token is of the expected type.
        bool Expect(TokenType tokenType);
        
        // Check and consume the next token if it matches the expected type. If it does not match, it should report an error.
        bool Consume(TokenType tokenType);
        //Advnace
        void Advance();



        std::vector<std::shared_ptr<ASTNode>> ParseDecl();
        std::shared_ptr<ASTNode> ParseTerm();
        std::shared_ptr<ASTNode> ParseFactor();
        std::shared_ptr<ASTNode> ParseExpr();
        
        Token tok;
    public:
        Parser(Lexer &lexer, Sema &sema) : lexer(lexer), sema(sema) {};
        std::shared_ptr<Program> ParseProgram();





};