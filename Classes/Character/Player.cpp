/**
* @file Player.cpp
*/

#include "Player.h"
#include "./Item/PlayerWeapon/Weapon.h"
#include "./Scene/HelloWorldScene.h"

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
		player->setTag(ME);

<<<<<<< Updated upstream
		player->walkRight = createAnimate("/MIKU/walk_right/walk_right", player->sprite_->getContentSize().width, player->sprite_->getContentSize().height);
		player->walkLeft = createAnimate("/MIKU/walk_left/walk_left", player->sprite_->getContentSize().width, player->sprite_->getContentSize().height);
		player->walkUp = createAnimate("/MIKU/walk_up/walk_up", player->sprite_->getContentSize().width, player->sprite_->getContentSize().height);
		player->walkDown = createAnimate("/MIKU/walk_down/walk_down", player->sprite_->getContentSize().width, player->sprite_->getContentSize().height);
		player->idleUp = createAnimate("/MIKU/idle_up/idle_up", player->sprite_->getContentSize().width, player->sprite_->getContentSize().height, 1);
		player->idleDown = createAnimate("/MIKU/idle_down/idle_down", player->sprite_->getContentSize().width, player->sprite_->getContentSize().height, 1);
		player->idleLeft = createAnimate("/MIKU/idle_left/idle_left", player->sprite_->getContentSize().width, player->sprite_->getContentSize().height, 1);
		player->idleRight = createAnimate("/MIKU/idle_right/idle_right", player->sprite_->getContentSize().width, player->sprite_->getContentSize().height, 1);

		//初始化角色武器和弹药
		player->primaryWeapon_ = Weapon::create("default_weapon.png");
		player->secondaryWeapon_ = Weapon::create("default_sec_weapon.png");
		player->bulletFilename = "dart.png";
=======
		player->bindAnimate("MIKU");

		//初始化角色武器和弹药
		player->primaryWeapon_ = Weapon::create("AK47.png");
		player->secondaryWeapon_ = Weapon::create("MP5.png");
		player->primaryWeapon_->bulletFilename_ = "dart.png";
		player->secondaryWeapon_->bulletFilename_ = "dart.png";//临时代码
		player->primaryWeapon_->setScale(0.3f, 0.3f);
		player->secondaryWeapon_->setScale(0.3f, 0.3f);//临时代码
>>>>>>> Stashed changes
		player->addChild(player->primaryWeapon_);
		player->addChild(player->secondaryWeapon_);
		player->primaryWeapon_->setVisible(true);			//默认显示主武器，不显示副武器
		player->secondaryWeapon_->setVisible(false);
		player->moveSpeed_ = 400.f;
<<<<<<< Updated upstream
		player->health_ = 3;
=======
		player->health_ = 100;
>>>>>>> Stashed changes
		player->shield_ = 0.5f;
		//为角色设置物理躯干
		player->bindPhysicsBody();

		player->autorelease();

		return player;
	}
	return nullptr;
}

<<<<<<< Updated upstream
void Player::playMoveAnimate()
{
	if (Status != preStatus)
	{
		sprite_->stopAction(animateBeingPlayed);
		switch (Status)
		{
			case walk_back:
				animateBeingPlayed = walkUp;
				break;
			case walk_front:
				animateBeingPlayed = walkDown;
				break;
			case walk_left:
				animateBeingPlayed = walkLeft;
				break;
			case walk_right:
				animateBeingPlayed = walkRight;
				break;
			case idle_back:
				animateBeingPlayed = idleUp;
				break;
			case idle_front:
				animateBeingPlayed = idleDown;
				break;
			case idle_left:
				animateBeingPlayed = idleLeft;
				break;
			case idle_right:
				animateBeingPlayed = idleRight;
				break;
			default:
				break;
		}
		sprite_->runAction(animateBeingPlayed);
	}
}

bool Player::bindPhysicsBody()
{
	auto physicsBody = cocos2d::PhysicsBody::createBox(getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
=======
bool Player::bindPhysicsBody()
{
	auto physicsBody = cocos2d::PhysicsBody::createBox(sprite_->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setMass(0.1);
>>>>>>> Stashed changes
	physicsBody->setContactTestBitmask(1);
	physicsBody->setCategoryBitmask(5);
	setPhysicsBody(physicsBody);

	return true;
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
<<<<<<< Updated upstream
	if (keyCode == K::KEY_SPACE)
	{
		dodge();
	}

	for (auto i : keyPressed_)
	{
		if (i)
		{
			keyPressed_[4] = true;
		}
	}
=======
	if (keyCode == K::KEY_R)
	{
		primaryWeapon_->Reload();
	}
	if (keyCode == K::KEY_SPACE)
	{
		dodge();
	}
		
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
	keyPressed_[4] = false;
	for (auto i : keyPressed_)
	{
		if (i)
		{
			keyPressed_[4] = true;
		}
	}
}

void Player::listenToMouseEvent(cocos2d::Vec2 direction, bool isPressed)
{
	FacingDir = direction;
	preStatus = Status;
	if (direction.x > 0 && abs(direction.y) <= direction.x)
	{
		Status = keyPressed_[4] ? walk_right : idle_right;
	}
	else if (direction.x < 0 && abs(direction.y) <= abs(direction.x))
	{
		Status = keyPressed_[4] ? walk_left : idle_left;
	}
	else if (direction.y > 0 && abs(direction.x) <= direction.y)
	{
		Status = keyPressed_[4] ? walk_back : idle_back;
	}
	else if (direction.y < 0 && abs(direction.x) <= abs(direction.y))
	{
		Status = keyPressed_[4] ? walk_front : idle_front;
	}
	else
	{
		Status -= Status > 3 && keyPressed_[4] ? 4 : 0;
	}

	if (isPressed)
	{
		//Attack
	}
}

void Player::getInjured(int damage)
{
	if (!superBody)
	{
		int realDamage = static_cast<int>(damage * (1 - shield));
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

bool Player::isAlive()
{
	return isAlive_;
}

void Player::die()
{
	health_ = 0;
	isAlive_ = false;
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
		dodgeDirection.x = -FacingDir.x;
		dodgeDirection.y = -FacingDir.y;		
	}
	
	dodgeDirection.normalize();
	auto delay = cocos2d::DelayTime::create(dodgeTime);
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

void Player::DodgeAnimeStart()
{
	canDodge = false;
	superBody = true;
	allowMove = false;
}

void Player::DodgeAnime(cocos2d::Vec2 dir)
{
	getPhysicsBody()->setVelocity(moveSpeed_ * dodgeSpeedBoost * dir);	
}

void Player::DodgeAnimeEnd()
{
	canDodge = true;
	superBody = false;
	allowMove = true;
}

void Player::switchWeapon()
{
	if (secondaryWeapon_ != nullptr)
	{
		primaryWeapon_->setVisible(false);
		secondaryWeapon_->setVisible(true);
		auto t = primaryWeapon_;
		primaryWeapon_ = secondaryWeapon_;
		secondaryWeapon_ = t;
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
	return bulletFilename;
}

void Player::update(float dt)
{
	auto velocity = cocos2d::Vec2::ZERO;
	if (allowMove)
	{
		if (keyPressed_[W])
		{
			velocity.y += moveSpeed_ * speedBoostFactor;
		}
		if (keyPressed_[A])
		{
			velocity.x -= moveSpeed_ * speedBoostFactor;
		}
		if (keyPressed_[S])
		{
			velocity.y -= moveSpeed_ * speedBoostFactor;
		}
		if (keyPressed_[D])
		{
			velocity.x += moveSpeed_ * speedBoostFactor;
		}

		//先进行标准化，再乘以模长
		velocity.normalize();
		velocity *= moveSpeed_;
		getPhysicsBody()->setVelocity(velocity);
	}	
	playMoveAnimate();
=======
void Player::receiveDamage(int damage)
{
	if (!superBody_)
	{
		int realDamage = static_cast<int>(damage * (1 - shield_));
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

bool Player::isAlive()
{
	return isAlive_;
}

void Player::Attack(cocos2d::Vec2 pos, cocos2d::Vec2 dir)
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
		dodgeDirection.x = facingDir.x;
		dodgeDirection.y = facingDir.y;
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
	auto direction = facingPoint_ - getPosition();
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

void Player::switchWeapon()
{
	if (secondaryWeapon_ != nullptr)
	{
		auto t = primaryWeapon_;
		primaryWeapon_ = secondaryWeapon_;
		secondaryWeapon_ = t;
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

void Player::update(float dt)
{
	auto velocity = cocos2d::Vec2::ZERO;
	cocos2d::Vec2 offset = facingPoint_ - this->getPosition();
	offset.normalize();

	if (isAttacking)
	{		
		Attack(this->getPosition(), offset);
	}

	float WeaponRotation = CC_RADIANS_TO_DEGREES(cocos2d::Vec2::angle(offset, cocos2d::Vec2::ANCHOR_BOTTOM_RIGHT));
	if (offset.y >= 0)
	{
		WeaponRotation = 360 - WeaponRotation;
	}
	primaryWeapon_->setRotation(WeaponRotation);
	secondaryWeapon_->setRotation(WeaponRotation);

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
	}	
>>>>>>> Stashed changes
}