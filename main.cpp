#include "gc_pointer.h"


int main() {
    // Initial setup and display
    std::cout << "Initial state:\n";
    Pointer<int>::showlist();

    // Test 1: Simple pointer usage
    {
        Pointer<int> p1(new int(42));
        Pointer<int> p2 = p1; // Reference count should increase

        std::cout << "\nAfter creating p1 and p2:\n";
        Pointer<int>::showlist();

        // Reassign p2, should not delete the int 42, but the old p2 data if exists
        p2 = new int(100);

        std::cout << "\nAfter reassigning p2:\n";
        Pointer<int>::showlist();
    }

    std::cout << "\nAfter p1 and p2 go out of scope:\n";
    Pointer<int>::showlist();  // Both p1 and p2 are out of scope, should collect garbage

    // Test 2: Array handling
    {
        Pointer<int, 5> pArray(new int[5]{1, 2, 3, 4, 5});

        std::cout << "\nAfter creating pArray:\n";
        Pointer<int, 5>::showlist();

        // Reassign pArray to a new array
        pArray = new int[3]{10, 20, 30};

        std::cout << "\nAfter reassigning pArray:\n";
        Pointer<int, 5>::showlist();
    }

    std::cout << "\nAfter pArray goes out of scope:\n";
    Pointer<int>::showlist();

    // Final garbage collection
    std::cout << "\nFinal garbage collection:\n";
    Pointer<int>::collect();
    Pointer<int>::showlist();

    return 0;
}