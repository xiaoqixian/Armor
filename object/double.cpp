#include "object/double.hpp"
#include "object/pyObject.hpp"
#include "object/klass.cpp"
#include "object/pyInteger.hpp"
#include "runtime/universe.hpp"
#include "object/pyString.hpp"
#include "object/pyDict.hpp"

#include <stdio.h>

DoubleKlass* DoubleKlass::instance = NULL;

DoubleKlass::DoubleKlass() {

}

DoubleKlass* DoubleKlass::get_instance() {
    if (instance == NULL) {
        instance = new DoubleKlass();
    }
    return instance;
}

void DoubleKlass::initialize() {
    set_klass_dict(new PyDict());
    set_name(new PyString("int")); 
    (new TypeObject())->set_own_klass(this);
    add_super(ObjectKlass::get_instance());
}

Double::Double(double x) {
    _value = x;
    set_klass(DoubleKlass::get_instance());
}

void DoubleKlass::print(PyObject* obj) {
    Double* doub = (Double*)obj;
    assert(doub && doub->klass() == (Klass*)this);
    printf("%.12g",doub->value());
}

PyObject* DoubleKlass::allocate_instance(PyObject* callable, ArrayList<PyObject*>* args) {
    if (args == NULL || args->length() == 0) {
        return new Double(0);
    }
    else {
        return NULL;
    }
}

PyObject* DoubleKlass::greater(PyObject* x, PyObject* y) {
    Double* dx = (Double*)x;
    Double* dy = (Double*)y;
    
    assert(dx && dx->klass() == (Klass*)this);
    assert(dy && dy->klass() == (Klass*)this);
    
    if (dx->value() > dy->value()) {
        return Universe::ptrue;
    }
    else {
        return Universe::pfalse;
    }
}
