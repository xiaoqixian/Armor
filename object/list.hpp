#ifndef LIST_HPP
#define LIST_HPP

#include "object/klass.hpp"
#include "object/pyObject.hpp"
#include "util/arrayList.hpp"
#include <stdio.h>

class ListKlass: public Klass {
    private:
        ListKlass();
        static ListKlass* instance;

    public:
        void initialize();
        static ListKlass* get_instance();
        
        virtual PyObject* allocate_instance(ArrayList<PyObject*>* args);

        virtual void print(PyObject* x);
        
        virtual PyObject* subscr(PyObject* x,PyObject* y); //get value by index
        virtual PyObject* contains(PyObject* x,PyObject* y);
        virtual void store_subscr(PyObject* obj,PyObject* x,PyObject* y);//modify value by index
        virtual PyObject* iter(PyObject* obj);
};


class List: public PyObject {
    friend class ListKlass;

    private:
        ArrayList<PyObject*>* _inner_list;

    public:
        List();
        List(ObjList ol);
        ArrayList<PyObject*>* inner_list() {
            return _inner_list;
        }

        int size() {
            return _inner_list->size();
        }
        
        void set_size(int size) {
            _inner_list->set_size(size);
        }

        void append(PyObject* obj) {
            _inner_list->add(obj);
        }
        
        void add(PyObject* obj) {
            _inner_list->add(obj);
        }

        PyObject* pop() {
            return _inner_list->pop();
        }
        
        void delete_index(int index) {
            _inner_list->delete_index(index);
        }
        
        int index(PyObject* obj);

        PyObject* get(int index) {
            if (index > _inner_list->size() - 1) {
                printf("ERROR!!! index out of size\n");
            }
            return _inner_list->get(index);
        }

        void set(int i,PyObject* o) {
            _inner_list->set(i,o);
        }

        PyObject* top() {
            return _inner_list->get(_inner_list->size() - 1);
        }
        
        void insert(int index,PyObject* obj) {
            _inner_list->insert(index,obj);
        }
};

// the list iterator
class ListIteratorKlass: public Klass {
    private:
        ListIteratorKlass();
        static ListIteratorKlass* instance;
    public:
        static ListIteratorKlass* get_instance();
        
};

class ListIterator: public PyObject {
    private:
        List* _owner;
        int _iter_count;
    public:
        ListIterator(List* owner);
        List* owner() {
            return _owner;
        }
        int iter_count() {
            return _iter_count;
        }
        void iter_plus() {
            _iter_count++;
        }
};

PyObject* append(ObjList args);
PyObject* insert(ObjList args);
PyObject* index(ObjList args);
PyObject* pop(ObjList args);
PyObject* remove(ObjList args);
PyObject* removeall(ObjList args); //this function does not belong to list in the original python, I add it cause I thought it is useful

PyObject* iter_next(ObjList args);

#endif
