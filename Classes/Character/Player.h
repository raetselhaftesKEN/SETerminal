/**
* @file Player.h
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include "./Item/PlayerWeapon/Weapon.h"
#include "./Item/PlayerWeapon/PlayerAimPoint.h"
#include "Character.h"
#include <string>

/**
*@brief 角色类
*/

class Player : public Character
{
public:

	//生成一个Player实例
	static Player* create(const std::string& filename);


	//为Player对象绑定物理躯干
	virtual bool bindPhysicsBody();

	//加载准星
	void loadAimPoint();

	//响应键盘按下，移动角色
	void listenToKeyPress(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* unusedEvent);


	//响应键盘松开，停止移动
	void listenToKeyRelease(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* unusedEvent);


	//接收鼠标事件
	void listenToMouseEvent(cocos2d::Vec2, bool isPressed);


	//受伤
	virtual void receiveDamage(int damage);


	//获取角色是否生存
	bool isAlive();

	/*
	攻击，会使用目前的主武器攻击
	杨孟臻
	*/
	void Attack(cocos2d::Vec2 pos, cocos2d::Vec2 dir);

	/*
	闪避
	杨孟臻
	*/
	void dodge();


	virtual void updateFacingStatus();


	virtual void updateWalkingStatus();
	
	//闪避动画
	void DodgeAnimeStart();
	void DodgeAnime(cocos2d::Vec2 dir);
	void DodgeAnimeEnd();


	//切换武器
	void switchWeapon();

	void getAimPointInstance();


	// 获取角色当前装备的武器对象
	Weapon* getPrimaryWeaponInstance();


	//获取角色当前未装备的副武器对象
	Weapon* getSecondaryWeaponInstance();

	//更新角色状态
	void update(float dt);


	bool isAttacking = false;

	float WeaponRotation = 0.0f;

protected:

	
	 
	friend class HealthBar;
	
	enum Key { W, A, S, D };

	bool keyPressed_[4]{};	

	float speedBoostFactor_ = 1;	

	bool allowMove_ = true;

	bool canDodge_ = true;

	float dodgeRate_ = 100.0f;												//闪避倍率

	float dodgeSpeedBoost_ = 3.0f;

	float dodgeTime_ = 0.1f;

	bool superBody_ = false;

	Weapon* primaryWeapon_;											//使用中的武器

	Weapon* secondaryWeapon_;										//副武器

	//待更新属性、血量、buff等
	PlayerAimPoint* ActiveAimPoint;
};

#endif // !PLAYER_H
