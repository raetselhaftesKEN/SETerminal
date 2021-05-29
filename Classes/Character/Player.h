/**
* @file Player.h
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include "./Item/PlayerWeapon/Weapon.h"
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


	//响应键盘按下，移动角色
	void listenToKeyPress(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* unusedEvent);


	//响应键盘松开，停止移动
	void listenToKeyRelease(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* unusedEvent);


	//受伤
	void getInjured(int damage);


	//获取角色是否生存
	bool isAlive();


	//闪避
	void dodge();
	
	//闪避动画
	void DodgeAnimeStart();
	void DodgeAnime(cocos2d::Vec2 dir);
	void DodgeAnimeEnd();

	//切换武器
	void switchWeapon();


	// 获取角色当前装备的武器对象
	Weapon* getPrimaryWeaponInstance();


	//获取角色当前未装备的副武器对象
	Weapon* getSecondaryWeaponInstance();


	//获取角色当前装备的子弹种类对应的素材文件名
	const std::string getBulletName() const;


	//更新角色状态
	void update(float dt);


protected:

	enum PlayerStatus{idle_front,idle_back,idle_left,idle_right,walk_front,walk_back,walk_left,walk_right};

	int Status;

	enum Key { W, A, S, D };

	float speedBoostFactor = 1;

	bool allowMove = true;

	bool canDodge = true;

	float dodgeRate_ = 100.0f;												//闪避倍率

	float dodgeSpeedBoost = 3.0f;

	float dodgeTime = 0.1f;

	bool superBody = false;

	bool keyPressed_[5]{};														//方向键状态，true表示按下

	Weapon* primaryWeapon_;											//使用中的武器

	Weapon* secondaryWeapon_;										//副武器

	std::string bulletFilename;												//当前装备的子弹

	//待更新属性、血量、buff等

};

#endif // !PLAYER_H
