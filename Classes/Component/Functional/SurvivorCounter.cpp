#include "cocos2d.h"
#include "SurvivorCounter.h"
#include "Scene/FightScene/FightScene.h"

SurvivorCounter* SurvivorCounter::create()
{
	auto survivorCounter = new(std::nothrow) SurvivorCounter();
	if (survivorCounter == nullptr)
	{
		return nullptr;
	}

	auto sizeOfWin = cocos2d::Director::getInstance()->getWinSize();

	survivorCounter->surviverInfoBG_ = cocos2d::Sprite::create("UI/SEUISur.png");
	survivorCounter->surviverInfoBG_->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);


	if (survivorCounter && survivorCounter->surviverInfoBG_)
	{
		std::string countInfo = std::to_string((int)(survivorCounter->RemainingSurvivorCount));
		survivorCounter->surviverInfo_ = cocos2d::Label::createWithTTF(countInfo, "fonts/IRANYekanBold.ttf", 72);
		survivorCounter->surviverInfo_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_RIGHT);
		survivorCounter->surviverInfo_->setPosition(cocos2d::Vec2(300, -50));
		survivorCounter->addChild(survivorCounter->surviverInfo_, 2);
		survivorCounter->addChild(survivorCounter->surviverInfoBG_, 1);

		survivorCounter->autorelease();

		survivorCounter->setCameraMask(2, true);

		survivorCounter->schedule(CC_SCHEDULE_SELECTOR(SurvivorCounter::update), 0.1f);
		return survivorCounter;
	}

	return nullptr;
}

void SurvivorCounter::update(float dt)
{
	auto CurScene = dynamic_cast<FightScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG));
	if (CurScene && RemainingSurvivorCount != CurScene->RemainingSurvivor)
	{
		RemainingSurvivorCount = CurScene->RemainingSurvivor;
		surviverInfo_->setString(std::to_string(RemainingSurvivorCount));
	}
}