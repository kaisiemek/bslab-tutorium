//
// Created by Kai Siemek on 14.04.21.
//

#ifndef MODERN_CPP_DEMO_SMART_PTR_DEMO_H
#define MODERN_CPP_DEMO_SMART_PTR_DEMO_H

int smart_ptr_main();

class VeryLargeObj {
public:
    int aLotOfData;
    VeryLargeObj() : aLotOfData(0) {};
    explicit VeryLargeObj(int data) : aLotOfData(data) {};
    ~VeryLargeObj()
    {
        std::cout << "VLO 0x" << std::hex << std::uppercase
        << aLotOfData << " is being destructed." << std::endl;
    };
    void method() const
    {
        std::cout << "VLO content: 0x" << std::hex << std::uppercase
        << aLotOfData << std::endl;
    };
};

#endif //MODERN_CPP_DEMO_SMART_PTR_DEMO_H
