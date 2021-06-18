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
	if (weaponUI->player->getPrimaryWeaponInstance() != nullptr)
	{
		std::string bulletInfo = std::to_string(weaponUI->player->getPrimaryWeaponInstance()->getCurrentMagazine());
		weaponUI->bulletInfo_ = cocos2d::Label::createWithTTF(bulletInfo, "fonts/IRANYekanBold.ttf", 24);
		weaponUI->bulletInfo_->setAnchorPoint(cocos2d::Point(0.5f, 0.5f));
		weaponUI->bulletInfo_->setPosition(cocos2d::Vec2(0, 20));
		weaponUI->addChild(weaponUI->bulletInfo_, 3);

		std::string bulletStockInfo = std::to_string(weaponUI->player->getBulletStock()[weaponUI->player->getPrimaryWeaponInstance()->TypeOfBullet]);
		weaponUI->bulletStockInfo = cocos2d::Label::createWithTTF(bulletStockInfo, "fonts/IRANYekanBold.ttf", 20);
		weaponUI->bulletStockInfo->setAnchorPoint(cocos2d::Point(0.5f, 0.5f));
		weaponUI->bulletStockInfo->setPosition(cocos2d::Vec2(40, 20));
		weaponUI->bulletStockInfo->setColor(cocos2d::Color3B::GRAY);
		weaponUI->addChild(weaponUI->bulletStockInfo, 3);

		weaponUI->weaponIcon_ = cocos2d::Sprite::create(weaponUI->player->getPrimaryWeaponInstance()->weaponFilename_);
		weaponUI->weaponIcon_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
		weaponUI->addChild(weaponUI->weaponIcon_, 3);
	}
	else
	{
		weaponUI->bulletInfo_ = cocos2d::Label::createWithTTF("0", "fonts/IRANYekanBold.ttf", 24);
		weaponUI->bulletInfo_->setAnchorPoint(cocos2d::Point(0.5f, 0.5f));
		weaponUI->bulletInfo_->setPosition(cocos2d::Vec2(0, 20));
		weaponUI->addChild(weaponUI->bulletInfo_, 3);

		weaponUI->bulletStockInfo = cocos2d::Label::createWithTTF("0", "fonts/IRANYekanBold.ttf", 20);
		weaponUI->bulletStockInfo->setAnchorPoint(cocos2d::Point(0.5f, 0.5f));
		weaponUI->bulletStockInfo->setPosition(cocos2d::Vec2(40, 20));
		weaponUI->bulletStockInfo->setColor(cocos2d::Color3B::GRAY);
		weaponUI->addChild(weaponUI->bulletStockInfo, 3);

		weaponUI->weaponIcon_ = cocos2d::Sprite::create("EmptyHanded.png");
		weaponUI->weaponIcon_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
		weaponUI->addChild(weaponUI->weaponIcon_, 3);
	}

	weaponUI->reloadWarning1_ = cocos2d::Sprite::create("ReloadWarning1.png");
	weaponUI->reloadWarning2_ = cocos2d::Sprite::create("ReloadWarning2.png");
	weaponUI->reloadWarning1_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	weaponUI->reloadWarning2_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	weaponUI->addChild(weaponUI->reloadWarning1_, 3);
	weaponUI->addChild(weaponUI->reloadWarning2_, 3);
	weaponUI->reloadWarning1_->setPosition(cocos2d::Vec2(0, 150));
	weaponUI->reloadWarning2_->setPosition(cocos2d::Vec2(0, 150));
	weaponUI->reloadWarning1_->setVisible(false);
	weaponUI->reloadWarning2_->setVisible(false);

	weaponUI->damageReact_ = cocos2d::Sprite::create("DamageReact.png");
	weaponUI->damageReact_->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_BOTTOM);
	weaponUI->damageReact_->setOpacity(0);
	weaponUI->damageReact_->setPosition(cocos2d::Vec2(0, -50));
	weaponUI->addChild(weaponUI->damageReact_, 4);

	weaponUI->weaponIcon_->setPosition(cocos2d::Vec2(0, 50));

	weaponUI->autorelease();
	weaponUI->schedule(CC_SCHEDULE_SELECTOR(WeaponUI::update), 0.1f);

	weaponUI->setCameraMask(2, true);

	return weaponUI;

}

void WeaponUI::damageReactAnime()
{
	damageReact_->setOpacity(255);
}

void WeaponUI::update(float dt)
{
	if (player->getPrimaryWeaponInstance() != nullptr)
	{
		if (player->getPrimaryWeaponInstance()->getCurrentMagazine() != bulletInMagazine_)
		{
			bulletInMagazine_ = player->getPrimaryWeaponInstance()->getCurrentMagazine();
			if (bulletInMagazine_ <= (player->getPrimaryWeaponInstance()->getMagazineSize() / 5) && bulletInMagazine_ > 0)
			{
				reloadWarning1_->setVisible(true);
				reloadWarning2_->setVisible(false);
			}
			else if (bulletInMagazine_ == 0)
			{
				reloadWarning1_->setVisible(false);
				reloadWarning2_->setVisible(true);
			}
			else
			{
				reloadWarning1_->setVisible(false);
				reloadWarning2_->setVisible(false);
			}
		}

		if (player->getBulletStock()[player->getPrimaryWeaponInstance()->TypeOfBullet] != bulletInStock_)
		{
			bulletInStock_ = player->getBulletStock()[player->getPrimaryWeaponInstance()->TypeOfBullet];
		}
	}
	else
	{
		bulletInMagazine_ = 0;
		bulletInStock_ = 0;
		reloadWarning1_->setVisible(false);
		reloadWarning2_->setVisible(false);
	}

	if (player->getPrimaryWeaponInstance() != nullptr)
	{
		weaponIcon_->setTexture(player->getPrimaryWeaponInstance()->weaponFilename_);
	}
	else
	{
		weaponIcon_->setTexture("EmptyHanded.png");
	}

	bulletInfo_->setString(std::to_string(bulletInMagazine_));
	bulletStockInfo->setString(std::to_string(bulletInStock_));

	if (player->receiveDamageMessage)
	{
		player->receiveDamageMessage = false;
		damageReactAnime();
	}
	if (damageReact_->getOpacity() > 0)
	{
		int tempOpacity = (int)((float)(damageReact_->getOpacity()) - (float)510 / 60);
		damageReact_->setOpacity(tempOpacity <= 0 ? 0 : tempOpacity);
	}
}