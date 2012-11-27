//
//  MenuWithClickTime.h
//  Vox4All
//
//  Created by TopQX 02 on 21/11/12.
//
//

#ifndef __Vox4All__MenuWithClickTime__
#define __Vox4All__MenuWithClickTime__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class MenuWithClickTime : public CCMenu
{
public:
    // Create
    static MenuWithClickTime* createWithTime(int clickSeconds);
    
    MenuWithClickTime(int clickSeconds);
    
	// On enter overridden event
	virtual void onEnter();
    
	// On exit overridden event
	virtual void onExit();
    
    // Touch events
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
private:
    // Click secondes
    int _click_seconds;
    
    // Timer
    time_t _seconds_on_begin;
};

#endif /* defined(__Vox4All__MenuWithClickTime__) */
