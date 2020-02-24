#ifndef CODE_OBJECT_HPP
#define CODE_OBJECT_HPP
 
//#include "/home/lunar/pythonFiles/pythonVM/object/PyObject.hpp"
#include "object/pyObject.hpp"
//#include "/home/lunar/pythonFiles/pythonVM/binaryFile/arrayList.cpp"
#include "util/arrayList.cpp"
#include "object/pyString.hpp"

class CodeObject : public PyObject {
 public:
     int _argcount;
     int _nlocals;
     int _stack_size;
     int _flag;

     PyString* _bytecodes;
     ArrayList<PyObject*>*  _names;
     ArrayList<PyObject*>*  _consts;
     ArrayList<PyObject*>*  _var_names;

     ArrayList<PyObject*>*  _free_vars;
     ArrayList<PyObject*>*  _cell_vars;

     PyString* _co_name;
     PyString* _file_name;

     int _lineno;
     PyString* _notable;
     CodeObject(int argcount, int nlocals, int stacksize, int flag, PyString* bytecodes,
             ArrayList<PyObject*>* consts,ArrayList<PyObject*>* names,
             ArrayList<PyObject*>* varnames,
             ArrayList<PyObject*>* freevars, ArrayList<PyObject*>* cellvars, PyString* file_name, PyString* co_name, int lineno, PyString* notable);
};

#endif
