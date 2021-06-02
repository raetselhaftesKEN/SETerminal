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


	//接收鼠标事件
	void listenToMouseEvent(cocos2d::Vec2, bool isPressed);


	//受伤
<<<<<<< Updated upstream
	void getInjured(int damage);
=======
	virtual void receiveDamage(int damage);
>>>>>>> Stashed changes


	//获取角色是否生存
	bool isAlive();

<<<<<<< Updated upstream

	//角色死亡
	void die();

	void playMoveAnimate();

	//闪避
	void dodge();
=======
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
>>>>>>> Stashed changes
	
	//闪避动画
	void DodgeAnimeStart();
	void DodgeAnime(cocos2d::Vec2 dir);
	void DodgeAnimeEnd();

<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
	//切换武器
	void switchWeapon();


	// 获取角色当前装备的武器对象
	Weapon* getPrimaryWeaponInstance();


	//获取角色当前未装备的副武器对象
	Weapon* getSecondaryWeaponInstance();

<<<<<<< Updated upstream

	//获取角色当前装备的子弹种类对应的素材文件名
	const std::string getBulletName() const;


=======
>>>>>>> Stashed changes
	//更新角色状态
	void update(float dt);


	bool isAttacking = false;

protected:

<<<<<<< Updated upstream
	cocos2d::Vec2 FacingDir;

	enum PlayerStatus{
		idle_back, 
		idle_front,
		idle_left,
		idle_right,
		walk_back,
		walk_front,
		walk_left,
		walk_right
	};

	cocos2d::Animate* walkUp;
	cocos2d::Animate* walkDown;
	cocos2d::Animate* walkLeft;
	cocos2d::Animate* walkRight;
	cocos2d::Animate* idleUp;
	cocos2d::Animate* idleDown;
	cocos2d::Animate* idleLeft;
	cocos2d::Animate* idleRight;
	cocos2d::Animate* animateBeingPlayed;

	int Status;

	int preStatus;

	enum Key{W, A, S, D};

	 int moveSpeed_ = 400;													//角色移动速度

	 float speedBoostFactor = 1;

	 bool allowMove = true;

	 bool canDodge = true;

	float dodgeRate_ = 100.0f;												//闪避倍率

	float dodgeSpeedBoost = 3.0f;

	float dodgeTime = 0.1f;

	bool superBody = false;

	bool keyPressed_[5]{};														//方向键状态，true表示按下

	int health_ = 3;																//生命值

	bool isAlive_ = true;														//是否存活

	float shield = 0.5f;															//护甲减伤率

	Weapon* primaryWeapon_;											//使用中的武器

	Weapon* secondaryWeapon_;										//副武器

	std::string bulletFilename;												//当前装备的子弹
=======
	

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
>>>>>>> Stashed changes

	//待更新属性、血量、buff等

};

#endif // !PLAYER_H
