/**
* @file Item.h
*/

#ifndef __ITEM_H__
#define __ITEM_H__

#include "cocos2d.h"

class Item : public cocos2d::Sprite
{
public:

	virtual ~Item() = default;


	/**
*@brief 绑定贴图精灵
* @param 指向被绑定给物品对象的精灵
* @author 孟宇
*/
	void bindPictureSprite(cocos2d::Sprite* sprite);


	/**
*@brief 交互
* @author 孟宇
*/
	//virtual void interact() = 0


		/**
*@brief 绑定物理躯干
* @author 孟宇
*/
	virtual bool bindPhysicsBody() = 0;

protected:

	cocos2d::Sprite* sprite_;
};

#endif	// !__ITEM_H__