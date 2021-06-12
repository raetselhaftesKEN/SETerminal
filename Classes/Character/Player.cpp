/**
* @file Player.cpp
*/

#include "Player.h"
#include "Item/Medkit/Medkit.h"
#include "./Item/PlayerWeapon/Weapon.h"
#include "Scene/GameOverScene.h"
#include "Const/Const.h"

Player* Player::create(const std::string& filename)
{
	auto player = new(std::nothrow) Player();
	if (!player)
	{
		return nullptr;
	}
	player->bindPictureSprite(cocos2d::Sprite::create(filename));

	if (player && player->sprite_)
	{
		//设置角色初始位置
		player->setPosition(cocos2d::Vec2(50, 50));
		//标记角色
		player->setTag(PLAYER_TAG);

		player->bindCharacterAnimate("MIKU");

		//初始化角色武器和弹药
		player->primaryWeapon_ = Weapon::create("MP5.png");
		player->secondaryWeapon_ = Weapon::create("AK47.png");
		player->primaryWeapon_->setScale(0.3f, 0.3f);
		player->secondaryWeapon_->setScale(0.3f, 0.3f);

		player->bulletFilename_ = "dart.png";
		player->addChild(player->primaryWeapon_);
		player->addChild(player->secondaryWeapon_);

		player->primaryWeapon_->Active(true);
		player->secondaryWeapon_->Active(false);

		player->primaryWeapon_->setVisible(true);			//默认显示主武器，不显示副武器
		player->secondaryWeapon_->setVisible(false);
		player->secondaryWeapon_->TypeOfBullet = bulletType_::type556;
		player->moveSpeed_ = PLAYER_DEFAULT_MOVE_SPEED;
		player->health_ = PLAYER_MAX_HEALTH;
		player->maxHealth_ = PLAYER_MAX_HEALTH;
		player->shield_ = PLAYER_DEFAULT_SHIELD;
		player->maxShield_ = PLAYER_DEFAULT_MAX_SHIELD;
		player->shieldProtectionRate = PLAYER_DEFAULT_SHIELD_PROTECTION;
		player->medkitMaxNum_ = MEDKIT_MAX_NUM;

		player->bulletStock_.push_back(90);
		player->bulletStock_.push_back(90);
		player->bulletStock_.push_back(90);

		//为角色设置物理躯干
		player->bindPhysicsBody();

		player->getAimPointInstance();

		player->autorelease();

		player->setCameraMask(1, true);

		return player;
	}
	return nullptr;
}

bool Player::bindPhysicsBody()
{
	auto physicsBody = cocos2d::PhysicsBody::createBox(sprite_->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setContactTestBitmask(PLAYER_CONTACT_MASK);
	physicsBody->setCategoryBitmask(PLAYER_CATEGORY_MASK);
	setPhysicsBody(physicsBody);

	return true;
}

void Player::getAimPointInstance()
{
	this->addChild(primaryWeapon_->MyAimPoint);
	this->addChild(primaryWeapon_->ReloadAimPoint);
	this->addChild(secondaryWeapon_->MyAimPoint);
	this->addChild(secondaryWeapon_->ReloadAimPoint);
}

void Player::listenToKeyPress(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unusedEvent)
{
	using K = cocos2d::EventKeyboard::KeyCode;

	//WASD按下时，数组的对应元素更新为true
	if (keyCode == K::KEY_W)
	{
		keyPressed_[W] = true;
	}
	if (keyCode == K::KEY_A)
	{
		keyPressed_[A] = true;
	}
	if (keyCode == K::KEY_S)
	{
		keyPressed_[S] = true;
	}
	if (keyCode == K::KEY_D)
	{
		keyPressed_[D] = true;
	}
	if (keyCode == K::KEY_Q)
	{
		switchWeapon();
	}
	if (keyCode == K::KEY_SPACE)
	{
		dodge();
	}
	if (keyCode == K::KEY_E)
	{
		useMedkit();
	}
	if (keyCode == K::KEY_F)
	{
		if (interactItem_ != nullptr)
		{
			interactItem_->interact();
		}
	}
	if (keyCode == K::KEY_R)
	{
		primaryWeapon_->PlayerReload(bulletStock_);
	}
		
}

void Player::listenToKeyRelease(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unusedEvent)
{
	using K = cocos2d::EventKeyboard::KeyCode;

	//WASD按下时，数组的对应元素更新为false
	if (keyCode == K::KEY_W)
	{
		keyPressed_[W] = false;
	}
	if (keyCode == K::KEY_A)
	{
		keyPressed_[A] = false;
	}
	if (keyCode == K::KEY_S)
	{
		keyPressed_[S] = false;
	}
	if (keyCode == K::KEY_D)
	{
		keyPressed_[D] = false;
	}
}

void Player::listenToMouseEvent(cocos2d::Vec2 facingPoint, bool isPressed)
{
	facingPoint_ = facingPoint;

	if (isPressed)
	{
		//Attack
	}

}

void Player::receiveDamage(int damage)
{
	if (!superBody_)
	{
		int realDamage;
		if (shield_ > 0)
		{
			realDamage = static_cast<int>(damage * (1 - shieldProtectionRate));
			shield_ -= static_cast<int>(damage * (shieldProtectionRate));
			shield_ = shield_ < 0 ? 0 : shield_;
		}
		else
		{
			realDamage = damage;
		}
		if (realDamage >= health_)
		{
			die();
		}
		else
		{
			health_ -= realDamage;
		}
	}	
}

void Player::die()
{
	isAlive_ = false;
	health_ = 0;
	cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionSlideInT::create(0.2f, GameOver::create()));
}

void Player::attack(cocos2d::Vec2 pos, cocos2d::Vec2 dir)
{
	primaryWeapon_->Attack(pos, dir);
}

void Player::dodge()
{
	auto dodgeDirection = cocos2d::Vec2::ZERO;
	if (keyPressed_[W])
	{
		dodgeDirection.y += 1;
	}
	if (keyPressed_[A])
	{
		dodgeDirection.x -= 1;
	}
	if (keyPressed_[S])
	{
		dodgeDirection.y -= 1;
	}
	if (keyPressed_[D])
	{
		dodgeDirection.x += 1;
	}
	if (dodgeDirection.x == 0 && dodgeDirection.y == 0)
	{
		auto facingDir = facingPoint_ - getPosition();
		dodgeDirection.x = -facingDir.x;
		dodgeDirection.y = -facingDir.y;
	}

	dodgeDirection.normalize();
	auto delay = cocos2d::DelayTime::create(dodgeTime_);
	auto start = cocos2d::CallFunc::create([=]() {
		DodgeAnimeStart();
		});
	auto act = cocos2d::CallFunc::create([=]() {
		DodgeAnime(dodgeDirection);
		});
	auto end = cocos2d::CallFunc::create([=]() {
		DodgeAnimeEnd();
		});
	this->runAction(cocos2d::Sequence::create(start, act, delay, end, nullptr));
}

void Player::updateFacingStatus()
{
	auto direction = facingPoint_;
	preFacingStatus_ = curFacingStatus_;
	
	if (direction.x > 0 && abs(direction.y) <= direction.x)
	{
		curFacingStatus_ = FacingStatus::right;
	}
	else if (direction.x < 0 && abs(direction.y) <= abs(direction.x))
	{
		curFacingStatus_ = FacingStatus::left;
	}
	else if (direction.y > 0 && abs(direction.x) <= direction.y)
	{
		curFacingStatus_ = FacingStatus::up;
	}
	else if (direction.y < 0 && abs(direction.x) <= abs(direction.y))
	{
		curFacingStatus_ = FacingStatus::down;
	}

	if (preFacingStatus_ != curFacingStatus_)
	{
		statusChanged_ = true;
	}
}

void Player::updateWalkingStatus()
{
	preWalkingStatus_ = curWalkingStatus_;
	curWalkingStatus_ = WalkingStatus::idle;
	for (auto i : keyPressed_)
	{
		if (i)
			curWalkingStatus_ = WalkingStatus::walk;
	}
	if (preWalkingStatus_ != curWalkingStatus_)
	{
		statusChanged_ = true;
	}
}

void Player::DodgeAnimeStart()
{
	canDodge_ = false;
	superBody_ = true;
	allowMove_ = false;
}

void Player::DodgeAnime(cocos2d::Vec2 dir)
{
	getPhysicsBody()->setVelocity(moveSpeed_ * dodgeSpeedBoost_ * dir);	
}

void Player::DodgeAnimeEnd()
{
	canDodge_ = true;
	superBody_ = false;
	allowMove_ = true;
}

Item* Player::getInteractItem()
{
	return interactItem_;
}

void Player::setInteractItem(Item* interactItem)
{
	interactItem_ = interactItem;
}

void Player::switchWeapon()
{
	if (secondaryWeapon_ != nullptr)
	{
		auto t = primaryWeapon_;
		primaryWeapon_ = secondaryWeapon_;
		secondaryWeapon_ = t;
		primaryWeapon_->Active(true);
		secondaryWeapon_->Active(false);
	}
}

Weapon* Player::getPrimaryWeaponInstance()
{
	return primaryWeapon_;
}

Weapon* Player::getSecondaryWeaponInstance()
{
	return secondaryWeapon_;
}

const std::string Player::getBulletName() const
{
	return bulletFilename_;
}

int Player::getMedkitNum()
{
	return medkit_.size();
}

bool Player::isMedkitFull()
{
	return medkit_.size() == medkitMaxNum_;
}

std::stack<Medkit*>* Player::getMedkitBagInstance()
{
	return &medkit_;
}

void Player::useMedkit()
{
	if (health_ != maxHealth_ && getMedkitNum() != 0)
	{
		recoverHealth(medkit_.top()->getRecovery());
		medkit_.top()->release();
		medkit_.pop();
	}
}

void Player::update(float dt)
{
	auto velocity = cocos2d::Vec2::ZERO;
	auto TargetPos = primaryWeapon_->ActiveAimPoint->getPosition();
	TargetPos.normalize();

	if (primaryWeapon_->ActiveAimPoint != nullptr)
	{
		primaryWeapon_->ActiveAimPoint->SetTarget(facingPoint_);
		primaryWeapon_->RecoverRecoil(recoilRecoverBoost_ / 100);
	}

	if (isAttacking)
	{
		recoilRecoverBoost_ = 100;
		attack(this->getPosition(), TargetPos);
	}
	else
	{
		recoilRecoverBoost_ += 120;		
	}

	weaponRotation_ = CC_RADIANS_TO_DEGREES(cocos2d::Vec2::angle(TargetPos, cocos2d::Vec2::ANCHOR_BOTTOM_RIGHT));
	if (TargetPos.y >= 0)
	{
		weaponRotation_ = 360 - weaponRotation_;
	}
	/*if (TargetPos.x <= 0)
	{
		primaryWeapon_->setFlippedX(true);
		secondaryWeapon_->setFlippedX(true);
	}
	else
	{
		primaryWeapon_->setFlippedX(false);
		secondaryWeapon_->setFlippedX(false);
	}*/
	primaryWeapon_->setRotation(weaponRotation_);
	secondaryWeapon_->setRotation(weaponRotation_);

	if (allowMove_)
	{
		if (keyPressed_[W])
		{
			velocity.y += moveSpeed_ * speedBoostFactor_;
		}
		if (keyPressed_[A])
		{
			velocity.x -= moveSpeed_ * speedBoostFactor_;
		}
		if (keyPressed_[S])
		{
			velocity.y -= moveSpeed_ * speedBoostFactor_;
		}
		if (keyPressed_[D])
		{
			velocity.x += moveSpeed_ * speedBoostFactor_;
		}

		//先进行标准化，再乘以模长
		velocity.normalize();
		velocity *= moveSpeed_;
		getPhysicsBody()->setVelocity(velocity);

		updateFacingStatus();
		updateWalkingStatus();
		updateMoveAnimate();
		statusChanged_ = false;
		detectCollision();
	}	
}