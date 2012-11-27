#include "Stdafx.h"
#include "Constants.h"
#include "cocos2d.h"

std::string Constants::getResourcesPath(std::string path)
{
	return cocos2d::CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(path.c_str());
}


float Constants::getAnchorValue(AlignX alignX)
{
	switch (alignX)
	{
	case Left:
		return 0;
	case Centered:
		return 0.5;
	case Right:
		return 1;
	default:
		return 0;
	}
}

float Constants::getAnchorValue(AlignY alignY)
{
	switch (alignY)
	{
	case Bottom:
		return 0;
	case Middle:
		return 0.5;
	case Top:
		return 1;
	default:
		return 0;
	}
}
