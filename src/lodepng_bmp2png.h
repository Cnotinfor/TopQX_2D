#ifndef LODEPNG_BMP2PNG_H
#define LODEPNG_BMP2PNG_H

#include "lodepng.h"

class lodepng_bmp2png
{
public:
	static bool bmp2png(std::string bmpfilename, std::string filename);
};

#endif // LODEPNG_BMP2PNG_H