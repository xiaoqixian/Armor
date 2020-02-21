#ifndef ARRAYLIST_HPP
#define ARRAYLIST_HPP

template <typename T>
class ArrayList {
    private:
        int _length;
        T* _array;
        int _size;
        void expand();

    public:
        ArrayList(int n = 8);
        void add(T t);
        void insert(int index, T t);
        T get(int index);
        T pop();
        void set(int index, T t);
        void set_size(int size) {
            _size = size;
        }
        int index(T t);
        int size();
        int length();
        void delete_index(int index);
};

class PyObject;
typedef ArrayList<PyObject*>* ObjList;

#endif
