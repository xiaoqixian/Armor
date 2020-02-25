#ifndef PYSTRING_HPP
#define PYSTRING_HPP

#include "object/pyObject.hpp"
#include "object/klass.hpp"
#include "util/arrayList.hpp"

class StringKlass:public Klass {
    private:
        StringKlass();
        static StringKlass* instance;

    public:
        static StringKlass* get_instance();
        virtual void print(PyObject* o);
        void initialize();
        
        virtual PyObject* allocate_instance(ArrayList<PyObject*>* args);

        virtual PyObject* equal(PyObject* x,PyObject* y);
        virtual PyObject* add(PyObject* x,PyObject* y);
        
        virtual PyObject* subscr(PyObject* x,PyObject* y);
        virtual PyObject* contains(PyObject* x,PyObject* y);
        virtual PyObject* len(PyObject* x);
        virtual PyObject* slice(PyObject* object,PyObject* x,PyObject* y);
};

class PyString:public PyObject {
    public:
        char *_value;
        int _length;

    public:
        PyString(const char *x);
        PyString(const char *x, const int length);
        PyString(int len);

        const char *value() {
            return _value;
        }

        int length() {
            return _length;
        }
        
        void set(int i,char x) {
            _value[i] = x;
        }
        
        PyString* join(PyObject* iterable);

};

#endif
