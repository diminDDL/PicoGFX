#pragma once

#include <gfxmath.h>
#include <compression.h>

class compression_encoder : public compression
{
public:
	void encode(stream_metadata_t* metadata, stream_config_t config, unsigned short* frameBuffer, unsigned char* stream);

private:
	void encodeMonochrome(stream_metadata_t* metadata, stream_config_t config, unsigned short* frameBuffer, unsigned char* stream);
	void encodeMonochromeRLE(stream_metadata_t* metadata, stream_config_t config, unsigned short* frameBuffer, unsigned char* stream);
	void encodeRunLengthEncoding(stream_metadata_t* metadata, stream_config_t config, unsigned short* frameBuffer, unsigned char* stream);
	void encodeLossy(stream_metadata_t* metadata, stream_config_t config, unsigned short* frameBuffer, unsigned char* stream);
	void encodeReducedColor(stream_metadata_t* metadata, stream_config_t config, unsigned short* frameBuffer, unsigned char* stream);
	void encodeReducedColorRLE(stream_metadata_t* metadata, stream_config_t config, unsigned short* frameBuffer, unsigned char* stream);
	void encodeRaw(stream_metadata_t* metadata, stream_config_t config, unsigned short* frameBuffer, unsigned char* stream);
	void monochromeDither(stream_metadata_t* metadata, unsigned short* frameBuffer, unsigned short strength);
};