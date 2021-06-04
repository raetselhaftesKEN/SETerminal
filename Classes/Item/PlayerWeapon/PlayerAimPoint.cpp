/**
* @file PlayerAimPoint.cpp
*/

#include "cocos2d.h"
#include "PlayerAimPoint.h"


PlayerAimPoint* PlayerAimPoint::create(const std::string& filename)
{
	auto aimpoint = new(std::nothrow) PlayerAimPoint();
	if (aimpoint == nullptr)
	{
		return nullptr;
	}
	aimpoint->bindPictureSprite(cocos2d::Sprite::create(filename));

	if (aimpoint && aimpoint->sprite_)
	{
		aimpoint->autorelease();
		return aimpoint;
	}

	return nullptr;
}

void PlayerAimPoint::SetTarget(cocos2d::Vec2 Target)
{
	setPosition(Target);
}