#include <iostream>
#include <memory>
#include <cstdlib>
#include <unistd.h>
#include "lexer.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/ErrorOr.h"
#include "parser.h"
#include "printVisitor.h"
#include "codegen.h"
#include "llvm/Support/TargetSelect.h"
#include "sema.h"
//main
int main(int argc, char **argv)
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
    if (std::getenv("WAIT_FOR_DEBUGGER")) {
        std::cerr << "WAIT_FOR_DEBUGGER set, pid=" << getpid() << ", waiting 60 seconds for attach...\n";
        sleep(60);
    }
    if (argc < 2)
    {
        printf("Please provide input file.\n");
        return -1;
    }

    const char *FileName = argv[1];

    static llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> file = llvm::MemoryBuffer::getFile(FileName);
    if (!file)
    {
        llvm::errs() << "Error reading file: " << FileName << "\n";
        return -1;
    }
    std::unique_ptr<llvm::MemoryBuffer> fileBuffer = std::move(*file);
    Lexer lex(fileBuffer->getBuffer());

    Token tok;
    while (tok.tokenType != TokenType::eof)
    {
        lex.NextToken(tok);
        if (tok.tokenType != TokenType::eof)
            tok.Dump();
        }
    Sema sema;
    Parser parser(lex,sema);
    auto program = parser.ParseProgram();
    std::cout << "Parsing completed.\n";
    PrintVisitor printVisitor(program);
    // CodeGen codeGen(program);
    // std::cout << "Codegen completed.\n";
    return 1;
}
