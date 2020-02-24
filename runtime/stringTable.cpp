#include "runtime/stringTable.hpp"
#include <stdlib.h>
#include "object/pyString.hpp"

StringTable* StringTable::instance = NULL;

StringTable::StringTable() {
    next_str = new PyString("next");
    mod_str = new PyString("__mudule__");
    init_str = new PyString("__init__");
    add_str = new PyString("__add__");
    getattr_str = new PyString("__getattr__");
    name_str = new PyString("__name__");
    pyc_suf_str = new PyString(".pyc");
}

StringTable* StringTable::get_instance() {
    if (instance == NULL) {
        instance = new StringTable();
    }
    return instance;
}


