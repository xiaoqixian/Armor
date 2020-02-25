#include "inc/armor.hpp"

PyObject* add(ObjList args) {
    PyInteger* a = (PyInteger*)args->get(0);
    PyInteger* b = (PyInteger*)args->get(1);
    return new PyInteger(a->value() + b->value());
}

RGMethod math_methods[] = {
    {"add", add, 0, "add two intergers",}, 
    {NULL,NULL,0,NULL} 
};

//__cplusplus is already defined by c++ compiler,so this macro sentence is to make sure this function is compiled in C language style
#ifdef __cplusplus
extern "C" {//extern "C" makes sure the function name will not be changed
#endif
    
SO_PUBLIC RGMethod* init_libmath() {
    return math_methods;
}
#ifdef __cplusplus
}
#endif
