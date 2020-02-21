#include "runtime/frameObject.hpp"
#include "runtime/functionObject.hpp"
#include "object/pyString.hpp"
#include "object/pyDict.hpp"
#include "object/list.hpp"
#include <stdio.h>

FrameObject::FrameObject(CodeObject* codes) {
    _consts = codes->_consts;
    _names = codes->_names;

    _locals = new PyDict();
    _globals = _locals;
    _locals->put(new PyString("__name__"), new PyString("__main__"));

    _stack = new List();
    _loop_stack = new ArrayList<Block*>();

    _codes = codes;
    _pc = 0;
    _entry_frame = false;
}

FrameObject::FrameObject(FunctionObject* func, ObjList args) { //ObjList was defined in util/arrayList.hpp
    _codes = func->_func_code;
    _consts = _codes->_consts;
    _names = _codes->_names;

    _locals = new PyDict();
    _globals = func->_globals;
    _fast_locals = new List();

    if (func->_defaults) {
        int dft_cnt = func->_defaults->length();
        int arg_cnt = _codes->_argcount;
        printf("args count is %d\n",arg_cnt);

        while (dft_cnt--) {
            //printf("get default: %d\n",func->_defaults->get(dft_cnt));
            _fast_locals->set(--arg_cnt,func->_defaults->get(dft_cnt));
        }
    }

    if (args) {
        for (int i = 0;i < args->length();i++) {
            _fast_locals->set(i,args->get(i));
        }
    }

    _stack = new List();
    _loop_stack = new ArrayList<Block*>();

    _pc = 0;
    _sender = NULL;
    _entry_frame = false;
}

int FrameObject::get_op_arg() {
    int byte1 = _codes->_bytecodes->value()[_pc++] & 0xff;
    int byte2 = _codes->_bytecodes->value()[_pc++] & 0xff;
    return byte2 << 8 | byte1;
}

unsigned char FrameObject::get_op_code() {
    return _codes->_bytecodes->value()[_pc++];
}

bool FrameObject::has_more_codes() {
    return _pc < _codes->_bytecodes->length();
}
