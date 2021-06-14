#include "cocos2d.h"
#include "HealthBar.h"

HealthBar* HealthBar::create(Character* character)
{
	auto healthBar = new(std::nothrow) HealthBar();
	if (healthBar == nullptr)
	{
		return nullptr;
	}

	auto sizeOfWin = cocos2d::Director::getInstance()->getWinSize();

	healthBar->sprite_ = cocos2d::Sprite::create("UI/UI-BG.png");
	healthBar->sprite_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	

	if (healthBar && healthBar->sprite_)
	{
		healthBar->addChild(healthBar->sprite_, 1);

		healthBar->character_ = character;
		healthBar->curHealth_ = healthBar->character_->health_;

		healthBar->health_ = cocos2d::ProgressTimer::create(cocos2d::Sprite::create("UI/Health.png"));
		healthBar->health_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
		healthBar->health_->setType(cocos2d::ProgressTimer::Type::BAR);
		healthBar->health_->setMidpoint(cocos2d::Point(0, 0.5f));
		healthBar->health_->setBarChangeRate(cocos2d::Point(1.f, 0));
		healthBar->health_->setPercentage(static_cast<float>(healthBar->character_->Character::health_) / 100 * 100);

		healthBar->shield_ = cocos2d::ProgressTimer::create(cocos2d::Sprite::create("UI/Shield.png"));
		healthBar->shield_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
		healthBar->shield_->setType(cocos2d::ProgressTimer::Type::BAR);
		healthBar->shield_->setMidpoint(cocos2d::Point(0, 0.5f));
		healthBar->shield_->setBarChangeRate(cocos2d::Point(1.f, 0));
		healthBar->shield_->setPercentage(static_cast<float>(healthBar->character_->Character::shield_) / 100 * 100);

		healthBar->healthR_ = cocos2d::ProgressTimer::create(cocos2d::Sprite::create("UI/Health-R.png"));
		healthBar->healthR_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
		healthBar->healthR_->setType(cocos2d::ProgressTimer::Type::BAR);
		healthBar->healthR_->setMidpoint(cocos2d::Point(0, 0.5f));
		healthBar->healthR_->setBarChangeRate(cocos2d::Point(1.f, 0));
		healthBar->healthR_->setPercentage(static_cast<float>(healthBar->character_->Character::health_) / 100 * 100);

		healthBar->shieldR_ = cocos2d::ProgressTimer::create(cocos2d::Sprite::create("UI/Shield-R.png"));
		healthBar->shieldR_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
		healthBar->shieldR_->setType(cocos2d::ProgressTimer::Type::BAR);
		healthBar->shieldR_->setMidpoint(cocos2d::Point(0, 0.5f));
		healthBar->shieldR_->setBarChangeRate(cocos2d::Point(1.f, 0));
		healthBar->shieldR_->setPercentage(static_cast<float>(healthBar->character_->Character::shield_) / 100 * 100);

		healthBar->addChild(healthBar->health_, 3);
		healthBar->addChild(healthBar->shield_, 3);
		healthBar->addChild(healthBar->healthR_, 2);
		healthBar->addChild(healthBar->shieldR_, 2);
		healthBar->autorelease();

		healthBar->setCameraMask(2, true);

		healthBar->schedule(CC_SCHEDULE_SELECTOR(HealthBar::update), (1/60));
		return healthBar;
	}

	return nullptr;
}

void HealthBar::update(float dt)
{
	if (character_->health_ != curHealth_)
	{
		curHealth_ = character_->health_;
		health_->setPercentage(static_cast<float>(character_->Character::health_) / static_cast<float>(character_->Character::maxHealth_) * 100);
	}

	if (character_->shield_ != surShield_)
	{
		surShield_ = character_->shield_;
		shield_->setPercentage(static_cast<float>(character_->Character::shield_) / static_cast<float>(character_->Character::maxShield_) * 100);
	}

	if (healthR_->getPercentage() > health_->getPercentage())
	{
		healthR_->setPercentage(healthR_->getPercentage() - rBarShrinkRate / 60 * rBarShrinkBoost1);
		rBarShrinkBoost1 *= 1.1;
	}
	else
	{
		healthR_->setPercentage(health_->getPercentage());
		rBarShrinkBoost1 = 1;
	}

	if (shieldR_->getPercentage() > shield_->getPercentage())
	{
		shieldR_->setPercentage(shieldR_->getPercentage() - rBarShrinkRate / 60 * rBarShrinkBoost2);
		rBarShrinkBoost2 *= 1.1;
	}
	else
	{
		shieldR_->setPercentage(shield_->getPercentage());
		rBarShrinkBoost2 = 1;
	}
}