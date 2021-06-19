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
* @author 戴仁杰
*/
	static Monster* create(const std::string& filename);
	static Monster* create(enemyType_ type);

	/**
* @brief Monster的移动以及发射子弹函数，未来可以将怪物发射子弹单独独立
* @return 无
* @author 戴仁杰
*/
	void move();

	void shoot();

	virtual void die();


	/**
* @brief 为Monster对象绑定物理躯干
* @return 绑定是否成功
* @author 戴仁杰
*/
	virtual bool bindPhysicsBody();


	virtual void receiveDamage(int damage);

	virtual void updateFacingStatus();


	virtual void updateWalkingStatus();


	void update(float dt);

	static bool isPlayerSuperDamage_;

protected:

	int ShootFreq;

	enemyType_ typeOfThisMonster;

	bool autoShoot = false;

	float shootGap = 2.f;

};




#endif // !MONSTER_H
