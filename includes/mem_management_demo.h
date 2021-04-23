//
// Created by Kai Siemek on 11.04.21.
//

#ifndef MODERN_CPP_DEMO_MEM_MANAGEMENT_DEMO_H
#define MODERN_CPP_DEMO_MEM_MANAGEMENT_DEMO_H

class Data
{
public:
    static int current_id;
    int id;
    int associated_file;

    explicit Data(int file_id);
    ~Data();
};

class File
{
public:
    static int current_id;
    int m_id;
    std::string m_name;
    std::vector<Data> data;

    explicit File(const std::string& name);
    File(const File& f);
    ~File();

    void addData();
};

#endif //MODERN_CPP_DEMO_MEM_MANAGEMENT_DEMO_H
