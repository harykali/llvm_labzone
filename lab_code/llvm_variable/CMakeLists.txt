cmake_minimum_required(VERSION 3.20)

project(llvm_variable LANGUAGES C CXX)

include(../../cmake/LLVMProject.cmake)

add_llvm_lab(llvm_variable
    main.cc
    lexer.cc
    parser.cc
    printVisitor.cc
    codegen.cc
    type.cc
    scope.cc
    sema.cc
)
