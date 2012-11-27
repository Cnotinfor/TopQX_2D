//
//  Dialog2.h
//  Vox4All
//
//  Created by TopQX 02 on 21/11/12.
//
//

#ifndef __Vox4All__Dialog2__
#define __Vox4All__Dialog2__

#include <iostream>
#include "cocos2d.h"
#include <list>

using namespace std;

class Dialog
{
public:
	Dialog(cocos2d::CCNode* scene, string text, string normal_button_url, string selected_button_url, string disabled_button_url, ...);
	void menuNothingCallback(cocos2d::CCObject* pSender);
	void menuDialogButtonCallback(cocos2d::CCObject* pSender);
	void removeButtons(float dt);
private:
	static list<cocos2d::CCNode*> _dialog_nodes;
};

#endif /* defined(__Vox4All__Dialog2__) */
