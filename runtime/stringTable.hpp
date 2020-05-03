#ifndef STRING_TABLE_HPP
#define STRING_TABLE_HPP

//to save some permenat strings like some local functions' names,so the vm doesn't need to create a new PyString object every time and save some memory space.
class PyString;
class StringTable {
    private:
        StringTable();
        static StringTable* instance;
        
    public:
        static StringTable* get_instance();
        PyString *next_str, *mod_str, *init_str, *add_str, *getattr_str;
};

#endif
