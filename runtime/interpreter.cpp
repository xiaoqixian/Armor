#include "runtime/interpreter.hpp"
#include <stdio.h>
#include "runtime/frameObject.hpp"
#include "runtime/functionObject.hpp"
#include "util/arrayList.hpp"
#include "object/pyInteger.hpp"
#include "object/pyString.hpp"
#include "object/pyObject.hpp"
#include "runtime/universe.hpp"
#include <stdlib.h>
#include "object/pyDict.hpp"
#include "object/list.hpp"
#include "runtime/stringTable.hpp"
#include "object/klass.hpp"

#include <iostream>
using namespace std;

#define TRUE Universe::ptrue
#define FALSE Universe::pfalse

Interpreter* Interpreter::instance = NULL;

Interpreter::Interpreter() {
    _builtins = new Map<PyObject*,PyObject*>();

    _builtins->put(new PyString("True"), Universe::ptrue);
    _builtins->put(new PyString("False"), Universe::pfalse);
    _builtins->put(new PyString("None"), Universe::pNone);
    _builtins->put(new PyString("len"), new FunctionObject(len));
    _builtins->put(new PyString("int"), IntegerKlass::get_instance()->type_object());
    _builtins->put(new PyString("str"), StringKlass::get_instance()->type_object());
    _builtins->put(new PyString("object"), ObjectKlass::get_instance()->type_object());
    _builtins->put(new PyString("list"), ListKlass::get_instance()->type_object());
    _builtins->put(new PyString("dict"), DictKlass::get_instance()->type_object());
    _builtins->put(new PyString("isinstance"), new FunctionObject(isinstance));
    _builtins->put(new PyString("type"), new FunctionObject(type_of));

}

Interpreter* Interpreter::get_instance() {
    if (instance == NULL) {
        instance = new Interpreter();
    }
    return instance;
}

void Interpreter::run(CodeObject* codes) {

    _frame = new FrameObject(codes);
    printf("locals start :");_frame->locals()->print();
    eval_frame();
    destory_frame();
}

void Interpreter::eval_frame() {
    Block* b;
    FunctionObject* fo;
    ArrayList<PyObject*>* args = NULL;
    while (_frame->has_more_codes()) {
        unsigned char op_code = _frame->get_op_code();
        bool has_argument = (op_code & 0xFF) >= ByteCode::HAVE_ARGUMENT;
        int op_arg = -1;

        if (has_argument) {
            op_arg = _frame->get_op_arg();
        }
        PyObject *v, *w, *u, *attr;

        //printf("op_code:%d\n",(int)op_code);
        switch(op_code) {
            case ByteCode::POP_TOP:
                printf("POP_TOP\n");
                _frame->_stack->pop();
                break;
            case ByteCode::LOAD_CONST:
                printf("LOAD_CONST\n");
                _frame->_stack->add(_frame->_consts->get(op_arg));
                break;
            case ByteCode::PRINT_ITEM:
                printf("PRINT_ITEM\n");
                v = _frame->_stack->pop();
                v->print();
                break;
            case ByteCode::BINARY_ADD:
                v = _frame->_stack->pop();
                w = _frame->_stack->pop();
                _frame->_stack->add(v->add(w));
                break;
            case ByteCode::BINARY_SUBTRACT:
                break;
            case ByteCode::PRINT_NEWLINE:
                printf("\nPRINT_NEWLINE");
                printf("\n");
                break;
            case ByteCode::COMPARE_OP:
                w = _frame->_stack->pop();
                v = _frame->_stack->pop();
                switch(op_arg) {
                    case ByteCode::GREATER:
                        _frame->_stack->add(v->greater(w));
                        break;
                    case ByteCode::LESS:
                        _frame->_stack->add(v->less(w));
                        break;
                    case ByteCode::EQUAL:
                        _frame->_stack->add(v->equal(w));
                        break;
                    case ByteCode::NOT_EQUAL:
                        _frame->_stack->add(v->equal(w));
                        break;
                    case ByteCode::GREATER_EQUAL:
                        _frame->_stack->add(v->ge(w));
                        break;
                    case ByteCode::LESS_EQUAL:
                        _frame->_stack->add(v->le(w));
                        break;
                    case ByteCode::IN:
                        _frame->_stack->add(w->contains(v));
                        break;
                    case ByteCode::NOT_IN:
                        if (w->contains(v)) {
                            _frame->_stack->add(FALSE);
                            break;
                        }
                        _frame->_stack->add(TRUE);
                        break;
                    case ByteCode::IS:
                        if (v == w) {//directly compare the pointer addresses of two objects.
                            _frame->_stack->add(TRUE); 
                        }
                        else _frame->_stack->add(FALSE);
                        break;
                    case ByteCode::IS_NOT:
                        if (v == w) {
                            _frame->_stack->add(FALSE);
                        }
                        else _frame->_stack->add(TRUE);
                        break;
                    default:
                        printf("Error:Unrecognized op_arg %d\n", op_arg);
                }
                break;
            case ByteCode::POP_JUMP_IF_FALSE:
                v = _frame->_stack->pop();
                if (((PyInteger*)v)->value() == 0) {
                    _frame->set_pc(op_arg);
                }
                break;
            case ByteCode::JUMP_FORWARD:
                _frame->set_pc(_frame->get_pc() + op_arg);
                break;
            case ByteCode::JUMP_ABSOLUTE:
                _frame->set_pc(op_arg);
                break;
            case ByteCode::SETUP_LOOP:
                _frame->_loop_stack->add(new Block(op_code,_frame->get_pc() + op_arg,_frame->_stack->size()));
                break;
            case ByteCode::POP_BLOCK:
                b = _frame->_loop_stack->pop();
                while (_frame->_stack->size() > b->_level) {
                    _frame->_stack->pop();
                }
                break;
            case ByteCode::BREAK_LOOP:
                b = _frame->_loop_stack->pop();
                while (_frame->_stack->size() > b->_level) {
                    _frame->_stack->pop();
                }
                _frame->set_pc(b->_target);
                break;
                
            case ByteCode::GET_ITER:
                v = _frame->_stack->pop();
                _frame->_stack->add(v->iter());
                break;

            case ByteCode::FOR_ITER:
                v = _frame->_stack->pop();
                //w = v->getattr(StringTable::get_instance()->next_str);
                w = v->getattr(new PyString("next"));
                build_frame(w,NULL);
                
                if (_frame->_stack->get(_frame->_stack->size() - 1) == NULL) {//the top of the stack is the return value,if it is null,means the iteration is over.
                    _frame->_pc += op_arg;
                    _frame->_stack->pop();
                }
                break;

            case ByteCode::STORE_NAME:
                printf("STORE_NAME\n");
                //printf("names length is %d\n",_frame->names()->length());
                v = _frame->names()->get(op_arg);
                if (!_frame->names() || !v) {
                    printf("ERROR!!! frame names is null or name is null\n");
                }
                _frame->locals()->put(v,_frame->_stack->pop());
                break;
            
            //SLICK op_code,I add it myself,this op_code is to slice a string by two args.
            case ByteCode::SLICE:
                v = _frame->_stack->pop(); //the arg on the right side of the colon
                w = _frame->_stack->pop(); //left side arg
                u = _frame->_stack->pop(); //the string
                _frame->_stack->add(u->slice(w,v));
                break;

            case ByteCode::LOAD_NAME:
                printf("LOAD_NAME\n");
                v = _frame->names()->get(op_arg);
                v->print();
                printf(" is the map key \n");

                w = _frame->locals()->get(v); // locals is a Map class

                //w = new PyString("__main__");
                //first find from local variables
                if (w != Universe::pNone) {
                    printf("find from locals\n");
                    _frame->_stack->add(w);
                    break;
                }
                //if don't find any,find in global variables
                w = _frame->globals()->get(v);
                if (w != Universe::pNone) {
                    printf("find from global\n");
                    _frame->_stack->add(w);
                    break;
                }
                //find in builtin table,include True,False,None
                w = _builtins->get(v);
                if (w != Universe::pNone) {
                    _frame->_stack->add(w);
                    break;
                }

                printf("can't find the name\n");
                _frame->_stack->add(Universe::pNone);
                break;
                
            case ByteCode::LOAD_LOCALS: //load loacls is to load local varianbles in the current frame into the stack
                printf("LOAD_LOCALS\n");
                _frame->_stack->add(_frame->_locals);
                break;

            case ByteCode::LOAD_GLOBAL:
                v = _frame->names()->get(op_arg);
                w = _frame->globals()->get(v);
                if (w != Universe::pNone) {
                    _frame->_stack->add(w);
                    break;
                }

                w = _builtins->get(v);
                if (w != Universe::pNone) {
                    _frame->_stack->add(w);
                    break;
                }
                _frame->_stack->add(Universe::pNone);
                break;
            case ByteCode::MAKE_FUNCTION:
                printf("MAKE_FUNCTION\n");
                v = _frame->_stack->pop(); //v is CodeObject type
                fo = new FunctionObject(v);
                fo->set_globals(_frame->globals());
                if (op_arg > 0) {
                    args = new ArrayList<PyObject*>(op_arg);
                    while (op_arg--) {
                        args->set(op_arg,_frame->_stack->pop());
                    }
                }
                fo->set_default(args);
                if (args != NULL) {
                    delete args;
                    args = NULL;
                }
                _frame->_stack->add(fo);
                break;
            case ByteCode::CALL_FUNCTION:
                printf("CALL_FUNCTION\n");
                if (op_arg > 0) { //op_arg represents the number of args
                    args = new ArrayList<PyObject*>(op_arg);
                    while (op_arg--) {
                        args->set(op_arg, _frame->_stack->pop());
                    }
                }
                
                build_frame(_frame->_stack->pop(),args);
                if (args != NULL) {
                    delete args;
                    args = NULL;
                }
                break;
            case ByteCode::LOAD_FAST:
                printf("LOAD_FAST\n");
                //printf("load_fast op_arg is %d\n",op_arg);
                //_frame->fast_locals()->print();printf("\n");
                //assert(_frame->fast_locals()->get(op_arg));
                _frame->_stack->add(_frame->fast_locals()->get(op_arg));
                break;
            case ByteCode::STORE_FAST:
                printf("STORE_FAST\n");
                _frame->fast_locals()->set(op_arg,_frame->_stack->pop());
                break;
                
            //method in class
            case ByteCode::LOAD_ATTR:
                printf("LOAD_ATTR\n");
                v = _frame->_stack->pop(); //the class
                w = _frame->_names->get(op_arg); //method in class
                printf("the attr to load is ");
                w->print();printf("\n");
                _frame->_stack->add(v->getattr(w));
                break;
            case ByteCode::STORE_ATTR:
                v = _frame->_stack->pop(); //the instance
                u = _frame->_stack->pop(); // the new value to set
                w = _frame->_names->get(op_arg); //the attribute name
                v->setattr(w,u);
                break;

            //list operations
            case ByteCode::BUILD_TUPLE://for convenience,I use list to replace tuple,they both work fine.
            case ByteCode::BUILD_LIST:
                v = new List();
                while (op_arg--) {
                    ((List*)v)->set(op_arg,_frame->_stack->pop());
                }
                _frame->_stack->add(v);
                break;
            
            case ByteCode::STORE_SUBSCR:
                v = _frame->_stack->pop(); //the index
                w = _frame->_stack->pop(); //the list or the map
                u = _frame->_stack->pop(); //the value to be seted
                w->store_subscr(v,u);
                break;
            case ByteCode::BINARY_SUBSCR:
                v = _frame->_stack->pop();//index
                w = _frame->_stack->pop();//list
                _frame->_stack->add(w->subscr(v));
                break;

            //map operations
            case ByteCode::BUILD_MAP:
                v = new PyDict();
                _frame->_stack->add(v);
                break;
            case ByteCode::STORE_MAP://store key_value in the stack into map
                w = _frame->_stack->pop();
                u = _frame->_stack->pop();
                v = _frame->_stack->get(_frame->_stack->size() - 1);
                ((PyDict*)v)->put(w,u);
                break;
                
            case ByteCode::BUILD_CLASS:
                printf("BUILD_CLASS\n");
                v = _frame->_stack->pop(); // class locals
                printf("locals in BUILD_CLASS is ");
                v->print();
                printf("\n");
                w = _frame->_stack->pop(); //class supers
                u = _frame->_stack->pop();// class name
                v = Klass::create_klass(v, w, u);
                _frame->_stack->add(v);
                break;

            case ByteCode::RETURN_VALUE:
                printf("RETURN_VALUE\n");
                _ret_value = _frame->_stack->pop();
                if (_frame->is_first_frame() || _frame->is_entry_frame()) {
                    return ;
                }
                leave_frame();
                break;
            default:
                printf("Error:Unrecognized op_code %d\n",op_code);
        }
    }
}

void Interpreter::leave_frame() {
    destory_frame();
    _frame->_stack->add(_ret_value);
}

void Interpreter::destory_frame() {
    FrameObject* temp = _frame;
    _frame = _frame->sender();
    delete temp;
}

void Interpreter::enter_frame(FrameObject* frame) {
    frame->set_sender(_frame);
    _frame = frame;
}

void Interpreter::build_frame(PyObject* callable, ObjList args) {
    if (callable->klass() == NativeFunctionKlass::get_instance()) {
        printf("native function\n");
        _frame->_stack->add(((FunctionObject*)callable)->call(args));
    }

    else if (callable->klass() == MethodKlass::get_instance()) {
        printf("method function\n");
        MethodObject* method = (MethodObject*)callable;
        if (!args) {
            args = new ArrayList<PyObject*>(1);
        }
        args->insert(0,method->owner());
        build_frame(method->func(),args);
    }

    else if (callable->klass() == FunctionKlass::get_instance()){
        printf("normal function\n");
        FrameObject* frame = new FrameObject((FunctionObject*)callable, args);
        frame->set_sender(_frame);
        _frame = frame;
    }
    
    else if (callable->klass() == TypeKlass::get_instance()) {
        printf("instantial a class\n");
        PyObject* ins = ((TypeObject*)callable)->own_klass()->allocate_instance(callable, args);
        _frame->_stack->add(ins);
    }
}

PyObject* Interpreter::call_virtual(PyObject* func,ObjList args) {
    if (func->klass() == NativeFunctionKlass::get_instance()) {
        return ((FunctionObject*)func)->call(args);
    }
    else if (func->klass() == MethodKlass::get_instance()) {
        printf("call_virtual method\n");
        MethodObject* method = (MethodObject*)func;
        if (!args) {
            args = new ArrayList<PyObject*>(1);
        }
        args->insert(0,method->owner());
        return call_virtual(method->func(),args);
    }
    else if (MethodObject::is_function(func)) {
        printf("call_virtual function\n");
        FrameObject* frame = new FrameObject((FunctionObject*)func,args);
        frame->set_entry_frame(true);
        enter_frame(frame);
        eval_frame();
        destory_frame();
        return _ret_value;
    }
    return Universe::pNone;
}
