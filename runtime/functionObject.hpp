#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "object/klass.hpp"
#include "code/codeObject.hpp"
#include "util/map.hpp"
#include "object/pyObject.hpp"
#include "runtime/universe.hpp"
#include "object/pyDict.hpp"

class FunctionKlass : public Klass {
    private:
        FunctionKlass();
        static FunctionKlass* instance;

    public:
        static FunctionKlass* get_instance();
        virtual void print(PyObject* o);
};

typedef PyObject* (*NativeFuncPointer)(ObjList args); //define a function pointer

class FunctionObject : public PyObject {
    friend class FunctionKlass;
    friend class FrameObject;

    private:
        CodeObject* _func_code;
        PyString* _func_name;

        unsigned int _flags;
        PyDict* _globals; //store variables function refered to in the file which defines the function
        ObjList _defaults;
        NativeFuncPointer _native_func;

    public:
        FunctionObject(PyObject* code_object);
        FunctionObject(Klass* klass) {
            _func_code = NULL;
            _func_name = NULL;
            _flags = 0;

            set_klass(klass);
            _defaults = NULL;
        }
        
        FunctionObject(NativeFuncPointer nfp);

        PyString* func_name() {
            return _func_name;
        }
        int flags() {
            return _flags;
        }
        
        PyDict* globals() {
            return _globals;
        }
        void set_globals(PyDict* x) {
            _globals = x;
        }

        ObjList defaults() {
            return _defaults;
        }
        void set_default(ObjList x);
        
        PyObject* call(ObjList args);
};

class NativeFunctionKlass: public Klass {
    private:
        NativeFunctionKlass();
        static NativeFunctionKlass* instance;

    public:
        static NativeFunctionKlass* get_instance();
};

//Method Klass
class MethodKlass: public Klass {
    private:
        MethodKlass();
        static MethodKlass* instance;
    public:
        static MethodKlass* get_instance();
};

class MethodObject: public PyObject {
    friend class MethodKlass;
    private:
        PyObject* _owner;
        FunctionObject* _func;
        
    public:
        MethodObject(FunctionObject* func):_owner(NULL),_func(func) {
            set_klass(MethodKlass::get_instance());
        }
        MethodObject(FunctionObject* func,PyObject* owner):_owner(owner),_func(func) {
            set_klass(MethodKlass::get_instance());
        }
        
        void set_owner(PyObject* owner) {
            _owner = owner;
        }
        PyObject* owner() {
            return _owner;
        }
        FunctionObject* func() {
            return _func;
        }
        static bool is_function(PyObject* func);
        static bool is_method(PyObject* func);
};

PyObject* len(ObjList args);
PyObject* iter(ObjList args);
PyObject* type_of(ObjList args);
PyObject* isinstance(ObjList args);
PyObject* builtin_super(ObjList args);
PyObject* sysgc(ObjList args);

#endif
