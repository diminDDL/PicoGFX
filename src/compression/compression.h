#pragma once

enum encoding_type_t
{
	MONOCHROME,
	MONOCHROME_RLE,
	RUN_LENGHT_ENCODING,
	LOSSY,
	REDUCED_COLOR,
	REDUCED_COLOR_RLE,
	RAW,
};

// This has to be manually set, as the precompiler is a bit dumb
#define METADATA_BYTES 9
struct stream_metadata_t
{
	unsigned char type;			// 1 byte
	unsigned short width;		// 2 bytes
	unsigned short height;		// 2 bytes
	unsigned int totalBytes;	// 4 bytes
};

struct stream_config_t
{
	unsigned short monochromeCutoff;
	bool monochromeDithering;
	bool isReceiverBigEndian;
};

class compression
{
public:
    void addMetadata(stream_metadata_t* metadata, unsigned char* stream);
	void stripMetadata(stream_metadata_t* metadata, unsigned char* stream);
};