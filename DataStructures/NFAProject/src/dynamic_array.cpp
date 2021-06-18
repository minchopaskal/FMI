#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "../headers/utils.hpp"

template <typename T>
class DynamicArray {
private:
    T* data;
    int count;
    int maxCount;

    void eraseArray();
public:
    DynamicArray();
    DynamicArray(const DynamicArray&);
    ~DynamicArray();
    DynamicArray& operator=(const DynamicArray&);

    void add(T);
    void purgeArray();

    bool isEmpty() const { return count == 0; };
    unsigned int getLength() const { return count; }
    T getElemAt(unsigned int);
    T* toPointer();

};

template <typename T>
DynamicArray<T>::DynamicArray() : data(nullptr),
                               count(0), maxCount(UTILS::MAX_SIZE) {
    data = new T[maxCount];
}
template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other) : data(nullptr) {
    *this = other;
}

template <typename T>
void DynamicArray<T>::eraseArray() {
    count = 0;
    delete[] data;
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    eraseArray();
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& rhs) {
    if(this != &rhs) {
        eraseArray();
        count = rhs.count;
        maxCount = rhs.maxCount;
        data = new T[maxCount];
        for(int i = 0; i < count; ++i)
            data[i] = rhs.data[i];
    }
    return *this;
}


template <typename T>
void DynamicArray<T>::add(T value) {
    if(count == maxCount) {
        maxCount *= 2;
        T* newData = new T[maxCount];
        for(int i = 0; i < count; ++i)
            newData[i] = data[i];
        eraseArray();
        data = newData;
    }
    data[count++] = value;

}

template <typename T>
void DynamicArray<T>::purgeArray() {
    eraseArray();
    data = new T[maxCount];
}

template <typename T>
T DynamicArray<T>::getElemAt(unsigned int pos) {
    if(pos < count)
        return data[pos];
    return T();
}

template <typename T>
T* DynamicArray<T>::toPointer() {
    T* newData = new T[maxCount];
    for(int i = 0; i < count; ++i)
        newData[i] = data[i];
    return data;
}

#endif // DYNAMIC_ARRAY_H
