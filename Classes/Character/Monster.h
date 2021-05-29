#pragma once
/**
* @file Monster.h
*/

#ifndef MONSTER_H
#define MONSTER_H

#include "cocos2d.h"
#include "Item/PlayerWeapon/Weapon.h"
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
	static Monster* create(const std::string& filename);


	/**
* @brief 为Monster对象绑定物理躯干
* @return 绑定是否成功
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

};




#endif // !MONSTER_H
