#include "runtime/universe.hpp"
#include <stdlib.h>
#include "object/pyString.hpp"
#include "object/pyDict.hpp"
#include "object/list.hpp"
#include "runtime/functionObject.hpp"

PyInteger* Universe::ptrue = NULL;
PyInteger* Universe::pfalse = NULL;
PyObject* Universe::pNone = NULL;

void Universe::genesis() {
    
    //This part is really IMPORTANT!!! If the order is not right, the whole system will crush down because of circular reference.
    Klass* object_klass = ObjectKlass::get_instance();
    Klass* type_klass = TypeKlass::get_instance();
    
    TypeObject* tp_obj = new TypeObject();
    tp_obj->set_own_klass(type_klass);
    
    TypeObject* obj_obj = new TypeObject();
    obj_obj->set_own_klass(object_klass);

    type_klass->add_super(object_klass);
    
    DictKlass::get_instance()->initialize();
    StringKlass::get_instance()->initialize();
    IntegerKlass::get_instance()->initialize();
    ListKlass::get_instance()->initialize();


    type_klass->set_klass_dict(new PyDict());
    object_klass->set_klass_dict(new PyDict());
 
    type_klass->set_name(new PyString("type"));
    object_klass->set_name(new PyString("object"));

   
    //initialize three global variables 
    ptrue = new PyInteger(1);
    pfalse = new PyInteger(0);
    pNone = new PyObject();
    
    IntegerKlass::get_instance()->order_supers();
    StringKlass::get_instance()->order_supers();
    DictKlass::get_instance()->order_supers();
    ListKlass::get_instance()->order_supers();
    type_klass->order_supers();

    FunctionKlass::get_instance()->order_supers();
    NativeFunctionKlass::get_instance()->order_supers();
    MethodKlass::get_instance()->order_supers();
    
    //Interpreter::get_instance()->initialize();

}
