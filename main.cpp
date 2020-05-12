#include "util/bufferedInputStream.hpp"
#include "code/binaryFileParser.hpp"
#include "code/codeObject.hpp"
#include <stdio.h>
#include "runtime/interpreter.hpp"
#include "runtime/universe.hpp"
#include "object/pyDict.hpp"
#include "object/pyString.hpp"
#include "object/pyInteger.hpp"
#include "util/figure.h"

#define PROJECT_NAME "ARMOR"

int main(int argc,char** argv) {
    if (argc < 1){
        printf("vm needs a paramter:finename\n");
        return 0;
    }
    printf("-----Welcome to Armor!-----\n");
    print_str(PROJECT_NAME);
    Universe::genesis();
    printf("1\n");
    BufferedInputStream stream(argv[1]);
    BinaryFileParser parser(&stream);
    CodeObject* main_code = parser.parse();

    Interpreter* interpreter = Interpreter::get_instance();
    //dict->get(new PyString("__name__"))->print();
    interpreter->run(main_code);
    return 0;
}
