cmake_minimum_required(VERSION 2.8)

PROJECT(ARMOR)

SET(CMAKE_CXX_FLAGS_DEBUG "O0 -Wall -g -ggdb -rdynamic")
SET(CMAKE_CXX_FLAGS_RELEASE "O3 -Wall -rdynamic")

ADD_EXECUTABLE(armor main.cpp
    code/binaryFileParser.cpp
    code/codeObject.cpp
    object/klass.cpp
    object/pyString.cpp
    object/pyInteger.cpp
    object/pyObject.cpp
    runtime/interpreter.cpp
    runtime/universe.cpp
    util/map.cpp
    runtime/frameObject.cpp
    runtime/functionObject.cpp
    object/list.cpp
    object/pyDict.cpp
    runtime/stringTable.cpp
    runtime/module.cpp
)

ADD_LIBRARY(math SHARED
    extlib/math.cpp
)

ADD_CUSTOM_COMMAND(TARGET math
    POST_BUILD
    COMMAND mkdir -p lib
    COMMAND cp libmath.so lib/
    COMMAND cp ../lib/*.py lib/
    COMMAND python -m compileall lib/*.py
)

INCLUDE_DIRECTORIES(./)
TARGET_LINK_LIBRARIES(armor ${CMAKE_DL_LIBS})
