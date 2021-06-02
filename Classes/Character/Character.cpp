/**
* @file Character.cpp
*/

#include "cocos2d.h"
#include "Character.h"
<<<<<<< Updated upstream
=======
#include <string>
using namespace std::string_literals;
>>>>>>> Stashed changes

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

<<<<<<< Updated upstream
cocos2d::Animate* Character::createAnimate(const char* animateName, int width, int height, int frames)
=======
void Character::receiveDamage(int damage)
{
	int realDamage = static_cast<int>(damage * (1 - shield_));
	if (realDamage >= health_)
	{
		die();
	}
	else
	{
		health_ -= realDamage;
	}
}

void Character::updateMoveAnimate()
{
	if (statusChanged_)
	{
		sprite_->stopAction(animateBeingPlayed_);
		switch (curFacingStatus_)
		{
			case FacingStatus::up:
				animateBeingPlayed_ = (curWalkingStatus_ == WalkingStatus::walk ? walkUp_ : idleUp_);
				break;
			case FacingStatus::down:
				animateBeingPlayed_ = (curWalkingStatus_ == WalkingStatus::walk ? walkDown_ : idleDown_);
				break;
			case FacingStatus::left:
				animateBeingPlayed_ = (curWalkingStatus_ == WalkingStatus::walk ? walkLeft_ : idleLeft_);
				break;
			case FacingStatus::right:
				animateBeingPlayed_ = (curWalkingStatus_ == WalkingStatus::walk ? walkRight_ : idleRight_);
				break;
			default:
				break;
		}
		sprite_->runAction(animateBeingPlayed_);
	}
}

cocos2d::Animate* Character::createAnimate(const char* animateName, cocos2d::Size size, int frames)
>>>>>>> Stashed changes
{
	cocos2d::SpriteFrame* frame = nullptr;
	cocos2d::Vector<cocos2d::SpriteFrame*> frameVector;
	for (int i = 1; i <= frames; i++)
	{
<<<<<<< Updated upstream
		frame = cocos2d::SpriteFrame::create(cocos2d::StringUtils::format("%s%d.png", animateName, i), cocos2d::Rect(0, 0, width, height));
=======
		frame = cocos2d::SpriteFrame::create(cocos2d::StringUtils::format("%s%d.png", animateName, i), cocos2d::Rect(0, 0, size.width, size.height));
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
=======
}

void Character::bindAnimate(const std::string& characterName)
{
	walkRight_ = createAnimate((characterName + "/"s +"walk_right/walk_right"s).c_str(), sprite_->getContentSize());
	walkLeft_ = createAnimate((characterName + "/"s + "walk_left/walk_left"s).c_str(), sprite_->getContentSize());
	walkUp_ = createAnimate((characterName + "/"s + "walk_up/walk_up"s).c_str(), sprite_->getContentSize());
	walkDown_ = createAnimate((characterName + "/"s + "walk_down/walk_down"s).c_str(), sprite_->getContentSize());
	idleUp_ = createAnimate((characterName + "/"s + "idle_up/idle_up"s).c_str(), sprite_->getContentSize(), 1);
	idleDown_ = createAnimate((characterName + "/"s + "idle_down/idle_down"s).c_str(), sprite_->getContentSize(), 1);
	idleLeft_ = createAnimate((characterName + "/"s + "idle_left/idle_left"s).c_str(), sprite_->getContentSize(), 1);
	idleRight_ = createAnimate((characterName + "/"s + "idle_right/idle_right"s).c_str(), sprite_->getContentSize(), 1);
>>>>>>> Stashed changes
}