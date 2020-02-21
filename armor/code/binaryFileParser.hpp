#ifndef BINARU_FILE_PARSER_HPP
#define BINATY_FILE_PARSER_HPP
#include <iostream>
#include "util/bufferedInputStream.hpp"
#include "util/arrayList.hpp"
#include "object/pyObject.hpp"
#include "object/pyInteger.hpp"
#include "object/pyString.hpp"
#include "code/codeObject.hpp"

class BinaryFileParser {
    private:
        int cur;
        ArrayList<PyString*> _string_table; //to save the strings so we can unread.
        BufferedInputStream* file_stream;
        
    public:
        BinaryFileParser(BufferedInputStream* stream);
        CodeObject* parse();
        PyString* get_byte_codes();
        PyString* get_no_table();
        int get_int();
        PyString* get_string();
        PyString* get_name();
        PyString* get_file_name();

        ArrayList<PyObject*>* get_consts();
        ArrayList<PyObject*>* get_names();
        ArrayList<PyObject*>* get_var_names();
        ArrayList<PyObject*>* get_free_vars();
        ArrayList<PyObject*>* get_cell_vars();
        ArrayList<PyObject*>* get_tuple();
        CodeObject* get_code_object();
};

#endif
