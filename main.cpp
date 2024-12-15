#include <fstream>
#include <iostream>
#include <JpegDecoder.h>

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <input_jpeg>" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }

    jpeg_decoder::JpegDecoder jpegDecoder(argv[1]);
    jpegDecoder.readJpegFile();

    return 0;
}
