#ifndef DOUBLE_HPP
#define DOUBLE_HPP

class PyObject;
class PyString;
class Klass;

class DoubleKlass: public Klass {
    private:
        PyString* name;
        DoubleKlass* instance;
        DoubleKlass();
        
    public:
        static DoubleKlass* get_instance();
        void set_name(PyString* name) {
            this->name = name;
        }
        void initialize();
        virtual PyObject* add(PyObject* x, PyObject* y);
        virtual PyObject* allocate_instance(PyObject* callable, ArrayList<PyObject*>* args);
        virtual void print(PyObject* obj);
        virtual PyObject* greater(PyObject* x,PyObject* y);
};

class Double: public PyObject {
    private:
        double _value;
        
    public:
        Double(double x);
        void set_value(double val) {
            _value = val;
        }
        double value() {
            return _value;
        }
};

#endif
