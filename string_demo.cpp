//
// Created by Kai Siemek on 12.04.21.
//

#include <string>
#include <iostream>

#include "includes/string_demo.h"

void basic_usage_string();
void interfacing_with_c_string();

int string_main() {
    basic_usage_string();
    interfacing_with_c_string();
    return 0;
}

void basic_usage_string() {
    using namespace std;
    // With unicode support included
    std::string str{"Hello 世界 мир 세계"};
    std::cout << str << std::endl;

    // Get the length of the string
    size_t size = str.length();
    size = str.size();

    // Appending to a string
    str.append(" append this");
    str += " same effect as .append()";

    // Comparing strings
    if (str == "are these equal?")
        str = "Assign new string, delete old string";

    if (str.empty())
        str = "Fill it up with something";

    // Find a substring, -1 if not in the string
    int pos = str.find("xyz");

    // Deletes the string, frees up any allocated resources
    str.clear();
}

void interfacing_with_c_string()
{
    using namespace std;

    char c_str[] = "This is an old style C string";
    const char* str_ptr = c_str;

    // Create a string from C-style strings (char[] or char*)
    // COPIES the contents, doesn't point to the same memory region
    auto str = string(c_str) + " - not anymore";
    auto str2 = string(str_ptr);

    // Compare std::string with an C-style string via == operator
    if (str2 == c_str)
        cout << "These are the same (" << str2 << ")" << endl;

    // Get a const pointer to the underlying char array in the std::string class
    // For a mutable non-const pointer use .data() - be very careful with this!
    str_ptr = str.c_str();

    // Create a C-style buffer
    char* buf = new char[128];
    // Copy the std::strings' contents in the buffer, offset 1
    str.copy(buf, str.length(), 1);
    printf("%s\n", buf);
}