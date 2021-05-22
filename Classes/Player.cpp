/**
* @file Player.cpp
*/

#include "Player.h"
#include "Weapon.h"
#include "HelloWorldScene.h"

Player* Player::create(const std::string& filename)
{
	auto player = new(std::nothrow) Player();
	if (!player)
	{
		return nullptr;
	}
	auto picture = cocos2d::Sprite::create(filename);

	if (picture &&player)
	{
		//设置角色贴图
		player->addChild(picture);
		//设置角色初始位置
		player->setPosition(cocos2d::Vec2(player->x_, player->y_));
		//标记角色
		player->setTag(ME);
		//初始化角色武器和弹药
		player->weapon_ = Weapon::create("default_weapon.png");
		player->bulletFilename = "dart.png";
		player->addChild(player->weapon_);
		//为角色设置物理躯干
		auto physicsBody = cocos2d::PhysicsBody::createBox(player->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setContactTestBitmask(1);
		physicsBody->setCategoryBitmask(5);
		player->setPhysicsBody(physicsBody);
		player->autorelease();
		return player;
	}
	return nullptr;
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

Weapon* Player::getWeaponInstance()
{
	return weapon_;
}

const std::string Player::getBulletName()
{
	return bulletFilename;
}

void Player::update(float dt)
{
	x_ = getPosition().x;
	y_ = getPosition().y;

	if (keyPressed_[W]) {
		y_ += stepLength_;
	}
	if (keyPressed_[A]) {
		x_ -= stepLength_;
	}
	if (keyPressed_[S]) {
		y_ -= stepLength_;
	}
	if (keyPressed_[D]) {
		x_ += stepLength_;
	}

	setPosition(x_, y_);
}