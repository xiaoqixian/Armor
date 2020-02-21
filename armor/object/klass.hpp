#ifndef KLASS_HPP
#define KLASS_HPP

#include <stdio.h>
#include "util/arrayList.hpp"
class PyObject;
class PyString;
class PyDict;
class TypeObject;
class List;

class Klass {
    private:
        PyString* _name;
        
        PyDict* _klass_dict; //used to store methods of specific klass
        
        List* _super; // may used to store the class's father class
        List* _mfo; //method find order.

        TypeObject* _type_obj;

    public:
        Klass();
        
        static PyObject* create_klass(PyObject* locals,PyObject* supers,PyObject* name);  
        
        void add_super(Klass* klass);
        
        TypeObject* super();
        
        PyObject* find_in_parents(PyObject* obj,PyObject* attr);

        void set_super_list(List* super) {
            _super = super;
        }
        
        List* mfo() {
            return _mfo;
        }
        
        void order_supers();

        void set_name(PyString* x) {
            _name = x;
        }
        PyString* name() {
            return _name;
        }
        
        PyObject* getattr(PyObject* obj,PyObject* attr);
        PyObject* setattr(PyObject* obj,PyObject* attr_name,PyObject* val);

        void set_type_object(TypeObject* obj) {
            _type_obj = obj;
        }
        
        TypeObject* type_object() {
            return _type_obj;
        }
        
        virtual PyObject* allocate_instance(PyObject* callable, ArrayList<PyObject*>* args);
        
        void set_klass_dict(PyDict* x) { //seems all the klasses share one klass_dict
            _klass_dict = x;
        }
        
        PyDict* klass_dict() {
            return _klass_dict;
        }

        virtual void print(PyObject* obj);
        PyObject* find_and_call(PyObject* x,ObjList args,PyObject* func_name);

        virtual PyObject* greater(PyObject* x,PyObject* y) {
            return 0;
        }
        virtual PyObject* less(PyObject* x,PyObject* y) {
            return 0;
        }
        virtual PyObject* equal(PyObject* x,PyObject* y) {
            return 0;
        }
        virtual PyObject* not_equal(PyObject* x,PyObject* y) {
             return 0;
        }
        virtual PyObject* le(PyObject* x,PyObject* y) {
             return 0;
        }
        virtual PyObject* ge(PyObject* x,PyObject* y) {
             return 0;
        }  
        virtual PyObject* add(PyObject* x,PyObject* y);
        virtual PyObject* sub(PyObject* x,PyObject* y) {
              return 0;
         }
        virtual PyObject* mul(PyObject* x,PyObject* y) {
              return 0;
         }
        virtual PyObject* div(PyObject* x,PyObject* y) {
              return 0;
         }
        virtual PyObject* mod(PyObject* x,PyObject* y) {
              return 0;
         }
        
        virtual PyObject* subscr(PyObject* x,PyObject* y) {
            return 0;
        }
        
        virtual PyObject* contains(PyObject* x,PyObject* y) {
            return 0;
        }
        
        virtual PyObject* len(PyObject* x) {
            return 0;
        }
        
        virtual PyObject* slice(PyObject* object,PyObject* x,PyObject* y) {
            return 0;
        }

        virtual void store_subscr(PyObject* obj,PyObject* x,PyObject* y) {
            return ;
        }
        
        virtual PyObject* iter(PyObject* obj) {
            return 0;
        }
};

#endif
