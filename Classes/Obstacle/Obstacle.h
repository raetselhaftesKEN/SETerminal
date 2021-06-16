/**
* @file Obstacle.h
*/

#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include "cocos2d.h"
#include "Character/Character.h"
#include <vector>

class Obstacle: public cocos2d::Node
{
public:
	/**
* @brief 绑定贴图精灵
* @param 指向被绑定给物品对象的精灵
*/
	static Obstacle* create(const std::string& filename);

	/**
* @brief 设置该物体不能被走上去
*@param 发生碰撞的物体指针，是否让该函数生效(默认生效)
*/
	void collision(Character* targetSprite, bool flag = true);

	/**
*@brief 绑定贴图精灵
* @param 指向被绑定给物品对象的精灵
*/
	void bindPictureSprite(cocos2d::Sprite* sprite);


	bool bindPhysicsBody();


	static cocos2d::Vector<Obstacle*> createObsSet(const int& serial);


protected:

	cocos2d::Sprite* sprite_;
};

#endif // !__OBSTACLE_H__