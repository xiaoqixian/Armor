#ifndef ARMOR_HPP
#define ARMOR_HPP

#include "runtime/functionObject.hpp"
#include "object/pyInteger.hpp"

#define SO_PUBLIC __attribute__((visibility("default")))

struct RGMethod {
    const char* meth_name;
    NativeFuncPointer meth;
    int meth_info;
    const char* meth_doc;
};

typedef RGMethod* (*INIT_FUNC)();

#endif
