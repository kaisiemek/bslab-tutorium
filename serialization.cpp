#include <iostream>
#include <vector>
#include <fstream>

#include "includes/serialization.h"

using namespace std;

static constexpr char filePath[] {"Path/to/example/file"};
static constexpr int blockSize {64};

vector<char> serializeInt(int x);
int deserializeInt(const vector<char>& buf);
void writeToFile(char* buf);
void readFromFile(char* buf);

void serialization_main()
{
    // Serialize and save an int to a file
    int y = 10;
    auto intBytes = serializeInt(y);
    auto writeBuf = vector<char>(blockSize);
    memcpy(writeBuf.data(), intBytes.data(), intBytes.size());
    writeToFile(writeBuf.data());

    // --- break, restart program ----
    // Read and deserialize an int from a file
    auto readBuf = vector<char>(blockSize);
    readFromFile(readBuf.data());
    int x = deserializeInt(readBuf);
    cout << x << endl; // Will restore the integer 'y'
}

// Transform an integer to a vector of raw bytes
vector<char> serializeInt(int x)
{
    auto vec = vector<char>(sizeof(int));
    memcpy(vec.data(), &x, sizeof(int));
    return vec;
}

// Transform a vector of raw bytes into an integer
int deserializeInt(const vector<char>& buf)
{
    int x;
    // Careful with offsets here, the int we're looking for might
    // not be the first 4 bytes.
    memcpy(&x, buf.data(), sizeof(int));
    return x;
}

// Write a buffer to a file, simulates a blockdevice
void writeToFile(char* buf)
{
    auto file = fstream(filePath, ios::out | ios::binary);
    file.write(buf, blockSize);
}

// Read from a file, simulates a blockdevice
void readFromFile(char* buf)
{
    auto file = fstream(filePath, ios::in | ios::binary);
    file.read(buf, blockSize);
}