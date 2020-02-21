#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include "object/pyInteger.hpp"
#include "object/pyObject.hpp"
#include "util/arrayList.hpp"

class Universe {
    public:
        static PyInteger* ptrue;
        static PyInteger* pfalse;
        static PyObject* pNone;

    public:
        static void genesis();//call this method when the vm starts.
        static void destory();//call this method when the vm ends.
};

PyObject* string_upper(ObjList args);

#endif
