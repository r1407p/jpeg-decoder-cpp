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
        if (!imgaefile_stream.is_open() || !imgaefile_stream.good())
        {
            std::cerr << "Unable scan image file: \'" + std::string(inputFilename) + "\'" << std::endl;
            return ResultCode::ERROR;
        }

        uint8_t byte;  
        
        ResultCode status = ResultCode::DECODE_DONE;
        
        while (imgaefile_stream >> std::noskipws >> byte)
        {
            std::cout << "Byte: " << byte << std::endl;
            if (byte == JFIF_BYTE_FF)
            {
                imgaefile_stream >> std::noskipws >> byte;

            }
            else
            {
                std::cout << "[ FATAL ] Invalid JFIF file! Terminating..." << std::endl;
                status = ResultCode::ERROR;
                break;
            }
        }

        return status;
    }
}
