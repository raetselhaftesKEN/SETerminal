#include "cocos2d.h"
#include "HealthBar.h"

HealthBar* HealthBar::create(Character* character)
{
	auto healthBar = new(std::nothrow) HealthBar();
	if (healthBar == nullptr)
	{
		return nullptr;
	}

	healthBar->sprite_ = cocos2d::Sprite::create("health_bar.png");
	healthBar->sprite_->setAnchorPoint(cocos2d::Point(0.f, 1.f));
	
	if (healthBar && healthBar->sprite_)
	{
		healthBar->addChild(healthBar->sprite_, 1);
		healthBar->character_ = character;
		healthBar->curHealth_ = healthBar->character_->health_;

		healthBar->health_ = cocos2d::ProgressTimer::create(cocos2d::Sprite::create("health_max.png"));
		healthBar->health_->setAnchorPoint(cocos2d::Point(0.f, 1.f));
		healthBar->health_->setType(cocos2d::ProgressTimer::Type::BAR);
		healthBar->health_->setMidpoint(cocos2d::Point(0, 0.5f));
		healthBar->health_->setBarChangeRate(cocos2d::Point(1.f, 0));
		healthBar->health_->setPercentage(static_cast<float>(healthBar->character_->Character::health_) / 100 * 100);
		std::string healthInfo = std::to_string(healthBar->character_->Character::health_) + "/100";
		healthBar->healthInfo_ = cocos2d::Label::createWithTTF(healthInfo, "fonts/IRANYekanBold.ttf", 18.f);
		healthBar->healthInfo_->setAnchorPoint(cocos2d::Point(0.f, 1.f));
		healthBar->healthInfo_->setPosition(cocos2d::Vec2(10, -50));
		healthBar->addChild(healthBar->health_, 2);
		healthBar->addChild(healthBar->healthInfo_, 3);
		healthBar->autorelease();

		healthBar->schedule(CC_SCHEDULE_SELECTOR(HealthBar::update), 0.1f);
		return healthBar;
	}

	return nullptr;
}

void HealthBar::update(float dt)
{
	if (character_->health_ != curHealth_)
	{
		curHealth_ = character_->health_;
		health_->setPercentage(static_cast<float>(character_->Character::health_) / 100 * 100);
		std::string healthInfo = std::to_string(character_->Character::health_) + "/100";
		healthInfo_->setString(healthInfo);
	}
}