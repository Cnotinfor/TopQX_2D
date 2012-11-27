//
//  CSPasswordTextBox.h
//  MasterOfRessuction
//
//  Created by Filipe Teixeira on 24/09/12.
//
//

#ifndef __MasterOfRessuction__CSPasswordTextBox__
#define __MasterOfRessuction__CSPasswordTextBox__
#include "TextBox.h"
//#include "../Dependencies.h"
#include <iostream>
class PasswordTextBox:public TextBox
{
    bool mIsPassword;
public:
    PasswordTextBox(std::string placeHolderText, float size, string fontName, float width, float height, CCTextAlignment textAlignment, char delimiterChar = '|', int maxNumberOfChars = 999, ccColor3B textColor = ccc3(255, 255, 255), ccColor3B placeHolderColor = ccc3(127, 127, 127));
    void setString(const char *text);
    std::string fillWithStars( int length);
};

#endif /* defined(__MasterOfRessuction__CSPasswordTextBox__) */
