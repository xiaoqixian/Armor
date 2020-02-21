#include "code/codeObject.hpp"
#include "object/pyString.hpp"

CodeObject::CodeObject(int argcount, int nlocals, int stacksize, int flag, PyString* bytecodes,
        ArrayList<PyObject*>* consts, ArrayList<PyObject*>* names,
        ArrayList<PyObject*>* varnames, ArrayList<PyObject*>* freevars,
        ArrayList<PyObject*>* cellvars, PyString* file_name, PyString* co_name,int lineno, PyString* notable){
        _argcount = argcount;
        _nlocals = nlocals;
        _flag = flag;
        _stack_size = stacksize;
        _bytecodes = bytecodes;
        _names = names;
        _consts = consts;
        _var_names = varnames;
        _free_vars = freevars;
        _cell_vars = cellvars;
        _co_name = co_name;
        _file_name = file_name;
        _lineno = lineno;
        _notable = notable;
    }
