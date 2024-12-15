#include "JpegDecoder.h"
#include <fstream>
#include <iostream>

namespace jpeg_decoder {
    JpegDecoder::JpegDecoder(const char* inputFilename) : inputFilename(inputFilename) {
    }

    JpegDecoder::~JpegDecoder() {
    }

    void JpegDecoder::readJpegFile() {
        imgaefile_stream.open(inputFilename, std::ios::in | std::ios::binary);
        if (!imgaefile_stream.is_open()) {
            std::cerr << "Input file does not exist: " << inputFilename << std::endl;
            return;
        }else if (!imgaefile_stream.good()){
            std::cerr << "Error reading input file: " << inputFilename << std::endl;
            return;
        }else{
            std::cout << "Read input file: " << inputFilename << std::endl;
        }
    }
}
