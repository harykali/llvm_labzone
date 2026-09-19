#pragma once
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"
#include "type.h"
/// char stream -> Token

enum class TokenType
{
    unknown,
    number,
    minus,    // -
    plus,     // +
    star,     // *
    slash,    // '/'
    l_parent, // {
    r_parent, // }
    semi,     // ';'
    mod,      // '%'
    pow,      // '^'
    sharp,   // '#'
    eof,       // end

    //Variable Tokens
    identifier, // variable name
    comma, // e.g: int a,b = 2;
    kw_int, // int
    equal, // =
};

class Token
{
public:
    int row, col;
    TokenType tokenType;
    int value;
    llvm::StringRef content;
    CType *type;
    Token()
    {
        row = col = -1;
        tokenType = TokenType::unknown;
        value = -1;
        type = nullptr;
    }

    void Dump()
    {
        llvm::outs() << "{ " << content  << ", row = " << row << ", col = " << col << "}\n";
    }
};

class Lexer
{
public:
    Lexer(llvm::StringRef sourceCode);

    void NextToken(Token &tok);

private:
    const char *BufPtr;
    const char *LineHeadPtr;
    const char *BufEnd;
    int row;
};
