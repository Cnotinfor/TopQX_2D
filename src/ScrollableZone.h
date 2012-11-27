#ifndef SCROLLABLEZONE_H
#define SCROLLABLEZONE_H

#include "cocos2d.h"
#include "TargetedTouchDelegate.h"

using namespace cocos2d;

// Scroll directions
typedef enum ScrollDirection
{
	Horizontal,
	Vertical
}ScrollDirection;

class ScrollableZone : public CCNode, public TargetedTouchDelegate
{
public:
	/* Constructor

	parent = node where to put the scrollable zone into
	scroll_direction = if scroll is horizontal or vertical
	left = left of the scrollable zone (showed square, not scroll layer)
	bottom = bottom of the scrollable zone (showed square, not scroll layer)
	width = width of the scrollable zone (showed square, not scroll layer)
	height = height of the scrollable zone (showed square, not scroll layer)
	*/
	ScrollableZone(cocos2d::CCNode* parent, ScrollDirection scroll_direction, float left, float bottom, float width, float height, int touchPriority = 0);

	// Set showed square
	void setShowedSquare(float left = -1, float bottom = -1, float width = -1, float height = -1);
	
	// Get showed square position and size
	float getShowedSquareLeft();
	float getShowedSquareBottom();
	float getShowedSquareWidth();
	float getShowedSquareHeight();

	// Get scroll layer positions
	float getScrollLayerLeft();
	float getScrollLayerBottom();
	
	// Get scroll layer sizes
	float getScrollLayerWidth();
	float getScrollLayerHeight();
	
	// Visit overridden event
	virtual void visit();

	// Touch overridden events
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* thisevent);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* thisevent);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* thisevent);

	// On enter overridden event
	virtual void onEnter();

	// On exit overridden event
	virtual void onExit();
	
	// Add child overridden event
	virtual void addChild(cocos2d::CCNode* child);
    virtual void addChild(cocos2d::CCNode* child, int zOrder);
    virtual void addChild(cocos2d::CCNode* child, int zOrder, int tag);
	virtual void removeChild(cocos2d::CCNode* pChild, bool bCleanup);

	// Mark a child as draggable, which have to call the scene (parent of scrollable zone) touch events
	void addDraggableChild(CCNode* child);
	
	// Mark a child as not draggable, which have to call the scene (parent of scrollable zone) touch events
	void removeDraggableChild(CCNode* child);

	// Get the draggable children
	list<CCNode*> getDraggableChildren();
	
	// Enumeration of bounce directions
	enum BounceDirection
	{
		None,
		Up,
		Down
	};

	// Bounce effect on scroll
	void bounceEffect(float delta);

	// Check if a given position is inside a scrollable zone
	bool isPositionInsideScrollableZone(cocos2d::CCPoint* position);
	
	// Update content size, because some child's position changed
	void updateContentSize(float newContentSize);
	
	// Move scroll to bottom
	void moveToPosition(float position, bool withBounce);
private:
	// Update content size
	void updateContentHeight(float contentHeight);
	void updateContentWidth(float contentWidth);

	float _left, _bottom, _width, _height, _content_size;
	ScrollDirection _scroll_direction;
	CCSprite* _scroll_layer;
	CCSprite* _scroll_bar;
	list<CCNode*> _draggable_children;
	bool _is_child_being_dragged;
	BounceDirection _bounce_direction;
	float _bounce_velocity;
	bool _is_dragging;
	float _last_position;
	int _scrzone_priority;
	static const float BOUNCE_TIME;
	static const float FRICTION;
	bool _is_scrollable;
};

#endif // SCROLLABLEZONE_H