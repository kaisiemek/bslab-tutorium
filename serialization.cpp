#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

#include "includes/serialization_object.h"

static constexpr char filePath[] {"/Path/to/demo/file"};
static constexpr int blockSize {64};

void showcase_deserialization();
void showcase_serialization();

// These two methods simulate the functionality of the block device from part 2
void writeToFile(char* buf, int blockNo);
void readFromFile(char* buf, int blockNo);

void printByteVec(const std::vector<obj::byte>& vec);

int serialization_main()
{
    showcase_serialization();
    showcase_deserialization();
}

void showcase_serialization()
{
    using std::cout, std::endl;

    // Create an example object with standard values
    Object obj {true};
    auto objData { obj.serialize() };
//    printByteVec(objData);

    cout << "Size of objData Vector: " << objData.size() << endl;
    unsigned int blocksNeeded = objData.size() / blockSize + 1;
    cout << "Blocks needed to save objData to the file: " << blocksNeeded << endl;

    // Create a write buffer large enough to hold the 2 blocks needed to
    // persist the Object in the file, copy the objData in the buffer
    // the rest of the buffer is uninitialised (0x00)
    // NOTE: Using a char buffer here instead of our bytes, write method expects char*
    auto buffer { std::vector<char>(blocksNeeded * blockSize) };
    std::memcpy(buffer.data(), objData.data(), objData.size());

    // Write both blocks to the file, note the offset on the second call
    writeToFile(buffer.data(), 0);
    writeToFile(buffer.data() + blockSize, 1);
    // You can now inspect the file with the hexdump utility, it will contain
    // the serialised object
}

void showcase_deserialization()
{
    using std::cout, std::endl;
    using obj::byte;

    static constexpr int objectBytes {69};
    static constexpr int blocksNeeded {2};

    // Create an empty object that will be restored from the file content
    Object emptyObj {false};

    // Create a buffer to hold the serialised object and the data read from the file
    auto objData { std::vector<byte>(objectBytes) };
    auto readBuffer { std::vector<char>(blocksNeeded * blockSize) };

    // Read the data in the file, write it into the read buffer (note the offset on the second call)
    readFromFile(readBuffer.data(), 0);
    readFromFile(readBuffer.data() + blockSize, 1);

    // Copy the content from the file into the objData buffer and deserialise the object
    memcpy(objData.data(), readBuffer.data(), objData.size());
    emptyObj.deserialize(objData);

    cout << "Object data: " << endl;
    cout << "\t size:\t\t0x" << std::uppercase << std::hex << emptyObj.size << endl;
    cout << "\t used:\t\t" << (emptyObj.used ? "true" : "false") << endl;
    cout << "\t data[0]: \t0x" << std::uppercase << std::hex << static_cast<int>(emptyObj.data[0]) << endl;
    cout << "\t data[59]: \t0x" << std::uppercase << std::hex << static_cast<int>(emptyObj.data[59]) << endl;
}

void writeToFile(char* buf, int blockNo)
{
    // Open in both ios::in and ios::out mode so you can write to file without truncating it
    auto file { std::fstream(filePath, std::ios::binary | std::ios::in | std::ios::out) };
    file.seekg(blockNo * blockSize, std::ios::beg);
    file.write(buf, blockSize);
}

void readFromFile(char* buf, int blockNo)
{
    auto file { std::fstream(filePath,std::ios::in | std::ios::binary) };
    file.seekg(blockNo * blockSize, std::ios::beg);
    file.read(buf, blockSize);
}

void printByteVec(const std::vector<obj::byte>& vec)
{
    // Casting to int since printing chars tries to output printable characters
    for (auto byte : vec)
        std::cout << "0x" << std::uppercase << std::hex << static_cast<int>(byte) << std::endl;
}
