#ifndef MODULE_HPP
#define MODULE_HPP

#include "object/pyObject.hpp"

class PyDict;

class ModuleKlass: public Klass {
    private:
        static ModuleKlass* instance;
        ModuleKlass();
        
    public:
        static ModuleKlass* get_instance();
        void initialize();
};

class ModuleObject: public PyObject {
    friend class ModuleKlass;
    private:
        PyString* _module_name;
        
    public:
        ModuleObject(PyDict* x);
        static ModuleObject* import_module(PyObject* mod_name);
        static ModuleObject* import_so(PyString* mod_name);
        
        void put(PyObject*name, PyObject* module);
        PyObject* get(PyObject* name);
        void extend(ModuleObject* mo);
};

#endif
