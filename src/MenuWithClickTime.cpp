//
//  MenuWithClickTime.cpp
//  Vox4All
//
//  Created by TopQX 02 on 21/11/12.
//
//

#include "MenuWithClickTime.h"

using namespace cocos2d;

void MenuWithClickTime::onEnter()
{
	// Add touch delegate
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

void MenuWithClickTime::onExit()
{
	// Remove touch delegate
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
	CCMenu::onExit();
}

MenuWithClickTime::MenuWithClickTime(int clickSeconds)
{
    _click_seconds = clickSeconds;
    
	va_list ap;
    initWithItems(NULL, ap);
}

MenuWithClickTime* MenuWithClickTime::createWithTime(int clickSeconds)
{
    return new MenuWithClickTime(clickSeconds);
}

bool MenuWithClickTime::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    // Define seconds on begin
    _seconds_on_begin = time(NULL);
    
    return CCMenu::ccTouchBegan(touch, event);
}

void MenuWithClickTime::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    // If click seconds didn't pass, nothing occurs 
    if (time(NULL) - _seconds_on_begin < _click_seconds)
    {
        m_pSelectedItem->unselected();
        m_eState = kCCMenuStateWaiting;
        return;
    }
    
    CCMenu::ccTouchEnded(touch, event);
}