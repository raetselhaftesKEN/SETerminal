#include "cocos2d.h"
#include "Const/Const.h"
#include "Scene/FightScene/FightScene.h"
#include "SETimer.h"

SETimer* SETimer::timerInstance_ = nullptr;

SETimer* SETimer::create()
{
	auto timer = new(std::nothrow) SETimer();
	if (timer == nullptr)
	{
		return nullptr;
	}

	auto sizeOfWin = cocos2d::Director::getInstance()->getWinSize();

	timer->timeInfoBG_ = cocos2d::Sprite::create("UI/SEUITime.png");
	timer->timeInfoBG_->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_LEFT);


	if (timer && timer->timeInfoBG_)
	{
		std::string timeInfo = std::to_string((int)(timer->TimePassed));
		timer->timeInfo_ = cocos2d::Label::createWithTTF(timeInfo, "fonts/IRANYekanBold.ttf", 72);
		timer->timeInfo_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_RIGHT);
		timer->timeInfo_->setPosition(cocos2d::Vec2(380, -50));
		timer->addChild(timer->timeInfo_, 2);
		timer->addChild(timer->timeInfoBG_, 1);

		timer->autorelease();

		timer->setCameraMask(2, true);

		timer->schedule(CC_SCHEDULE_SELECTOR(SETimer::update), 1);
		SETimer::timerInstance_ = timer;
		return timer;
	}

	return nullptr;
}

float SETimer::getCurrentTime()
{
	return SETimer::timerInstance_->TimePassed;
}

void SETimer::update(float dt)
{
	TimePassed++;
	timeInfo_->setString(std::to_string(static_cast<int>(TimePassed)));

}