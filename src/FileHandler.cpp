#include "Stdafx.h"
#include "FileHandler.h"
#include "Objects2dFactory.h"
#include "Constants.h"
#include "cocos2d.h"

using namespace cocos2d;

unsigned char* FileHandler::getFileBuffer(string file_name, unsigned long* buffersize)
{
	bool wasPopupNotify = CCFileUtils::sharedFileUtils()->isPopupNotify();
	if (wasPopupNotify)
	{
		// Prevent message box to appear
		CCFileUtils::sharedFileUtils()->setPopupNotify(false);
	}

	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(file_name.c_str());
	unsigned char* pBuffer = NULL;
	pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "r", buffersize);
	
	if (wasPopupNotify)
	{
		// Reset popup permission
		CCFileUtils::sharedFileUtils()->setPopupNotify(true);
	}
	
	return pBuffer;
}

list<string> FileHandler::getFileLines(string path)
{
	list<string> lines;

	bool wasPopupNotify = CCFileUtils::sharedFileUtils()->isPopupNotify();
	if (wasPopupNotify)
	{
		// Prevent message box to appear
		CCFileUtils::sharedFileUtils()->setPopupNotify(false);
	}

	// Get file
	unsigned long lng = 0;
	unsigned long* outputfiledata = &lng;
	unsigned char* filedata = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", outputfiledata);
		
	if (wasPopupNotify)
	{
		// Reset popup permission
		CCFileUtils::sharedFileUtils()->setPopupNotify(true);
	}
	
	if (!filedata)
	{
		return lines;
	}
	
	// Get lines
	lines = StringUtils::split(string((char*)filedata), "\n");

	// After this, lines will have an extra entry and will have the value '\x04'.
	// We should remove this by popping it out the list.
	lines.pop_back( );
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	// Each line also have an extra character (on androids)
	list<string> wrongLines = lines;
	lines.clear();
	for (list<string>::iterator iteratLine = wrongLines.begin(); iteratLine != wrongLines.end(); iteratLine++)
	{
		string corrected = iteratLine->substr(0, iteratLine->length() - 1);
		lines.push_back(corrected);
	}
#endif

	return lines;
}
