//
// Created by Kai Siemek on 30.04.21.
//

#ifndef SERIALISATION_OBJECT_H
#define SERIALISATION_OBJECT_H

#include <vector>

static constexpr int dataSize {60};

namespace obj {
    using byte = unsigned char;
}

class Object {
public:
    std::vector<obj::byte> data;
    size_t size;
    bool used;

    explicit Object(bool standardValues) {
        if (standardValues) {
            this->size = 0xABCDEF;
            this->used = true;
            this->data = std::vector<obj::byte>(dataSize);
            this->data[0] = 0xFF;
            this->data[this->data.size() - 1] = 0xFF;
        } else {
            this->size = 0;
            this->used = false;
            this->data = std::vector<obj::byte>(dataSize);
        }
    }

    std::vector<obj::byte> serialize() const;
    void deserialize(const std::vector<obj::byte>& rawData);
};

#endif //SERIALISATION_OBJECT_H