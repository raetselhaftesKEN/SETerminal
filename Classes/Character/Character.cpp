/**
* @file Character.cpp
*/

#include "cocos2d.h"
#include "Character.h"

void Character::bindPictureSprite(cocos2d::Sprite* sprite)
{
	sprite_ = sprite;
	addChild(sprite_);
}

void Character::die()
{
	isAlive_ = false;
	health_ = 0;
}

cocos2d::Animate* Character::createAnimate(const char* animateName, int width, int height, int frames)
{
	cocos2d::SpriteFrame* frame = nullptr;
	cocos2d::Vector<cocos2d::SpriteFrame*> frameVector;
	for (int i = 1; i <= frames; i++)
	{
		frame = cocos2d::SpriteFrame::create(cocos2d::StringUtils::format("%s%d.png", animateName, i), cocos2d::Rect(0, 0, width, height));
		if (frame == nullptr)
		{
			cocos2d::log("animate loading failed");
		}
		else
		{
			frameVector.pushBack(frame);
		}
	}
	cocos2d::Animation* animation = cocos2d::Animation::createWithSpriteFrames(frameVector, 0.2f);
	animation->setLoops(-1);
	cocos2d::Animate* action = cocos2d::Animate::create(animation);
	action->retain();

	return action;
}