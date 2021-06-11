/**
* @file Weapon.cpp
*/

#include "cocos2d.h"
#include "Weapon.h"
#include "././Scene/HelloWorldScene.h"
#include "../Bullet/Bullet.h"

Weapon* Weapon::create(const std::string& filename)
{
	auto weapon = new(std::nothrow) Weapon();
	if (weapon == nullptr)
	{
		return nullptr;
	}
	weapon->bindPictureSprite(cocos2d::Sprite::create(filename));

	if (weapon && weapon->sprite_)
	{
		weapon->autorelease();
		weapon->bulletFilename_ = "DefaultBullet-2.png";
		weapon->aimPointFilename_ = "DefaultAimPoint.png";

		weapon->MyAimPoint = PlayerAimPoint::create(weapon->aimPointFilename_);
		weapon->ReloadAimPoint = PlayerAimPoint::create("Reloading.png");
		weapon->MyAimPoint->retain();
		weapon->ReloadAimPoint->retain();

//		weapon->MyAimPoint->setScale(3, 3);
//		weapon->ReloadAimPoint->setScale(3, 3);

		weapon->MyAimPoint->setVisible(true);
		weapon->ReloadAimPoint->setVisible(false);
		weapon->ActiveAimPoint = weapon->MyAimPoint;

		return weapon;
	}

	return nullptr;
}

void Weapon::Active(bool ActivateStatus)
{
	setVisible(ActivateStatus);
	MyAimPoint->setVisible(ActivateStatus);
}

void Weapon::Attack(cocos2d::Vec2 pos, cocos2d::Vec2 dir)//暂时先通过这个方式来生成子弹  如果有特殊子弹的武器，需要重载Attack
{
	if (CanShoot)
	{
		if (CurrentMagazine > 0)
		{
			CanShoot = false;
			auto shoot = cocos2d::CallFunc::create([=]()
				{
					auto bullet = Bullet::create(bulletFilename_);
					//				bullet->setScale(0.3f, 0.3f);
					bullet->setRotation(getRotation());
					bullet->setPosition(pos);
					cocos2d::Director::getInstance()->getRunningScene()->addChild(bullet, 1);
					//为子弹实例绑定播放发射的飞行动画
					cocos2d::Vec2 Spread;
					Spread.x = dir.y / dir.x;
					Spread.y = -1;
					Spread.normalize();
					Spread *= ((float)(rand() % (100 - Accuracy)) - (float)(100 - Accuracy) / 2) / 100.0f;
					bullet->shoot(dir + Spread, BulletSpeed);//////////////////
					CurrentMagazine--;
				});
			auto recover = cocos2d::CallFunc::create([=]()
				{
					CanShoot = true;
				});
			auto delay = cocos2d::DelayTime::create(1 / ShootingSpeed);
			this->runAction(cocos2d::Sequence::create(shoot, delay, recover, nullptr));

			ReloadAimPoint->RecoilStatus += Recoil;
			MyAimPoint->RecoilStatus += Recoil;//后坐力叠加
			if (ReloadAimPoint->RecoilStatus > 250)
			{
				ReloadAimPoint->RecoilStatus = 250;
			}
			if (MyAimPoint->RecoilStatus > 250)
			{
				MyAimPoint->RecoilStatus = 250;
			}
		}
		else
		{
			CanShoot = false;
			MyAimPoint->setVisible(false);
			ReloadAimPoint->setVisible(true);

			ActiveAimPoint = ReloadAimPoint;
						Reload();
		}
	}

}

void Weapon::PlayerReload()
{
	MyAimPoint->setVisible(false);
	ReloadAimPoint->setVisible(true);
	ActiveAimPoint = ReloadAimPoint;
	Reload();
}

void Weapon::Reload()
{
	CurrentMagazine = 0;
	CanShoot = false;
	auto reload = cocos2d::CallFunc::create([=]()
		{
			MyAimPoint->setVisible(true);
			ReloadAimPoint->setVisible(false);
			ActiveAimPoint = MyAimPoint;
			CanShoot = true;
			CurrentMagazine = MagazineSize;
		});
	auto delay = cocos2d::DelayTime::create(ReloadTime);
	this->runAction(cocos2d::Sequence::create(delay, reload, nullptr));
}

void Weapon::RecoverRecoil(float Boost)
{
	MyAimPoint->RecoverRecoil(Boost * RecoilRecover / 60);
	ReloadAimPoint->RecoverRecoil(Boost * RecoilRecover / 60);
}
