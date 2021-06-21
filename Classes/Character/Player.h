/**
* @file Player.h
*/

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "./Item/PlayerWeapon/Weapon.h"
#include "Character.h"
#include "Item/PlayerWeapon/PlayerAimPoint.h"
#include <string>
#include "Const/Const.h"
#include "Item/Medkit/Medkit.h"


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


	//响应键盘按下，移动角色
	void listenToKeyPress(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* unusedEvent);
	//响应键盘松开，停止移动
	void listenToKeyRelease(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* unusedEvent);
	//接收鼠标事件
	void listenToMouseEvent(cocos2d::Vec2, bool isPressed);


	//受伤
	virtual void receiveDamage(int damage);

	virtual void die();

	virtual void updateFacingStatus();
	virtual void updateWalkingStatus();



	void dodge();
	void DodgeAnimeStart();
	void DodgeAnime(cocos2d::Vec2 dir);
	void DodgeAnimeEnd();


	Item* getInteractItem();
	void setInteractItem(Item* interactItem);


	//切换武器
	void switchWeapon();


	// 获取角色当前装备的武器对象
	Weapon* getPrimaryWeaponInstance();
	void setPrimaryWeaponInstance(Weapon* weapon);
	//获取角色当前未装备的副武器对象
	Weapon* getSecondaryWeaponInstance();
	void setSecondaryWeaponInstance(Weapon* weapon);

	void abandonPrimaryWeapon();

	//获取角色当前装备的子弹种类对应的素材文件名
	const std::string getBulletName() const;


	int getMedkitNum();
	bool isMedkitFull();
	std::stack<Medkit*>* getMedkitBagInstance();

	void useMedkit();


	//更新角色状态
	void update(float dt);


	/*
攻击，会使用目前的主武器攻击
杨孟臻
*/
	virtual void attack(cocos2d::Vec2 pos, cocos2d::Vec2 dir);

	void setAttackStatus(bool status);

	void getAimPointInstance();
	void removeAimPoint(Weapon* weapon);

	std::vector<int>& getBulletStock();

	bool isAttacking = false;

	bool settingSuperBody_ = false;

protected:

	std::vector<int> bulletStock_;

	//移动
	enum Key { W, A, S, D };
	bool keyPressed_[4]{};
	float speedBoostFactor_ = 1;
	bool allowMove_ = true;

	//闪避数据
	bool canDodge_ = true;
	float dodgeRate_ = 100.0f;
	float dodgeSpeedBoost_ = 3.0f;
	float dodgeTime_ = 0.1f;

	bool superBody_ = false;

	//交互道具
	Item* interactItem_ = nullptr;

	//武器
	Weapon* primaryWeapon_;											//使用中的武器
	Weapon* secondaryWeapon_;										//副武器
	float weaponRotation_ = 0.0f;
	float recoilRecoverBoost_ = 100;


	std::string bulletFilename_;												//当前装备的子弹

	//医疗包
	std::stack<Medkit*> medkit_;
	int medkitMaxNum_ = MEDKIT_MAX_NUM;

	int bulletMaxNum_ = 180;

	cocos2d::Vec2 weaponPosLeft = cocos2d::Vec2(-10, 0);
	cocos2d::Vec2 weaponPosRight = cocos2d::Vec2(10, 0);
	cocos2d::Vec2 weaponPosFront = cocos2d::Vec2(0, -10);

	
};

#endif // !__PLAYER_H__
