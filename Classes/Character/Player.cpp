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
		player->addChild(player->primaryWeapon_);
		player->addChild(player->secondaryWeapon_);
		player->primaryWeapon_->setVisible(true);			//默认显示主武器，不显示副武器
		player->secondaryWeapon_->setVisible(false);
		player->moveSpeed_ = 400.f;
		player->health_ = 3;
		player->shield_ = 0.5f;
		//为角色设置物理躯干
		player->bindPhysicsBody();

		player->autorelease();
		return player;
	}
	return nullptr;
}

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
}