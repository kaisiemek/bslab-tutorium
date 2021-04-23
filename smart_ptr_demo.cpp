//
// Created by Kai Siemek on 14.04.21.
//

#include <memory>
#include <vector>
#include <iostream>

#include "includes/smart_ptr_demo.h"

using std::cout, std::endl;

void basic_unique_usage();
void unique_demo();
void shared_demo();
std::unique_ptr<VeryLargeObj> unique_resource();

int smart_ptr_main()
{
    basic_unique_usage();
    unique_demo();
    shared_demo();
    return 0;
}

void basic_unique_usage()
{
    /*
     * =============== INITIALISATION ===============
     */
    // Single objects
    std::unique_ptr<VeryLargeObj> vlo_ptr {new VeryLargeObj(0x1A4)};
    auto vlo_ptr2 = std::make_unique<VeryLargeObj>(0x1A4);

    // Arrays (char buffers)
    std::unique_ptr<char> buf_ptr {new char[1024]};
    auto buf_ptr2 = std::make_unique<char[]>(1024);

    // Arrays (objects)
    // This only works if a standard constructor is available
    std::unique_ptr<VeryLargeObj[]> vlo_arr {new VeryLargeObj[5]};
    auto vlo_arr2 = std::make_unique<VeryLargeObj[]>(0);

    /*
     * =============== ACCESSING ===============
     */
    // Single objects
    vlo_ptr->aLotOfData = 0xFFFF;
    vlo_ptr->method();
    (*vlo_ptr).method();

    // Arrays
    vlo_arr[3].aLotOfData = 0x1234;
    vlo_arr[3].method();

    /*
     * =============== RAW POINTERS ===============
     */
    // Be very careful what you do with this, by using the underlying
    // raw pointer you lose the benefits of the smart pointer.
    // Useful for interfacing with C code.
    VeryLargeObj* raw_ptr = vlo_ptr.get();
    raw_ptr->aLotOfData = 0xABCD;

    // Releases the managed raw pointer, caller is now responsible
    // for the memory management!
    VeryLargeObj* raw_ptr2 = vlo_ptr.release();
    delete raw_ptr2;

    /*
     * =============== RELEASING MANUALLY ===============
     */
    cout << "Replacing vlo_ptr2..." << endl;
    // If you re-assign the unique pointer, the old resource will be released
    // and the new resource will be managed
    vlo_ptr2 = std::make_unique<VeryLargeObj>(0xAAAA);

    cout << "Resetting vlo_ptr2..." << endl;
    // Releases the old resource, replaces managed pointer with nullptr.
    vlo_ptr2.reset();

    if (vlo_ptr2 == nullptr)
        cout << "vlo_ptr2 is now a nullptr" << endl;

    cout << "------------------------ Exiting" << endl;
}

void unique_demo()
{
    cout << "Requesting a big object from unique_resource()" << endl;
    std::unique_ptr<VeryLargeObj> resource { unique_resource() };

    // Now you can use the resource
    resource->method();
    resource->aLotOfData = 0x1A4;
    resource->method();

    // Now we'll build a struct that owns a resource
    struct ptrHolder {
        std::unique_ptr<VeryLargeObj> m_ptr;
    } ptrHolder;

    // This throws a compile time error, unique_ptr can not be copied!
//    ptrHolder.m_ptr = resource;
    // We can move it though! :)
    ptrHolder.m_ptr = std::move(resource);

    cout << "Exiting unique_demo(), resources will be released now, since ptrHolder goes out of scope" << endl;
}

void shared_demo()
{
    struct resourceManager {
        std::shared_ptr<VeryLargeObj> res;
    } resourceManager;

    auto shared_ptr = std::make_shared<VeryLargeObj>(0x1234);
    auto res_manager {std::make_unique<struct resourceManager>()};

    // Pointer was copied, ref count is now 2
    res_manager->res = shared_ptr;

    cout << "Ref count: " << shared_ptr.use_count() << endl;

    // Now we reset the shared_ptr, leaving only our struct to keep a reference to it
    shared_ptr.reset();

    // Ref count will be 1 now
    cout << "Ref count: " << res_manager->res.use_count() << endl;

    // If we clear the unique_ptr of the resource manager, the struct will be deleted
    // which leads to the shared_ptr being deleted as well, dropping the ref count to 0
    // deleting the object being managed.
    res_manager.reset();

    cout << "Exiting shared_demo()..." << endl;
}

std::unique_ptr<VeryLargeObj> unique_resource()
{
    // The resource will be created here and can be used freely
    auto vlo = std::make_unique<VeryLargeObj>(0xFF);
    vlo->aLotOfData = 0xABCD;

    // This object will be destructed upon exiting the function
    auto vlo2 = std::make_unique<VeryLargeObj>(0xAAAA);

    cout << "Exiting unique_resource()..." << endl;

    // Returns the unique_ptr, it will NOT be copied but moved.
    return vlo;

    // Usually you'd do something like this:
    // return std::make_unique<VeryLargeObj>(0xFF);
}