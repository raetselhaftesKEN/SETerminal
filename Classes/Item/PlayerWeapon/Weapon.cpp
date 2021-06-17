/**
* @file Weapon.cpp
*/

#include "cocos2d.h"
#include "AudioEngine.h"
#include "Weapon.h"
#include "Character/Player.h"
#include "../Bullet/Bullet.h"
#include "Const/Const.h"
#include <string>
using namespace std::string_literals;

int Weapon::getAccuracy()
{
	return Accuracy;
}

void Weapon::setAccuracy(int accuracy)
{
	if (accuracy > 0)
		Accuracy = accuracy;
}

float Weapon::getRecoil()
{
	return Recoil;
}

void Weapon::setRecoil(float recoil)
{
	if (recoil >= 0)
		Recoil = recoil;
}

float Weapon::getRecoiRecoverl()
{
	return RecoilRecover;
}
void Weapon::setRecoilRecover(float recoilRecover)
{
	if (recoilRecover >= 0)
		RecoilRecover = recoilRecover;
}

float Weapon::getReloadTime()
{
	return ReloadTime;
}
void Weapon::setReloadTime(float reloadTime)
{
	if (reloadTime >= 0)
		ReloadTime = reloadTime;
}

int Weapon::getMagazineSize()
{
	return MagazineSize;
}
void Weapon::setMagazineSize(int magSize)
{
	if (magSize > 0)
		MagazineSize = magSize;
}

Weapon* Weapon::create(const std::string& filename)
{
	auto weapon = new(std::nothrow) Weapon();
	if (weapon == nullptr)
	{
		return nullptr;
	}
	weapon->bindPictureSprite(cocos2d::Sprite::create(filename));
	weapon->weaponFilename_ = filename;

	if (weapon && weapon->sprite_)
	{
		weapon->autorelease();
		weapon->bindPhysicsBody();
		weapon->setTag(ITEM_TAG);
		weapon->bulletFilename_ = "DefaultBullet-2.png";
		weapon->aimPointFilename_ = "DefaultAimPoint.png";

		weapon->MyAimPoint = PlayerAimPoint::create(weapon->aimPointFilename_);
		weapon->ReloadAimPoint = PlayerAimPoint::create("Reloading.png");
		weapon->MyAimPoint->retain();
		weapon->ReloadAimPoint->retain();



		weapon->MyAimPoint->setVisible(true);
		weapon->ReloadAimPoint->setVisible(false);
		weapon->ActiveAimPoint = weapon->MyAimPoint;

		return weapon;
	}

	return nullptr;
}

Weapon* Weapon::create(weaponType_ type)
{
	auto weapon = new(std::nothrow) Weapon();
	if (weapon == nullptr)
	{
		return nullptr;
	}

	std::string filename;
	switch (type)
	{
		case weaponType_::AK47:
			filename = "Weapon/AK47.png";
			break;
		case weaponType_::AKM:
			filename = "Weapon/AKM.png";
			break;
		case weaponType_::FAL:
			filename = "Weapon/FAL.png";
			break;
		case weaponType_::M4:
			filename = "Weapon/M4.png";
			break;
		case weaponType_::MP5:
			filename = "Weapon/MP5.png";
			break;
		case weaponType_::SVD:
			filename = "Weapon/SVD.png";
			break;
		default:
			return nullptr;
			break;
	}
	weapon->weaponFilename_ = filename;
	weapon->bindPictureSprite(cocos2d::Sprite::create(filename));
	weapon->sprite_->setScale(0.3f);

	if (weapon && weapon->sprite_)
	{
		weapon->autorelease();
		weapon->bindPhysicsBody();
		weapon->setTag(ITEM_TAG);
		switch (type)
		{
			case weaponType_::AK47:
				weapon->bulletFilename_ = "Bullet/Bullet1.png";
				weapon->aimPointFilename_ = "AimPoint/AimPoint1.png";
				weapon->TypeOfBullet = bulletType_::type762;
				weapon->BulletSpeed = 2000;
				weapon->ShootingSpeed = 10;
				weapon->BulletDamage = 12;
				weapon->Accuracy = 95;
				weapon->Recoil = 15;
				weapon->RecoilRecover = 60;
				weapon->MagazineSize = 30;
				weapon->ReloadTime = 2.f;
				weapon->Reset();
				break;
			case weaponType_::AKM:
				weapon->bulletFilename_ = "Bullet/Bullet4.png";
				weapon->aimPointFilename_ = "AimPoint/AimPoint1.png";
				weapon->TypeOfBullet = bulletType_::type762;
				weapon->BulletSpeed = 1800;
				weapon->ShootingSpeed = 8;
				weapon->BulletDamage = 15;
				weapon->Accuracy = 92;
				weapon->Recoil = 18;
				weapon->RecoilRecover = 50;
				weapon->MagazineSize = 35;
				weapon->ReloadTime = 2.f;
				weapon->Reset();
				break;
			case weaponType_::FAL:
				weapon->bulletFilename_ = "Bullet/Bullet3.png";
				weapon->aimPointFilename_ = "AimPoint/AimPoint3.png";
				weapon->TypeOfBullet = bulletType_::type556;
				weapon->BulletSpeed = 3000;
				weapon->ShootingSpeed = 2;
				weapon->BulletDamage = 20;
				weapon->Accuracy = 98;
				weapon->Recoil = 30;
				weapon->RecoilRecover = 60;
				weapon->MagazineSize = 20;
				weapon->ReloadTime = 3.f;
				weapon->Reset();
				break;
			case weaponType_::M4:
				weapon->bulletFilename_ = "Bullet/Bullet2.png";
				weapon->aimPointFilename_ = "AimPoint/AimPoint1.png";
				weapon->TypeOfBullet = bulletType_::type556;
				weapon->BulletSpeed = 2000;
				weapon->ShootingSpeed = 15;
				weapon->BulletDamage = 10;
				weapon->Accuracy = 97;
				weapon->Recoil = 8;
				weapon->RecoilRecover = 80;
				weapon->MagazineSize = 40;
				weapon->ReloadTime = 3.f;
				weapon->Reset();
				break;
			case weaponType_::MP5:
				weapon->bulletFilename_ = "Bullet/Bullet5.png";
				weapon->aimPointFilename_ = "AimPoint/AimPoint2.png";
				weapon->TypeOfBullet = bulletType_::type9mm;
				weapon->BulletSpeed = 1200;
				weapon->ShootingSpeed = 15;
				weapon->BulletDamage = 8;
				weapon->Accuracy = 90;
				weapon->Recoil = 5;
				weapon->RecoilRecover = 100;
				weapon->MagazineSize = 45;
				weapon->ReloadTime = 2.5f;
				weapon->Reset();
				break;
			case weaponType_::SVD:
				weapon->bulletFilename_ = "Bullet/Bullet4.png";
				weapon->aimPointFilename_ = "AimPoint/AimPoint3.png";
				weapon->TypeOfBullet = bulletType_::type9mm;
				weapon->BulletSpeed = 5000;
				weapon->ShootingSpeed = 1;
				weapon->BulletDamage = 40;
				weapon->Accuracy = 99;
				weapon->Recoil = 60;
				weapon->RecoilRecover = 60;
				weapon->MagazineSize = 5;
				weapon->ReloadTime = 2.f;
				weapon->Reset();
				break;
			default:
				return nullptr;
				break;
		}


		weapon->MyAimPoint = PlayerAimPoint::create(weapon->aimPointFilename_);
		weapon->ReloadAimPoint = PlayerAimPoint::create("Reloading.png");
		weapon->MyAimPoint->retain();
		weapon->ReloadAimPoint->retain();

		weapon->MyAimPoint->setVisible(true);
		weapon->ReloadAimPoint->setVisible(false);
		weapon->ActiveAimPoint = weapon->MyAimPoint;
		weapon->retain();

		return weapon;
	}

	return nullptr;
}

bool Weapon::bindPhysicsBody()
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

void Weapon::Active(bool ActivateStatus)
{
	setVisible(ActivateStatus);
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
					shootMusicID_ = cocos2d::AudioEngine::play2d("Audio/impacter.mp3", false, .5f);
					auto bullet = Bullet::create(bulletFilename_);
					//				bullet->setScale(0.3f, 0.3f);
					bullet->setRotation(getRotation());
					bullet->setPosition(pos);
					bullet->setBulletAtk(BulletDamage);
					cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG)->addChild(bullet, 1);
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
			//Reload();
		}
	}

}

int Weapon::getCurrentMagazine()
{
	return CurrentMagazine;
}

void Weapon::PlayerReload(std::vector<int>& BulletStock)
{
	if (CurrentMagazine != MagazineSize)
	{
		MyAimPoint->setVisible(false);
		ReloadAimPoint->setVisible(true);
		ActiveAimPoint = ReloadAimPoint;
		Reload(BulletStock);
	}
}

void Weapon::Reload()
{
	if (CurrentMagazine != MagazineSize)
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
}

void Weapon::Reload(std::vector<int>& BulletStock)
{
	if (CurrentMagazine != MagazineSize)
	{
		BulletStock[TypeOfBullet] += CurrentMagazine;
		CurrentMagazine = 0;
		CanShoot = false;
		if (BulletStock[TypeOfBullet] > 0)
		{
			auto reload = cocos2d::CallFunc::create([&]()
				{
					MyAimPoint->setVisible(true);
					ReloadAimPoint->setVisible(false);
					ActiveAimPoint = MyAimPoint;
					CanShoot = true;
					if (BulletStock[TypeOfBullet] >= MagazineSize)
					{
						CurrentMagazine = MagazineSize;
						BulletStock[TypeOfBullet] -= MagazineSize;
					}
					else
					{
						CurrentMagazine = BulletStock[TypeOfBullet];
						BulletStock[TypeOfBullet] = 0;
					}

				});
			auto delay = cocos2d::DelayTime::create(ReloadTime);
			this->runAction(cocos2d::Sequence::create(delay, reload, nullptr));
		}
	}
}

void Weapon::ReloadingStatusReset()
{
	if (CurrentMagazine > 0)
	{
		MyAimPoint->setVisible(true);
		ReloadAimPoint->setVisible(false);
		CanShoot = true;
	}
	else
	{
		MyAimPoint->setVisible(false);
		ReloadAimPoint->setVisible(true);
		CanShoot = false;
		Reload();
	}
}

void Weapon::Reset()
{
	CurrentMagazine = MagazineSize;
}

void Weapon::RecoverRecoil(float Boost)
{
	MyAimPoint->RecoverRecoil(Boost * RecoilRecover / 60);
	ReloadAimPoint->RecoverRecoil(Boost * RecoilRecover / 60);
}

void Weapon::interact()
{
	auto player = dynamic_cast<Player*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG)->getChildByTag(PLAYER_TAG));
	if (player && player->getInteractItem() == this)
	{
		if (player->getPrimaryWeaponInstance() && player->getSecondaryWeaponInstance())
		{
			player->abandonPrimaryWeapon();
			player->setSecondaryWeaponInstance(this);
			this->Active(false);
		}
		else if (player->getPrimaryWeaponInstance() != nullptr && player->getSecondaryWeaponInstance() == nullptr)
		{
			player->setSecondaryWeaponInstance(this);
			this->Active(false);
		}
		else
		{
			player->setPrimaryWeaponInstance(this);
			this->Active(true);
		}
	}

	this->retain();
	this->removeFromParent();
	this->setPosition(cocos2d::Vec2::ZERO);
	this->Item::pickUp();

	player->addChild(this);
	player->setInteractItem(nullptr);
	player->getAimPointInstance();
	player->getPrimaryWeaponInstance()->ReloadingStatusReset();
}

PlayerAimPoint* Weapon::getMyAimPoint()
{
	return MyAimPoint;
}
void Weapon::setMyAimPoint(PlayerAimPoint* aimPoint)
{
	MyAimPoint = aimPoint;
}

PlayerAimPoint* Weapon::getReloadAimPoint()
{
	return ReloadAimPoint;
}

void Weapon::setReloadAimPoint(PlayerAimPoint* aimPoint)
{
	ReloadAimPoint = aimPoint;
}