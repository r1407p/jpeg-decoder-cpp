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
            if (byte == JFIF_BYTE_0 || byte == JFIF_BYTE_FF)
                return ERROR;
            
            switch(byte)
            {
                case JFIF_SOI  : std::cout  << "Found segment, Start of Image (FFD8)" << std::endl; return ResultCode::SUCCESS;
                case JFIF_APP0 : std::cout  << "Found segment, JPEG/JFIF Image Marker segment (APP0)" << std::endl; parseAPP0Segment(); return ResultCode::SUCCESS;
                case JFIF_COM  : std::cout  << "Found segment, Comment(FFFE)" << std::endl; parseCOMSegment(); return ResultCode::SUCCESS;
                case JFIF_DQT  : std::cout  << "Found segment, Define Quantization Table (FFDB)" << std::endl; parseDQTSegment(); return ResultCode::SUCCESS;
                case JFIF_SOF0 : std::cout  << "Found segment, Start of Frame 0: Baseline DCT (FFC0)" << std::endl; return parseSOF0Segment();
                case JFIF_DHT  : std::cout  << "Found segment, Define Huffman Table (FFC4)" << std::endl; parseDHTSegment(); return ResultCode::SUCCESS;
                case JFIF_SOS  : std::cout  << "Found segment, Start of Scan (FFDA)" << std::endl; parseSOSSegment(); return ResultCode::SUCCESS;
            }
            
        }

        if (!headerParsed) {
            std::cerr << "Error parsing JPEG header" << std::endl;
            return ResultCode::ERROR;
        }

    }
    ResultCode JpegDecoder::parseAPP0Segment() {
        // Ignore the image thumbnail data
        std::cout << "Parsing JPEG/JFIF marker segment (APP-0)..." << std::endl;
        uint16_t lenByte = 0;
        uint8_t byte = 0;
        imgaefile_stream.read(reinterpret_cast<char *>(&lenByte), 2);
        lenByte = htons(lenByte);
        std::size_t curPos = imgaefile_stream.tellg();

        std::cout << "JFIF Application marker segment length: " << lenByte << std::endl;

        // Skip the 'JFIF\0' bytes
        imgaefile_stream.seekg(5, std::ios_base::cur);

        // Read the major and minor version numbers
        uint8_t majVersionByte, minVersionByte;
        imgaefile_stream >> std::noskipws >> majVersionByte >> minVersionByte;

        std::cout << "JFIF version: " << (int)majVersionByte << "." << (int)(minVersionByte >> 4) << (int)(minVersionByte & 0x0F) << std::endl;

        std::string majVersion = std::to_string(majVersionByte);
        std::string minVersion = std::to_string((int)(minVersionByte >> 4));
        minVersion +=  std::to_string((int)(minVersionByte & 0x0F));

        // Read the image density unit
        uint8_t densityByte;
        imgaefile_stream >> std::noskipws >> densityByte;

        std::string densityUnit = "";
        switch(densityByte)
        {
            case 0x00: densityUnit = "Pixel Aspect Ratio"; break;
            case 0x01: densityUnit = "Pixels per inch (DPI)"; break;
            case 0x02: densityUnit = "Pixels per centimeter"; break;
        }

        std::cout << "Image density unit: " << densityUnit << std::endl;

        // Read the horizontal and vertical image density
        uint16_t xDensity = 0, yDensity = 0;

        imgaefile_stream.read(reinterpret_cast<char *>(&xDensity), 2);
        imgaefile_stream.read(reinterpret_cast<char *>(&yDensity), 2);

        xDensity = htons(xDensity);
        yDensity = htons(yDensity);

        std::cout << "Horizontal image density: " << xDensity << std::endl;
        std::cout << "Vertical image density: " << yDensity << std::endl;

        // Ignore the image thumbnail data
        uint8_t xThumb = 0, yThumb = 0;
        imgaefile_stream >> std::noskipws >> xThumb >> yThumb;
        imgaefile_stream.seekg(3 * xThumb * yThumb, std::ios_base::cur);

        std::cout << "Finished parsing JPEG/JFIF marker segment (APP-0) [OK]" << std::endl;
        return ResultCode::SUCCESS;
    }

    ResultCode JpegDecoder::parseCOMSegment() { ///v///
        std::cout << "Parsing comment segment..." << std::endl;
        uint16_t lenByte = 0;
        uint8_t byte = 0;
        std::string comment;
        imgaefile_stream.read(reinterpret_cast<char *>(&lenByte), 2);
        lenByte = htons(lenByte);
        std::size_t curPos = imgaefile_stream.tellg();
        std::cout<< "Comment segment length: " << lenByte << std::endl;

        for (auto i = 0; i < lenByte - 2; ++i)
        {
            imgaefile_stream >> std::noskipws >> byte;
            
            if (byte == JFIF_BYTE_FF)
            {
                std::cout << "Unexpected start of marker at offest: " << curPos + i << std::endl;
                std::cout << "Comment segment content: " << comment << std::endl;
                return ResultCode::ERROR;
            }
            
            comment.push_back(static_cast<char>(byte));
        }
        std::cout << "Comment segment content: " << comment << std::endl;

        std::cout << "Finished parsing comment segment [OK]" << std::endl;
        return ResultCode::SUCCESS;
    }

    ResultCode JpegDecoder::parseDQTSegment() {
        std::cout << "Parsing DQT segment..." << std::endl;

        std::cout << "Finished parsing DQT segment [OK]" << std::endl;
        return ResultCode::SUCCESS;
    }

    ResultCode JpegDecoder::parseSOF0Segment() {    ////
        std::cout << "Parsing SOF-0 segment..." << std::endl;

        std::cout << "Finished parsing SOF-0 segment [OK]" << std::endl;
        return ResultCode::SUCCESS;
    }

    ResultCode JpegDecoder::parseDHTSegment() {
        std::cout << "Parsing DHT segment..." << std::endl;

        std::cout << "Finished parsing DHT segment [OK]" << std::endl;
        return ResultCode::SUCCESS;
    }

    ResultCode JpegDecoder::parseSOSSegment() {
        std::cout << "Parsing SOS segment..." << std::endl;

        std::cout << "Finished parsing SOS segment [OK]" << std::endl;
        return ResultCode::SUCCESS;
    }
}