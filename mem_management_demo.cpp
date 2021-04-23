//
// Created by Kai Siemek on 11.04.21.
//

#include <iostream>
#include <vector>

#include "includes/mem_management_demo.h"

int Data::current_id = 0;

Data::Data(int file_id)
{
    id = current_id++;
    associated_file = file_id;
    std::cout << "Constructing Data " << id << ", File: " << associated_file << std::endl;
}

Data::~Data()
{
    std::cout << "Destructing Data " << id << ", File: " << associated_file << std::endl;
}

int File::current_id = 0;

File::File(const std::string& name)
{
    m_name = name;
    m_id = current_id++;
    std::cout << "Constructing File " << m_name << " (" << m_id << ")" << std::endl;
}

File::File(const File& f)
{
    m_name = f.m_name;
    m_id = current_id++;
    // Deep copy!
    data = f.data;
    std::cout << "Copying File " << f.m_name << " (" << f.m_id << ")" << " >> " << m_id << std::endl;
}

File::~File()
{
    std::cout << "Destructing File "<< m_name << " (" << m_id << ")" << std::endl;
}

void File::addData()
{
    auto new_data = Data(m_id);
    data.push_back(new_data);
}

