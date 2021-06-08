#include "cocos2d.h"
#include "WeaponUI.h"

WeaponUI* WeaponUI::create(Player* player_)
{
	auto weaponUI = new(std::nothrow) WeaponUI();
	if (weaponUI == nullptr)
	{
		return nullptr;
	}

	weaponUI->player = player_;
	std::string bulletInfo = std::to_string(weaponUI->player->primaryWeapon_->CurrentMagazine);
	weaponUI->bulletInfo_ = cocos2d::Label::createWithTTF(bulletInfo, "fonts/IRANYekanBold.ttf", 28);
	weaponUI->bulletInfo_->setAnchorPoint(cocos2d::Point(0.5f, 0.f));
	weaponUI->bulletInfo_->setPosition(cocos2d::Vec2(0, 0));
	weaponUI->addChild(weaponUI->bulletInfo_, 3);
	weaponUI->autorelease();
	weaponUI->schedule(CC_SCHEDULE_SELECTOR(WeaponUI::update), 0.1f);

	weaponUI->setCameraMask(2, true);

	return weaponUI;

}

void WeaponUI::update(float dt)
{
	if (player->primaryWeapon_->CurrentMagazine != bulletInMagazine_)
	{
		bulletInMagazine_ = player->primaryWeapon_->CurrentMagazine;
		bulletInfo_->setString(std::to_string(bulletInMagazine_));
	}
}