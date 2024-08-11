// This class defines an element that is stored
// in the garbage collection information list.
//
template <class T>
class PtrDetails
{
  public:
    unsigned refcount; 
    T *memPtr;        
    bool isArray;
    unsigned arraySize; 

    PtrDetails()
    {
        refcount = 0;
        memPtr = nullptr;
        isArray = false;
        arraySize = 0;
    }
};

template <class T>
bool operator==(const PtrDetails<T> &ob1,
                const PtrDetails<T> &ob2)
{
    return ob1.memPtr == ob2.memPtr;
}

