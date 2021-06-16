#pragma once
/**
* @file Monster.h
*/

#ifndef MONSTER_H
#define MONSTER_H

#include "cocos2d.h"
#include "Const/Const.h"
#include "Character.h"

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
* @author 戴仁杰、杨孟臻
*/
	static Monster* create(const std::string& filename);
	static Monster* create(enemyType_ type);

	/**
* @brief Monster的移动以及发射子弹函数，未来可以将怪物发射子弹单独独立
* @return 无
* @author 戴仁杰、杨孟臻
*/
	void Monster::move();


	virtual void die();


	/**
* @brief 为Monster对象绑定物理躯干
* @return 绑定是否成功
* @author 戴仁杰
*/
	virtual bool bindPhysicsBody();


	/**
* @brief 产生一个随机位置的坐标值
* @return 返回一个Vec2类型的坐标(x,y)数对
* @author 戴仁杰
*/
	cocos2d::Vec2 getRandomPosition();


	virtual void receiveDamage(int damage);

	virtual void updateFacingStatus();


	virtual void updateWalkingStatus();


	void update(float dt);


protected:

	float MoveTime;

	int Health;

	int ShootFreq;

};




#endif // !MONSTER_H
