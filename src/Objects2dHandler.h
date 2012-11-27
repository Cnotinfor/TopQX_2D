#ifndef OBJECTS2DHANDLER_H
#define OBJECTS2DHANDLER_H

#include "cocos2d.h"
#include <string>
#include <list>

using namespace std;
using namespace cocos2d;

class Objects2dHandler
{
public:
	// Resize a given node
	static void setNewSize(cocos2d::CCNode* nodeToResize, float width, float height);
	
	// Move a list of nodes incrementing it position horizontally and vertically
	static void incrementPosition(float horizontalIncrement, float verticalIncrement, list<cocos2d::CCNode*> nodesToMove);

	// Check if a given position is inside a node
	static bool isPositionInsideNode(cocos2d::CCPoint* position, cocos2d::CCNode* destination);
	
	// Add a draggable child to a node and remove it from parent
	static void changeDraggableNodeParent(cocos2d::CCNode* nodeToAdd, cocos2d::CCNode* newParent, CCNode* oldParent); // Old parent is mandatory, because if it is a scrollable zone, node is not a direct child
};

#endif // OBJECTS2DHANDLER_H