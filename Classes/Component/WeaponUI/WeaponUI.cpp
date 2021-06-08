#include "cocos2d.h"
#include "WeaponUI.h"

WeaponUI* WeaponUI::create(Player* player)
{
	auto weaponUI = new(std::nothrow) WeaponUI();
	if (weaponUI == nullptr || player == nullptr)
	{
		return nullptr;
	}
	weaponUI->player_ = player;

	std::string bulletInfo;

	if (weaponUI->player_->getPrimaryWeaponInstance() != nullptr)
	{
		bulletInfo = std::to_string(weaponUI->player_->getPrimaryWeaponInstance()->getCurrentMagazine());
	}
	else
	{
		bulletInfo = "¡Þ";
	}
	weaponUI->bulletInfo_ = cocos2d::Label::createWithTTF(bulletInfo, "fonts/IRANYekanBold.ttf", 28);
	weaponUI->bulletInfo_->setAnchorPoint(cocos2d::Point(0.5f, 0.f));
	weaponUI->bulletInfo_->setPosition(cocos2d::Vec2(0, 0));
	weaponUI->addChild(weaponUI->bulletInfo_, 3);
	weaponUI->setPosition(cocos2d::Vec2((cocos2d::Director::getInstance()->getWinSize()).width / 2, 40));
	weaponUI->autorelease();
	weaponUI->schedule(CC_SCHEDULE_SELECTOR(WeaponUI::update), 0.1f);

	weaponUI->setCameraMask(2, true);

	return weaponUI;

}

void WeaponUI::update(float dt)
{
	auto weapon = player_->getPrimaryWeaponInstance();
	if (weapon != nullptr)
	{
		if (weapon->getCurrentMagazine() != bulletInMagazine_)
		{
			bulletInMagazine_ = weapon->getCurrentMagazine();
			bulletInfo_->setString(std::to_string(bulletInMagazine_));
		}
	}
	else
	{
		bulletInfo_->setString(std::string("no weapon"));
	}
}