#include "Stdafx.h"
#include "TargetedTouchDelegate.h"

using namespace cocos2d;

bool TargetedTouchDelegate::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* thisevent)
{
	// Set the default values
	resetDefaultValues();

    CCSize size = CCDirector::sharedDirector()->getWinSize();
	float x = touch->getLocationInView().x;
	float y = size.height - touch->getLocationInView().y;

	for (list<CCNode*>::iterator iterat = _draggable_nodes.begin(); iterat != _draggable_nodes.end(); iterat++)
	{
		float nodeWidth = (*iterat)->boundingBox().size.width;
		float nodeHeight = (*iterat)->boundingBox().size.height;
		float nodeLeft = (*iterat)->getPositionX() - ((*iterat)->getAnchorPoint().x * nodeWidth);
		float nodeBottom = (*iterat)->getPositionY() - ((*iterat)->getAnchorPoint().y * nodeHeight);

		// If it is a position where node is and it is visible
		if ((*iterat)->isVisible() &&
			x >= nodeLeft && x <= nodeLeft + nodeWidth &&
			y >= nodeBottom && y <= nodeBottom + nodeHeight)
		{
			// Saves original node position
			_original_node_x = (*iterat)->getPositionX();
			_original_node_y = (*iterat)->getPositionY();
			
			// Saves node position relative to mouse
			_node_relative_to_mouse_x = (*iterat)->getPositionX() - x;
			_node_relative_to_mouse_y = (*iterat)->getPositionY() - y;

			// Initiate drag
			_dragging_node = (CCNode*)*iterat;

			break;
		}
	}

	return true;
}

void TargetedTouchDelegate::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* thisevent)
{
	if (_dragging_node != NULL)
	{
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		float x = touch->getLocationInView().x;
		float y = size.height - touch->getLocationInView().y;

		// Go to the mouse position - stay with the same position relative to mouse // OLD - If you want the same position relative to the mouse, you have to save the mouse original position, and check the difference of mouse and node positions
		_dragging_node->setPosition(ccp(x + _node_relative_to_mouse_x, y + _node_relative_to_mouse_y));
	}
}

void TargetedTouchDelegate::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* thisevent)
{
	// Reset dragging values
	resetDefaultValues();
}

void TargetedTouchDelegate::addDraggableNode(CCNode* node)
{
	_draggable_nodes.push_back(node);
}

void TargetedTouchDelegate::removeDraggableNode(cocos2d::CCNode* node)
{
	_draggable_nodes.remove(node);
}

cocos2d::CCNode* TargetedTouchDelegate::getDraggingNode()
{
	return _dragging_node;
}

void TargetedTouchDelegate::resetPosition()
{
	if (_dragging_node != NULL)
	{
		// Put it back to the original position
		_dragging_node->setPosition(ccp(_original_node_x, _original_node_y));

		//resetDefaultValues();
	}
}

cocos2d::CCPoint TargetedTouchDelegate::getNodeRelativePosition()
{
	return ccp(_node_relative_to_mouse_x, _node_relative_to_mouse_y);
}

void TargetedTouchDelegate::resetDefaultValues()
{
	_dragging_node = NULL;
	_original_node_x = -1;
	_original_node_y = -1;
}

list<cocos2d::CCNode*> TargetedTouchDelegate::getDraggableNodes()
{
	return _draggable_nodes;
}
