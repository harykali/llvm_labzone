#pragma once
enum class TypeKind {
    Int,
    
};

class CType{
    private:
    
        TypeKind kind;
        int size; // size in bytes
        int align; // alignment requirement in bytes
    public:
        CType(TypeKind kind, int size, int align): kind(kind), size(size), align(align){};
        static CType *GetIntType();
 };