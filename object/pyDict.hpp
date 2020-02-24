#ifndef DICT_HPP
#define DICT_HPP

#include "object/pyObject.hpp"
#include "util/map.hpp"
#include "object/klass.hpp"
#include "util/arrayList.hpp"
#include <stdio.h>

class DictKlass: public Klass {
    private:
        DictKlass();
        static DictKlass* instance;

    public:
        static DictKlass* get_instance();
        void initialize();
        virtual PyObject* allocate_instance(ArrayList<PyObject*>* args);

        virtual PyObject* subscr(PyObject* obj,PyObject* index);
        virtual void print(PyObject* obj);
        virtual void store_subscr(PyObject* obj,PyObject* index,PyObject* val);
};

class PyDict: public PyObject {
friend class DictKlass;

    private:
        Map<PyObject*,PyObject*>* _map;

    public:
        PyDict();
        PyDict(Map<PyObject*,PyObject*>* map);

        Map<PyObject*,PyObject*>* map() {
            return _map;
        }

        void put(PyObject* k,PyObject* v) {
            _map->put(k,v);
        }

        PyObject* get(PyObject* k) {
            return _map->get(k);
        }

        bool has_key(PyObject* k) {
            return _map->has_key(k);
        }

        int size() {
            return _map->size();
        }

        PyObject* remove(PyObject* k) {
            return _map->remove(k);
        }

        void update(PyDict* d);
};

#endif
