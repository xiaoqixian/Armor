#include "object/pyDict.hpp"
#include "object/pyInteger.hpp"
#include "object/pyString.hpp"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

DictKlass* DictKlass::instance = NULL;

DictKlass::DictKlass() {
}

void DictKlass::initialize() {
    PyDict* klass_dict = new PyDict();
    set_klass_dict(klass_dict);
    (new TypeObject())->set_own_klass(this);
    set_name(new PyString("dict"));
    add_super(ObjectKlass::get_instance());
}

DictKlass* DictKlass::get_instance() {
    if (instance == NULL) {
        instance = new DictKlass();
    }
    return instance;
}

PyObject* DictKlass::allocate_instance(ArrayList<PyObject*>* args) {
    if (!args || args->length() == 0) {
        return new PyDict();
    }
    else return NULL;
}

PyDict::PyDict() {
    _map = new Map<PyObject*,PyObject*>();
    set_klass(DictKlass::get_instance());
}

PyDict::PyDict(Map<PyObject*,PyObject*>* x) {
    _map = x;
    set_klass(DictKlass::get_instance());
}

void DictKlass::print(PyObject* x) {
    PyDict* dx = (PyDict*)x;
    assert(x->klass() == this);
    
    printf("{");
    int size = dx->_map->size();
    if (size >= 1) {
        dx->_map->entries()[0]._k->print();
        printf(":");
        dx->_map->entries()[0]._v->print();
    }

    for (int i = 1;i < size;i++) {
        printf(",");
        dx->_map->entries()[i]._k->print();
        printf(":");
        dx->_map->entries()[i]._v->print();
    }
    printf("}");
}

PyObject* DictKlass::subscr(PyObject* obj,PyObject* index) {
    assert(obj && obj->klass() == (Klass*)this);
    
    PyDict* dict = (PyDict*)obj;
    return dict->get(index);
}

void DictKlass::store_subscr(PyObject* obj,PyObject* index,PyObject* val) {
    assert(obj && obj->klass() == (Klass*)this);
    
    PyDict* dict = (PyDict*)obj;
    dict->put(index,val);
    return ;
}
