//
// Created by Kai Siemek on 30.04.21.
//

#include <cstring>

#include "includes/serialization_object.h"

std::vector<obj::byte> Object::serialize() const {
    // Create a vector large enough to hold all important data from the object
    auto rawData = std::vector<obj::byte>(dataSize + sizeof(size_t) + sizeof(bool));
    int currentOffset {0};

    // Now copy each important data member one by one in the buffer
    // Mind the offsets and sizes!
    std::memcpy(rawData.data(), this->data.data(), dataSize);
    currentOffset += dataSize;

    std::memcpy(rawData.data() + currentOffset, &this->size, sizeof(size_t));
    currentOffset += sizeof(size_t);

    std::memcpy(rawData.data() + currentOffset, &this->used, sizeof(bool));

    return rawData;
}

void Object::deserialize(const std::vector<obj::byte>& rawData) {
    // From the vector memcpy each important piece of data back in the member variables
    // Mind the order of the elements! It needs to be done in the same order as the
    // serialisation.
    // Same as with the serialisation: mind the offsets and the lengths!
    int currentOffset {0};

    std::memcpy(this->data.data(), rawData.data(), dataSize);
    currentOffset += dataSize;

    std::memcpy(&this->size, rawData.data() + currentOffset, sizeof(size_t));
    currentOffset += sizeof(size_t);

    std::memcpy(&this->used, rawData.data() + currentOffset, sizeof(bool));
}
