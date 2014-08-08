#include "djah/resources/image_loader.hpp"
//#include <FreeImage.h>
#include "SOIL.h"
#include "djah/filesystem/stream.hpp"

namespace djah { namespace resources {

	//----------------------------------------------------------------------------------------------
	bool image_loader::loadFromStream(filesystem::stream &strm, const std::string &fileName, image_sptr &spImage)
	{
		bool success = false;

		size_t bufferSize = strm.size();
		std::unique_ptr<byte[]> buffer(new byte[bufferSize]);
		strm.read(buffer.get(), bufferSize);

		int width = 0, height = 0, channels = 0;
		byte *pImgData = SOIL_load_image_from_memory(buffer.get(), bufferSize, &width, &height, &channels, SOIL_LOAD_AUTO);

		if( pImgData != nullptr )
		{
			int i, j;
			for( j = 0; j*2 < height; ++j )
			{
				int index1 = j * width * channels;
				int index2 = (height - 1 - j) * width * channels;
				for( i = width * channels; i > 0; --i )
				{
					unsigned char temp = pImgData[index1];
					pImgData[index1] = pImgData[index2];
					pImgData[index2] = temp;
					++index1;
					++index2;
				}
			}

			spImage.reset( new image(width, height, channels, pImgData) );
			SOIL_free_image_data(pImgData);
			success = true;
		}

		return success;
	}
	//----------------------------------------------------------------------------------------------

} /*resources*/ } /*djah*/