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
)

INCLUDE_DIRECTORIES(./)
