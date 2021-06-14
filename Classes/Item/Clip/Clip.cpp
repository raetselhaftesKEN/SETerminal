/**
* @file Clip.cpp
*/

#include "Item/Clip/Clip.h"
#include "Character/Player.h"
#include "Scene/FightScene/FightScene.h"

Clip* Clip::create(bulletType_ typeTag)
{
	auto clip = new(std::nothrow) Clip();
	if (clip == nullptr)
	{
		return nullptr;
	}
	clip->Item::sprite_ = cocos2d::Sprite::create(bulletName[typeTag] + ".png");

	if (clip && clip->Item::sprite_)
	{
		clip->retain();
		clip->setTag(ITEM_TAG);
		clip->type_ = typeTag;
		clip->bulletNum_ = CLIP_BULLET_NUM;
		clip->Item::isHeld_ = false;
		clip->addChild(clip->Item::sprite_);
		clip->Item::itemInfo_ = cocos2d::Label::createWithTTF(bulletName[typeTag], "fonts/IRANYekanBold.ttf", 36);
		clip->addChild(clip->Item::itemInfo_, 2);
		clip->Item::itemInfo_->setTextColor(cocos2d::Color4B(0, 0, 0, 255));
		clip->Item::itemInfo_->setVisible(false);
		clip->bindPhysicsBody();

		return clip;
	}
	return nullptr;
}

bool Clip::bindPhysicsBody()
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

bulletType_ Clip::getType()
{
	return type_;
}

void Clip::interact()
{
	auto runningScene = dynamic_cast<FightScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG));
	Player* player = nullptr;
	if (runningScene != nullptr)
	{
		player = dynamic_cast<Player*>(runningScene->getChildByTag(PLAYER_TAG));
	}
	if (player && player->getInteractItem() == this)
	{
		player->getBulletStock()[type_] += bulletNum_;
		player->setInteractItem(nullptr);
		this->removeFromParentAndCleanup(true);
	}
}