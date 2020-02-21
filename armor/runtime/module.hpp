#define MODULE_HPP
#define MODULE_HPP

#include "object/pyObject.hpp"

class PyDict;

class ModuleKlass: public Klass {
    private:
        static ModuleKlass* instance;
        ModuleKlass();
        
    public:
        static ModuleKlass* get_instance();
};

#endif
