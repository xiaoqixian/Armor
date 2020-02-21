#include "object/pyObject.hpp"
#include "runtime/functionObject.hpp"
#include "object/pyDict.hpp"
#include "runtime/universe.hpp"
#include "runtime/stringTable.hpp"

#include <iostream>
using namespace std;

void PyObject::print() {
    klass()->print(this);
}

PyObject* PyObject::add(PyObject* x) {
    return klass()->add(this,x);
}

PyObject* PyObject::sub(PyObject* x) {
    return klass()->sub(this,x);
}

PyObject* PyObject::mul(PyObject* x) {
    return klass()->mul(this,x);
}

PyObject* PyObject::div(PyObject* x) {
     return klass()->div(this,x);
 }

PyObject* PyObject::mod(PyObject* x) {
     return klass()->mod(this,x);
 }


PyObject* PyObject::greater(PyObject* x) {
    return klass()->greater(this,x);
}

PyObject* PyObject::less(PyObject* x) {
    return klass()->less(this,x);
}

PyObject* PyObject::equal(PyObject* x) {
    return klass()->equal(this,x);
}

PyObject* PyObject::not_equal(PyObject* x) {
    return klass()->not_equal(this,x);
}  

PyObject* PyObject::le(PyObject* x) {
    return klass()->le(this,x);
}

PyObject* PyObject::ge(PyObject* x) {
    return klass()->ge(this,x);
}

PyObject* PyObject::subscr(PyObject* x) {
    return klass()->subscr(this,x);
}

PyObject* PyObject::contains(PyObject* x) {
    return klass()->contains(this,x);
}

PyObject* PyObject::len() {
    return klass()->len(this);
}

PyObject* PyObject::getattr(PyObject* attr_name) {
    return klass()->getattr(this,attr_name);
}

PyObject* PyObject::setattr(PyObject* attr_name,PyObject* val) {
    return klass()->setattr(this,attr_name,val);
}

void PyObject::init_dict() {
    _obj_dict = new PyDict();
}

/*void PyObject::setattr(PyObject* attr_name,PyObject* val) {
    PyObject* res = Universe::pNone;
    res = klass()->klass_dict()->get(attr_name);
    if (res == Universe::pNone) {
        printf("ERROR!!! No such attribute\n");
        return ;
    }
    klass()->klass_dict()->put(attr_name,val);
}*/

/*PyObject* PyObject::getattr(PyObject* x) {
    TypeObject* type = (TypeObject*)this;
    if (!type->own_klass()) {
        printf("own_klass is null\n");
    }
    else if (!type->own_klass()->klass_dict()) {
        printf("klass_dict is null\n");
    }
    printf("object 1 step\n");
    PyObject* result = Universe::pNone;
    printf("not crushed\n");
    if (klass() == (Klass*)TypeKlass::get_instance()) {
        printf("x is type klass\n");
        TypeObject* type_self = (TypeObject*)this;
        result = type_self->own_klass()->klass_dict()->get(x);
    }
    else {
        printf("not type klaas\n");
        result = klass()->klass_dict()->get(x);
    }
    //result = ((Lis))->get_function();
    printf("object 5 step\n");
    if (result == Universe::pNone) {
        printf("does not find the attribute\n");
        return result;
    }
    
    //only klass attribute need to bind
    printf("object 2 step\n");
    if (MethodObject::is_function(result)) {
        result = new MethodObject((FunctionObject*)result,this);
        printf("object 3 step\n");
    }
    else {
        printf("it is not a function\n");
    }
    return result;
}*/

PyObject* PyObject::slice(PyObject* x,PyObject* y) {
    return klass()->slice(this,x,y);
}

void PyObject::store_subscr(PyObject* x,PyObject* y) {
    klass()->store_subscr(this,x,y);
}

PyObject* PyObject::iter() {
    return klass()->iter(this);
}

//object-klass operatios----------------------------------------
ObjectKlass* ObjectKlass::instance = NULL;

ObjectKlass* ObjectKlass::get_instance() {
    if (instance == NULL) {
        instance = new ObjectKlass();
    }
    return instance;
}

ObjectKlass::ObjectKlass() {
}

//meta-klass operations------------------------------------------
TypeKlass* TypeKlass::instance = NULL;

TypeKlass* TypeKlass::get_instance() {
    if (instance == NULL) {
        instance = new TypeKlass();
    }
    return instance;
}

TypeObject::TypeObject() {
    set_klass(TypeKlass::get_instance());
}

void TypeObject::set_own_klass(Klass* k) {
    _own_klass = k;
    k->set_type_object(this);
}

void TypeKlass::print(PyObject* obj) {
    assert(obj->klass() == (Klass*)this);
    printf("<type ");
    Klass* own_klass = ((TypeObject*)obj)->own_klass();
    
    PyDict* attr_dict = own_klass->klass_dict();
    if (attr_dict) {
        PyObject* mod = attr_dict->get(StringTable::get_instance()->mod_str);
        if (mod != Universe::pNone) {
            mod->print();
            printf(".");
        }
    }
    own_klass->name()->print();
    printf(">");
}

PyObject* TypeKlass::setattr(PyObject* obj, PyObject* attr_name, PyObject* val) {
    TypeObject* type_obj = (TypeObject*)obj;
    type_obj->own_klass()->klass_dict()->put(attr_name,val);
    return Universe::pNone;
}
