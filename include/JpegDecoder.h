#ifndef JPEGDECODER_H
#define JPEGDECODER_H

namespace jpeg_decoder {
    class JpegDecoder {
    public:
        JpegDecoder(const char* inputFilename);
        ~JpegDecoder();
    private:
        const char* inputFilename;
    };
}
#endif // JPEGDECODER_H