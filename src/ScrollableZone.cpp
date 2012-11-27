
#include "ScrollableZone.h"
#include "Objects2dFactory.h"

const float ScrollableZone::BOUNCE_TIME = 0.2f;
const float ScrollableZone::FRICTION = 0.96f;

ScrollableZone::ScrollableZone(CCNode* parent, ScrollDirection scroll_direction, float left, float bottom, float width, float height, int touchPriority)
{
	_scroll_direction = scroll_direction;

	_left = left;
	_bottom = bottom;
	_width = width;
	_height = height;
	_content_size = 0;
	
	// Default values
	_bounce_direction = None;
	_bounce_velocity = 0;
	_is_dragging = false;
	_last_position = 0;
	_scrzone_priority = touchPriority;
	_is_scrollable = false;

	// Scroll layer
	_scroll_layer = Objects2dFactory::rectangle(parent, ccc3(255, 255, 255), _left, _bottom, 1, 1, Left, Bottom, 0);
	if (_scroll_direction == Vertical)
		_scroll_layer->setContentSize(CCSize(_width, _content_size));
	else if (_scroll_direction == Horizontal)
		_scroll_layer->setContentSize(CCSize(_content_size, _height));
	_scroll_layer->setParent(NULL);
	addChild(_scroll_layer);
	parent->removeChild(_scroll_layer, false);
	_scroll_layer->setParent(this);

	// Mark scroll layer as draggable
	addDraggableNode(_scroll_layer);
	
	// Scroll bar
	_scroll_bar = Objects2dFactory::rectangle(parent, ccc3(0, 0, 0), 0, 0, 1, 1, Left, Bottom, 100);
	_scroll_bar->setParent(NULL);
	addChild(_scroll_bar);
	parent->removeChild(_scroll_bar, false);
	_scroll_bar->setParent(this);
	_scroll_bar->setVisible(false);

	// Add scrollable zone to parent
	parent->addChild(this);
}

void ScrollableZone::setShowedSquare(float left, float bottom, float width, float height)
{
	if (left >= 0)
	{
		_left = left;
		_scroll_layer->setPositionX(_left);
	}
	
	if (bottom >= 0)
	{
		_bottom = bottom;
		_scroll_layer->setPositionY(_bottom);
	}
	
	// If size is not changed, it is not necessary any content size change
	if (width >= 0 || height >= 0)
	{
		if (width >= 0)
			_width = width;
	
		if (height >= 0)
			_height = height;

		if (_scroll_direction == Vertical)
		{
			updateContentHeight(_content_size);
		}
		else if (_scroll_direction == Horizontal)
		{
			updateContentWidth(_content_size);
		}
	}
}

float ScrollableZone::getShowedSquareLeft()
{
	return _left;
}

float ScrollableZone::getShowedSquareBottom()
{
	return _bottom;
}

float ScrollableZone::getShowedSquareWidth()
{
	return _width;
}

float ScrollableZone::getShowedSquareHeight()
{
	return _height;
}
	
float ScrollableZone::getScrollLayerLeft()
{
	return _scroll_layer->getPositionX();
}

float ScrollableZone::getScrollLayerBottom()
{
	return _scroll_layer->getPositionY();
}

float ScrollableZone::getScrollLayerWidth()
{
	return _scroll_layer->boundingBox().size.width;
}

float ScrollableZone::getScrollLayerHeight()
{
	return _scroll_layer->boundingBox().size.height;
}

bool ScrollableZone::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* thisevent)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
	
	// Clicked position
	CCPoint* clickedPosition = new CCPoint(touch->getLocationInView().x, size.height - touch->getLocationInView().y);
	
	_is_child_being_dragged = false;

	// If touch point is inside scrollable zone...
	if (isPositionInsideScrollableZone(clickedPosition) && Objects2dHandler::isPositionInsideNode(clickedPosition, _scroll_layer))
	{
		// If it is a child, the parent touch events must be called
		TargetedTouchDelegate* parent = dynamic_cast<TargetedTouchDelegate*>(getParent());
		if (parent != NULL)
		{
			CCPoint* inScrollPosition = new CCPoint(
				clickedPosition->x - getScrollLayerLeft(), 
				clickedPosition->y - getScrollLayerBottom()
				);

			for (list<CCNode*>::iterator iteratDragChild = _draggable_children.begin(); iteratDragChild != _draggable_children.end(); iteratDragChild++)
			{
				// If node is being clicked
				if (Objects2dHandler::isPositionInsideNode(inScrollPosition, *iteratDragChild))
				{
					// Mark as child being dragged
					_is_child_being_dragged = true;

					// Call scene touch began event
					return parent->ccTouchBegan(touch, thisevent);
				}
			}
		}
		
		// Each scroll layer child
		CCArray* scrLayerChildren = _scroll_layer->getChildren();
		if (scrLayerChildren != NULL)
		{
			for (unsigned int index = 0; index < scrLayerChildren->count(); index++)
			{
				// If it is a CCTouchDelegate
				if (dynamic_cast<CCTouchDelegate*>(scrLayerChildren->objectAtIndex(index)) != NULL)
				{
					// If child is a text box
					TextBox* textBox = dynamic_cast<TextBox*>(scrLayerChildren->objectAtIndex(index));
					if (textBox != NULL)
					{
						// Call text box touch event
						textBox->ccTouchBegan(touch, thisevent);
					}

					CCPoint* inScrollPosition = new CCPoint(
						clickedPosition->x - getScrollLayerLeft(), 
						clickedPosition->y - getScrollLayerBottom()
						);
				
					// If menu is being touched
					CCMenu* menu = dynamic_cast<CCMenu*>(scrLayerChildren->objectAtIndex(index));
					CCNode* node = dynamic_cast<CCNode*>(scrLayerChildren->objectAtIndex(index));
					if (menu != NULL)
					{
						CCArray* menuChildren = menu->getChildren();
						if (menuChildren != NULL)
						{
							for (unsigned int indexMenuChild = 0; indexMenuChild < menuChildren->count(); indexMenuChild++)
							{
								CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(menuChildren->objectAtIndex(indexMenuChild));
								if (menuItem != NULL && Objects2dHandler::isPositionInsideNode(inScrollPosition, menuItem))
								{
									// Call node touch event
									return false;
								}
							}
						}
					}
					// If node is not a menu and is beign touched
					else if (node != NULL && Objects2dHandler::isPositionInsideNode(inScrollPosition, node))
					{
						// Call node touch event
						return false;
					}
				}
			}
		}

		if (_is_scrollable)
			_is_dragging = true;

		// Could start dragging
		return TargetedTouchDelegate::ccTouchBegan(touch, thisevent);
	}
	// If touch is outside scrollable zone
	else
	{
		// Each scroll layer child
		CCArray* scrLayerChildren = _scroll_layer->getChildren();
		if (scrLayerChildren != NULL)
		{
			for (unsigned int index = 0; index < scrLayerChildren->count(); index++)
			{
				// If text box is being clicked
				TextBox* textBox = dynamic_cast<TextBox*>(scrLayerChildren->objectAtIndex(index));
				if (textBox != NULL)
				{
					// Call not only scrollable zone touch event, but also text box touch event
					textBox->ccTouchBegan(touch, thisevent);
				}
			}
		
			// Each scroll layer child
			for (unsigned int index = 0; index < scrLayerChildren->count(); index++)
			{
				// If it is a CCTouchDelegate
				CCTouchDelegate* touchDel = dynamic_cast<CCTouchDelegate*>(scrLayerChildren->objectAtIndex(index));
				if (touchDel != NULL)
				{
					CCPoint* inScrollPosition = new CCPoint(
						clickedPosition->x - getScrollLayerLeft(), 
						clickedPosition->y - getScrollLayerBottom()
						);

					// If menu is being touched
					CCMenu* menu = dynamic_cast<CCMenu*>(scrLayerChildren->objectAtIndex(index));
					CCNode* node = dynamic_cast<CCNode*>(scrLayerChildren->objectAtIndex(index));
					if (menu != NULL)
					{
						CCArray* menuChildren = menu->getChildren();
						if (menuChildren != NULL)
						{
							for (unsigned int indexMenuChild = 0; indexMenuChild < menuChildren->count(); indexMenuChild++)
							{
								CCMenuItem* menuItem = dynamic_cast<CCMenuItem*>(menuChildren->objectAtIndex(indexMenuChild));
								if (menuItem != NULL && Objects2dHandler::isPositionInsideNode(inScrollPosition, menuItem))
								{
									// Don't call node touch event
									return TargetedTouchDelegate::ccTouchBegan(touch, thisevent);
								}
							}
						}
					}
					// If node is not a menu and is beign touched
					else if (node != NULL && Objects2dHandler::isPositionInsideNode(inScrollPosition, node))
					{
						// Don't call node touch event
						return TargetedTouchDelegate::ccTouchBegan(touch, thisevent);
					}
				}
			}
		}
	}

	// Call other touch events
	return false;
}

void ScrollableZone::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* thisevent)
{
	// If it is a child being dragged
	if (_is_child_being_dragged)
	{
		// Call scene touch moved event
		TargetedTouchDelegate* parent = dynamic_cast<TargetedTouchDelegate*>(getParent());
		parent->ccTouchMoved(touch, thisevent);
		return;
	}

	if (_is_dragging && getDraggingNode() != NULL)
	{
		if (_scroll_layer == getDraggingNode())
		{
			// Get touch Y position (given Y position is counted from top)
			CCSize size = CCDirector::sharedDirector()->getWinSize();

			if (_scroll_direction == Vertical)
			{
				float touchY = size.height - touch->getLocationInView().y;

				_scroll_layer->setPositionY(touchY + getNodeRelativePosition().y);
			
				// Set scroll bar position
				float scrollBarPosition = _content_size != 0 ? _bottom + (_bottom - getScrollLayerBottom()) / _content_size * _height : 0;
				_scroll_bar->setPosition(ccp(_left + _width - _scroll_bar->boundingBox().size.width, scrollBarPosition));
			}
			else if (_scroll_direction == Horizontal)
			{
				float touchX = touch->getLocationInView().x;

				_scroll_layer->setPositionX(touchX + getNodeRelativePosition().x);
			
				// Set scroll bar position
				float scrollBarPosition = _content_size != 0 ? _left + (_left - getScrollLayerLeft()) / _content_size * _width : 0;
				_scroll_bar->setPosition(ccp(scrollBarPosition, _bottom));
			}
		}
	}
}

void ScrollableZone::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* thisevent)
{
	_is_dragging = false;

	// If it is a child being dragged
	if (_is_child_being_dragged)
	{
		// Get dragging node
		TargetedTouchDelegate* parent = dynamic_cast<TargetedTouchDelegate*>(getParent());
		
		// Call scene touch ended event
		parent->ccTouchEnded(touch, thisevent);

		// Reset variable value
		_is_child_being_dragged = false;

		return;
	}
}

void ScrollableZone::visit() 
{
	// Cut the content outside the given dimensions
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	glEnable(GL_SCISSOR_TEST);
	cocos2d::CCDirector::sharedDirector()->getOpenGLView()->setScissorInPoints(_left, _bottom, _width, _height);
	CCNode::visit();
	glDisable(GL_SCISSOR_TEST);
}

void ScrollableZone::onEnter()
{
	// Add touch delegate
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate((TargetedTouchDelegate*)this, _scrzone_priority, true);
	
	// Bounce effect call
	schedule(schedule_selector(ScrollableZone::bounceEffect));

	CCNode::onEnter();
	
	// Each scroll layer child
	CCArray* scrLayerChildren = _scroll_layer->getChildren();
	if (scrLayerChildren != NULL)
	{
		for (unsigned int index = 0; index < scrLayerChildren->count(); index++)
		{
			// If it is a CCTouchDelegate
			CCTouchDelegate* touchDel = dynamic_cast<CCTouchDelegate*>(scrLayerChildren->objectAtIndex(index));
			if (touchDel != NULL)
			{
				// Set lower priority than scrollable zone
				CCDirector::sharedDirector()->getTouchDispatcher()->setPriority(_scrzone_priority + 2, touchDel);
			
				// Is a textbox?
				TextBox* textBox = dynamic_cast<TextBox*>(scrLayerChildren->objectAtIndex(index));
				if (textBox != NULL)
					// Set lower priority than scrollable zone but higher than others touch delegates, so that it can be dettached
					CCDirector::sharedDirector()->getTouchDispatcher()->setPriority(_scrzone_priority + 1, touchDel);
			}
		}
	}
}

void ScrollableZone::onExit()
{
	// Remove touch delegate
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate((TargetedTouchDelegate*)this);

	CCNode::onExit();
}

void ScrollableZone::addChild(CCNode* child)
{
	addChild(child, 0);
}

void ScrollableZone::addChild(CCNode* child, int zOrder)
{
	addChild(child, zOrder, kCCNodeTagInvalid);
}

void ScrollableZone::addChild(CCNode* child, int zOrder, int tag)
{
	if (child == _scroll_layer || child == _scroll_bar)
	{
		// Original child
		CCNode::addChild(child, zOrder, tag);
		return;
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	
	list<CCNode*> itemsList;

	// if child is a menu, nodeTop will always be size.height... So, we have to check menu items
	if(child->boundingBox().size.height == size.height && child->getChildrenCount() > 0)
	{
		// Add children to the list
		CCArray* menuItems = child->getChildren();
		if (menuItems != NULL)
			for (unsigned int index = 0; index < menuItems->count(); index++)
			{
				itemsList.push_back((CCNode*)menuItems->objectAtIndex(index));
			}
	}

	// If child is not a menu
	else
		// List has only the child
		itemsList.push_back(child);
	
	if (_scroll_direction == Vertical)
	{
		float maxTop = 0, nodeTop;
		for (list<CCNode*>::iterator iteratNode = itemsList.begin(); iteratNode != itemsList.end(); iteratNode++)
		{
			// Item top
			nodeTop = (*iteratNode)->getPositionY() + (1 - (*iteratNode)->getAnchorPoint().y) * (*iteratNode)->boundingBox().size.height;

			// Check what is the maximum top of checked items
			if (nodeTop > maxTop)
				maxTop = nodeTop;
		}
	
		if (maxTop > _content_size)
		{
			updateContentHeight(maxTop);
		}
	}
	else if (_scroll_direction == Horizontal)
	{
		float maxRight = 0, nodeRight;
		for (list<CCNode*>::iterator iteratNode = itemsList.begin(); iteratNode != itemsList.end(); iteratNode++)
		{
			// Item right
			nodeRight = (*iteratNode)->getPositionX() + (1 - (*iteratNode)->getAnchorPoint().x) * (*iteratNode)->boundingBox().size.width;

			// Check what is the maximum right of checked items
			if (nodeRight > maxRight)
				maxRight = nodeRight;
		}
	
		if (maxRight > _content_size)
		{
			updateContentWidth(maxRight);
		}
	}

	// Original child
	_scroll_layer->addChild(child, zOrder, tag);
}

void ScrollableZone::removeChild(CCNode* pChild, bool bCleanup)
{
	_scroll_layer->removeChild(pChild, bCleanup);
}

void ScrollableZone::updateContentSize(float newContentSize)
{
	if (_scroll_direction == Vertical)
	{
		updateContentHeight(newContentSize);
	}
	else if (_scroll_direction == Horizontal)
	{
		updateContentWidth(newContentSize);
	}
}

void ScrollableZone::updateContentHeight(float contentHeight)
{
	// Reset bounce effect
	_bounce_direction = None;

	// Update content size
	_content_size = contentHeight;
	
	// Check if it is scrollabel
	_is_scrollable = _content_size > _height;

	// Put it on the top
	_scroll_layer->setPositionY(_bottom + _height - _content_size);

	// Set new content size
	_scroll_layer->setContentSize(CCSize(_width, _content_size));
	
    CCSize size = CCDirector::sharedDirector()->getWinSize();

	// Scroll bar new size
	float scrollBarWidth = size.width * 0.01f;
	float scrollBarHeight = MIN(_height / _content_size, 1) * _height;
	
	// Set size
	float imageWidth = _scroll_bar->boundingBox().size.width / _scroll_bar->getScaleX();
    _scroll_bar->setScaleX(scrollBarWidth / imageWidth);
	float imageHeight = _scroll_bar->boundingBox().size.height / _scroll_bar->getScaleY();
    _scroll_bar->setScaleY(scrollBarHeight / imageHeight);

	// Set scroll bar position (on the top too)
	_scroll_bar->setPosition(ccp(_left + _width - scrollBarWidth, _bottom + _height - scrollBarHeight));
	
	// Scroll bar visibility
	_scroll_bar->setVisible(_is_scrollable);
}

void ScrollableZone::updateContentWidth(float contentWidth)
{
	// Reset bounce effect
	_bounce_direction = None;

	_content_size = contentWidth;
	
	// Put it on the left
	_scroll_layer->setPositionX(_left);

	// Set new content size
	_scroll_layer->setContentSize(CCSize(_content_size, _height));
	
    CCSize size = CCDirector::sharedDirector()->getWinSize();

	// Scroll bar new size
	float scrollBarWidth = MIN(_width / _content_size, 1) * _width;
	float scrollBarHeight = size.height * 0.01f;
	
	// Set size
	float imageWidth = _scroll_bar->boundingBox().size.width / _scroll_bar->getScaleX();
    _scroll_bar->setScaleX(scrollBarWidth / imageWidth);
	float imageHeight = _scroll_bar->boundingBox().size.height / _scroll_bar->getScaleY();
    _scroll_bar->setScaleY(scrollBarHeight / imageHeight);

	// Set scroll bar position (on the top too)
	_scroll_bar->setPosition(ccp(_left + _width - scrollBarWidth, _bottom + _height - scrollBarHeight));

	// Scroll bar visibility
	_is_scrollable = _content_size > _width;
	_scroll_bar->setVisible(_is_scrollable);
}

void ScrollableZone::addDraggableChild(CCNode* child)
{
	TargetedTouchDelegate* parent = dynamic_cast<TargetedTouchDelegate*>(getParent());
	if (parent == NULL)
		return;

	//// Add child as a parent draggable node
	//parent->addDraggableNode(child);

	// Add to draggable children
	_draggable_children.push_back(child);
}

void ScrollableZone::removeDraggableChild(CCNode* child)
{
	TargetedTouchDelegate* parent = dynamic_cast<TargetedTouchDelegate*>(getParent());
	if (parent == NULL)
		return;

	//// Add child as a parent draggable node
	//parent->removeDraggableNode(child);

	// Add to draggable children
	_draggable_children.remove(child);
}

list<CCNode*> ScrollableZone::getDraggableChildren()
{
	return _draggable_children;
}

void ScrollableZone::bounceEffect(float delta)
{
	// positions for scrollLayer
	float bottom, min, max; // bottom/down = left & top/up = right, in horizontal direction;
	if (_scroll_direction == Vertical)
	{
		bottom = _scroll_layer->getPositionY();

		// Bounding area of scrollview
		//min = _bottom;
		//max = _bottom + (_height < _content_size ? _height : _content_size);
		max = _bottom + _height;
		min = max - (_height < _content_size ? _height : _content_size);
	}
	else if (_scroll_direction == Horizontal)
	{
		bottom = _scroll_layer->getPositionX();

		// Bounding area of scrollview
		min = _left;
		max = min + (_width < _content_size ? _width : _content_size);
	}
	float top = bottom + _content_size;

	
	if(!_is_dragging)
	{
		// If bounce didn't start yet
		if(top < max &&_bounce_direction != Up) // Needed to be here, to fix in the top when height > content_size
		{
			_bounce_velocity = 0;
			_bounce_direction = Up;
		}
		else if(bottom > min && _bounce_direction != Down)
		{
			_bounce_velocity = 0;
			_bounce_direction = Down;
		}

		// Make bounce movement
		if(_bounce_direction == Up)
		{
			if(_bounce_velocity >= 0)
			{
				float delta = (max - top);
				float deltaPerFrame = (delta / (BOUNCE_TIME * 60));
				_bounce_velocity = deltaPerFrame;
			}
		}
		else if(_bounce_direction == Down)
		{
			if(_bounce_velocity <= 0)
			{
				float delta = (min - bottom);
				float deltaPerFrame = (delta / (BOUNCE_TIME * 60));
				_bounce_velocity = deltaPerFrame;
			}
		}
		else
		{
			_bounce_velocity *= FRICTION;
		}
		
		if (_scroll_direction == Vertical)
		{
			_scroll_layer->setPositionY(_scroll_layer->getPositionY() + _bounce_velocity);
		
			// Set scroll bar position
			float scrollBarPosition = _content_size != 0 ? _bottom + (_bottom - getScrollLayerBottom()) / _content_size * _height : 0;
			_scroll_bar->setPosition(ccp(_left + _width - _scroll_bar->boundingBox().size.width, scrollBarPosition));
		}
		else if (_scroll_direction == Horizontal)
		{
			_scroll_layer->setPositionX(_scroll_layer->getPositionX() + _bounce_velocity);
		
			// Set scroll bar position
			float scrollBarPosition = _content_size != 0 ? _left + (_left - getScrollLayerLeft()) / _content_size * _width : 0;
			_scroll_bar->setPosition(ccp(scrollBarPosition, _bottom));
		}
	}
	else
	{
		if(bottom <= min || top >= max)
		{
			_bounce_direction = None;
			
			if (_scroll_direction == Vertical)
			{
				_bounce_velocity = (_scroll_layer->getPositionY() - _last_position)/2;
				_last_position = _scroll_layer->getPositionY();
			}
			else if (_scroll_direction == Horizontal)
			{
				_bounce_velocity = (_scroll_layer->getPositionX() - _last_position)/2;
				_last_position = _scroll_layer->getPositionX();
			}
		}
	}
}

bool ScrollableZone::isPositionInsideScrollableZone(CCPoint* position)
{
	return position->x >= _left && 
		   position->x <= _left + _width &&
		   position->y >= _bottom && 
		   position->y <= _bottom + _height;
}

void ScrollableZone::moveToPosition(float position, bool withBounce)
{
	_bounce_direction = None;
	_bounce_velocity = 0;
	
	// If it is not scrollable, there is nowhere to move
	if (!_is_scrollable)
		return;

	if (!withBounce)
	{
		if (_scroll_direction == Vertical)
			_scroll_layer->setPositionY(position);
	}
	else
	{
		float delta;
		if (_scroll_direction == Vertical)
		{
			// If it is almost in top/bottom (can't bounce more...), it must be exactly in top/bottom
			if (_scroll_layer->getPositionY() > _bottom)
				_scroll_layer->setPositionY(_bottom);
			else if (_scroll_layer->getPositionY() + _content_size < _bottom + _height)
				_scroll_layer->setPositionY(_bottom + _height - _content_size);

			delta = position - _scroll_layer->getPositionY();
		}

		float deltaPerFrame = (delta / (BOUNCE_TIME * 60 * 2)); // Why? I don't know, but it works!
		_bounce_velocity = deltaPerFrame;
	}
}
