// ===============================================================================
// UniquePtr.cpp // std::unique_ptr
// ===============================================================================

module;

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

module modern_cpp:unique_ptr;

import :dummy;


namespace UniquePointerGeneral {

    std::unique_ptr<int> loadUniquePointer()
    {
        std::unique_ptr<int> ptr{ std::make_unique<int>(100) };
        return ptr;
    }

    void storeUniquePointer(std::unique_ptr<int>& ptr)
    {
        std::cout << "*ptr:    " << *ptr << std::endl;
        (*ptr)++;
        std::cout << "*ptr:    " << *ptr << std::endl;

        ptr = nullptr;

        // take ownership right now:
        // std::unique_ptr<int> ptr2{ std::move(ptr) };
    }

    //void storeUniquePointerSafe(const std::unique_ptr<int>& ptr)
    //{
    //    std::cout << "*ptr:    " << *ptr << std::endl;
    //    (*ptr)++;
    //    std::cout << "*ptr:    " << *ptr << std::endl;

    //    // take ownership right now:
    //    // std::unique_ptr<int> ptr2{ std::move(ptr) };
    //}

    void storeUniquePointerSafe(const std::unique_ptr<int>& ptr)
    {
        std::cout << "*ptr:    " << *ptr << std::endl;
        (*ptr)++;
        std::cout << "*ptr:    " << *ptr << std::endl;
    }

    void storeUniquePointerAlternate(int* ptr)
    {
        std::cout << "*ptr:    " << *ptr << std::endl;
        (*ptr)++;
        std::cout << "*ptr:    " << *ptr << std::endl;

        // A) taking ownership right now: MAKES NO SENSE
        // B) delete: Under no circumstances: 
        //    pointer is owned by accompanied Unique Ptr
    }

    void test_01() 
    {
        //int* ip1;
        //int* ip2;

        //ip1 = ip2;

        //std::unique_ptr<int> x;
        //std::unique_ptr<int> y;

        //x = y;



        // create a unique_ptr to an int with value 123
        std::unique_ptr<int> ptr1{ new int{ 123 } };
        // or
        // std::unique_ptr<int> ptr1{ std::make_unique<int>(123) };
        // or
        // auto ptr1{ std::make_unique<int>(123) };

        // access value behind smart pointer
        std::cout << "*ptr1:   " << *ptr1 << std::endl;

        // access value using raw pointer
        int* ip1{ ptr1.get() };
        (*ip1)++;
        std::cout << "*ip:     " << *ip1 << std::endl;

        // access value - again - behind smart pointer
        std::cout << "*ptr1:   " << *ptr1 << std::endl;

        // second std::unique_ptr by moving 'ptr1' to 'ptr2',
        // 'ptr1' doesn't own the object anymore
        std::unique_ptr<int> ptr2{ std::move(ptr1) };
       // std::cout << "*ptr1: " << *ptr1 << std::endl;  // crashes 
        std::cout << "*ptr2:   " << *ptr2 << std::endl;

        std::unique_ptr<int> ptr3;
        ptr3 = std::move(ptr2);

        int* ip3{ ptr3.get() };
        (*ip3)++;
        std::cout << "*ptr3:   " << *ptr3 << std::endl;

      //  Dummy d;

        std::unique_ptr<Dummy> myDummy { new Dummy{ 123 } };
    }

    void test_02()
    {
        // retrieving a unique pointer from a function
        std::unique_ptr<int> ptr{ loadUniquePointer() };
        std::cout << "*ptr:   " << *ptr << std::endl;

        // provide a function with a unique pointer: who owns the pointer now?
        storeUniquePointer(ptr);

        const std::unique_ptr<const int> ptr2 = std::move(ptr);
        // ++ptr2;  // Error
        int tmp = *ptr2;

        // C++ Core Guidelines
        storeUniquePointerAlternate(ptr.get());

        // does this work?
        std::cout << "*ptr:   " << *ptr << std::endl;
    }

    void test_03()
    {
        // creates a unique_ptr to an array of 20 ints
        std::unique_ptr<int[]> ptr{ std::make_unique<int[]>(20) };
    }
}

namespace UniquePointer_SourceSinkPattern
{
    std::unique_ptr<int> createResource(int value)
    {
        std::unique_ptr<int> ptr{ std::make_unique<int>(value) };
        return ptr;
    }

    void consumeResource(std::unique_ptr<int> ptr)  // call by-value (!)
    {
        std::cout << "*ptr:    " << *ptr << std::endl;
        // now *ptr is deleted 
    }

    void test_04()
    {
        // creating a unique pointer with the help of a creator function
        std::unique_ptr<int> ptr{ createResource(123) };
        std::cout << "*ptr:   " << *ptr << std::endl;

        // Pass unique pointer to another function:
        // No need for explicit std::move, temporary is captured as R-Value reference
        // so the std::unique_ptr's move constructor is automatically invoked
        consumeResource(createResource(456));
    }
}

void main_unique_ptr()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    int* ip = new int[10];
    delete[] ip;

    // Dummy* d1 = new Dummy(123);
    std::unique_ptr<Dummy> d2{ new Dummy(123) };

    using namespace UniquePointerGeneral;
  //  test_01();
   // test_02();
    //test_03();

    //using namespace UniquePointer_SourceSinkPattern;
    //test_04();
}

// =================================================================================
// End-of-File
// =================================================================================
