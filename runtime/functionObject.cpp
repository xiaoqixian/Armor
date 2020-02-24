#include "runtime/functionObject.hpp"
#include "object/pyString.hpp"
#include "object/list.hpp"
#include <iostream>
using namespace std;

FunctionKlass* FunctionKlass::instance = NULL;
NativeFunctionKlass* NativeFunctionKlass::instance = NULL;
MethodKlass* MethodKlass::instance = NULL;

FunctionKlass::FunctionKlass() {
    add_super(ObjectKlass::get_instance());
    set_name(new PyString("function"));
    TypeObject* tp_obj = new TypeObject();
    tp_obj->set_own_klass(this);
}

NativeFunctionKlass::NativeFunctionKlass() {
    add_super(FunctionKlass::get_instance());
    set_name(new PyString("native function"));
    TypeObject* tp_obj = new TypeObject();
    tp_obj->set_own_klass(this);
}

MethodKlass::MethodKlass() {
    //set_klass_dict(new PyDict());
    add_super(FunctionKlass::get_instance());
    set_name(new PyString("method"));
    TypeObject* tp_obj = new TypeObject();
    tp_obj->set_own_klass(this);
}

MethodKlass* MethodKlass::get_instance() {
    if (instance == NULL) {
        instance = new MethodKlass();
    }
    return instance;
}

FunctionKlass* FunctionKlass::get_instance() {
    if (instance == NULL) {
        instance = new FunctionKlass();
    }
    return instance;
}

NativeFunctionKlass* NativeFunctionKlass::get_instance() {
    if (instance == NULL) {
        instance = new NativeFunctionKlass();
    }
    return instance;
}

FunctionObject::FunctionObject(PyObject* code_object) {
    assert(code_object);
    CodeObject* co = (CodeObject*)code_object;

    _func_code = co;
    _func_name = co->_co_name;
    _flags = co->_flag;

    set_klass(FunctionKlass::get_instance());
}

void FunctionKlass::print(PyObject* obj) {
    printf("<function: ");
    FunctionObject* fo = static_cast<FunctionObject*>(obj);

    assert(fo && fo->klass() == (Klass*)this);
    fo->func_name()->print();
    printf(">");
}

void FunctionObject::set_default(ObjList x) {
    if (x == NULL) {
        _defaults = NULL;
        return ;
    }

    _defaults = new ArrayList<PyObject*>(x->length());

    for (int i = 0;i < x->length();i++) {
        _defaults->set(i,x->get(i));
    }
}


// operations for native calls
PyObject* FunctionObject::call(ObjList args) {
    return (*_native_func)(args); //_native_func is an attribute of FunctionObject,which is NativeFuncPointer type
} 

// Any function returns PyObject* can be put in this consturctor and get called
FunctionObject::FunctionObject(NativeFuncPointer nfp) {
    _func_code = NULL;
    _func_name = NULL;
    _flags = 0;
    _globals = NULL;
    _native_func = nfp;
    set_klass(NativeFunctionKlass::get_instance());
}

bool MethodObject::is_function(PyObject* x) {
    Klass* k = x->klass();
    if (k == (Klass*)FunctionKlass::get_instance())
        return true;
    
    if (k->mfo() == NULL) {
        return false;
    }
    
    for (int i = 0;i < k->mfo()->size();i++) {
        if (k->mfo()->get(i) == FunctionKlass::get_instance()->type_object()) {
            return true;
        }
    }
    return false;
}

bool MethodObject::is_method(PyObject* func) {
   Klass* k = func->klass(); 
   if (k == (Klass*)MethodKlass::get_instance()) {
       printf("is method klass\n");
       return true;
   }
   return false;
}

//----------------local functions define-------------------------
PyObject* len(ObjList args) {
    return args->get(0)->len();
}

PyObject* string_upper(ObjList args) {
    PyObject* arg0 = args->get(0);
    assert(arg0->klass() == StringKlass::get_instance());
    
    PyString* str = (PyString*)arg0;
    
    int length = str->length();
    if (length <= 0)
        return Universe::pNone;
    
    char* v = new char[length];
    char c;
    for (int i = 0;i < length;i++) {
        c = str->value()[i];
        if (c >= 'a' && c <= 'z') {
            v[i] = c - 0x20;
        }
        else v[i] = c;
    }
    PyString* res = new PyString(v,length);
    delete[] v;
    return res;
}

PyObject* isinstance(ObjList args) { //check if an instance belongs to an object
    PyObject* ins = args->get(0);
    PyObject* obj = args->get(1);
    
    assert(obj && obj->klass() == TypeKlass::get_instance());
    
    Klass* k = ins->klass();
    if (k->type_object() == obj) {
        return Universe::ptrue;
    }
    
    if (k->mfo() == NULL) {
        return Universe::pfalse;
    }
    for (int i = 0;i < k->mfo()->size();i++) {
        if (k->mfo()->get(i) == obj) {
            return Universe::ptrue;
        }
    }
    
    return Universe::pfalse;
}

PyObject* type_of(ObjList args) {
    PyObject* ins = args->get(0);
    return ins->klass()->type_object();
}
