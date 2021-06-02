/**
* @file Medkit.cpp
*/

#include "cocos2d.h"
#include "Medkit.h"
#include "Const/Const.h"

Medkit* Medkit::create()
{
	auto medkit = new(std::nothrow) Medkit();
	if (medkit == nullptr)
	{
		return nullptr;
	}
	medkit->sprite_ = cocos2d::Sprite::create("medkit.png");

	
	if (medkit && medkit->sprite_)
	{
		medkit->addChild(medkit->sprite_);
		medkit->recovery_ = MEDKIT_DEFAULT_RECOVERABILITY;
		medkit->itemInfo_ = cocos2d::Label::createWithTTF("Medkit", "fonts/IRANYekanBold.ttf", 360);
		medkit->addChild(medkit->itemInfo_, 2);
		medkit->itemInfo_->setTextColor(cocos2d::Color4B(0, 0, 0, 255));
		medkit->itemInfo_->setPosition(cocos2d::Vec2(0, 400));
		medkit->itemInfo_->setVisible(false);
		medkit->setTag(ITEM_TAG);
		medkit->setScale(0.05f);
		medkit->bindPhysicsBody();
		medkit->retain();

		return medkit;
	}
	return nullptr;
}

bool Medkit::bindPhysicsBody()
{
	auto physicsBody = cocos2d::PhysicsBody::createBox(sprite_->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setContactTestBitmask(ITEM_CONTACT_MASK);
	physicsBody->setCategoryBitmask(ITEM_CATEGORY_MASK);
	setPhysicsBody(physicsBody);

	return true;
}

int Medkit::getRecovery()
{
	return recovery_;
}