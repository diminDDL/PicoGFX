#include <compression_decoder.h>

unsigned int compression_decoder::decode(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer)
{
	switch (metadata->type)
	{
	case encoding_type_t::MONOCHROME:
		return this->decodeMonochrome(metadata, stream, streamSize, frameBuffer);
	case encoding_type_t::MONOCHROME_RLE:
		return this->decodeMonochromeRLE(metadata, stream, streamSize, frameBuffer);
	case encoding_type_t::RUN_LENGHT_ENCODING:
		return this->decodeRunLengthEncoding(metadata, stream, streamSize, frameBuffer);
	case encoding_type_t::LOSSY:
		return this->decodeLossy(metadata, stream, streamSize, frameBuffer);
	case encoding_type_t::REDUCED_COLOR:
		return this->decodeReducedColor(metadata, stream, streamSize, frameBuffer);
	case encoding_type_t::REDUCED_COLOR_RLE:
		return this->decodeReducedColorRLE(metadata, stream, streamSize, frameBuffer);
	case encoding_type_t::RAW:
		default:
		return this->decodeRaw(metadata, stream, streamSize, frameBuffer);
	}
}

unsigned int compression_decoder::decodeMonochrome(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer)
{
	// Here we reverse what the monochrome encoder did
	unsigned int framebufferIndex = 0;

	// copy the framebuffer to the display
	for (int i = 0; i < streamSize; i++)
	{
		// get each pixel
		unsigned char pixel = 0;
		pixel = stream[i];

		// copy each bit of the pixel to the output
		for(int j = 0; j < 8; j++)
			frameBuffer[i * 8 + j] = (pixel & (0x1 << (7 - j))) ? 0xffff : 0x0000;

		framebufferIndex += 8;
	}

	return framebufferIndex;
}

unsigned int compression_decoder::decodeMonochromeRLE(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer)
{
	unsigned int outputBufferIndex = 0;

	// loop through all the bytes
	for (int streamIndex = 0; streamIndex < streamSize;)
	{
		// get the count and pixel value from the stream with an offset
		unsigned int count = stream[streamIndex] >> 0x01;
		unsigned char pixel = stream[streamIndex++] & 0x1;

		// reverse the run length encoding
		for (int i = 0; i < count; i++)
			frameBuffer[outputBufferIndex++] = pixel ? 0xffff : 0x0000;
	}

	return outputBufferIndex;
}

unsigned int compression_decoder::decodeRunLengthEncoding(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer)
{
	unsigned int outputBufferIndex = 0;

	for (int streamIndex = 0; streamIndex < streamSize;)
	{
		// get the count and pixel value from the stream with an offset
		unsigned int count = stream[streamIndex++];
		unsigned short pixel = (stream[streamIndex++] << 0x8) | stream[streamIndex++];

		// reverse the run length encoding
		for (int i = 0; i < count; i++)
			frameBuffer[outputBufferIndex++] = pixel;
	}

	return outputBufferIndex;
}

unsigned int compression_decoder::decodeLossy(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer)
{
	int r, g, b;
	int y, cb, cr;
	int pixelIndex = 0;
	int lastY = 0, lastCB = 128, lastCR = 128; // Initialize chroma to mid-range.

	for (unsigned int streamIndex = 0; streamIndex < streamSize;) // Loop through all pixels in the frame
	{
		unsigned int count = stream[streamIndex++];
		y = stream[streamIndex++];
		cb = stream[streamIndex++];
		cr = stream[streamIndex++];

		for (unsigned int i = 0; i < count * 2; i++)
		{
			// Duplicate the chroma values for every other pixel
			if (i % 2 == 0)
			{
				lastY = y;
				lastCB = cb;
				lastCR = cr;
			}
			else
			{
				y = lastY;
				cb = lastCB;
				cr = lastCR;
			}

			// Integer approximation of conversion constants with scaling by 1024
			// To perform the operation we shift right by 10 (2^10 = 1024)
			r = y + ((cr - 128) * 1436 >> 10);
			g = y - ((cb - 128) * 352 >> 10) - ((cr - 128) * 731 >> 10);
			b = y + ((cb - 128) * 1815 >> 10);

			r = imax(0, imin(255, r));
			g = imax(0, imin(255, g));
			b = imax(0, imin(255, b));

			// Scale to the 565 color scheme
			r = (r >> 3) & 0x1F;
			g = (g >> 2) & 0x3F;
			b = (b >> 3) & 0x1F;

			unsigned short pixel = (r << 11) | (g << 5) | b;
			frameBuffer[pixelIndex++] = pixel;
		}
	}

	return pixelIndex;
}

unsigned int compression_decoder::decodeReducedColor(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer)
{
	// Here we reverse what the reduced color encoder does
	unsigned int pixelIndex = 0;

	// loop through each pixel in the framebuffer
	for (; pixelIndex < streamSize; pixelIndex++)
	{
		// build up the pixel based on the stream buffer
		unsigned char pixel = stream[pixelIndex];

		// extract the rgb values
		unsigned char r = (pixel >> 5) & 0x7;
		unsigned char g = (pixel >> 2) & 0x7;
		unsigned char b = pixel & 0x3;

		// convert the pixel to RGB565
		r = (r << 0x2) | (r >> 0x1);
		g = (g << 0x3) | (g << 0x1) | (g >> 0x2);
		b = (b << 0x3) | (b << 0x1) | (b >> 0x1);

		// reassemble the pixel
		unsigned short newPixel = (r << 0xb) | (g << 0x5) | b;
		frameBuffer[pixelIndex] = newPixel;
	}

	return pixelIndex;
}

unsigned int compression_decoder::decodeReducedColorRLE(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer)
{
	unsigned char pixel;
	unsigned char count;
	unsigned short newPixel;
	int pixelIndex = 0;

	for (int streamIndex = 0; streamIndex < streamSize;)
	{
		count = stream[streamIndex++];
		pixel = stream[streamIndex++];

		// extract the rgb values
		unsigned char r = (pixel >> 5) & 0x7;
		unsigned char g = (pixel >> 2) & 0x7;
		unsigned char b = pixel & 0x3;

		// convert the pixel to RGB565
		r = (r << 0x2) | (r >> 0x1);
		g = (g << 0x3) | (g << 0x1) | (g >> 0x2);
		b = (b << 0x3) | (b << 0x1) | (b >> 0x1);

		// reassemble the pixel
		newPixel = (r << 0xb) | (g << 0x5) | b;

		for (int i = 0; i < count; i++)
			frameBuffer[pixelIndex++] = newPixel;
	}

	return pixelIndex;
}

unsigned int compression_decoder::decodeRaw(stream_metadata_t* metadata, unsigned char* stream, size_t streamSize, unsigned short* frameBuffer)
{
	// Here we reverse what the raw encoder does
	unsigned int pixelIndex = 0;

	// loop through each pixel in the framebuffer
	for (unsigned int streamIndex = 0; streamIndex < streamSize;)
		// build up the pixel based on the stream buffer
		frameBuffer[pixelIndex++] = (stream[streamIndex++] << 0x8) | stream[streamIndex++];

	return pixelIndex;
}