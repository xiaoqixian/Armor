#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "code/codeObject.hpp"
#include "code/bytecode.hpp"
#include "util/map.hpp"
class FrameObject;
class ModuleObject;
class PyDict;

class Interpreter {
    private:
        ModuleObject* _builtins;
        FrameObject* _frame;
        PyObject* _ret_value;
        Interpreter();
        static Interpreter* instance;
        PyDict* _modules;

    public:
        static Interpreter* get_instance();
        void run(CodeObject* codes);
        void build_frame(PyObject* callable,ObjList args);
        void enter_frame(FrameObject* frame);
        void leave_frame();
        void eval_frame();
        void destory_frame();
        PyObject* call_virtual(PyObject* func,ObjList args);
        PyDict* run_mod(CodeObject* code, PyString* mod_name);
        void initialize();
};


#endif
