#include "TextBox.h"
#include "Objects2dFactory.h"

TextBox::TextBox(string placeHolderText, float size, string fontName, float width, float height, CCTextAlignment textAlignment, char delimiterChar, int maxNumberOfChars, ccColor3B textColor, ccColor3B placeHolderColor)
{
	// It is not editable
	_is_active = false;

	// Delimiter character
	_delimiter_char = delimiterChar;

	// Init and add place holder text
	initWithPlaceHolder(placeHolderText.c_str(), CCSize(width, height), textAlignment, fontName.c_str(), size);

	// Set empty string, so that place holder is not the string
	setString("");
    
    // Set color
    setColor(textColor);
    
    // Set place holder color
    setColorSpaceHolder(placeHolderColor);
	
	// Default value;
	_pholder_original_color = m_ColorSpaceHolder;
    
    // Maximum number of characters
    _max_chars = maxNumberOfChars;
}

void TextBox::onEnter()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate((TargetedTouchDelegate*)this, kCCMenuHandlerPriority - 1, false); // kCCMenuHandlerPriority - 1 has a higher priority than any CCMenu

	CCNode::onEnter();
}

void TextBox::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate((TargetedTouchDelegate*)this);
	
	CCNode::onExit();
}

bool TextBox::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	// Final position
	CCPoint* finalPosition = new CCPoint(pTouch->getLocationInView().x, size.height - pTouch->getLocationInView().y);
	*finalPosition = this->getParent()->convertToNodeSpace(*finalPosition);

	// If it is active and user clicked outside text box
	if (_is_active && !Objects2dHandler::isPositionInsideNode(finalPosition, this))
	{
		// Activate text box
		detachWithIME();
	}
	// If it is not active and user clicked inside text box
	else if (!_is_active && Objects2dHandler::isPositionInsideNode(finalPosition, this))
	{
		// Deactivate text box
		attachWithIME();
	}
	
	// Call other touch events
	return false;
}

bool TextBox::attachWithIME()
{
	// Remove place holder color, because when text box has an empty input text, it mustn't show place holder text
	_pholder_original_color = m_ColorSpaceHolder;
	m_ColorSpaceHolder = getColor();

	// Update state
	_is_active = true;

	// Update input
	setString(getString());
	
	return CCTextFieldTTF::attachWithIME();
}

bool TextBox::detachWithIME()
{
	// Reset place holder color
	m_ColorSpaceHolder = _pholder_original_color;

	// Update state
	_is_active = false;
	
	// Update input
	setString(getString());

	return CCTextFieldTTF::detachWithIME();
}

void TextBox::deleteBackward()
{
	// Remove text
	CCTextFieldTTF::deleteBackward();
	
	// Update input - necessary because last character deletion doesn't call CCTextFieldTTF::setString (or TextBox::setString)
	setString(getString());
}

void TextBox::setString(const char *text)
{
	// Isn't active?
	if (!_is_active)
	{
		// Call parent set string, which defines if it necessary to show place holder or not
		string inputText = string(text);
		CCTextFieldTTF::setString(inputText.c_str()); // Sometimes text point to the same location that m_pInputText; so, if it is sent to set string (where m_pInputText is deleted), it is deleted
		return;
	}
    
    // If it has more than the allowed characters
    if (strlen(text) > _max_chars)
    {
        // Characteres after the maximum characters allowed are removed
        string textStrAux = string(text).substr(0, _max_chars);
        text = new char[textStrAux.length() + 1];
        text = textStrAux.c_str();
    }

	// Update m_pInputText
	m_pInputText = new std::string(text);

	// Add delimiter character
	string textStr = string(text);
	textStr += _delimiter_char;

	// Update string
	CCLabelTTF::setString(textStr.c_str());

	// Update number of characters
	m_nCharCount = strlen(m_pInputText->c_str());
}
