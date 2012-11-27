#ifndef OBJECTS2DFACTORY_H
#define OBJECTS2DFACTORY_H

#include "cocos2d.h"
#include "Objects2dHandler.h"
#include "Constants.h"
#include "ScrollableZone.h"
#include "PasswordTextBox.h"
#include "Dialog.h"
#include "MenuWithClickTime.h"
#include <string>

using namespace std;

class Objects2dFactory
{
public:
	// Create a menu
	static cocos2d::CCMenu* menu(CCNode* scene, int zOrder = 0);
    
	// Create a menu with click time
	static MenuWithClickTime* menuWithClickTime(CCNode* scene, int clickSeconds, int zOrder = 0);

	// Create an image as a button
	static cocos2d::CCMenuItemImage* imageButton(cocos2d::CCNode* scene, cocos2d::CCMenu* menu, string normalImagePath, string selectedImagePath, string disabledImagePath, 
		float positionX, float positionY, float width, float height, cocos2d::SEL_MenuHandler selector, void* selectorArg = NULL, AlignX alignX = Left, AlignY alignY = Bottom, 
		int zOrder = 0);
	
	// Create a label
	static cocos2d::CCLabelTTF* label(cocos2d::CCNode* scene, string value, float positionX, float positionY, float fontSize, ccColor3B color = ccc3(255, 255, 255), 
		AlignX alignX = Left, AlignY alignY = Bottom, float width = 0, float height = 0, CCTextAlignment textAlignment = kCCTextAlignmentLeft, int zOrder = 0);

	// Create a button with a text
	static cocos2d::CCMenuItemImage* textButton(cocos2d::CCNode* scene, cocos2d::CCMenu* menu, string value, string normalImagePath, string selectedImagePath, 
		string disabledImagePath, float positionX, float positionY, float width, float height, cocos2d::SEL_MenuHandler selector, void* selectorArg = NULL, 
		AlignX alignX = Left, AlignY alignY = Bottom, ccColor3B textColor = ccc3(255, 255, 255), int zOrder = 0);

	// Create a input text field (text box)
	static TextBox* inputTextField(cocos2d::CCNode* scene, string initialText, float positionX, float positionY, float fontSize, AlignX alignX = Left, 
		AlignY alignY = Bottom, float width = 0, float height = 0, CCTextAlignment textAlignment = kCCTextAlignmentLeft, int maxNumberOfChars = 999, ccColor3B textColor = ccc3(255, 255, 255), ccColor3B placeHolderColor = ccc3(127, 127, 127), int zOrder = 0);
    
    
	// Create a input password field (text box)
	static PasswordTextBox* inputPasswordField(cocos2d::CCNode* scene, string initialText, float positionX, float positionY, float fontSize, AlignX alignX = Left,
        AlignY alignY = Bottom, float width = 0, float height = 0, CCTextAlignment textAlignment = kCCTextAlignmentLeft, int maxNumberOfChars = 999, ccColor3B textColor = ccc3(255, 255, 255), ccColor3B placeHolderColor = ccc3(127, 127, 127), int zOrder = 0);
    
	// Draw an image
	static cocos2d::CCSprite* image(cocos2d::CCNode* scene, string imagePath, float positionX, float positionY, float width, float height, AlignX alignX = Left, 
		AlignY alignY = Bottom, int zOrder = 0, void* argument = NULL);

	// Draw an image holding the image ratio
	static cocos2d::CCSprite* imageHoldingRatio(cocos2d::CCNode* scene, string imagePath, float positionX, float positionY, float maxWidth, float maxHeight, AlignX alignX = Left, 
		AlignY alignY = Bottom, int zOrder = 0, void* argument = NULL);

	//static void messageBox(string text, string title); // Use dialog box
	
	// Show a dialog box, with optional "cancel" (or what you want to be written there) button
	static void dialogBox(CCNode* scene, string text, string normal_button_url, string selected_button_url, string disabled_button_url, string button1Text, 
		void (*button1Selector)(), bool includeButton2 = false, string button2Text = "", void (*button2Selector)() = NULL);

	// Create a scrollable zone
	static ScrollableZone* scrollableZone(CCNode* scene, ScrollDirection scroll_direction, float left, float bottom, float width, float height, int touchPriority = 0);
	
	// Draw a rectangle
	static cocos2d::CCSprite* rectangle(cocos2d::CCNode* scene, ccColor3B color, float positionX, float positionY, float width, float height, AlignX alignX = Left, 
		AlignY alignY = Bottom, GLubyte opacity = 255, int zOrder = 0, void* argument = NULL);

	// Create a clickable rectangle
	static cocos2d::CCMenuItemImage* clickableRectangle(cocos2d::CCNode* scene, cocos2d::CCMenu* menu, ccColor3B color, float positionX, float positionY, float width, float height, 
		cocos2d::SEL_MenuHandler selector, void* selectorArg = NULL, AlignX alignX = Left, AlignY alignY = Bottom, GLubyte opacity = 255, int zOrder = 0);
};

#endif // OBJECTS2DFACTORY_H