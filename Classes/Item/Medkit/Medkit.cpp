/**
* @file Medkit.cpp
*/

#include "cocos2d.h"
#include "Medkit.h"
#include "Character/Player.h"
#include "Scene/FightScene/FightScene.h"
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

Medkit* Medkit::create(cocos2d::Vec2 pos)
{
	auto medkit = Medkit::create();
	medkit->setPosition(pos);
	return medkit;
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

void Medkit::interact()
{
	auto runningScene = dynamic_cast<FightScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG));
	Player* player = nullptr;
	if (runningScene != nullptr)
	{
		player = dynamic_cast<Player*>(runningScene->getChildByTag(PLAYER_TAG));
	}
	if (player && player->getInteractItem() == this && player->isMedkitFull() == false)
	{
		player->getMedkitBagInstance()->push(this);
		retain();
		player->setInteractItem(nullptr);
		removeFromParent();
	}
}