#include "inc/armor.hpp"

#include <math.h>

double get_double(ObjList args) {
    PyObject* x = args->get(0);
    double y = 0;
    if (x && x->klass() == IntegerKlass::get_instance()) {
        y = ((PyInteger*)x)->value();
    }
    else if (x && x->klass() == DoubleKlass::get_instance()) {
        y = ((Double*)x)->value();
    }
    return y;
}



PyObject* math_sqrt(ObjList args) {
    double x = get_double(args);
    return new Double(sqrt(x));
}

PyObject* math_sin(ObjList args) {
    double x = get_double(args);
    return new Double(sin(x));
}

RGMethod math_methods[] = {
    {"sin", math_sin, 0, "sin(x)",}, 
    {"sqrt", math_sqrt, 0, "square root of x", }, 
    {NULL,NULL,0,NULL},
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
