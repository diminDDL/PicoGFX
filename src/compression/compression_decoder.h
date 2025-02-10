#pragma once

#include <gfxmath.h>
#include <compression.h>

class compression_decoder : public compression
{
public:
	unsigned int decode(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer);

private:
    unsigned int decodeMonochrome(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer);
	unsigned int decodeMonochromeRLE(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer);
	unsigned int decodeRunLengthEncoding(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer);
	unsigned int decodeLossy(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer);
	unsigned int decodeReducedColor(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer);
	unsigned int decodeReducedColorRLE(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer);
	unsigned int decodeRaw(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer);
};