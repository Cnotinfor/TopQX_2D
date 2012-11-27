#ifndef TARGETEDTOUCHDELEGATE_H
#define TARGETEDTOUCHDELEGATE_H

#include "cocos2d.h"
#include <string>
#include <list>

using namespace std;

class TargetedTouchDelegate : public cocos2d::CCTargetedTouchDelegate
{
public:
	// Begin drag event
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* thisevent);

	// Dragging event
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* thisevent);
	
	// End drag event
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* thisevent);

	// Add a draggable node
	void addDraggableNode(cocos2d::CCNode* node);
	
	// Remove a node from draggable nodes list
	void removeDraggableNode(cocos2d::CCNode* node);
	
	// Get the node which is being dragged
	cocos2d::CCNode* getDraggingNode();
protected:
	// Return node to the original position
	void resetPosition();

	// Get the node position relative to mouse
	cocos2d::CCPoint getNodeRelativePosition();

	// Reset the default values
	void resetDefaultValues();

	// Get the node which is being dragged
	list<cocos2d::CCNode*> getDraggableNodes();
private:	
	// All draggable nodes
	list<cocos2d::CCNode*> _draggable_nodes;

	// The node which is being dragged
	cocos2d::CCNode* _dragging_node;

	// Original position X and Y
	float _original_node_x;
	float _original_node_y;

	// Position relative to mouse X and Y
	float _node_relative_to_mouse_x;
	float _node_relative_to_mouse_y;
};

#endif //TARGETEDTOUCHDELEGATE_H