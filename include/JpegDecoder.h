#ifndef JPEGDECODER_H
#define JPEGDECODER_H
#include <fstream>


namespace jpeg_decoder {
    class JpegDecoder {
    public:
        // Public member functions
        JpegDecoder(const char* inputFilename);
        ~JpegDecoder();
        void readJpegFile();

    private:
        // Private member variables
        const char* inputFilename;
        std::ifstream imgaefile_stream;

        // Private member functions

    };
}
#endif // JPEGDECODER_H
