/**
* @file Player.h
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include "Weapon.h"
#include <string>

/**
*@brief 角色类
*/

class Player : public cocos2d::Sprite
{
public:

	/**
* @brief 生成一个Player实例
* @param Player实例对应的素材文件名
* @return 指向该Player实例的指针
*/
	static Player* create(const std::string& filename);


	/**
* @brief 响应键盘按下，移动角色
* @param 按下键键码
*/
	void listenToKeyPress(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* unusedEvent);


	/**
* @brief 响应键盘松开，停止移动
* @param 松开键键码
*/
	void listenToKeyRelease(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* unusedEvent);


	/**
* @brief 获取角色当前装备的武器对象
* @return 指向角色当前装备的武器对象的指针
*/
	Weapon* getWeaponInstance();


	/**
* @brief 获取角色当前装备的子弹种类对应的素材文件名
* @return 角色当前装备的子弹种类对应的素材文件名的字符串
*/
	const std::string getBulletName();


	/**
* @brief 更新角色状态
*/
	void update(float dt);


protected:

	enum Key{W, A, S, D};

	static constexpr int stepLength_ = 4;		//角色移动速度

	float x_ = 50.0, y_ = 50.0f;						//初始默认位置

	bool keyPressed_[4]{};								//方向键状态，true表示按下

	Weapon* weapon_;									//当前装备的武器

	std::string bulletFilename;						//当前装备的子弹

	//待更新属性、血量、buff等

};

#endif // !PLAYER_H
