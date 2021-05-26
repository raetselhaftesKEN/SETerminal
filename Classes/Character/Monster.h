#pragma once
/**
* @file Monster.h
*/

#ifndef MONSTER_H
#define MONSTER_H

#include "cocos2d.h"
#include "./Item/Weapon.h"
#include "Character.h"
#include <string>

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
	static Monster* create(const std::string& filename, float sizeX, float sizeY);


	/**
* @brief 为Monster对象绑定物理躯干
* @return 绑定是否成功
* @author 戴仁杰
*/
	void Monster::move(float sizeX, float sizeY, cocos2d::Vec2 playerPostion);


	/**
* @brief 为Monster对象绑定物理躯干
* @return 绑定是否成功
* @author 戴仁杰
*/
	virtual bool bindPhysicsBody();




protected:

	int health_ = 3;																//生命值

	bool isAlive_ = true;														//是否存活

	float shield = 0.5f;															//护甲减伤率
	//怪物移动速度
	float monsterSpeed = 160;

	//待更新属性、血量、buff等

};




#endif // !MONSTER_H
