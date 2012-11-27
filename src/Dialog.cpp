//
//  Dialog.cpp
//  Vox4All
//
//  Created by TopQX 02 on 21/11/12.
//
//

#include "Dialog.h"
#include "Objects2dFactory.h"
#include <stdarg.h>

using namespace cocos2d;

list<cocos2d::CCNode*> Dialog::_dialog_nodes;

Dialog::Dialog(CCNode* scene, string text, string normal_button_url, string selected_button_url, string disabled_button_url, ...)
{
    // List of buttons text
    list<string> texts;
    
    // List of buttons callback functions
    list<void*> selectors;
    
    // Get extra arguments
    va_list vl;
    va_start(vl,disabled_button_url);
    for (char* i = va_arg(vl, char*); i != NULL && string(i) != ""; i = va_arg(vl, char*))
    {
        texts.push_back(string(i));
        selectors.push_back(va_arg(vl, void*)); // It could be NULL
    }
    va_end(vl);
    
    // Clear nodes
    _dialog_nodes.clear();
    
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	
	// Bottom menu
	CCMenu* menu = Objects2dFactory::menu(scene);
	if (!menu)
		return;
    
	// Transparent image to prevent other buttons clicking
	CCMenuItemImage* transparentSprite = Objects2dFactory::clickableRectangle(scene, menu, ccc3(255, 255, 255), 0, 0, size.width, size.height,
                                                                              menu_selector(Dialog::menuNothingCallback), NULL, Left, Bottom, 100);
	if (!transparentSprite)
		return;
    
	_dialog_nodes.push_back(transparentSprite);
	
	// Box background
	float backCenter = size.width / 2;
	float backMiddle = size.height / 2;
	float minBackWidth = size.width * 0.3f;
	float minBackHeight = size.height * 0.3f;
	CCSprite* dialogBackgr = Objects2dFactory::rectangle(scene, ccc3(120, 120, 120), backCenter, backMiddle, minBackWidth, minBackHeight, Centered, Middle);
	if (!dialogBackgr)
		return;
    
	_dialog_nodes.push_back(dialogBackgr);
	
	// Label
	float labelCenter = size.width / 2;
	float labelBottom = size.height / 2;
	float labelMaxWidth = size.width * 0.8f;
	float labelMaxHeight = size.height * 0.5f;
	CCLabelTTF* dialogLabel = Objects2dFactory::label(scene, text, labelCenter, labelBottom, size.height * 0.04, ccc3(255, 255, 255), Centered);
	if (!dialogLabel)
		return;
    
	_dialog_nodes.push_back(dialogLabel);
    
	// Set maximum label width
	float labelWidth = 0;
	if (dialogLabel->boundingBox().size.width > labelMaxWidth)
		labelWidth = labelMaxWidth;
    
	// Set provisory label dimensions, so that we can calculate text height
	dialogLabel->setDimensions(CCSize(labelWidth, 0));
	
	// Set maximum label height
	float labelHeight = 0;
	if (dialogLabel->boundingBox().size.height > labelMaxHeight)
		labelHeight = labelMaxHeight;
    
	// Set label dimensions
	dialogLabel->setDimensions(CCSize(labelWidth, labelHeight));
    
	// Calculate box background size depending on label
	float backWidth = dialogLabel->boundingBox().size.width * 1.05f;
	if (minBackWidth > backWidth)
		backWidth = minBackWidth;
    
	float backHeight = dialogLabel->boundingBox().size.height * 2.1f;
	if (minBackHeight > backHeight)
		backHeight = minBackHeight;
    
	// Button size
	float horizSpace = size.width * 0.02f;
	float verticSpace = size.height * 0.02f;
	float btnWidth = size.width * 0.12f;
	float btnHeight = size.height * 0.06f;
    
	// Calculate box background size depending on buttons
    float dialogLeft = dialogBackgr->getPositionX() - dialogBackgr->getAnchorPoint().x * dialogBackgr->boundingBox().size.width;
    float allButtonsWidth = texts.size() * (btnWidth + horizSpace) + horizSpace;
	if (allButtonsWidth > dialogLeft)
		backWidth = allButtonsWidth;
    
	// Resize box background
	Objects2dHandler::setNewSize(dialogBackgr, backWidth, backHeight);
    
	// Button position
	float btnRight = dialogBackgr->getPositionX() + (1 - dialogBackgr->getAnchorPoint().x) * dialogBackgr->boundingBox().size.width - horizSpace;
	float btnBottom = dialogBackgr->getPositionY() - dialogBackgr->getAnchorPoint().y * dialogBackgr->boundingBox().size.height + verticSpace;
	
	// Dialog menu
	CCMenu* dialogMenu = Objects2dFactory::menu(scene);
	if (!dialogMenu)
		return;
    
    for (list<string>::reverse_iterator iteratBtnText = texts.rbegin(); iteratBtnText != texts.rend(); iteratBtnText++)
    {
		CCMenuItemImage* button = Objects2dFactory::textButton(scene, dialogMenu, *iteratBtnText, normal_button_url, selected_button_url, disabled_button_url, btnRight, btnBottom,
                                                                   btnWidth, btnHeight, menu_selector(Dialog::menuDialogButtonCallback), selectors.back(), Right, Bottom);
		if (!button)
			return;
        
        selectors.pop_back();
        
		_dialog_nodes.push_back(button);
        
		// Decrement button right
		btnRight -= btnWidth + horizSpace;
    }
}

void Dialog::menuNothingCallback(CCObject* pSender)
{
}

void Dialog::menuDialogButtonCallback(cocos2d::CCObject* pSender)
{
    void (*selector)() = (void(*)())((CCNode*)pSender)->getUserData();
    
	if (selector != NULL)
		(*selector)();
	
	((CCNode*)pSender)->schedule(schedule_selector(Dialog::removeButtons), 0, 0, 0);
}

void Dialog::removeButtons(float dt)
{
	for (list<CCNode*>::iterator iteratNode = _dialog_nodes.begin(); iteratNode != _dialog_nodes.end(); iteratNode++)
	{
		(*iteratNode)->removeFromParentAndCleanup(true);
	}
    
	_dialog_nodes.clear();
}
