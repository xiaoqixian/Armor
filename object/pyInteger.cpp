#include "object/pyInteger.hpp"
#include "runtime/universe.hpp"
#include "object/klass.hpp"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "object/pyString.hpp"

IntegerKlass* IntegerKlass::instance = NULL; //IMPORTANT!!!

IntegerKlass::IntegerKlass() {
}

void IntegerKlass::initialize() {
    (new TypeObject())->set_own_klass(this);
    set_name(new PyString("int"));
    add_super(ObjectKlass::get_instance());
}

PyInteger::PyInteger(int x) {
    _value = x;
    set_klass(IntegerKlass::get_instance());
}


IntegerKlass* IntegerKlass::get_instance() {
    if (instance == NULL) {
        instance = new IntegerKlass();
    }
    return instance;
}

PyObject* IntegerKlass::allocate_instance(PyObject* callable, ArrayList<PyObject*>* args) {
    if (!args || args->length() == 0) {
        return new PyInteger(0); //so this is the default value of the integer object
    }
    else
        return NULL;
}

void IntegerKlass::print(PyObject* x) {
    printf("%d",((PyInteger*)x)->value());
}


PyObject* IntegerKlass::greater(PyObject* x,PyObject* y) {
    PyInteger* ix = (PyInteger*)x;
    PyInteger* iy = (PyInteger*)y;

    assert(ix && (ix->klass() == (Klass*)this));
    assert(iy && (iy->klass() == (Klass*)this));

    if (ix->value() > iy->value()) {
        return Universe::ptrue;
    }
    else return Universe::pfalse;
}

PyObject* IntegerKlass::less(PyObject* x,PyObject* y) {
     PyInteger* ix = (PyInteger*)x;
     PyInteger* iy = (PyInteger*)y;
 
     assert(ix && (ix->klass() == (Klass*)this));
     assert(iy && (iy->klass() == (Klass*)this));
 
     if (ix->value() < iy->value()) {
         return Universe::ptrue;
     }
     else return Universe::pfalse;
 }

PyObject* IntegerKlass::ge(PyObject* x,PyObject* y) {
     PyInteger* ix = (PyInteger*)x;
     PyInteger* iy = (PyInteger*)y;

     assert(ix && (ix->klass() == (Klass*)this));
     assert(iy && (iy->klass() == (Klass*)this));

     if (ix->value() >= iy->value()) {
         return Universe::ptrue;
     }
     else return Universe::pfalse;
 }

PyObject* IntegerKlass::le(PyObject* x,PyObject* y) {
     PyInteger* ix = (PyInteger*)x;
     PyInteger* iy = (PyInteger*)y;

     assert(ix && (ix->klass() == (Klass*)this));
     assert(iy && (iy->klass() == (Klass*)this));

     if (ix->value() <= iy->value()) {
         return Universe::ptrue;
     }
     else return Universe::pfalse;
 }


PyObject* IntegerKlass::equal(PyObject* x,PyObject* y) {
    if (x->klass() != y->klass()) {
        return Universe::pNone;
    }
    PyInteger* ix = (PyInteger*)x;
    PyInteger* iy = (PyInteger*)y;

    assert(ix && (ix->klass() == (Klass*)this));
    assert(iy && (iy->klass() == (Klass*)this));

    if (ix->value() == iy->value()) {
        return Universe::ptrue;
    }
    else return Universe::pfalse;
}

PyObject* IntegerKlass::not_equal(PyObject* x,PyObject* y) {
     PyInteger* ix = (PyInteger*)x;
     PyInteger* iy = (PyInteger*)y;

     assert(ix && (ix->klass() == (Klass*)this));
     assert(iy && (iy->klass() == (Klass*)this));

     if (ix->value() != iy->value()) {
         return Universe::ptrue;
     }
     else return Universe::pfalse;
 }

PyObject* IntegerKlass::add(PyObject* x,PyObject* y) {
    PyInteger* ix = (PyInteger*)x;
    PyInteger* iy = (PyInteger*)y;

    assert(ix && (ix->klass() == (Klass*)this));
    assert(iy && (iy->klass() == (Klass*)this));

    return new PyInteger(ix->value() + iy->value());
}

PyObject* IntegerKlass::sub(PyObject* x,PyObject* y) {
     PyInteger* ix = (PyInteger*)x;
     PyInteger* iy = (PyInteger*)y;
 
     assert(ix && (ix->klass() == (Klass*)this));
     assert(iy && (iy->klass() == (Klass*)this));
 
     return new PyInteger(ix->value() - iy->value());
 }

PyObject* IntegerKlass::mul(PyObject* x,PyObject* y) {
     PyInteger* ix = (PyInteger*)x;
     PyInteger* iy = (PyInteger*)y;

     assert(ix && (ix->klass() == (Klass*)this));
     assert(iy && (iy->klass() == (Klass*)this));

     return new PyInteger(ix->value() * iy->value());
 }

PyObject* IntegerKlass::div(PyObject* x,PyObject* y) {
     PyInteger* ix = (PyInteger*)x;
     PyInteger* iy = (PyInteger*)y;

     assert(ix && (ix->klass() == (Klass*)this));
     assert(iy && (iy->klass() == (Klass*)this));

     return new PyInteger(ix->value() / iy->value());
 }

PyObject* IntegerKlass::mod(PyObject* x,PyObject* y) {
     PyInteger* ix = (PyInteger*)x;
     PyInteger* iy = (PyInteger*)y;

     assert(ix && (ix->klass() == (Klass*)this));
     assert(iy && (iy->klass() == (Klass*)this));

     return new PyInteger(ix->value() % iy->value());
 }

