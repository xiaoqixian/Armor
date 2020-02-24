#ifndef FRAME_HPP
#define FRAME_HPP

#include "code/codeObject.hpp"
#include "util/arrayList.hpp"
#include "util/map.hpp"
#include "object/pyObject.hpp"
#include <stdlib.h>

class FunctionObject;
class List;
class PyDict;

class Block {
    public:
        unsigned char _type;
        unsigned char _target;
        int _level;

        Block() {
            _type = 0;
            _target = 0;
            _level = 0;
        }

        Block(unsigned char type,unsigned char target,int level):
            _type(type),
            _target(target),
            _level(level) {

            }
        Block(const Block& b) {
            _type = b._type;
            _target = b._target;
            _level = b._level;
        }
};

class FrameObject {
    public:
        FrameObject(CodeObject* codes);
        FrameObject(FunctionObject* func,ObjList args);

        FrameObject* _sender;
        FrameObject* sender() {
            return _sender;
        }
        void set_sender(FrameObject* x) {
            _sender = x;
        }

        List* _stack;
        ArrayList<Block*>* _loop_stack;
        ArrayList<PyObject*>* _consts;
        ArrayList<PyObject*>* _names;

        PyDict* _locals; //store local variables
        PyDict* _globals; //store global variables
        List* _fast_locals; //ArrayList<> is just temporary type,later I need to change it into PyList
        
        CodeObject* _codes;
        int _pc;
        bool _entry_frame;

    public:
        bool is_first_frame() {
            return _sender == NULL;
        }
        
        bool is_entry_frame() {
            return _entry_frame;
        }
        void set_entry_frame(bool x) {
            _entry_frame = x;
        }

        void set_pc(int x) {
            _pc = x;
        }

        int get_pc() {
            return _pc;
        }

        List* stack() {
            return _stack;
        }
        ArrayList<Block*>* loop_stack() {
            return _loop_stack;
        }
        ArrayList<PyObject*>* consts() {
            return _consts;
        }
        ArrayList<PyObject*>* names() {
            return _names;
        }
        PyDict* locals() {
            return _locals;
        }
        PyDict* globals() {
            return _globals;
        }
        List* fast_locals() {
            return _fast_locals;
        }

        bool has_more_codes();
        unsigned char get_op_code();
        int get_op_arg();
};

#endif
