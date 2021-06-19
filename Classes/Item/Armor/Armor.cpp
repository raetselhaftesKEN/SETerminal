/**
* @file Armor.cpp
*/

#include "Item/Armor/Armor.h"
#include "Character/Player.h"
#include "Scene/FightScene/FightScene.h"

Armor* Armor::create(int amount)
{
	auto armor = new(std::nothrow) Armor();
	if (armor == nullptr)
	{
		return nullptr;
	}
	armor->Item::sprite_ = cocos2d::Sprite::create("Armor.png");

	if (armor && armor->Item::sprite_)
	{
		armor->retain();
		armor->setTag(ITEM_TAG);
		armor->armorAmount = amount;
		armor->Item::isHeld_ = false;
		armor->sprite_->setScale(0.2f);
		armor->addChild(armor->Item::sprite_);
		armor->Item::itemInfo_ = cocos2d::Label::createWithTTF("Armor", "fonts/IRANYekanBold.ttf", 18);
		armor->Item::itemInfo_->setPosition(cocos2d::Vec2(0, 18));
		armor->addChild(armor->Item::itemInfo_, 2);
		armor->Item::itemInfo_->setTextColor(cocos2d::Color4B(0, 0, 0, 255));
		armor->Item::itemInfo_->setVisible(false);
		armor->bindPhysicsBody();
		return armor;
	}
	return nullptr;
}

bool Armor::bindPhysicsBody()
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

void Armor::interact()
{
	auto runningScene = dynamic_cast<FightScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG));
	Player* player = nullptr;
	if (runningScene != nullptr)
	{
		player = dynamic_cast<Player*>(runningScene->getChildByTag(PLAYER_TAG));
	}
	if (player && player->getInteractItem() == this)
	{
		player->shield_ += armorAmount;
		if (player->shield_ > player->maxShield_)
		{
			player->shield_ = player->maxShield_;
		}
		this->removeFromParentAndCleanup(true);
	}
}