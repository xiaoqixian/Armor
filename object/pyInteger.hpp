#ifndef PYINTEGER_HPP
#define PYINTEGER_HPP

#include "object/pyObject.hpp"
#include "object/klass.hpp"
#include "util/arrayList.hpp"

class IntegerKlass:public Klass{
    private:
        IntegerKlass();
        static IntegerKlass* instance;

    public:
        static IntegerKlass* get_instance();
        virtual void print(PyObject* obj);

        virtual PyObject* allocate_instance(PyObject* callable, ArrayList<PyObject*>* args);
        virtual void initialize();

        virtual PyObject* greater (PyObject* x,PyObject* y);
        virtual PyObject* less    (PyObject* x,PyObject* y);
        virtual PyObject* equal   (PyObject* x,PyObject* y);
        virtual PyObject* not_equal(PyObject* x,PyObject* y);
        virtual PyObject* le      (PyObject* x,PyObject* y);
        virtual PyObject* ge      (PyObject* x,PyObject* y);

        virtual PyObject* add     (PyObject* x,PyObject* y);
        virtual PyObject* sub     (PyObject* x,PyObject* y);
        virtual PyObject* mul     (PyObject* x,PyObject* y);
        virtual PyObject* div     (PyObject* x,PyObject* y);
        virtual PyObject* mod     (PyObject* x,PyObject* y);
};

class PyInteger:public PyObject {
    private:
        int _value;

    public:
        PyInteger(int x);
        int value() {
            return _value;
        }
        
};

#endif
