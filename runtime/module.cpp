#include "runtime/module.hpp"
#include "object/pyDict.hpp"
#include "runtime/universe.hpp"
#include "object/list.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/stringTable.hpp"
#include "util/bufferedInputStream.hpp"
#include "code/binaryFileParser.hpp"
#include "inc/armor.hpp"

#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

#define st(x) StringTable::get_instance()->str(x)
#define str(x) x##_str

ModuleKlass* ModuleKlass::instance = NULL;

ModuleKlass* ModuleKlass::get_instance() {
    if (instance == NULL) {
        instance = new ModuleKlass();
    }
    return instance;
}

ModuleKlass::ModuleKlass() {
    set_name(new PyString("Module"));
}

void ModuleKlass::initialize() {
    PyDict* dict = new PyDict();
    set_klass_dict(dict);
    set_name(new PyString("module"));
    (new TypeObject())->set_own_klass(this);
    add_super(ObjectKlass::get_instance());
}

ModuleObject::ModuleObject(PyDict* dict) {
    set_obj_dict(dict);
    set_klass(ModuleKlass::get_instance());
}

/*
 * access function introduction:
 * access function is to check is a file exits
 * usage: access(file_path,mode)
 * mode:
 * R_OK      测试读许可权
   W_OK      测试写许可权
   X_OK      测试执行许可权
   F_OK      测试文件是否存在
   
return:
    if success: return 0
    else return -1
    more specific details:
    成功执行时，返回0。失败返回-1，errno被设为以下的某个值 
EINVAL： 模式值无效 
EACCES： 文件或路径名中包含的目录不可访问 
ELOOP ： 解释路径名过程中存在太多的符号连接 
ENAMETOOLONG：路径名太长 
ENOENT：路径名中的目录不存在或是无效的符号连接 
ENOTDIR： 路径名中当作目录的组件并非目录 
EROFS： 文件系统只读 
EFAULT： 路径名指向可访问的空间外 
EIO：输入输出错误 
ENOMEM： 不能获取足够的内核内存 
ETXTBSY：对程序写入出错

 */

ModuleObject* ModuleObject::import_module(PyObject* name) {
    PyString* mod_name = (PyString*)name;
    
    //first search in so libs
    List* so_list = new List();
    so_list->add(st(libdir_pre));
    so_list->add(mod_name);
    so_list->add(st(so_suf));
    PyString* file_name = st(empty)->join(so_list);
    
    if (access(file_name->value(), R_OK) == 0) {
        return import_so(mod_name);
    }

    file_name = (PyString*)(mod_name->add(st(pyc_suf)));//pyc_suf refers to ".pyc"
    if (access(file_name->value(), R_OK) == -1) {
        List* pyc_list = new List();
        pyc_list->add(st(libdir_pre));
        pyc_list->add(mod_name); 
        pyc_list->add(st(pyc_suf));
        pyc_list->print();
        file_name = st(empty)->join(pyc_list);
        printf("lib file-name is ");file_name->print();printf("\n");
    }
    assert(access(file_name->value(), R_OK) == 0);
    
    BufferedInputStream stream(file_name->value());
    BinaryFileParser parser(&stream);
    CodeObject* module_code = parser.parse();
    PyDict* module_dict = Interpreter::get_instance()->run_mod(module_code, mod_name);
    return new ModuleObject(module_dict);
}

ModuleObject* ModuleObject::import_so(PyString* mod_name) {
    //all the dynamic libs are put in ./lib/
    char* error_msg = NULL;
    
    PyString* prefix = new PyString("./lib/");
    PyString* so_suffix = new PyString(".so");
    
    PyString* file_name = (PyString*)(prefix->add(mod_name)->add(so_suffix));
    printf("lib name is ");file_name->print();printf("\n");
    void* handle = dlopen(file_name->value(), RTLD_NOW);
    if (handle == NULL) {
        printf("error to open file: %s\n", dlerror());
        return NULL;
    }
    
    PyString* method_prefix = new PyString("init_");
    PyString* init_meth = (PyString*)(method_prefix->add(mod_name));
    INIT_FUNC init_func = (INIT_FUNC)dlsym(handle, init_meth->value());
    if ((error_msg = dlerror()) != NULL) {
        printf("Symbol init_methods not found: %s\n", error_msg);
        dlclose(handle);
        return NULL;
    }
    
    RGMethod* ml = init_func();
    ModuleObject* module = new ModuleObject(new PyDict());
    for (; ml->meth_name != NULL; ml++) {
        module->put(new PyString(ml->meth_name), new FunctionObject(ml->meth));//meth reprecents method
    }
    return module;
}

void ModuleObject::put(PyObject* x, PyObject* y) {
    obj_dict()->put(x, y);
}

PyObject* ModuleObject::get(PyObject* x) {
    return obj_dict()->get(x);
}

void ModuleObject::extend(ModuleObject* mo) {
    obj_dict()->update(mo->obj_dict());
}
