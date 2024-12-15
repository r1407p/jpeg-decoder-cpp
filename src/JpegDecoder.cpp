#include "JpegDecoder.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <arpa/inet.h> // htons


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

    void JpegDecoder::closeJpegFile() {
        imgaefile_stream.close();
    }

    ResultCode JpegDecoder::decodeJpegFile() {
        std::cout << "Decode input file: " << inputFilename << std::endl;

        // Check if file is opened and valid
        if (!imgaefile_stream.is_open() || !imgaefile_stream.good()) {
            std::cerr << "Unable to scan image file: '" + std::string(inputFilename) + "'" << std::endl;
            return ResultCode::ERROR;
        }

        uint8_t byte;
        
        // Step 1: Read the JPEG header
        // JPEG header starts with 0xFF 0xD8 (SOI marker)
        imgaefile_stream.read(reinterpret_cast<char*>(&byte), 1);
        if (byte != JFIF_BYTE_FF) {
            std::cerr << "Invalid JPEG file: Missing SOI marker" << std::endl;
            return ResultCode::ERROR;
        }
        imgaefile_stream.read(reinterpret_cast<char*>(&byte), 1);
        if (byte != JFIF_SOI) {
            std::cerr << "Invalid JPEG file: Missing SOI marker" << std::endl;
            return ResultCode::ERROR;
        }

        std::cout << "Found SOI marker, start parsing the JPEG header..." << std::endl;
    }
}