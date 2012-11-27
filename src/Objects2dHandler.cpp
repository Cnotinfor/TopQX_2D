#include "Stdafx.h"
#include "Objects2dHandler.h"
#include "ScrollableZone.h"

void Objects2dHandler::setNewSize(cocos2d::CCNode* nodeToResize, float width, float height)
{
	float imageWidth = nodeToResize->boundingBox().size.width / nodeToResize->getScaleX();
	nodeToResize->setScaleX(width / imageWidth);
	float imageHeight = nodeToResize->boundingBox().size.height / nodeToResize->getScaleY();
	nodeToResize->setScaleY(height / imageHeight);
}

void Objects2dHandler::incrementPosition(float horizontalIncrement, float verticalIncrement, list<cocos2d::CCNode*> nodesToMove)
{
	for (list<CCNode*>::iterator iteratNode = nodesToMove.begin(); iteratNode != nodesToMove.end(); iteratNode++)
	{
		(*iteratNode)->setPosition(ccp((*iteratNode)->getPositionX() + horizontalIncrement, (*iteratNode)->getPositionY() + verticalIncrement));
	}
}

bool Objects2dHandler::isPositionInsideNode(CCPoint* position, CCNode* destination)
{
	return position->x >= destination->getPositionX() - destination->getAnchorPoint().x * destination->boundingBox().size.width && 
		   position->x <= destination->getPositionX() + (1 - destination->getAnchorPoint().x) * destination->boundingBox().size.width &&
		   position->y >= destination->getPositionY() - destination->getAnchorPoint().y * destination->boundingBox().size.height && 
		   position->y <= destination->getPositionY() + (1 - destination->getAnchorPoint().y) * destination->boundingBox().size.height;
}

void Objects2dHandler::changeDraggableNodeParent(CCNode* nodeToAdd, CCNode* newParent, CCNode* oldParent)
{
	// Add to new parent
	nodeToAdd->setParent(NULL);
	newParent->addChild(nodeToAdd);
	oldParent->removeChild(nodeToAdd, false);
	nodeToAdd->setParent(newParent);
	
	// Remove draggable child from old parent
	ScrollableZone* scrZoneOldParent = dynamic_cast<ScrollableZone*>(oldParent);
	TargetedTouchDelegate* targTouchDelOldParent = dynamic_cast<TargetedTouchDelegate*>(oldParent);
	if (scrZoneOldParent != NULL)
		scrZoneOldParent->removeDraggableChild(nodeToAdd);
	else if (targTouchDelOldParent != NULL)
		targTouchDelOldParent->removeDraggableNode(nodeToAdd);

	// Add as draggable child to new parent
	ScrollableZone* scrZoneNewParent = dynamic_cast<ScrollableZone*>(newParent);
	TargetedTouchDelegate* targTouchDelNewParent = dynamic_cast<TargetedTouchDelegate*>(newParent);
	if (scrZoneNewParent != NULL)
		scrZoneNewParent->addDraggableChild(nodeToAdd);
	else if (targTouchDelNewParent != NULL)
		targTouchDelNewParent->addDraggableNode(nodeToAdd);
}
