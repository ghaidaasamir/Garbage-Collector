#include <iostream>
#include <list>
#include <typeinfo>
#include <cstdlib>
#include "gc_details.h"
#include "gc_iterator.h"

template <class T, int size = 0>
class Pointer{
    private:
        // refContainer maintains the garbage collection list.
        static std::list<PtrDetails<T>> refContainer;
        
        // addr points to the allocated memory to which this Pointer pointer currently points.
        T *addr;
        
        // isArray is true if this Pointer points to an allocated array. It is false otherwise.
        bool isArray;
        
        // If this Pointer is pointing to an allocated array, then arraySize contains its size.
        unsigned arraySize;
        
        // true when first Pointer is created
        static bool first; 

        // Return an iterator to pointer details in refContainer.
        typename std::list<PtrDetails<T> >::iterator findPtrInfo(T *ptr);
        
    public:
        // Define an iterator type for Pointer<T>.
        typedef Iter<T> GCiterator;

        Pointer() : Pointer(nullptr){}
        Pointer(T*);
        
        // Copy constructor.
        Pointer(const Pointer &);
        
        // Destructor for Pointer.
        ~Pointer();
        
        // Collect garbage. Returns true if at least one object was freed.
        static bool collect();

        // Overload assignment of pointer to Pointer.
        T *operator=(T *t);

        // Overload assignment of Pointer to Pointer.
        Pointer &operator=(Pointer &rv);

        // Return a reference to the object pointed to by this Pointer.
        T& operator*(){return *addr;}
        
        // Return the address being pointed to.
        T* operator->(){return addr;}

        // Return a reference to the object at the index specified by i.
        T& operator[](int i){return addr[i];}
        
        // Conversion function to T *.
        operator T *(){return addr;}
        
        // Return an Iter to the start of the allocated memory.
        Iter<T> begin(){
            if(isArray){
                return Iter<T>(addr,addr,addr+arraySize);
            }
            return Iter<T>(addr,addr,addr+1);
        }

        // Return an Iter to one past the end of an allocated array.
        Iter<T> end(){
            if(isArray){
                return Iter<T>(addr+arraySize,addr,addr+arraySize);
            }
            return Iter<T>(addr+1,addr,addr+1);
        }

        // Return the size of refContainer for this type of Pointer.
        static int refContainerSize(){
            return refContainer.size();
        }

        // A utility function that displays refContainer.
        static void showlist();
        
        // Clear refContainer when program exits.
        static void shutdown();
};

// STATIC INITIALIZATION
// Creates storage for the static variables
template <class T, int size>
std::list<PtrDetails<T> > Pointer<T, size>::refContainer;
template <class T, int size>
bool Pointer<T, size>::first = true;

template<class T,int size>
Pointer<T,size>::Pointer(T *t){

    if (first)
        atexit(shutdown);
    first = false;
    addr = t;
    arraySize = size;
    isArray = (size>0);

    auto p = findPtrInfo(t);
    if (p == refContainer.end()) {
        PtrDetails<T> ptr;
        ptr.memPtr = t;
        ptr.arraySize = arraySize;
        ptr.isArray = isArray;
        ptr.refcount = 1;
        refContainer.push_back(ptr);
    } else {
        p->refcount++;
    }
   
}

template< class T, int size>
Pointer<T,size>::Pointer(const Pointer &ob){
    
    addr = ob.addr;

    auto p = findPtrInfo(ob.addr);
    if (p == refContainer.end()) {
        PtrDetails<T> ptr;
        ptr.memPtr = addr;
        ptr.arraySize = ob.arraySize;
        ptr.isArray = ob.isArray;
        ptr.refcount = 1;
        refContainer.push_back(ptr);
    } else {
        p->refcount++;
    }
}

template <class T, int size>
Pointer<T, size>::~Pointer(){
    // When a smart pointer is destroyed, it decrements the reference count in the corresponding PtrDetails object. 
    // If the reference count reaches zero, the memory can be safely deallocated.

    auto p = findPtrInfo(addr);
    if (p != refContainer.end()) {
        p->refcount--;
        if(p->refcount == 0){
            collect();
        }
    }
}

template <class T, int size>
bool Pointer<T, size>::collect(){
    // Scans the refContainer and deallocates memory for pointers with a reference count of zero.
    // Called to perform garbage collection.
    bool flag = false;
    for(auto p = refContainer.begin(); p != refContainer.end();){
        if (p->refcount == 0){
            if(p->isArray){
                delete[] p->memPtr;
            }
            else
            {
                delete p->memPtr;
            }
            p = refContainer.erase(p);
            flag = true;
        }
        p++;
    }
    return flag;
}

template <class T, int size>
T *Pointer<T, size>::operator=(T *t){
    // Self-assignment check
    if (addr == t) {
        return addr;
    }
    //Handle old data
    auto old = findPtrInfo(addr);
    if (old != refContainer.end()) {
        old->refcount--;
        if (old->refcount == 0) {
            collect();
        }
    }

    addr = t;

    auto p = findPtrInfo(t);
    if (p == refContainer.end()) {
        PtrDetails<T> ptr;
        ptr.memPtr = t;
        ptr.arraySize = size;
        ptr.isArray = (size>0);;
        ptr.refcount = 1;
        refContainer.push_back(ptr);
    } else {
        p->refcount++;
    }
    return t;
}

template <class T, int size>
Pointer<T, size> &Pointer<T, size>::operator=(Pointer &rv){
    // Self-assignment check
    if (this == &rv) {
        return *this;
    }
    //Handle old data
    auto old = findPtrInfo(addr);
    if (old != refContainer.end()) {
        old->refcount--;
        if (old->refcount == 0) {
            collect();
        }
    }

    addr = rv.addr;

    PtrDetails<T> ptr;
    ptr.memPtr = rv.addr;
    ptr.arraySize = arraySize;
    ptr.isArray = isArray;
    ptr.refcount = 1;

    auto p = findPtrInfo(ptr.memPtr);
    if (p == refContainer.end()) {
        refContainer.push_back(ptr);
    } else {
        ptr.refcount++;
    }
    return *addr;
}

template <class T, int size>
void Pointer<T, size>::showlist(){
    typename std::list<PtrDetails<T> >::iterator p;
    std::cout << "refContainer<" << typeid(T).name() << ", " << size << ">:\n";
    std::cout << "memPtr refcount value\n ";
    if (refContainer.begin() == refContainer.end())
    {
        std::cout << "Container is empty!\n\n ";
    }
    for (p = refContainer.begin(); p != refContainer.end(); p++)
    {
        std::cout << "[" << (void *)p->memPtr << "]"
             << " refcount: " << p->refcount << " ";
        if (p->memPtr)
            std::cout << " value: " << *p->memPtr;
        else
            std::cout << "---";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template <class T, int size>
typename std::list<PtrDetails<T> >::iterator
Pointer<T, size>::findPtrInfo(T *ptr){
    typename std::list<PtrDetails<T> >::iterator p;
    // Find ptr in refContainer.
    for (p = refContainer.begin(); p != refContainer.end(); p++)
        if (p->memPtr == ptr)
            return p;
    return p;
}

// Clear refContainer when program exits.
template <class T, int size>
void Pointer<T, size>::shutdown(){
    if (refContainerSize() == 0)
        return; // list is empty
    typename std::list<PtrDetails<T> >::iterator p;
    for (p = refContainer.begin(); p != refContainer.end(); p++)
    {
        p->refcount = 0;
    }
    collect();
}

