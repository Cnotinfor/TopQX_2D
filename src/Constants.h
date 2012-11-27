#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

typedef enum AlignX
{
	Left,
	Centered,
	Right
}AlignX;
typedef enum AlignY
{
	Top,
	Middle,
	Bottom
}AlignY;

class Constants
{
public:
	// Get resources full path
	static std::string getResourcesPath(std::string path="");

	// Get anchor value from the enumerator
	static float getAnchorValue(AlignX alignX);
	static float getAnchorValue(AlignY alignY);
};

#endif // CONSTANTS_H