/**
* @file Character.h
*/

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h"

/**
*@brief 角色类，预计包含玩家和怪物
*/

class Character : public cocos2d::Sprite
{
public:

    virtual ~Character() = default;


    /**
*@brief 绑定贴图精灵
* @param 指向被绑定给角色对象的精灵
* @author 孟宇
*/
    void bindPictureSprite(cocos2d::Sprite* sprite);


    /**
*@brief 绑定物理躯干
* @author 孟宇
*/
    virtual bool bindPhysicsBody() = 0;


protected:

    cocos2d::Sprite* sprite_;

};

#endif // !__CHARACTER_H__
