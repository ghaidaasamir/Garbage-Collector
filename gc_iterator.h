// Exception thrown when an attempt is made to
// use an Iter that exceeds the range of the
// underlying object.
class OutOfRangeExc : public std::exception
{
    public:
        OutOfRangeExc() {
            message = "OutOfRangeExc..";
        }

        const char* what() const noexcept override {
            return message.c_str();
        }

    private:
        std::string message;

};

// An iterator-like class for cycling through arrays
// that are pointed to by GCPtrs. Iter pointers
// ** do not ** participate in or affect garbage
// collection. Thus, an Iter pointing to
// some object does not prevent that object
// from being recycled.
template <class T>
class Iter
{
    T *ptr;
    // current pointer value
    T *end;
    // points to element one past end

    T *begin;        // points to start of allocated array
    unsigned length; // length of sequence
  public:
    Iter()
    {
        ptr = nullptr;
        end = nullptr;
        begin = nullptr;
        length = 0;
    }
    Iter(T *p, T *first, T *last)
    {
        ptr = p;
        end = last;
        begin = first;
        length = last-first;
    }
    // Return length of sequence to which this
    // Iter points.
    unsigned size() 
    {
        return length;
    }
    // Return value pointed to by ptr.
    // Do not allow out-of-bounds access.
    T &operator*()
    {
        if ((ptr >= end) || (ptr < begin))
            throw OutOfRangeExc();
        return *ptr;
    }
    // Return address contained in ptr.
    // Do not allow out-of-bounds access.
    T *operator->()
    {
        if ((ptr >= end) || (ptr < begin))
            throw OutOfRangeExc();
        return ptr;
    }
    // Prefix ++.
    Iter operator++()
    {
        ptr++;
        if (ptr >= end)
            throw OutOfRangeExc();
        return *this;
    }
    // Prefix --.
    Iter operator--()
    {
        ptr--;
        if (ptr < begin)
            throw OutOfRangeExc();
        return *this;
    }
    // Postfix ++.
    Iter operator++(int notused)
    {
        Iter temp = *this;
        if (ptr >= end)
            throw OutOfRangeExc();
        ++ptr;
        return temp;
    }
    // Postfix --.
    Iter operator--(int notused)
    {
        Iter temp = *this;
        if (ptr <= begin)
            throw OutOfRangeExc();
        --ptr;
        return temp;
    }
    // Return a reference to the object at the
    // specified index. Do not allow out-of-bounds
    // access.
    T &operator[](int i)
    {

        if((i<0)||(i>=length)){
            throw OutOfRangeExc();
        }
        return *(begin + i);
    }
    // Define the relational operators.
    bool operator==(Iter op2)
    {
        return op2.ptr==ptr;
    }
    bool operator!=(Iter op2)
    {
        return op2.ptr!=ptr;
    }
    bool operator<(Iter op2)
    {
        return ptr<op2.ptr;
    }
    bool operator<=(Iter op2)
    {
        return ptr<=op2.ptr;
    }
    bool operator>(Iter op2)
    {
        return ptr>op2.ptr;
    }
    bool operator>=(Iter op2)
    {
        return ptr>=op2.ptr;
    }
    // Subtract an integer from an Iter.
    Iter operator-(int n)
    {
        ptr -= n;
        if (ptr < begin)
            throw OutOfRangeExc();
        return *this;
    }
    // Add an integer to an Iter.
    Iter operator+(int n)
    {
        ptr += n;
        if (ptr >= end)
            throw OutOfRangeExc();
        return *this;
    }
    // Return number of elements between two Iters.
    int operator-(Iter<T> &itr2)
    {
        if(ptr>itr2.ptr){
            return ptr-itr2.ptr;
        }
        return itr2.ptr-ptr;
    }
};