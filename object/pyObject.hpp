#ifndef PYOBJECT_HPP
#define PYOBJECT_HPP
#include "object/klass.hpp"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
class PyString;

class PyObject {
    private:
        Klass* _klass;
        PyDict* _obj_dict;

    public:
        Klass* klass() {
            return _klass;
        }
        void set_klass(Klass* x) {
            _klass = x;
        }
        
        PyDict* obj_dict() {
            return _obj_dict;
        }
        void set_obj_dict(PyDict* x) {
            _obj_dict = x;
        }

        void print();

        PyObject* add(PyObject* x);
        PyObject* sub(PyObject* x);
        PyObject* mul(PyObject* x);
        PyObject* div(PyObject* x);
        PyObject* mod(PyObject* x);

        PyObject* greater(PyObject* x);
        PyObject* less(PyObject* x);
        PyObject* equal(PyObject* x);
        PyObject* not_equal(PyObject* x);
        PyObject* le(PyObject* x);
        PyObject* ge(PyObject* x);
        PyObject* subscr(PyObject* x);
        PyObject* contains(PyObject* x);
        PyObject* len();

        PyObject* getattr(PyObject* attr_name);
        PyObject* setattr(PyObject* attr_name,PyObject* val);
        void init_dict();

        PyObject* slice(PyObject* x,PyObject* y);
        void store_subscr(PyObject* x,PyObject* y);
        PyObject* iter();
};

class ObjectKlass: public Klass {
    private:
        ObjectKlass();
        static ObjectKlass* instance;
        
    public:
        static ObjectKlass* get_instance();
};

//meta-klass for object system
class TypeKlass: public Klass {
    private:
        TypeKlass() {

        }
        static TypeKlass* instance;
    public:
        static TypeKlass* get_instance();
        virtual void print(PyObject* obj);
        virtual PyObject* setattr(PyObject* obj, PyObject* attr_name, PyObject* val);
};

class TypeObject: public PyObject {
    private:
        Klass* _own_klass;

    public:
        TypeObject();
        void set_own_klass(Klass* k);
        Klass* own_klass() {
            assert(_own_klass);
            return _own_klass;
        }
};

#endif
