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

	/**
* @brief 生成一个Player实例
* @param Player实例对应的素材文件名
* @return 指向该Player实例的指针
* @author 孟宇
*/
	static Player* create(const std::string& filename);


	/**
* @brief 为Player对象绑定物理躯干
* @return 绑定是否成功
* @author 孟宇
*/
	virtual bool bindPhysicsBody();


	/**
* @brief 响应键盘按下，移动角色
* @param 按下键键码
* @author 孟宇
*/
	void listenToKeyPress(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* unusedEvent);


	/**
* @brief 响应键盘松开，停止移动
* @param 松开键键码
* @author 孟宇
*/
	void listenToKeyRelease(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* unusedEvent);


	/**
* @brief 受伤
* @author 孟宇
*/
	void getInjured(int damage);


	/**
* @brief 获取角色是否生存
* @return 存活状态
* @author 孟宇
*/
	bool isAlive();


	/**
* @brief 角色死亡
* @author 孟宇
*/
	void die();


	/**
* @brief 闪避
* @author 孟宇
*/	
	void dodge();
	
	//闪避动画
	void DodgeAnimeStart();
	void DodgeAnime(cocos2d::Vec2 dir);
	void DodgeAnimeEnd();

	/**
* @brief 切换武器
* @author 孟宇
*/
	void switchWeapon();


	/**
* @brief 获取角色当前装备的武器对象
* @return 指向角色当前装备的武器对象的指针
* @author 孟宇
*/
	Weapon* getPrimaryWeaponInstance();


	/**
* @brief 获取角色当前未装备的副武器对象
* @return 指向角色当前未装备的副武器对象的指针
* @author 孟宇
*/
	Weapon* getSecondaryWeaponInstance();


	/**
* @brief 获取角色当前装备的子弹种类对应的素材文件名
* @return 角色当前装备的子弹种类对应的素材文件名的字符串
* @author 孟宇
*/
	const std::string getBulletName() const;


	/**
* @brief 更新角色状态
* @author 孟宇
*/
	void update(float dt);


protected:

	enum Key{W, A, S, D};

	 int moveSpeed_ = 400;													//角色移动速度

	 float speedBoostFactor = 1;

	 bool allowMove = true;

	 bool canDodge = true;

	float dodgeRate_ = 100.0f;												//闪避倍率

	float dodgeSpeedBoost = 3.0f;

	float dodgeTime = 0.1f;

	bool superBody = false;

	bool keyPressed_[4]{};														//方向键状态，true表示按下

	int health_ = 3;																//生命值

	bool isAlive_ = true;														//是否存活

	float shield = 0.5f;															//护甲减伤率

	Weapon* primaryWeapon_;											//使用中的武器

	Weapon* secondaryWeapon_;										//副武器

	std::string bulletFilename;												//当前装备的子弹

	//待更新属性、血量、buff等

};

#endif // !PLAYER_H
