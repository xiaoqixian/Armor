#include "object/pyString.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "runtime/universe.hpp"
#include "object/pyInteger.hpp"
#include "object/pyDict.hpp"
#include "runtime/functionObject.hpp"

#include <iostream>
using namespace std;

PyObject* string_upper(ObjList args);

StringKlass* StringKlass::instance = NULL;

StringKlass::StringKlass() {
}

void StringKlass::initialize() {
    (new TypeObject())->set_own_klass(this);
    
    PyDict* klass_dict = new PyDict();
    //klass_dict->put(new PyString)
    set_klass_dict(klass_dict);
    klass_dict->put(new PyString("upper"),new FunctionObject(string_upper));
    
    set_name(new PyString("str"));
    add_super(ObjectKlass::get_instance());
}

StringKlass* StringKlass::get_instance() {
    if (instance == NULL) {
        instance = new StringKlass();
    }
    return instance;
}

PyObject* StringKlass::allocate_instance(ArrayList<PyObject*>* args) {
    if (!args || args->length() == 0) {
        return new PyString("");
    }
}

PyString::PyString(const char *x) {
    _length = strlen(x);
    _value = new char[_length];
    strcpy(_value,x);
    set_klass(StringKlass::get_instance());
}

PyString::PyString(const char *x, int length) {
    _length = length;
    _value = new char[_length];

    for (int i = 0;i < length;i++) {
        _value[i] = x[i];
    }
    set_klass(StringKlass::get_instance());
}

PyString::PyString(int len) {
    _length = len;
    _value = new char[len + 1];
    set_klass(StringKlass::get_instance());
}

void StringKlass::print(PyObject* o) {
    PyString* so = (PyString*)o;
    assert(so && so->klass() == (Klass*)this);

    for (int i = 0;i < so->length();i++) {
        printf("%c",so->value()[i]);
    }
}

PyObject* StringKlass::equal(PyObject* x,PyObject* y) {
    if (x->klass() != y->klass()) {
        return Universe::pfalse;
    }
    PyString* sx = (PyString*)x;
    PyString* sy = (PyString*)y;

    assert(sx && (sx->klass() == (Klass*)this));
    assert(sy && (sy->klass() == (Klass*)this));

    if (sx->length() != sy->length()) {
        return Universe::pfalse;
    }

    for (int i = 0;i < sx->length();i++) {
        if (sx->value()[i] != sy->value()[i]) {
            return Universe::pfalse;
        }
    }
    return Universe::ptrue;
}

PyObject* StringKlass::subscr(PyObject* x,PyObject* y) {
    assert(x && x->klass() == (Klass*)this);
    assert(y && y->klass() == (Klass*)IntegerKlass::get_instance());
    
    PyString* sx = (PyString*)x;
    PyInteger* iy = (PyInteger*)y;
    
    return new PyString(&(sx->value()[iy->value()]),1);
}

PyObject* StringKlass::contains(PyObject* x,PyObject* y) {
    assert(x && x->klass() == (Klass*)this);
    assert(y && y->klass() == (Klass*)this);
    
    PyString* sx = (PyString*)x;
    PyString* sy = (PyString*)y;

    int xsize = sx->length();
    int ysize = sy->length();
    bool flag = true;

    for (int i = 0;i < xsize - ysize;i++) {
        for (int j = 0;j < ysize;j++) {
            if (sx->value()[i + j] != sy->value()[j]) {
                flag = false;
                break;
            }
        }
        if (flag) {
            return Universe::ptrue;
        }
        flag = true;
    }
    return Universe::pfalse;
}

PyObject* StringKlass::len(PyObject* x) {
    return new PyInteger(((PyString*)x)->length());
}

PyObject* StringKlass::slice(PyObject* object,PyObject* x,PyObject* y) {
    assert(object && object->klass() == (Klass*)this);
    assert(x && x->klass() == IntegerKlass::get_instance());
    assert(y && y->klass() == IntegerKlass::get_instance());
    PyString* str = (PyString*)object;
    PyInteger* ix = (PyInteger*)x;
    PyInteger* iy = (PyInteger*)y;

    int len = iy->value() - ix->value();
    if (len <= 0) {
        printf("len <= 0");
        return Universe::pNone;
    }
    char* res = new char[len];
    for (int i = ix->value();i < iy->value();i++) {
        res[i-ix->value()] = str->value()[i];
    }
    PyString* s = new PyString(res,len);
    delete[] res;
    return s;
}

PyObject* StringKlass::add(PyObject* x, PyObject* y) {
    assert(x && x->klass() == this);
    assert(y && y->klass() == this);
    
    PyString* sx = (PyString*)x;
    PyString* sy = (PyString*)y;
    
    PyString* sz = new PyString(sx->length() + sy->length());
    memcpy(sz->_value, sx->_value, sx->length());
    memcpy(sz->_value + sx->length(), sy->_value, sy->length());

    sz->set(sx->length() + sy->length(), '\0');
    return sz;
}
