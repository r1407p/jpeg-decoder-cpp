#include <fstream>
#include <iostream>

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <input_jpeg>" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }

    const char* inputFilename = argv[1];

    std::ifstream file(inputFilename);
    if (!file) {
        std::cerr << "Input file does not exist: " << inputFilename << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    file.close();

    return 0;
}
