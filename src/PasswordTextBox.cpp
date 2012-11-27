//
//  CSPasswordTextBox.cpp
//  MasterOfRessuction
//
//  Created by Filipe Teixeira on 24/09/12.
//
//

#include "PasswordTextBox.h"
PasswordTextBox::PasswordTextBox(std::string placeHolderText, float size, string fontName, float width, float height, CCTextAlignment textAlignment, char delimiterChar, int maxNumberOfChars, ccColor3B textColor, ccColor3B placeHolderColor):
                    TextBox(placeHolderText, size, fontName, width, height, textAlignment, delimiterChar, maxNumberOfChars, textColor, placeHolderColor)
{
    mIsPassword=true;
}

void PasswordTextBox::setString(const char *text)
{
    CC_SAFE_DELETE(m_pInputText);
    
	// Field is inactive and text is empty?
	if (!_is_active && string(text) == "")
	{
		// Call parent set string, which defines if it necessary to show place holder or not
        string finalString = string(text);
        
		CCTextFieldTTF::setString(finalString.c_str()); // Sometimes text point to the same location that m_pInputText; so, if it is sent to set string (where m_pInputText is deleted), it is deleted
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
    
    //begin code modification
    string finalString = string(m_pInputText->c_str());
    if (mIsPassword)
    {
        int length = strlen(finalString.c_str());
        
        finalString = fillWithStars(length);
    }
    //end code modification
    
	// Is active?
	if (_is_active)
	{
        // Add delimiter character
        finalString += _delimiter_char;
    }
    
	// Update string
    CCLabelTTF::setString(finalString.c_str());
    
    m_nCharCount = strlen(m_pInputText->c_str());
}

std::string PasswordTextBox::fillWithStars( int length)
{
    stringstream str;
    for(int i=0;i<length;i++)
    {
        str<<"*";
    }
    return str.str();
}
