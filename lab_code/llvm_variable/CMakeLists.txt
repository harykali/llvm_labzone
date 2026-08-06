cmake_minimum_required(VERSION 3.18)
project(LLVM_HELLO_WORLD)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

find_package(LLVM REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH ${LLVM_CMAKE_DIR})

include(AddLLVM)

include_directories("${LLVM_BINARY_DIR}/include" "${LLVM_INCLUDE_DIR}")
add_definitions(${LLVM_DEFINITIONS})

if (NOT ${LLVM_ENABLE_RTTI})
    # For non-MSVC compilers
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

# Add debug symbols explicitly
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

set(LLVM_LINK_COMPONENTS Support Core)

add_llvm_executable(${PROJECT_NAME} main.cc lexer.cc parser.cc printVisitor.cc codegen.cc type.cc scope.cc sema.cc)