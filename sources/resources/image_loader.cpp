#include "djah/resources/image_loader.hpp"
#include <FreeImage.h>
#include "djah/filesystem/stream.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	image_loader::image_loader()
	{
		#ifdef FREE_IMAGE_LIB
			FreeImage_Initialise();
		#endif
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	image_loader::~image_loader()
	{
		#ifdef FREE_IMAGE_LIB
			FreeImage_DeInitialise();
		#endif
	}
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	image* image_loader::loadFromStream(filesystem::stream &strm, const std::string &filename)
	{
		BYTE *buffer;
		size_t buffer_size = strm.size();
		buffer = new BYTE[buffer_size];
		strm.read(buffer, buffer_size);

		image *p_img = nullptr;

		// Load image from memory
		FIMEMORY *memory = FreeImage_OpenMemory(buffer, static_cast<DWORD>(buffer_size));
		// Reading and parsing image header
		FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(memory, 0);
		// If unknown, return failure
		if(fif != FIF_UNKNOWN)
		{
			//pointer to the image, once loaded
			FIBITMAP *dib = nullptr;
			//check that the plug-in has reading capabilities and load the file
			if(FreeImage_FIFSupportsReading(fif))
				dib = FreeImage_LoadFromMemory(fif, memory, 0);
			//if the image failed to load, return failure
			if(dib)
			{
				//retrieve the image data
				BYTE* bits = FreeImage_GetBits(dib);

				//get the image width and height
				const unsigned int width  = FreeImage_GetWidth(dib);
				const unsigned int height = FreeImage_GetHeight(dib);
				//if this somehow one of these failed (they shouldn't), return failure
				if((bits != nullptr) && (width != 0) && (height != 0))
				{
					const unsigned int dibSize = FreeImage_GetDIBSize(dib);
					const unsigned int channels = dibSize / (width * height);
					// Create the actual image
					p_img = new image(width, height, channels, bits);
				}

				// Free FreeImage's copy of the data
				FreeImage_Unload(dib);
			}
		}

		// Release memory
		FreeImage_CloseMemory(memory);
		delete [] buffer;

		return p_img;
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/