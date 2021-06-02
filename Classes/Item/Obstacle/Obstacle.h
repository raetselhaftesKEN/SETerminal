/**
* @file Obstacle.h
*/

#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include "cocos2d.h"
#include "../Item.h"

class Obstacle: public Item
{
public:
	/**
* @brief 绑定贴图精灵
* @param 指向被绑定给物品对象的精灵
*/
	static Obstacle* create(const std::string& filename);


	/**
*@brief 绑定物理躯干
*/
	virtual bool bindPhysicsBody();


	/**
* @brief 设置该物体不能被走上去
*@param 发生碰撞的物体指针，是否让该函数生效(默认生效)
*/
	void collision(Sprite* targetSprite, bool flag = true);


	/**
*@brief 交互
*/
	virtual void interact() {};
};

#endif // !__WEAPON_H__