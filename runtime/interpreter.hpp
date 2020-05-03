#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "code/codeObject.hpp"
#include "code/bytecode.hpp"
#include "util/map.hpp"
class FrameObject;

class Interpreter {
    private:
        Map<PyObject*,PyObject*>* _builtins;
        FrameObject* _frame;
        PyObject* _ret_value;
        Interpreter();
        static Interpreter* instance;
    public:
        static Interpreter* get_instance();
        void run(CodeObject* codes);
        void build_frame(PyObject* callable,ObjList args);
        void enter_frame(FrameObject* frame);
        void leave_frame();
        void eval_frame();
        void destory_frame();
        PyObject* call_virtual(PyObject* func,ObjList args);
};


#endif
