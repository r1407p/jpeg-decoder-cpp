#ifndef JPEGDECODER_H
#define JPEGDECODER_H
#include <fstream>
#include <iostream>
#include "Utils.h"

namespace jpeg_decoder {
    class JpegDecoder {
    public:
        // Public member functions
        JpegDecoder(const char* inputFilename);
        ~JpegDecoder();
        void readJpegFile();
        void closeJpegFile();
        ResultCode decodeJpegFile();

        uint16_t length;
        uint8_t precision;
        uint16_t height;
        uint16_t width;
        uint8_t numComponents;

    private:
        // Private member variables
        const char* inputFilename;
        std::ifstream imgaefile_stream;

        // Private member functions
        uint16_t readTwoBytes();
        uint8_t readByte();
        void processDQT();
        void processDHT();
        ResultCode parseAPP0Segment();

        ResultCode parseCOMSegment();

        ResultCode parseDQTSegment();
        ResultCode parseSOF0Segment();

        ResultCode parseDHTSegment();
        ResultCode parseSOSSegment();

    };
}
#endif // JPEGDECODER_H
