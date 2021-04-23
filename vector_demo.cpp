//
// Created by Kai Siemek on 11.04.21.
//

#include <iostream>
#include <vector>

#include "includes/vector_demo.h"
#include "includes/mem_management_demo.h"

template <typename T>
void print_vec(const std::vector<T>& vec);

void iterating();
void basic_usage_vector();
void memory_management_demo();
void interfacing_with_c();

int vector_main() {
    basic_usage_vector();
    iterating();
    memory_management_demo();
    interfacing_with_c();
    return 0;
}

template <typename T>
void print_vec(const std::vector<T>& vec)
{
    using namespace std;
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        cout << *it;
        if (it + 1 != vec.end())
            cout << ", ";
    }
    cout << endl;
}

void basic_usage_vector()
{
    using namespace std;

    /*
     * =============== INITIALISATION ===============
     */
    // Empty vector
    vector<int> vec;

    // Vector initialised with 10 elements, default value -1
    auto vec2 = vector<int>(10, -1);

    // Initializer-list, define elements contained in vector
    vector<int> vec3 {1, 2, 3, 4, 5};

    /*
     * =============== ADDING ELEMENTS ===============
     */
    // Insert at the end, automatic resizing
    vec3.push_back(6);

    // Insert in the middle, push back other elements, automatic resizing
    vec3.insert(vec3.begin() + 3, 17);

    /*
     * =============== ACCESSING ELEMENTS ===============
     */
    // Access element like array
    vec3[0] = 120;
    int element = vec3[0];

    // Better: access via .at() (Range checking included, catches off by one errors easily)
    vec3.at(0) = 120;
    element = vec3.at(0);
//    element = vec3.at(120); // Will throw an exception
//    element = vec3[120]; // Will not throw exception, but possible undefined behaviour or segfault - which is worse

    // Access first/last element
    element = vec3.front();
    element = vec3.back();

    /*
     * =============== GET META DATA ===============
     */
    // Number of elements
    int size = vec3.size();

    // True if empty
    bool empty = vec3.empty();

    /*
     * =============== ERASING ===============
     */

    // Delete elements, automatic resizing
    vec3.erase(vec3.begin() + 3);

    // Deletes the last element, weirdly enough doesn't return it like a usual pop operation
    vec3.pop_back();

    // Deletes all elements, automatic memory management if stack allocated or smart pointer
    // Won't call delete on raw pointers or shared pointers that are still used elsewhere
    vec3.clear();
}

void iterating()
{
    using namespace std;
    vector<string> str_vec {
            "test1",
            "test2",
            "test3",
            "test4",
    };

    // Basic range-based for loop
    // The contents of the vector will be COPIED upon iterating!
    cout << "---- Range-Based For Loop ----" << endl;
    for (auto str : str_vec)
    {
        str += "_CHANGED";
        cout << str << ", ";
    }
    cout << endl << endl;

    cout << "---- Vector Content ----" << endl;
    // For loop with const references, can not be accidentally changed, performance gains
    // (no copying + const optimisations)
    for (const auto& str : str_vec)
    {
        cout << str << ", ";
    }
    cout << endl << endl;

    // Ranged-based for loop with references, no copying but vector content is MUTABLE!
    cout << "---- Range-Based For Loop With References ----" << endl;
    for (auto &str : str_vec)
    {
        str += "_CHANGED";
    }

    // Content has changed
    cout << "---- Vector Content ----" << endl;
    for (const auto& str : str_vec)
    {
        cout << str << ", ";
    }
    cout << endl << endl;

    // Iterators, more control (indices without counter variable, easier deleting)
    cout << "---- Iterator Based For Loops ----" << endl;
    for (auto it = str_vec.begin(); it != str_vec.end(); ++it)
    {
        // Access element, dereference iterator like a pointer
        *it += "_AGAIN";
        if (*it == "test3_CHANGED_AGAIN")
        {
            // To get the index you can get std::distance to calculate distance between
            // vector begin and current iterator position
            auto index = std::distance(str_vec.begin(), it);
            cout << "Index of test3 string: " << index << endl;
        }
    }
}

void memory_management_demo()
{
    using namespace std;
    cout << "Entering memory_management()" << endl;
    // Create a vector on the stack
    vector<File> file_vec;
    // Create a file object, also on the stack
    File file {"test.txt"};

    // Files are being copied, test.txt is being copied into the vector (deep copy, copy constructor is called)
    file_vec.push_back(file);

    // A temporary object is being created, immediately deep-copied and then destroyed, this is very inefficient,
    // use .emplace_back or std::move here instead :) But this is going beyond our current scope, doing it like this
    // is also fine in non-performance-critical code.
    file_vec.push_back(File("test2.txt"));

    // Add Data objects to the vectors in the file, will be cleaned up automatically as the files are destroyed.
    file_vec.at(0).addData();
    file_vec.at(0).addData();
    file_vec.at(0).addData();
    file_vec.at(1).addData();
    file.addData();

    // As soon as file_vec goes out of scope, every item is being destructed.
    // Since all files use a data vector, during destruction of the files their data vectors go out of scope as well
    // deleting all associated Data elements, no garbage collection needed :)
    // Please note, this ONLY works with stack allocated objects and smart pointers, if you're using raw pointers
    // the vector won't call delete or free on them for you :) (it cannot know if there are other parts of the program
    // depending on the objects pointed to, so to avoid dangling pointers it doesn't clean it up)
    cout << "Exiting memory_management()" << endl;
}

// Imagine this is a fuse method
int write_data_to_c_buffer(char* lame_c_buf, int size)
{
    // Here you can see how to convert a string to a char vector, very easy with iterators .begin() & .end()
    // Please note this might be raw byte-data as well, we're just using a string here so we can print it out and
    // see the effects of the memcpy more easily, an std::vector<char> might not be null-terminated, just like
    // a char* buffer might not be null-terminated. (Not all char* buffers are strings :) )
    std::string msg = "Hello C, look at my cool standard library containers.";
    auto cool_cpp_vec = std::vector<char>(msg.begin(), msg.end());

    // If the size requested/buffer size is larger than our vector we need to limit the size to the current vector size.
    // The narrowing conversion error is being ignored for simplicity here, since we know that the size value will be
    // very small.
    int n = size > cool_cpp_vec.size() ? cool_cpp_vec.size() : size;

    // You can access the raw array/char pointer in the vector with .data()
    // Then you can use memcpy to write into the C-Style buffer
    std::memcpy(lame_c_buf, cool_cpp_vec.data(), n);

    // Since .data() returns a raw pointer you can also do it like this:
    char* raw_vec_data = cool_cpp_vec.data();
    std::memcpy(lame_c_buf, raw_vec_data, n);
    // Makes it a little clearer what is happening in the memcpy call above.
    return n;
}

void receive_data_from_c_buffer(const char* lame_c_buf, int size)
{
    // Use this constructor, no memcpy-ing needed
    auto cool_cpp_vec = std::vector<char>(lame_c_buf, lame_c_buf + size);

    // This only works because it's a string encoded as char vector, don't try this with ints or something.
    // We deliberately designed these functions to work with an example string to make it clear what is happening.
    auto str = std::string(cool_cpp_vec.begin(), cool_cpp_vec.end());

    std::cout << str << std::endl;
}

void interfacing_with_c()
{
    // Don't do stuff like this in C++, there are a lot of better options for memory allocation/management
    // However, this is perfectly fine C code
    char* buf = (char *) malloc(128);
    int buf_size = 128;

    // Here we get data from an std::vector<char> and write it into our char* buffer.
    // This will be \0 terminated since we deliberately designed the test data like a string.
    // Please be aware that in real situations this might be raw dara and the printf call might cause
    // a segfault if the returned data is not properly \0 terminated.
    int ret = write_data_to_c_buffer(buf, buf_size);
    printf("Vector content as C buffer: %s\n", buf);

    // Here we send our buffer data to a C++ function where it will be converted to an std::vector<char>
    receive_data_from_c_buffer(buf, ret);

    free(buf);
}
