/**
* @file Weapon.cpp
*/

#include "cocos2d.h"
#include "Weapon.h"
#include "./Scene/HelloWorldScene.h"

Weapon* Weapon::create(const std::string& filename)
{
	auto weapon = new(std::nothrow) Weapon();
	if (weapon == nullptr)
	{
		return nullptr;
	}
	auto picture = cocos2d::Sprite::create(filename);

	if (weapon && picture)
	{
		weapon->addChild(picture);
		weapon->autorelease();
		return weapon;
	}

	return nullptr;
}
