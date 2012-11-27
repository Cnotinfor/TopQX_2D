#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "cocos2d.h"
#include "TargetedTouchDelegate.h"

using namespace cocos2d;

class TextBox: public CCTextFieldTTF, public TargetedTouchDelegate
{
public:
	/* Constructor

	placeHolderText = string which appears when text box is empty and not editable
	size = font size
	width = maximum width of the text box
	height = maximum height of the text box
	textAlignment = alignment of the text
	delimiterChar = character which appears in the end of the text when it is editable
	*/
	TextBox(string placeHolderText, float size, string fontName, float width, float height, CCTextAlignment textAlignment, char delimiterChar = '|', int maxNumberOfChars = 999, ccColor3B textColor = ccc3(255, 255, 255), ccColor3B placeHolderColor = ccc3(127, 127, 127));

	// On enter event
    virtual void onEnter();

	//On exit event
    virtual void onExit();
    
	// Touch ended event
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	// Set as editable
	virtual bool attachWithIME();

	// Set as not editable
	virtual bool detachWithIME();

	// Set string
	virtual void setString(const char *text);
protected:
	// Delete last character
	virtual void deleteBackward();

	// Delimiter character
	char _delimiter_char;

	// Is editable?
	bool _is_active;
	
	// Saves the place holder original color
	cocos2d::ccColor3B _pholder_original_color;
    
    // Maximum number of characters
    int _max_chars;
};

#endif	// TEXTBOX_H
