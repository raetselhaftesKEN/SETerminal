/**
* @file Character.cpp
* @author √œ”Ó
*/

#include "cocos2d.h"
#include "Character.h"

void Character::bindPictureSprite(cocos2d::Sprite* sprite)
{
	sprite_ = sprite;
	addChild(sprite);
}
