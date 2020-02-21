#include "code/binaryFileParser.hpp"
#include "object/pyString.hpp"
#include "object/list.hpp"
#include "runtime/universe.hpp"
#include <assert.h>
#include <stdio.h>

BinaryFileParser::BinaryFileParser(BufferedInputStream* stream) {
    file_stream = stream;
}

CodeObject* BinaryFileParser::parse() {
    int magic_number = file_stream->read_int();
    printf("magic_number is 0x%x\n",magic_number);
    int moddate = file_stream->read_int();
    printf("moddate is 0x%x\n", moddate);

    char object_type = file_stream->read();
    if (object_type == 'c') {
            CodeObject* result = get_code_object();
            printf("parse OK!\n");
            return result;
    }

    return NULL;
}

CodeObject* BinaryFileParser::get_code_object() {
    int argcount = file_stream->read_int();
    //std::cout << "argcount is " << argcount << std::endl;
    int nlocals = file_stream->read_int();
    int stacksize = file_stream->read_int();
    int flags = file_stream->read_int();
    std::cout << "flags is " << flags << std::endl;

    PyString* byte_codes = get_byte_codes();
    ArrayList<PyObject*>* consts = get_consts();
    ArrayList<PyObject*>* names = get_names();
    ArrayList<PyObject*>* var_names = get_var_names();
    ArrayList<PyObject*>* free_vars = get_free_vars();
    ArrayList<PyObject*>* cell_vars = get_cell_vars();

    PyString* file_name = get_file_name();
    PyString* module_name = get_name();
    int begin_line_no = file_stream->read_int();
    PyString* lnotab = get_no_table();

    return new CodeObject(argcount,nlocals,stacksize,flags,byte_codes,consts,names,var_names,free_vars,cell_vars,file_name,module_name,begin_line_no,lnotab);
}

PyString* BinaryFileParser::get_byte_codes() {
    assert(file_stream->read() == 's');//assert(expression) method,if expression is wrong,print error information and stop the program.

    return get_string();
}

PyString* BinaryFileParser::get_string() {
    int length = file_stream->read_int();
    char* str_value = new char[length];
    
    for (int i = 0;i < length; i++) {
        str_value[i] = file_stream->read();
    }

    PyString* s = new PyString(str_value,length);
    delete[] str_value;
    return s;
}

PyString* BinaryFileParser::get_name() {
    char ch = file_stream->read();

    if (ch == 's') {
        return get_string();
    }
    else if (ch == 't') {
        PyString* str = get_string();
        _string_table.add(str);
        return str;
    }
    else if (ch == 'R') {
        return _string_table.get(file_stream->read_int());
    }

    return NULL;
}

PyString* BinaryFileParser::get_file_name() {
    return get_name();
}

PyString* BinaryFileParser::get_no_table() {
    char ch = file_stream->read();

    if (ch != 's' && ch != 't') {
        file_stream->unread();
        return NULL;
    }

    return get_string();
}

ArrayList<PyObject*>* BinaryFileParser::get_consts() {
    if (file_stream->read() == '(') {
        return get_tuple();
    }

    file_stream->unread();
    return NULL;
}

ArrayList<PyObject*>* BinaryFileParser::get_names() {
    if (file_stream->read() == '(') {
        return get_tuple();
    }

    file_stream->unread();
    return NULL;
}

ArrayList<PyObject*>* BinaryFileParser::get_var_names() {
    if (file_stream->read() == '(') {
        return get_tuple();
    }
    file_stream->unread();
    return NULL;
}

ArrayList<PyObject*>* BinaryFileParser::get_free_vars() {
    if (file_stream->read() == '(') {
        return get_tuple();
    }

    file_stream->unread();
    return NULL;
}

ArrayList<PyObject*>* BinaryFileParser::get_cell_vars() {
    if (file_stream->read() == '(') {
        return get_tuple();
    }
    file_stream->unread();
    return NULL;
}

ArrayList<PyObject*>* BinaryFileParser::get_tuple() {
    int length = file_stream->read_int();
    PyString* str;

    ArrayList<PyObject*>* list = new ArrayList<PyObject*>(length);
    for (int i = 0;i < length;i++) {
        char obj_type = file_stream->read();

        switch (obj_type) {
        case 'c':
            printf("got a code object\n");
            list->add(get_code_object());
            break;
        case 'i':
            list->add(new PyInteger(file_stream->read_int()));
            break;
        /*case 'g':
            list->add(new PyDouble(file_stream->read_double()));
            break;*/
        case 'N':
            list->add(NULL);//later I will change it to PyNone
            break;
        case 't':
            str = get_string();
            list->add(str);
            _string_table.add(str);
            break;
        case 's':
            list->add(get_string());
            break;
        case 'R':
            list->add(_string_table.get(file_stream->read_int()));
            break;
        case '(':
            list->add(new List(get_tuple()));
            break;
        default:
            printf("parser,unrecognized type: %c\n",obj_type);
        }
    }
    return list;
}

