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


protected:

	int health_ = 3;																//生命值

	bool isAlive_ = true;														//是否存活

	float shield = 0.5f;															//护甲减伤率
	//怪物移动速度
	float monsterSpeed = 160.0f;


};




#endif // !MONSTER_H
