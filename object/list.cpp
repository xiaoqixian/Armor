#include "object/list.hpp"
#include <assert.h>
#include <stdio.h>
#include "runtime/universe.hpp"
#include "object/pyDict.hpp"
#include "object/pyString.hpp"
#include "runtime/functionObject.hpp"

#include <iostream>
using namespace std;

ListKlass* ListKlass::instance = NULL;
ListIteratorKlass* ListIteratorKlass::instance = NULL;

ListKlass::ListKlass() {

}

int List::index(PyObject* obj) {
    for (int i = 0;i < size();i++) {
        if (get(i) == obj) {
            return i;
        }
    }
    return -1;
}


void ListKlass::initialize() {
    PyDict* klass_dict = new PyDict(); //seems list has a seperate dict
    klass_dict->put(new PyString("append"),new FunctionObject(append));//append is a function pointer whick is defined in this file
    klass_dict->put(new PyString("insert"),new FunctionObject(insert));
    klass_dict->put(new PyString("index"),new FunctionObject(index));
    klass_dict->put(new PyString("pop"),new FunctionObject(pop));
    klass_dict->put(new PyString("remove"),new FunctionObject(remove));
    klass_dict->put(new PyString("remove_all"),new FunctionObject(removeall));
    set_klass_dict(klass_dict);
    
    (new TypeObject())->set_own_klass(this);
    set_name(new PyString("list"));
    add_super(ObjectKlass::get_instance());
}

ListKlass* ListKlass::get_instance() {
    if (instance == NULL) {
        instance = new ListKlass();
    }
    return instance;
}

PyObject* ListKlass::allocate_instance(ArrayList<PyObject*>* args) {
    if (!args || args->length() == 0) {
        return new List();
    }
    else return NULL;
}

List::List() {
    set_klass(ListKlass::get_instance());
    _inner_list = new ArrayList<PyObject*>();
}

List::List(ObjList ol) {
    set_klass(ListKlass::get_instance());
    _inner_list = ol;
}

void ListKlass::print(PyObject* x) {
    List* lx = (List*)x;
    assert(lx->klass() == (Klass*)this);

    printf("[");
    int size = lx->size();

    if (size >= 1) {
        lx->_inner_list->get(0)->print();
    }
    for (int i = 1;i < size;i++) {
        printf(",");
        lx->_inner_list->get(i)->print();
    }
    printf("]");
}

PyObject* ListKlass::subscr(PyObject* x,PyObject* y) {
    assert(x && x->klass() == (Klass*)this);
    assert(y && y->klass() == (Klass*)IntegerKlass::get_instance());
    
    List* lx = (List*)x;
    PyInteger* iy = (PyInteger*)y;
    
    return lx->inner_list()->get(iy->value());
}

PyObject* ListKlass::contains(PyObject* x,PyObject* y) {
    assert(x && x->klass() == (Klass*)this);
    assert(y);
    
    List* lx = (List*)x;
    int size = lx->inner_list()->size();
    for (int i = 0;i < size;i++) {
        if (lx->inner_list()->get(i)->equal(y)) {
            return Universe::ptrue;
        }
    }
    return Universe::pfalse;
}

void ListKlass::store_subscr(PyObject* obj,PyObject* x,PyObject* y) {
    assert(obj && obj->klass() == (Klass*)this);
    assert(x && x->klass() == IntegerKlass::get_instance());

    List* lobj = (List*)obj;
    PyInteger* ix = (PyInteger*)x;
    lobj->inner_list()->set(ix->value(),y);
}

PyObject* ListKlass::iter(PyObject* obj) {
    assert(obj && obj->klass() == (Klass*)this);
    return new ListIterator((List*)obj);
}

//------------iterator define--------------------------------------
ListIteratorKlass::ListIteratorKlass() {
    PyDict* klass_dict = new PyDict();
    klass_dict->put(new PyString("next"),new FunctionObject(iter_next));
    set_klass_dict(klass_dict);
    
    set_name(new PyString("list_iterator"));
}

ListIterator::ListIterator(List* l) {
    _owner = l;
    _iter_count = 2;
    set_klass(ListIteratorKlass::get_instance());
}

ListIteratorKlass* ListIteratorKlass::get_instance() {
    if (instance == NULL) {
        instance = new ListIteratorKlass();
    }
    return instance;
}


//-------------local functions define-------------------------------

PyObject* append(ObjList args) { //args[0] is the list,args[1] is the element to add
    ((List*)(args->get(0)))->append(args->get(1));
    return Universe::pNone;
}

PyObject* insert(ObjList args) {
    assert(args->get(1)->klass() == (Klass*)IntegerKlass::get_instance());
    int index = ((PyInteger*)(args->get(1)))->value();
    ((List*)(args->get(0)))->insert(index,args->get(2));
    return Universe::pNone;
}

PyObject* index(ObjList args) {
    assert((args->get(0))->klass() == ListKlass::get_instance());
    List* l = (List*)(args->get(0));
    PyObject* obj = args->get(1);
    for (int i = 0;i < l->size();i++) {
        if (l->get(i)->equal(obj) == Universe::ptrue) {
            return new PyInteger(i);
        }
    }
    return Universe::pNone;
}

PyObject* pop(ObjList args) {
    List* l = (List*)(args->get(0));
    assert(l && l->klass() == ListKlass::get_instance());
    return l->pop();
}

PyObject* remove(ObjList args) {
    List* l = (List*)(args->get(0));
    assert(l && l->klass() == ListKlass::get_instance());     
    
    PyObject* obj = args->get(1);
    for (int i = 0;i < l->size();i++) {
        if (l->get(i)->equal(obj) == (PyObject*)Universe::ptrue) {
            l->inner_list()->delete_index(i);
            return Universe::pNone;
        }
    }
    return Universe::pNone;
}

PyObject* removeall(ObjList args) {
    List* l = (List*)(args->get(0));
    assert(l && l->klass() == ListKlass::get_instance());     
    
    PyObject* obj = args->get(1);
    for (int i = 0;i < l->size();i++) {
        if (l->get(i)->equal(obj) == Universe::ptrue) {
            l->inner_list()->delete_index(i);
            i--; //to make the right-adjacent element can also be checked
        }
    }
    return Universe::pNone;
}

PyObject* iter_next(ObjList args) {
    ListIterator* iter = (ListIterator*)(args->get(0));
    
    List* alist = iter->owner();
    int iter_cnt = iter->iter_count();
    printf("iter_cnt: %d\n",iter_cnt);
    if (iter_cnt < alist->size()) {
        PyObject* obj = alist->get(iter_cnt);
        iter->iter_plus();
        return obj;
    }
    else return Universe::pNone;
}
