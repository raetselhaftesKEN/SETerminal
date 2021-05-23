/**
* @file Item.cpp
*/

#include "cocos2d.h"
#include "Item.h"

void Item::bindPictureSprite(cocos2d::Sprite* sprite)
{
	sprite_ = sprite;
	addChild(sprite_);
}