#include "object/klass.hpp"
#include "object/pyObject.hpp"
#include "object/pyDict.hpp"
#include "object/list.hpp"
#include "object/pyString.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/universe.hpp"
#include "runtime/interpreter.hpp"
#include "object/pyInteger.hpp"
#include "runtime/stringTable.hpp"
#include <stdio.h>
#include <assert.h>

#define st(x) StringTable::get_instance()->str(x)
#define str(x) x##_str //sorry for not using upper case in macro define,but it's really unconvenient.

Klass::Klass() {
   _super = NULL; 
   _mfo = NULL;
}

TypeObject* Klass::super() {
    if (_super == NULL || _super->size() <= 0) {
        return NULL;
    }
    
    return (TypeObject*)_super->get(0);
}

void Klass::add_super(Klass* klass) {
    if (_super == NULL) {
        _super = new List();
    }
    _super->add(klass->type_object());
}

void Klass::order_supers() {
    if (_super == NULL) return ;
    
    if (_mfo == NULL) {
        _mfo = new List();
    }
    
    int cur = -1;
    for (int i = 0;i < _super->size();i++) {
        TypeObject* tp_obj = (TypeObject*)(_super->get(i));
        Klass* k = tp_obj->own_klass();
        _mfo->add(tp_obj);
        if (k->mfo() == NULL) {
            continue;
        }
        
        for (int j = 0;j < k->mfo()->size();j++) {
            TypeObject* tp_obj = (TypeObject*)(k->mfo()->get(j));
            int index = _mfo->index(tp_obj);
            
            if (index < cur) {
                printf("Error: method find order conflict\n");
                assert(false);
            }
            cur = index;
            
            if (index >= 0) {
                _mfo->delete_index(index);
            }
            _mfo->add(tp_obj);
        }
    }
    
    printf("%s's mfo is ", _name->value());
    if (_mfo->size() == 0) printf("empty");
    for (int i = 0;i < _mfo->size();i++) {
        TypeObject* tp_obj = (TypeObject*)(_mfo->get(i));
        Klass* k = tp_obj->own_klass();
        printf("%s,",k->name()->value());
    }
    printf("\n");
}

void Klass::print(PyObject* obj) {
    if (obj && obj->klass() && obj->klass()->name()) {
        printf("<object of ");
        obj->klass()->name()->print();
        printf(", at %p>",obj);//The wildcard %p will match the pointer
    }
    else {
        printf("the obj to print is null\n");
    }
}


PyObject* Klass::create_klass(PyObject* locals,PyObject* supers, PyObject* name) {
    printf("creater_klass start\n");
    assert(locals->klass() == DictKlass::get_instance());
    assert(supers->klass() == ListKlass::get_instance());
    assert(name && name->klass() == StringKlass::get_instance());
    
    Klass* new_klass = new Klass();
    new_klass->set_name((PyString*)name);
    new_klass->set_klass_dict((PyDict*)locals);
    
    //set supers
    List* super_list = (List*)supers;
    new_klass->set_super_list(super_list);
    new_klass->order_supers();

    TypeObject* type_obj = new TypeObject();
    type_obj->set_own_klass(new_klass);
    printf("create_klass ends\n");
    return type_obj;
}

PyObject* Klass::allocate_instance(PyObject* callable, ArrayList<PyObject*>* args) {
    PyObject* ins = new PyObject();
    ins->set_klass(((TypeObject*)callable)->own_klass());

    PyObject* init_method = ins->klass()->klass_dict()->get(StringTable::get_instance()->init_str);

    if (init_method != Universe::pNone) {
        printf("get an initial method\n");

        //util now,init_method is just a functionObject,so it can be recognized as a method od a class.So it needs to be bound to a class
        PyObject* constructor = new MethodObject((FunctionObject*)init_method,ins);
        Interpreter::get_instance()->call_virtual(constructor,args);
    }
    return ins;
}

PyObject* Klass::getattr(PyObject* obj,PyObject* attr) {
    PyObject* func = find_in_parents(obj,st(getattr));
    if (func && func->klass() == FunctionKlass::get_instance()) {
        func = new MethodObject((FunctionObject*)func,obj);
        ObjList args = new ArrayList<PyObject*>();
        args->add(attr);
        return Interpreter::get_instance()->call_virtual(func,args);
    }
    
    PyObject* result = Universe::pNone;
    if (obj->obj_dict() != NULL) {
        result = obj->obj_dict()->get(attr);
        if (result != Universe::pNone) {
            return result;
        }
    }
    
    result = find_in_parents(obj,attr);
    if (MethodObject::is_function(result)) {
        result = new MethodObject((FunctionObject*)result,obj);
    }
    assert(result);
    return result;
}

PyObject* Klass::find_in_parents(PyObject* obj,PyObject* attr) {
    PyObject* result = Universe::pNone;
    result = obj->klass()->klass_dict()->get(attr);
    if (result != Universe::pNone) {
        return result;
    }
    //find attributes in all parents
    if (obj->klass()->mfo() == NULL) {
        return result;
    }
    for (int i = 0;i < obj->klass()->mfo()->size();i++) {
        result = ((TypeObject*)(obj->klass()->mfo()->get(i)))->own_klass()->klass_dict()->get(attr);
        
        if (result != Universe::pNone) {
            break;
        }
    }
    
    return result;
}


PyObject* Klass::setattr(PyObject* obj,PyObject* attr_name,PyObject* val) {
    if (obj->obj_dict() == NULL) {
        obj->init_dict();
    }
    obj->obj_dict()->put(attr_name,val);
    return Universe::pNone;
}

PyObject* Klass::find_and_call(PyObject* x,ObjList args,PyObject* func_name) {
    PyObject* func = x->getattr(func_name);
    if (func != Universe::pNone) {
        return Interpreter::get_instance()->call_virtual(func,args);
    }
    
    printf("class ");
    x->klass()->name()->print();
    printf("Error: unsupport operation for class ");
    assert(false);
    return Universe::pNone;
}

PyObject* Klass::add(PyObject* x,PyObject* y) {
    ObjList args = new ArrayList<PyObject*>();
    args->add(y);
    return find_and_call(x,args,st(add));
}
