#include "Stdafx.h"
#include "Objects2dFactory.h"
#include "FileHandler.h"
#include "MenuWithClickTime.h"
#include <list>

using namespace cocos2d;

CCMenu* Objects2dFactory::menu(CCNode* scene, int zOrder)
{
	// Create a menu
	CCMenu* pMenu = CCMenu::create();
    
	if(!pMenu)
		return NULL;
	
	// Set position
	pMenu->setPosition(CCPointZero);

	if (scene != NULL)
		// Add menu to the scene
		scene->addChild(pMenu, zOrder);

	return pMenu;
}

MenuWithClickTime* Objects2dFactory::menuWithClickTime(CCNode* scene, int clickSeconds, int zOrder)
{
	// Create a menu
	MenuWithClickTime* pMenu = MenuWithClickTime::createWithTime(clickSeconds);
    
	if(!pMenu)
		return NULL;
	
	// Set position
	pMenu->setPosition(CCPointZero);
    
	if (scene != NULL)
		// Add menu to the scene
		scene->addChild(pMenu, zOrder);
    
	return pMenu;
}

CCMenuItemImage* Objects2dFactory::imageButton(CCNode* scene, CCMenu* menu, string normalImagePath, string selectedImagePath, string disabledImagePath, 
	float positionX, float positionY, float width, float height, SEL_MenuHandler selector, void* selectorArg, AlignX alignX, AlignY alignY, int zOrder)
{
	// Check arguments validity
	if(scene == NULL || menu == NULL || !selector)
		return NULL;

	// Create a button menu item
    CCMenuItemImage *button = CCMenuItemImage::create(
        normalImagePath.c_str(),
        selectedImagePath.c_str(),
        disabledImagePath.c_str(),
        scene,
        selector);
    if(!button)
		return NULL;

	if (selectorArg != NULL)
	{
		button->setUserData(selectorArg);
	}

    // Place the menu item in the given position
    button->setPosition(ccp(positionX, positionY));

	// Set anchor
	button->setAnchorPoint(ccp(Constants::getAnchorValue(alignX), Constants::getAnchorValue(alignY)));
	
	// Set menu item width
	float buttonWidth = button->boundingBox().size.width;
	if (width > 0) // if width is defined, it is setted
	{
		button->setScaleX(width / buttonWidth);
	}

	// Set menu item height
	float buttonHeight = button->boundingBox().size.height;
	if (height > 0) // if height is defined, it is setted
	{
		button->setScaleY(height / buttonHeight);
	}

	// Add button to the menu
	menu->addChild(button, zOrder);

	return button;
}

CCLabelTTF* Objects2dFactory::label(CCNode* scene, string value, float positionX, float positionY, float fontSize, ccColor3B color, AlignX alignX, AlignY alignY,
	float width, float height, CCTextAlignment textAlignment, int zOrder)
{
	// Check arguments validity
	if(scene == NULL)
		return NULL;

    // Create a label and initialize with the given string
	CCLabelTTF* pLabel = CCLabelTTF::create(value.c_str(), "Arial"/*string(Constants::getResourcesPath("SOResources/Fonts/AlphaFridgeMagnetsAllCap.ttf")).c_str()*/,
		fontSize, CCSize(width, height), textAlignment);
	//CCLabelTTF* pLabel = CCLabelTTF::create(value.c_str(), "Arial", fontSize, CCSize(width, height), textAlignment);
    if(!pLabel)
		return NULL;

    // Set color
    pLabel->setColor(color);
	
    // Place the label
    pLabel->setPosition(ccp(positionX, positionY));
	
	// Set anchor
	pLabel->setAnchorPoint(ccp(Constants::getAnchorValue(alignX), Constants::getAnchorValue(alignY)));
	
    // Add the label to the scene
    scene->addChild(pLabel, zOrder);

	return pLabel;
}

CCMenuItemImage* Objects2dFactory::textButton(CCNode* scene, CCMenu* menu, string value, string normalImagePath, string selectedImagePath, string disabledImagePath, 
	float positionX, float positionY, float width, float height, SEL_MenuHandler selector, void* selectorArg, AlignX alignX, AlignY alignY, ccColor3B textColor, int zOrder)
{
	// Check arguments validity
	if(scene == NULL ||menu == NULL || !selector)
		return NULL;

	// Create a button menu item
    CCMenuItemImage *button = CCMenuItemImage::create(
        normalImagePath.c_str(),
        selectedImagePath.c_str(),
		disabledImagePath.c_str(),
        scene,
        selector);
    if(!button)
		return NULL;

	if (selectorArg != NULL)
	{
		button->setUserData(selectorArg);
	}

    // Place the menu item in the given position
    button->setPosition(ccp(positionX, positionY));

	// Set menu item anchor
	button->setAnchorPoint(ccp(Constants::getAnchorValue(alignX), Constants::getAnchorValue(alignY)));
	
	// Set menu item height
	float buttonHeight = button->boundingBox().size.height;
	float buttonPosScaleHeight = buttonHeight;
	if (height > 0) // if height is defined, it is setted
	{
		button->setScaleY(height / buttonHeight);
		buttonPosScaleHeight = height;
	}
	
    // Create a label and initialize with the given string
	CCLabelTTF* pLabel = CCLabelTTF::create(value.c_str(), "Arial"/*string(Constants::getResourcesPath() + "SOResources/Fonts/AlphaFridgeMagnetsAllCap.ttf").c_str()*/, 
		buttonPosScaleHeight * 0.6f);
    if(!pLabel)
		return NULL;
	
    // Set color
    pLabel->setColor(textColor);
	
	// Set menu item width
	float buttonWidth = button->boundingBox().size.width;
	float buttonPosScaleWidth = buttonWidth;
	if (width > 0) // if width is defined, it is setted
	{
		button->setScaleX(width / buttonWidth);
		buttonPosScaleWidth = width;
	}
	else // otherwise, we will compare the text width with the button width
	{
		float textWidth = pLabel->boundingBox().size.width;
		if (textWidth * 1.25 > buttonWidth) // if the width adapted to the text is bigger than the button width, adapted width must be setted, otherwise, button width must stay as it was
		{
			button->setScaleX(textWidth * 1.25f / buttonWidth);
			buttonPosScaleWidth = textWidth * 1.25f;
		}
	}
	
    // Place the label; must be in the center of the button
	pLabel->setPosition(ccp(buttonPosScaleWidth * 0.5f * 1/button->getScaleX(), buttonPosScaleHeight * 0.5f * 1/button->getScaleY()));
	
	// Set label centered anchor
	pLabel->setAnchorPoint(ccp(Constants::getAnchorValue(Centered), Constants::getAnchorValue(Middle)));
	
	// Add label to button
	button->addChild(pLabel);

	// Set label scale; the inverse of button scale, so label can be as it would be if it was a child of scene
	pLabel->setScaleY(1/button->getScaleY());
	pLabel->setScaleX(1/button->getScaleX());

	// If label is larger than button
	if (pLabel->boundingBox().size.width * button->getScaleX() > button->boundingBox().size.width)
	{
		// Label must be rescaled
		float newScale = button->boundingBox().size.width / (pLabel->boundingBox().size.width * button->getScaleX());
		pLabel->setScaleX(pLabel->getScaleX() * newScale);
	}

	// Add button to the menu
	menu->addChild(button, zOrder);
	
	return button;
}

TextBox* Objects2dFactory::inputTextField(CCNode* scene, string initialText, float posX, float posY, float fontSize, AlignX alignX, AlignY alignY, float width, float height, 
	CCTextAlignment textAlignment, int maxNumberOfChars, ccColor3B textColor, ccColor3B placeHolderColor, int zOrder)
{
	// Check arguments validity
	if(scene == NULL)
		return NULL;

	// Get text field
	TextBox* pTextField = new TextBox(initialText, fontSize, "Arial"/*Constants::getResourcesPath() + "SOResources/Fonts/AlphaFridgeMagnetsAllCap.ttf"*/, width, height, textAlignment, '|', maxNumberOfChars, textColor, placeHolderColor);
	if(!pTextField)
		return NULL;

	// Set position
	pTextField->setPosition(ccp(posX,posY));

	// Set anchor
	pTextField->setAnchorPoint(ccp(Constants::getAnchorValue(alignX), Constants::getAnchorValue(alignY)));
	
	//// Mark to receive keyboard inputs (this is making the keyboard to open immediately)
	//pTextField->attachWithIME();

    // Add the label to the scene
    scene->addChild(pTextField, zOrder);

	// Isn't it necessary to save the object, to get the text?
	return pTextField;
}

PasswordTextBox* Objects2dFactory::inputPasswordField(CCNode* scene, string initialText, float posX, float posY, float fontSize, AlignX alignX, AlignY alignY, float width, float height,
    CCTextAlignment textAlignment, int maxNumberOfChars, ccColor3B textColor, ccColor3B placeHolderColor, int zOrder)
{
	// Check arguments validity
	if(scene == NULL)
		return NULL;
    
	// Get password text field
	PasswordTextBox* pTextField = new PasswordTextBox(initialText, fontSize, "Arial"/*Constants::getResourcesPath() + "SOResources/Fonts/AlphaFridgeMagnetsAllCap.ttf"*/, width, height, textAlignment, '|', maxNumberOfChars, textColor, placeHolderColor);
	if(!pTextField)
		return NULL;
    
	// Set position
	pTextField->setPosition(ccp(posX,posY));
    
	// Set anchor
	pTextField->setAnchorPoint(ccp(Constants::getAnchorValue(alignX), Constants::getAnchorValue(alignY)));
	
	//// Mark to receive keyboard inputs (this is making the keyboard to open immediately)
	//pTextField->attachWithIME();
    
    // Add the label to the scene
    scene->addChild(pTextField, zOrder);
    
	// Isn't it necessary to save the object, to get the text?
	return pTextField;
}

CCSprite* Objects2dFactory::image(CCNode* scene, string imagePath, float positionX, float positionY, float width, float height, AlignX alignX, 
	AlignY alignY, int zOrder, void* argument)
{
	// Check arguments validity
	if(scene == NULL)
		return NULL;

	// Create an image
	CCSprite* image = CCSprite::create(imagePath.c_str());
    if(!image)
		return NULL;

    // Place the image in the given position
    image->setPosition(ccp(positionX, positionY));
	
	// Set anchor
	image->setAnchorPoint(ccp(Constants::getAnchorValue(alignX), Constants::getAnchorValue(alignY)));
	
	// Set size
	float imageWidth = image->boundingBox().size.width;
    image->setScaleX(width / imageWidth);
	float imageHeight = image->boundingBox().size.height;
    image->setScaleY(height / imageHeight);

	// Set argument as user data
	if (argument != NULL)
		image->setUserData(argument);

	// Add menu to the scene
	scene->addChild(image, zOrder);

	return image;
}

CCSprite* Objects2dFactory::imageHoldingRatio(CCNode* scene, string imagePath, float positionX, float positionY, float maxWidth, float maxHeight, AlignX alignX, 
	AlignY alignY, int zOrder, void* argument)
{
	// Check arguments validity
	if(scene == NULL)
		return NULL;

	// Create an image
	CCSprite* image = CCSprite::create(imagePath.c_str());
    if(!image)
		return NULL;

    // Place the image in the given position
    image->setPosition(ccp(positionX, positionY));
	
	// Set anchor
	image->setAnchorPoint(ccp(Constants::getAnchorValue(alignX), Constants::getAnchorValue(alignY)));
	
	// Get original size
	float imageWidth = image->boundingBox().size.width;
	float imageHeight = image->boundingBox().size.height;

	// Calculate the scale to maximize holding the ratio
	float scale = maxWidth / imageWidth;
	if (maxHeight / imageHeight < maxWidth / imageWidth)
		scale = maxHeight / imageHeight;

	// Set scale
    image->setScale(scale);

	// Set argument as user data
	if (argument != NULL)
		image->setUserData(argument);

	// Add menu to the scene
	scene->addChild(image, zOrder);

	return image;
}

// Use dialog box
//void Objects2dFactory::messageBox(string text, string title)
//{
//	CCMessageBox(specialCharsConvertion(text).c_str(), specialCharsConvertion(title).c_str());
//}

//DEPRECATED... See dialog class
void Objects2dFactory::dialogBox(CCNode* scene, string text, string normal_button_url, string selected_button_url, string disabled_button_url, string button1Text, 
	void (*button1Selector)(), bool includeButton2, string button2Text, void (*button2Selector)())
{
    if (includeButton2)
        Dialog(scene, text, normal_button_url, selected_button_url, disabled_button_url, button1Text.c_str(), button1Selector, button2Text.c_str(), button2Selector, NULL);
    else
        Dialog(scene, text, normal_button_url, selected_button_url, disabled_button_url, button1Text.c_str(), button1Selector, NULL);
}

ScrollableZone* Objects2dFactory::scrollableZone(CCNode* scene, ScrollDirection scroll_direction, float left, float bottom, float width, float height, int touchPriority)
{
	return new ScrollableZone(scene, scroll_direction, left, bottom, width, height, touchPriority);
}

CCSprite* Objects2dFactory::rectangle(CCNode* scene, ccColor3B color, float positionX, float positionY, float width, float height, AlignX alignX, AlignY alignY, 
	GLubyte opacity, int zOrder, void* argument)
{
	// Check arguments validity
	if(scene == NULL)
		return NULL;

	// Create an image
	CCSprite* image = CCSprite::create("TopQXResources/white.png");
    if(!image)
		return NULL;

	// Set color
	image->setColor(color);

	// Set opacity
	image->setOpacity(opacity);

    // Place the image in the given position
    image->setPosition(ccp(positionX, positionY));
	
	// Set anchor
	image->setAnchorPoint(ccp(Constants::getAnchorValue(alignX), Constants::getAnchorValue(alignY)));
	
	// Set size
	float imageWidth = image->boundingBox().size.width;
    image->setScaleX(width / imageWidth);
	float imageHeight = image->boundingBox().size.height;
    image->setScaleY(height / imageHeight);

	// Set argument as user data
	if (argument != NULL)
		image->setUserData(argument);

	// Add menu to the scene
	scene->addChild(image, zOrder);

	return image;
}

CCMenuItemImage* Objects2dFactory::clickableRectangle(CCNode* scene, CCMenu* menu, ccColor3B color, float positionX, float positionY, float width, float height, 
	SEL_MenuHandler selector, void* selectorArg, AlignX alignX, AlignY alignY, GLubyte opacity, int zOrder)
{
	// Check arguments validity
	if(scene == NULL || menu == NULL || !selector)
		return NULL;

	// Create a button menu item
    CCMenuItemImage *button = CCMenuItemImage::create("TopQXResources/white.png", "TopQXResources/white.png", "TopQXResources/white.png", scene, selector);
    if(!button)
		return NULL;

	// Set argument
	if (selectorArg != NULL)
	{
		button->setUserData(selectorArg);
	}
	
	// Set color
	button->setColor(color);

	// Set opacity
	button->setOpacity(opacity);

    // Place the menu item in the given position
    button->setPosition(ccp(positionX, positionY));

	// Set anchor
	button->setAnchorPoint(ccp(Constants::getAnchorValue(alignX), Constants::getAnchorValue(alignY)));
	
	// Set menu item width
	float buttonWidth = button->boundingBox().size.width;
    button->setScaleX(width / buttonWidth);

	// Set menu item height
	float buttonHeight = button->boundingBox().size.height;
	button->setScaleY(height / buttonHeight);
    
	// Add button to the menu
	menu->addChild(button, zOrder);
	
	return button;
}
