/**
* @file Player.cpp
*/

#include "Player.h"
#include "Item/Medkit/Medkit.h"
#include "./Item/PlayerWeapon/Weapon.h"
#include "Scene/FightScene/FightScene.h"
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
		player->setPosition(cocos2d::Vec2(2048, 960));
		//标记角色
		player->setTag(PLAYER_TAG);

		player->bindCharacterAnimate("MIKU", 0.1f);

		//初始化角色武器和弹药
		player->primaryWeapon_ = Weapon::create(weaponType_::M4);
		player->primaryWeapon_->Item::pickUp();
		player->secondaryWeapon_ = Weapon::create(weaponType_::SVD);
		player->secondaryWeapon_->Item::pickUp();

		player->addChild(player->primaryWeapon_);
		player->addChild(player->secondaryWeapon_);

		player->primaryWeapon_->Active(true);
		player->secondaryWeapon_->Active(false);

		//////////////////////////////////////////

		//////////////////////////////////////////

		player->primaryWeapon_->setVisible(true);			//默认显示主武器，不显示副武器
		player->secondaryWeapon_->setVisible(false);

		player->moveSpeed_ = PLAYER_DEFAULT_MOVE_SPEED;
		player->health_ = PLAYER_MAX_HEALTH;
		player->maxHealth_ = PLAYER_MAX_HEALTH;
		player->shield_ = PLAYER_DEFAULT_SHIELD;
		player->maxShield_ = PLAYER_DEFAULT_MAX_SHIELD;
		player->shieldProtectionRate_ = PLAYER_DEFAULT_SHIELD_PROTECTION;
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
	if (primaryWeapon_ != nullptr)
	{
		if (primaryWeapon_->getMyAimPoint()->getParent() != this)
		{
			if (primaryWeapon_->getMyAimPoint()->getParent() != nullptr)
			{
				primaryWeapon_->getMyAimPoint()->removeFromParent();
			}
			this->addChild(primaryWeapon_->getMyAimPoint());
		}
		if (primaryWeapon_->getReloadAimPoint()->getParent() != this)
		{
			if (primaryWeapon_->getReloadAimPoint()->getParent() != nullptr)
			{
				primaryWeapon_->getReloadAimPoint()->removeFromParent();
			}
			this->addChild(primaryWeapon_->getReloadAimPoint());
		}
	}
	if (secondaryWeapon_ != nullptr)	//不加载副武器准星
	{
		if (secondaryWeapon_->getMyAimPoint()->getParent() == this)
		{
			secondaryWeapon_->getMyAimPoint()->retain();
			secondaryWeapon_->getMyAimPoint()->removeFromParent();
		}
		if (secondaryWeapon_->getReloadAimPoint()->getParent() == this)
		{
			secondaryWeapon_->getReloadAimPoint()->retain();
			secondaryWeapon_->getReloadAimPoint()->removeFromParent();
		}
	}
	primaryWeapon_->Active(true);
}

void Player::abandonPrimaryWeapon()
{
	if (primaryWeapon_ != nullptr)
	{
		isAttacking = false;

		removeAimPoint(primaryWeapon_);						//解除当前主武器准星
		auto offset = facingPoint_ - getPosition();
		offset.normalize();
		auto dropPosition = getPosition() + DISCARD_ITEM_DISTANCE * offset;
		primaryWeapon_->setPosition(dropPosition);		//主武器位置设为玩家位置
		primaryWeapon_->Active(false);
		primaryWeapon_->setVisible(true);
		primaryWeapon_->retain();
		primaryWeapon_->removeFromParent();				//解除与当前主武器父子关系

		auto runningScene = dynamic_cast<FightScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG));
		runningScene->setDropNode(primaryWeapon_);
		runningScene->scheduleOnce(CC_SCHEDULE_SELECTOR(FightScene::updateDropNode), 0);

		primaryWeapon_->Item::abandon();								//当前主武器设置为未被持有
		primaryWeapon_ = nullptr;
		switchWeapon();														//交换武器
		interactItem_ = nullptr;
	}
}

void Player::removeAimPoint(Weapon* weapon)
{
	if (weapon != nullptr)
	{
		if (weapon->getMyAimPoint()->getParent() == this)
		{
			weapon->getMyAimPoint()->retain();
			weapon->getMyAimPoint()->removeFromParent();
		}
		if (weapon->getReloadAimPoint()->getParent() == this)
		{
			weapon->getReloadAimPoint()->retain();
			weapon->getReloadAimPoint()->removeFromParent();
		}
	}
}

void Player::listenToKeyPress(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unusedEvent)
{
	using K = cocos2d::EventKeyboard::KeyCode;

	if (this->isAlive_)
	{
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
		if (keyCode == K::KEY_ESCAPE)
		{
			auto runningScene = dynamic_cast<FightScene*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(FIGHT_SCENE_TAG));
			{
				if (!(runningScene->settingLayer_->isOpen))
				{
					runningScene->settingLayer_->setPosition(0, 0);
					runningScene->settingLayer_->open();
				}
				else
				{
					runningScene->settingLayer_->close();
				}
			}
		}
		if (!isAttacking)
		{
			if (keyCode == K::KEY_F)
			{
				if (interactItem_ != nullptr)
				{
					interactItem_->interact();
				}
			}
			if (keyCode == K::KEY_R)
			{
				if (primaryWeapon_ != nullptr)
				{
					primaryWeapon_->PlayerReload(bulletStock_);
				}
			}
			if (keyCode == K::KEY_G)
			{
				abandonPrimaryWeapon();
			}
		}
	}
}

void Player::listenToKeyRelease(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unusedEvent)
{
	using K = cocos2d::EventKeyboard::KeyCode;
	if (this->isAlive_)
	{
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
}

void Player::listenToMouseEvent(cocos2d::Vec2 facingPoint, bool isPressed)
{
	facingPoint_ = facingPoint;
}

void Player::receiveDamage(int damage)
{
	if (!superBody_ && !settingSuperBody_)
	{
		receiveDamageMessage = true;
		int realDamage;
		if (shield_ > 0)
		{
			realDamage = static_cast<int>(damage * (1 - shieldProtectionRate_));
			shield_ -= static_cast<int>(damage * (shieldProtectionRate_));
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
	
}

void Player::attack(cocos2d::Vec2 pos, cocos2d::Vec2 dir)
{
	if (isAlive_)
	{
		if (primaryWeapon_ != nullptr && isAttacking)
		{
			primaryWeapon_->Attack(pos, dir);
		}
	}
}

void Player::setAttackStatus(bool status)
{
	isAttacking = status;
	if (!isAttacking)
	{
		if (primaryWeapon_ != nullptr)
		{
			primaryWeapon_->UnAttack();
		}
	}
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
	if (this->isAlive_)
	{
		auto direction = facingPoint_;
		preFacingStatus_ = curFacingStatus_;
		auto size = sprite_->getContentSize();

		if (direction.x > 0 && abs(direction.y) <= direction.x)
		{
			curFacingStatus_ = FacingStatus::right;
			if (primaryWeapon_)
			{
				primaryWeapon_->setPosition(weaponPosRight);
				primaryWeapon_->setLocalZOrder(1);
			}
			if (secondaryWeapon_)
			{
				secondaryWeapon_->setPosition(weaponPosRight);
				secondaryWeapon_->setLocalZOrder(1);
			}
		}
		else if (direction.x < 0 && abs(direction.y) <= abs(direction.x))
		{
			curFacingStatus_ = FacingStatus::left;
			if (primaryWeapon_)
			{
				primaryWeapon_->setPosition(weaponPosLeft);
				primaryWeapon_->setLocalZOrder(1);
			}
			if (secondaryWeapon_)
			{
				secondaryWeapon_->setPosition(weaponPosLeft);
				secondaryWeapon_->setLocalZOrder(1);
			}

		}
		else if (direction.y > 0 && abs(direction.x) <= direction.y)
		{
			curFacingStatus_ = FacingStatus::up;
			if (primaryWeapon_)
			{
				primaryWeapon_->setPosition(weaponPosFront);
				primaryWeapon_->setLocalZOrder(-1);
			}
			if (secondaryWeapon_)
			{
				secondaryWeapon_->setPosition(weaponPosFront);
				secondaryWeapon_->setLocalZOrder(-1);
			}
		}
		else if (direction.y < 0 && abs(direction.x) <= abs(direction.y))
		{
			curFacingStatus_ = FacingStatus::down;
			if (primaryWeapon_)
			{
				primaryWeapon_->setPosition(weaponPosFront);
				primaryWeapon_->setLocalZOrder(1);
			}
			if (secondaryWeapon_)
			{
				secondaryWeapon_->setPosition(weaponPosFront);
				secondaryWeapon_->setLocalZOrder(1);
			}
		}

		if (preFacingStatus_ != curFacingStatus_)
		{
			statusChanged_ = true;
		}
	}
}

void Player::updateWalkingStatus()
{
	if (isAlive_)
	{
		preWalkingStatus_ = curWalkingStatus_;
		curWalkingStatus_ = WalkingStatus::idle;
		for (auto i : keyPressed_)
		{
			if (i)
			{
				curWalkingStatus_ = WalkingStatus::walk;
				break;
			}
		}
		if (preWalkingStatus_ != curWalkingStatus_)
		{
			statusChanged_ = true;
		}
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
		isAttacking = false;
		auto t = primaryWeapon_;
		primaryWeapon_ = secondaryWeapon_;
		secondaryWeapon_ = t;
		primaryWeapon_->Active(true);

		if (secondaryWeapon_ != nullptr)
		{
			secondaryWeapon_->Active(false);
		}
		//primaryWeapon_->ReloadingStatusReset();
		getAimPointInstance();
	}
}

Weapon* Player::getPrimaryWeaponInstance()
{
	return primaryWeapon_;
}

void Player::setPrimaryWeaponInstance(Weapon* weapon)
{
	primaryWeapon_ = weapon;
}

Weapon* Player::getSecondaryWeaponInstance()
{
	return secondaryWeapon_;
}

void Player::setSecondaryWeaponInstance(Weapon* weapon)
{
	secondaryWeapon_ = weapon;
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

std::vector<int>& Player::getBulletStock()
{
	return bulletStock_;
}

void Player::update(float dt)
{
	auto velocity = cocos2d::Vec2::ZERO;
	cocos2d::Vec2 TargetPos = facingPoint_;

	if (primaryWeapon_ != nullptr)
	{
		TargetPos = primaryWeapon_->ActiveAimPoint->getPosition();
	}
	TargetPos.normalize();


	if (primaryWeapon_ != nullptr && primaryWeapon_->ActiveAimPoint != nullptr)
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

	if (primaryWeapon_ != nullptr)
	{
		primaryWeapon_->setRotation(weaponRotation_);
	}
	if (secondaryWeapon_ != nullptr)
	{
		secondaryWeapon_->setRotation(weaponRotation_);
	}

	if (allowMove_ && isAlive_)
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
		velocity.normalize();
		velocity *= moveSpeed_;		
		getPhysicsBody()->setVelocity(velocity);
		updateFacingStatus();
		updateWalkingStatus();
		updateMoveAnimate();
		statusChanged_ = false;
		detectCollision();
	}
	else if(!isAlive_)
	{
		velocity = cocos2d::Vec2::ZERO;
		getPhysicsBody()->setVelocity(velocity);
	}
	

}

