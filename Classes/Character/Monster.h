#pragma once
/**
* @file Monster.h
*/

#ifndef MONSTER_H
#define MONSTER_H

#include "cocos2d.h"
#include "Character.h"
#include <string>
#include "./Scene/HelloWorldScene.h"

/**
*@brief 怪物类
*/

class Monster : public Character
{
public:

	/**
* @brief 生成一个Monster实例
* @param Monster实例对应的素材文件名
* @return 指向该Monster实例的指针
* @author 戴仁杰
*/
	static Monster* create(const std::string& filename);


	/**
* @brief Monster的移动以及发射子弹函数，未来可以将怪物发射子弹单独独立
* @return 无
* @author 戴仁杰
*/
	void Monster::move();


	/**
* @brief 为Monster对象绑定物理躯干
* @return 绑定是否成功
* @author 戴仁杰
*/
	virtual bool bindPhysicsBody();


	/**
* @brief 受伤情况，通过原始伤害与后期护甲魔抗等减伤搭配使用
* @param 收到的原始伤害值
* @author 戴仁杰
*/
	void receiveDamage(int damage);


	/**
* @brief 获取角色是否生存
* @return 存活状态
* @author 戴仁杰
*/
	bool isAlive();


	/**
* @brief 角色死亡
* @author 戴仁杰
*/
	void die();



protected:

	//生命值
	int health_ = 3;																
	//是否存活
	bool isAlive_ = true;														
	//护甲减伤率
	float shield = 0.5f;
	//怪物移动速度
	float monsterSpeed = 160.0f;
	//无敌状态，默认为false
	bool superBody = false;

};




#endif // !MONSTER_H
