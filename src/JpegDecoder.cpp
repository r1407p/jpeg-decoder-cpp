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

    uint16_t JpegDecoder::readTwoBytes() {
        uint8_t byte1, byte2;
        imgaefile_stream.read(reinterpret_cast<char*>(&byte1), 1);
        imgaefile_stream.read(reinterpret_cast<char*>(&byte2), 1);
        return (static_cast<uint16_t>(byte1) << 8) | static_cast<uint16_t>(byte2);
    }

    uint8_t JpegDecoder::readByte() {
        uint8_t byte;
        imgaefile_stream.read(reinterpret_cast<char*>(&byte), 1);
        return byte;
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

        // Step 2: Parse the JPEG segments (SOF, DQT, DHT, etc.)
        bool headerParsed = false;
        while (!imgaefile_stream.eof()) {
            imgaefile_stream.read(reinterpret_cast<char*>(&byte), 1);
            if (byte != JFIF_BYTE_FF) {
                continue; 
            }

            imgaefile_stream.read(reinterpret_cast<char*>(&byte), 1);
            if (byte == JFIF_SOF0 || byte == JFIF_SOF2) {  // Start of Frame (SOF) marker (baseline or progressive)
                // Parse the SOF segment (contains height, width, and other info)
                this->length = readTwoBytes();
                this->precision = readByte();
                this->height = readTwoBytes();
                this->width = readTwoBytes();
                this->numComponents = readByte();

                std::cout << "SOF segment length: " << this->length << std::endl;
                std::cout << "Data precision: " << static_cast<uint16_t>(this->precision) << " bits" << std::endl;
                std::cout << "Image dimensions: " << this->width << "x" << this->height << std::endl;
                std::cout << "Number of components: " << static_cast<uint16_t>(this->numComponents) << std::endl;

                headerParsed = true;
                break;  // Exit once SOF segment is processed

            } else if (byte == JFIF_DQT) {  // DQT (quantization tables)
                // Process quantization table
                processDQT();
            } else if (byte == JFIF_DHT) {  // DHT (Huffman tables)
                // Process Huffman table
                processDHT();
            } else if (byte == JFIF_SOS) {  // SOS (start of scan)
                // Begin processing image data
                break;
            } else {
                // Skip over unknown or non-relevant segments
                uint16_t length = readTwoBytes();
                imgaefile_stream.seekg(length - 2, std::ios::cur);
            }
        }

        if (!headerParsed) {
            std::cerr << "Error parsing JPEG header" << std::endl;
            return ResultCode::ERROR;
        }

    }
    // Placeholder for DQT (quantization tables) processing
    void JpegDecoder::processDQT() {
        // Read and process DQT data
        uint16_t length = readTwoBytes();
        std::cout << "DQT segment length: " << length << std::endl;

    }

    // Placeholder for DHT (Huffman tables) processing
    void JpegDecoder::processDHT() {
        uint16_t length = readTwoBytes();
        std::cout << "DHT segment length: " << length << std::endl;

    }

}