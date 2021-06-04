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

cocos2d::Label* Item::getItemInfo()
{
	return itemInfo_;
}

void Item::pickUp()
{
	isHeld_ = true;
}

void Item::abandon()
{
	isHeld_ = false;
}

bool Item::isHeld()
{
	return isHeld_;
}